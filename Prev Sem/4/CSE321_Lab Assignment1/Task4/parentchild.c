#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){
    // char arr[] is a character array storing a string in contiguous memory, allowing modification.
    // char *arr[] is an array of pointers to strings, where each pointer refers to a string literal or dynamically allocated memory.
    char *cml_args[] = {"to_be_inputed", "10", "2222", "313", "-23", "0", "233", "1", "2", "-40", NULL};
    
    int a = fork();
    if (a == 0){
        cml_args[0] = "./sort";
        execv("./sort", cml_args);  //execv(path, is a null terminated array of character pointers)
      
    }
    else {
        wait(NULL);
        cml_args[0] = "./oddeven";
        execv("./oddeven", cml_args);
    }
    
    return 0;
}
