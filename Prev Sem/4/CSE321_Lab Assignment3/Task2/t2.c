#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>


struct msg {
    long int type;
    char txt[6];
};


int main() {
    pid_t otp;
    
    key_t key = 222;
    
    int msg_id;
    
    msg_id = msgget(key, 0666 | IPC_CREAT);

    if (msg_id == -1) {
        perror("msg_id");
    }
    
    struct msg msg_data;
    
    // taking input
    char workspace_name[100];
    printf("Please enter the workspace name:\n");
    
    scanf("%s", workspace_name);

    if (strcmp(workspace_name, "cse321") == 0) {
        ;  // do nothing
    }
    else {
        printf("Invalid workspace name\n");
        return 1;
    }
    
    // msg_data initialization
    strcpy(msg_data.txt,  workspace_name);
    int msg_type = 19;
    msg_data.type = msg_type;  // used a random positive integer
    
    int snt = msgsnd(msg_id, &msg_data, sizeof(msg_data), IPC_NOWAIT);
    if (snt == -1) {
        perror("msgsnt");
        return 1;
    }
    
    printf("Workspace name sent to otp generator from log in: %s\n\n", msg_data.txt);
    
    otp = fork();
    
    
    if (otp < 0) {
        perror("fork");
        return 1;
    }
    else if (otp == 0) {
        pid_t mail;
        
        /*
        printf("=====otp=====\n");
        printf("mid%d\n", msg_id);
        printf("type:%ld\n", msg_data.type);
        printf("txt:%s\n", msg_data.txt);
        */
        
        msgrcv(msg_id, &msg_data, sizeof(msg_data), msg_type, IPC_NOWAIT);
        
        printf("OTP generator received workspace name from log in: %s\n\n", msg_data.txt);
        
        // otp as pid
        char otp_text[10];
        int otp = getpid();
        
        // using sprintf to convert int to string
        sprintf(otp_text, "%d", otp);
        
        int otp_type = 321;
        int otp_type2 = 7;
        
        // msg data initialization
        strcpy(msg_data.txt, otp_text);
        msg_data.type = otp_type;
        
        /*
        printf("=====otp after init=====\n");
        printf("mid%d\n", msg_id);
        printf("type:%ld\n", msg_data.type);
        printf("txt:%s\n", msg_data.txt);
        */
        
        
        msgsnd(msg_id, &msg_data, sizeof(msg_data), IPC_NOWAIT);
        
        printf("OTP sent to log in from OTP generator: %s\n", msg_data.txt);
        
        
        // creating mail process
        mail = fork();
        
        if (mail < 0) {
            perror("fork");
        }
        else if (mail == 0) {
            /*
            printf("=====mail=====\n");
            printf("mid%d\n", msg_id);
            printf("type:%ld\n", msg_data.type);
            printf("txt:%s\n", msg_data.txt);
            */
            
            msgrcv(msg_id, &msg_data, sizeof(msg_data), otp_type2, IPC_NOWAIT);
            
            //printf("=====mail after rcv=====\n");
            
            printf("Mail received OTP from OTP generator: %s\n", msg_data.txt);
            
            /*
            printf("mid%d\n", msg_id);
            printf("type:%ld\n", msg_data.type);
            printf("txt:%s\n", msg_data.txt);
            */
            
            int mail_type = 9;
            
            msg_data.type = mail_type;
            
            msgsnd(msg_id, &msg_data, sizeof(msg_data), IPC_NOWAIT);
            
            printf("OTP sent to log in from mail: %s\n", msg_data.txt);
        }
        else {
            msg_data.type = otp_type2;
            msgsnd(msg_id, &msg_data, sizeof(msg_data), IPC_NOWAIT);
            
            printf("OTP sent to mail from OTP generator: %s\n", msg_data.txt);
            
            wait(NULL);
        }
        
        
    }
    else {
        /*
        printf("=====parent=====\n");
        printf("mid:%d\n", msg_id);
        printf("type:%ld\n", msg_data.type);
        printf("txt:%s\n", msg_data.txt);
        */
        wait(NULL);
        /*
        printf("=====parent after wait=====\n");
        printf("mid:%d\n", msg_id);
        printf("type:%ld\n", msg_data.type);
        printf("txt:%s\n", msg_data.txt);
        */
        
        char oto_from_otp_gen[10];
        char  oto_from_mail[10];
        int msg_type = 0; // to read first message
        
        msgrcv(msg_id, &msg_data, sizeof(msg_data), msg_type, IPC_NOWAIT);
        strcpy(oto_from_otp_gen, msg_data.txt);
        
        /*
        printf("=====parent after wait after rcv=====\n");
        printf("%d\n", getpid());
        printf("mid:%d\n", msg_id);
        printf("type:%ld\n", msg_data.type);
        printf("txt:%s\n", msg_data.txt);
        */
        
        msgrcv(msg_id, &msg_data, sizeof(msg_data), msg_type, IPC_NOWAIT);
        strcpy(oto_from_mail, msg_data.txt);
        
        /*
        printf("=====parent after wait after rcv=====\n");
        printf("%d\n", getpid());
        printf("mid:%d\n", msg_id);
        printf("type:%ld\n", msg_data.type);
        printf("txt:%s\n", msg_data.txt);
        */
        
        printf("Log in received OTP from OTP generator: %s\n", oto_from_otp_gen);
        printf("Log in received OTP from mail: %s\n", oto_from_mail);
        
        if (strcmp(oto_from_otp_gen, oto_from_mail) == 0) {
            printf("OTP  Verified\n");
        }
        else {
            printf("OTP Verification Failed\n");
        }
        
    }
    
    
    return 0;
    
}
