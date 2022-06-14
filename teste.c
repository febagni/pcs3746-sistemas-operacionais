#include<stdio.h>
#include<stdlib.h>

typedef struct {
	int* sem_id;
	int   value;
} semaphore;

semaphore* linear_search(semaphore* semaphores, size_t size, int* sem_id) {
  printf("sem_id: %d \n", *sem_id);
  for (size_t i = 0; i < size; i++) {
    printf("sem_id[%ld]: %d \n", i, *semaphores[i].sem_id);
    printf("value[%ld]: %d \n", i, semaphores[i].value);
    if (*semaphores[i].sem_id == *sem_id) {
      return &semaphores[i];
    }
  }
  return NULL;
}

// TODO: APAGAR
int main(void) {

  int sem_ids[] = {0, 1, 2};
  semaphore semaphores[] = {
    {&sem_ids[0], 1}, {&sem_ids[1], 0}, {&sem_ids[2], 5}
  };
  size_t num_semaphores = sizeof(semaphores) / sizeof(semaphore);
  int id_to_be_found = 2;
  semaphore* found = linear_search(semaphores, num_semaphores, &id_to_be_found);
  if (!found) {
    return 1;
  }
  printf("linear_search: value of %d is %d", *found->sem_id, found->value);
  return 0;
}
