#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "semaphore.h"

#define N 5
#define NUM_THREADS 2

int buffer[N] = {-1,-1,-1,-1,-1};

int * mutex;
int * empty;
int * full;

void print_buffer();
int produce_item();
void insert_item(int item);
int remove_item();
void consume_item(int item);

void *producer();
void *consumer();

int main()
{   
    mutex = init_semaphore(1);
    empty = init_semaphore(N);
    full = init_semaphore(0);
	print_buffer();
    pthread_t threads[NUM_THREADS];
    int rc;
    printf("In main: creating thread consumer \n");
    rc = pthread_create(&threads[0], NULL, consumer, (void *)0);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

    printf("In main: creating thread producer \n");
    rc = pthread_create(&threads[1], NULL, producer, (void *)1);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }

   pthread_exit(NULL);
   return 0;
}

void print_buffer(){
    printf("Conteúdo do buffer: \n");
    for(int i=0;i<N;++i) {
        printf("%d ", buffer[i]);
    }
    printf("\n \n");
}

int produce_item() {
    int item = rand() % 1000;
    printf("Item %d produzido\n", item);
    return item;
}

void insert_item(int item){
    print_buffer();
    if (buffer[N-1] != -1){
        printf("Buffer cheio! Item %d descartado \n", item);
        return;
    }
    else{
        for(int i=0; i<N; i++){
            if (buffer[i] == -1){
                buffer[i] = item;
                print_buffer();
                return;
            }
        }
        return;
    }
    print_buffer();
}

int remove_item() {
    int item;
    print_buffer();
    if(buffer[0] == -1){
        printf("Buffer vazio, não há o que consumir! \n");
        print_buffer();
        return -1;
    }
    for(int i=0; i < N; i++){
        if (buffer[N-i] != -1) {
            item = buffer[N-1];
            buffer[N-1] = -1;
            print_buffer();
            return item;
        }
    }
    return -1;
}

void consume_item(int item) {
    if (item == -1){
        return;
    }
    printf("Item %d consumido\n", item);
}

void *producer(void){
    int item;

    while(1){
        item = produce_item();
        down(empty);
        down(mutex);
        printf("Produtor entrando na região crítica! \n");
        insert_item(item);
        up(mutex);
        up(full);
        printf("Produtor saindo da região crítica! \n");
        fflush(stdout);
        sleep(3);
    }
}

void *consumer(void){
    int item;

    while(1){
        down(full);
        down(mutex);
        printf("Consumidor entrando na região crítica! \n");
        item = remove_item();
        up(mutex);
        up(empty);
        printf("Consumidor saindo da região crítica! \n");
        consume_item(item);
        fflush(stdout);
        sleep(3);
    }
}
