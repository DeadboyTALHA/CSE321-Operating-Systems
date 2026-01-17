#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int parent_pid = getpid();
    printf("Parent process ID : %d\n", parent_pid);

    int child_pid = fork(); // Create a child process

    if (child_pid == 0) { // Child process
        printf("Child process ID : %d\n", getpid());

        // Create three grandchild processes
        for (int i = 0; i < 3; i++) {
            int grandchild_pid = fork();
            if (grandchild_pid == 0) {
                printf("Grand Child process ID : %d\n", getpid());
                exit(0); // Grandchild exits after printing
            }
        }

        // Child waits for all grandchildren to finish
        while (wait(NULL) > 0);
        
        exit(0); // Child exits after all grandchildren finish
    }

    // Parent waits for child
    wait(NULL);
    return 0;
}

