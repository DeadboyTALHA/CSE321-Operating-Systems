#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int a, b, c;
    int parent_pid = getpid();
    int shmid;
    int *process_count;
    
    // comment block for debugging
    printf("a, b, c: %d, %d, %d\n", a, b, c);
    printf("Parent Pid:%d\n", parent_pid);
    //printf("shmid:%d\n", shmid);
    //printf("*pc:%d\n", *process_count);
    
    
    //Create shared memory segment
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    
    // Attach the shared memory to the process's address space
    process_count = (int *)shmat(shmid, NULL, 0);
    
    
    *process_count = 0;
    
    a = fork();
    b = fork();
    c = fork();
    
    // only parents child if they have odd pid will further fork()
    if (getpid() == parent_pid){
        //printf("Parent Pid:%d\n", parent_pid);
        printf("a, b, c: %d, %d, %d\n", a, b, c);
        if (a % 2 == 1) fork();
        if (b % 2 == 1) fork();
        if (c % 2 == 1) fork();
    }
    
    
    // waiting until all child or cascading child processes are finished
    while (wait(NULL) > 0);
    
    // increase process count by 1
    (*process_count)++;
    
    
    
    // only parent will print the process count
    if (getpid() == parent_pid){
        printf("Process %d sees process  count: %d\n", getpid(), *process_count);
    }
    
    // detaching shared memory from processes
    shmdt(process_count);
    
    // only parent will destroy shared memory thru shmctl
    if (getpid() == parent_pid){
        shmctl(shmid, IPC_RMID, NULL);
    }
    
    
    return 0;
}
