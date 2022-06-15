#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "semaphore.h"

#define N 5

long* mutex = init_semaphore(1);
long* empty = init_semaphore(N);
long* full = init_semaphore(0);

int produce_item();
void insert_item(int item);
int remove_item();
void consume_item(int item);

void producer();
void consumer();

int main()
{   
	return 0;
}

int produce_item() {
    int item = rand() % 1000;
    printf("item %d produzido", item);
    return item;
}

void insert_item(int item){
    if buffer[N] != -1 return;
    else{
        for(int i=0; i<N; i++){
            if buffer[i] == -1 buffer[i] = item;
        }
        return;
    }
}

int remove_item() {
    int item;
    for(int i=0; i < N; i++){
        if buffer[N-i] != -1 {
            item = buffer[N-1];
            buffer[N-1] = -1;
            return item;
        }
    }
    return -1;
}

void consume_item(int item) {
    printf("item %d consumido", item);
}

void producer(void){
    int item;

    while(1){
        item = produce_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}

void consumer(void){
    int item;

    while(1){
        down(&full);
        down(&mutex);
        item = remove_item();
        up(&mutex);
        up(&empty);
        consume_item(item);
    }
}
