#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


struct  shared{
    char sel[100];  // selection of user
    int b;   // balance
};


int main() {
    char thank_msg[100];
    int shm_id;
    int fd[2];
    struct shared *shared_struct;
    
    
    
    //create /get access
    //shmget(key of the shared memory, size, access)
    shm_id = shmget((key_t)111, sizeof(struct shared), 0666 | IPC_CREAT);
    //printf("%ld", sizeof(struct shared));
    
    // for debugging
    //printf("ID of sender process: %d\n", getpid());
    //printf("Key of Shared memory: %d\n",shm_id);
    
    // attaching shared memory to struct shared
    shared_struct = (struct shared *)shmat(shm_id, NULL, 0);
    
    if ((void *)shared_struct == (void *)-1) {
        perror("shmat failed");
        return 1;
    }
      
    
    //creating  pipe
    int p = pipe(fd);
    
    //printf("Pipe p: %d\n", p);   // for debugging
    if (p == -1) {
        perror("pipe");
        return 1;
    }
    
    printf("Provide Your Input From Given Options:\n");
    printf("1. Type a to Add Money\n");
    printf("2. Type w to Withdraw Money\n");
    printf("3. Type c to Check Balance\n");
    
    //using read() system call to  read user input as read(0, buffer, sizeof(buffer)) works like scanf()
    read(0, shared_struct->sel, sizeof(shared_struct->sel));
    
    // setting balance as 1000
    shared_struct->b = 1000;
    
    // for debugging
    //printf("%d\n", shared_struct->b);
    //printf("%s\n", shared_struct->sel);
    
    
    printf("Your selection: %s\n", shared_struct->sel);
    
    
    //creating child process
    pid_t opr;
    opr = fork();
    
    if (opr < 0) {
        perror("fork");
        return 1;
    }
    else if (opr == 0) {
        //  child process
        if (strcmp("a\n", shared_struct->sel) == 0) {
            int add_amount;
            //printf("Current amount: %d\n", shared_struct->b);  // for debugging
            printf("Enter amount to be added: \n");
            scanf("%d", &add_amount);
            if (add_amount > 0) {
                shared_struct->b += add_amount;
                printf("Balance added successfully\n");
                printf("Updated balance after addition:\n%d\n", shared_struct->b);
            }
            else {
                printf("Adding failed, Invalid amount\n");
            }
        }
        else if (strcmp("w\n", shared_struct->sel) == 0) {
            int withdraw_amount;
            //printf("Current amount: %d\n", shared_struct->b);
            printf("Enter amount to be withdrawn: \n");
            scanf("%d", &withdraw_amount);
            if (withdraw_amount > 0 && withdraw_amount < shared_struct->b) {
                shared_struct->b -= withdraw_amount;
                printf("Balance withdrawn successfully\n");
                printf("Updated balance after withdrawal:\n%d\n", shared_struct->b);
            }
            else {
                printf("Withdrawal failed, Invalid amount\n");
            }
        }
        else if (strcmp("c\n", shared_struct->sel) == 0) {
            printf("Your current balance is:\n%d\n", shared_struct->b);
        }
        else {
            printf("Invalid selection\n");
        }
        
        //pipping
        // this thank_msg has a different address space than the parents thank_msg
        strcpy(thank_msg, "Thank you for using\n");  // copying the thanks msg to thank_msg var for ez writing
        close(fd[0]);  // fd[0] for reading so closing
        write(fd[1], thank_msg, sizeof(thank_msg));  // fd[1] for writing
        close(fd[1]);
        
    }
    else {
        // parent process
        wait(NULL);  // interrupting parent
        
        //printf("t:%s\n", thank_msg);  // for debugging  if piping worked
        close(fd[1]);  // fd[1] for writing so closing
        read(fd[0], thank_msg, sizeof(thank_msg));  // fd[0] for reading
        close(fd[0]);
        
        printf("%s", thank_msg);
        
    
    }
    return 0;
}
