#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall>

#define __NR_stop	401
#define __NR_start	402
#define __NR_init_semaphore 403
#define __NR_down 404
#define __NR_up 405

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

long down(long *sem_id){
    return syscall(__NR_down, sem_id);
}

long up(long *sem_id){
    return syscall(__NR_up, sem_id)
}