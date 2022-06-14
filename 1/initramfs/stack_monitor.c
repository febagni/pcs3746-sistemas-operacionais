#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))

#define sysfs_path "/sys/kernel/sys_stack"

typedef struct {
    int fd;
    char *path;
} my_files_desc;

static my_files_desc my_files[] = {
    { .fd = -1, .path = sysfs_path "/count"},
    { .fd = -1, .path = sysfs_path "/first"},
    { .fd = -1, .path = sysfs_path "/last"},
};

void init_fds() {
    for (size_t i = 0; i < ARRAY_LENGTH(my_files); i++) {
        my_files[i].fd = open(my_files[i].path, O_RDONLY);
        printf("%s: %d\n", my_files[i].path, my_files[i].fd);
    }
}

int main() {
    char stack_buf[5];

    init_fds();
    while(1) {
        for (size_t i = 0; i < ARRAY_LENGTH(my_files); i++) {
            if (lseek(my_files[i].fd, 0, SEEK_SET)) {
                printf(lseek(my_files[i].fd, 0, SEEK_SET));
                perror("\nlseek\n");
            } 
            else {
                int size = read(my_files[i].fd, stack_buf, 4);
                if (size < 0) {
                    perror("read");
                } 
                else {
                    stack_buf[size] = 0;
                    printf("Stack item %s: %s",
                    &my_files[i].path[sizeof(sysfs_path)], stack_buf);
                }
            }
        }
        printf("\n");
        sleep(rand() % 5 + 1);
    }
    return 0;
}