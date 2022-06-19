#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_stop	401
#define __NR_start	402
#define __NR_init_semaphore 404
#define __NR_down 405
#define __NR_up 406

long stop_process(int pid)
{
	return syscall(__NR_stop, pid);
}

long continue_process()
{
	return syscall(__NR_start);
}

long init_semaphore(int initial_value){
    return syscall(__NR_init_semaphore, initial_value);
}

long down(int sem_id){
    return syscall(__NR_down, sem_id);
}

long up(int sem_id){
    return syscall(__NR_up, sem_id);
}