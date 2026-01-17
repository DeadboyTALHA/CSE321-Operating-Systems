#include <stdio.h>
#include <unistd.h>   // Provides fork() and other system calls for process creation and management
#include <sys/wait.h>  // Provides wait() to make the parent process wait for child processes to finish


int main(){
        
    int pid;    // variable to store the process ID of child
    int pid_1;  // variable to store the process ID of grandchild
	
	
    // Create a new process (child) using fork()
    pid = fork();
	
    if (pid == 0) {  // If pid is 0, we are in the child process
        // Create another new process (grandchild) using fork()
	pid_1 = fork();
		
	if (pid_1 == 0){  // If pid_1 is 0, we are in the grand child process
	    // Grandchild process
	    printf("I am grandchild\n");
	} else {
		        
	    // Child proces
	    wait(NULL);  // Wait for the grandchild process to finish before
	    printf("I am child\n");
		}
    } else {
        // Parent process
	wait(NULL);   // Wait for the child process to finish before
	printf("I am parent\n");
	
    }
    return 0;
}
