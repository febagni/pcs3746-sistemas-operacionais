#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 5
#define SHM_KEY 0x1234

struct shmseg {
    int cnt;
    int complete;
    int * mutex;
    int * empty;
    int * full;
    int buf[BUF_SIZE];
};

void init_buffer(int * bufptr, int size);
void print_buffer(int * bufptr, int size);
int produce_item();
int insert_item(int * bufptr, int size, int item);

int main(int argc, char *argv[]) {
   int shmid;
   struct shmseg *shmp;
   int *bufptr;
   int spaceavailable;
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
   
   /* Transfer blocks of data from buffer to shared memory */
   bufptr = shmp->buf;
   spaceavailable = BUF_SIZE;

   init_buffer(bufptr, spaceavailable);
   int item;
   while(1){
      item = produce_item();
      // down(&empty);
      // down(&mutex);
      shmp->cnt = insert_item(bufptr, spaceavailable, item);
      shmp->complete = 0;
      print_buffer(bufptr, spaceavailable);
      bufptr = shmp->buf;
      spaceavailable = BUF_SIZE;
      // up(&mutex);
      // up(&full);
      sleep(2);
   }
   return 0;
}

void print_buffer(int * bufptr, int size){
   printf("Conteúdo do buffer: \n");
   for(int i=0; i<size; i++){
      printf("%d ", bufptr[i]);
   }
   printf("\n");
}

void init_buffer(int * bufptr, int size){
   for(int i=0; i<size; i++){
      bufptr[i] = -1;
   }
}

int produce_item() {
    int item = rand() % 1000;
    printf("Item %d produzido\n", item);
    return item;
}

int insert_item(int * bufptr, int size, int item){
   int filled_count;
   if (bufptr[size-1] != -1){
      printf("Buffer cheio! Item %d descartado \n", item);
      return -1;
   }
   else{
      for(int i=0; i<size; i++){
         if (bufptr[i] == -1){
            bufptr[i] = item;
            filled_count = i;
            break;
         }
      }
   }
   return filled_count;
}