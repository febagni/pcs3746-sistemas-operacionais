#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "semaphore.h"

#define BUF_SIZE 5
#define SHM_KEY 0x1234

struct shmseg {
   int cnt;
   int complete;
   int mutex;
   int empty;
   int full;
   int buf[BUF_SIZE];
};

void print_buffer(int * bufptr, int size);
void consume_item(int item);
int remove_item(int * bufptr, int size);

int main(int argc, char *argv[]) {
   int shmid;
   struct shmseg *shmp;
   int item;
   int *bufptr;
   int space_available;
   shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
   if (shmid == -1) {
      perror("Shared memory");
      return 1;
   }
   
   // Attach to the segment to get a pointer to it.
   shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1) {
      perror("Shared memory attach");
      return 1;
   }
   
   bufptr = shmp->buf;
   space_available = BUF_SIZE;
   /* Transfer blocks of data from shared memory to stdout*/
   while (1) {
      if (shmp->cnt == -1) {
         perror("read");
         return 1;
      }
      printf("%d", down(shmp->full));
      printf("%d", down(shmp->mutex));
      down(shmp->full);
      down(shmp->mutex);
      printf("Consumidor entrando na região crítica!\n");
      item = remove_item(bufptr, space_available);
      printf("Consumidor saindo da região crítica!\n");
      up(shmp->mutex);
      up(shmp->empty);
      consume_item(item);
      sleep(3);
   }
   return 0;
}

void print_buffer(int * bufptr, int size){
   printf("\nConteúdo do buffer: \n");
   for(int i=0; i<size; i++){
      printf("%d ", bufptr[i]);
   }
   printf("\n\n");
}

int remove_item(int * bufptr, int size) {
   int item;
  //if(bufptr[0] == -1){
  //   printf("Buffer vazio, não há o que consumir! \n");
  //   return -1;
  //}
  //else {
      for(int i=1; i <= size; i++){
         if (bufptr[size-i] != -1) {
            item = bufptr[size-i];
            bufptr[size-i] = -1;
            return item;
         }
      }
   //}
   return -1;
}

void consume_item(int item) {
    if (item == -1) return;
    printf("Item %d consumido\n\n", item);
}