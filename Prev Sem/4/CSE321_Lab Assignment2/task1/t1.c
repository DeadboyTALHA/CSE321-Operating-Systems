#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

int get_zeroth_fibo();
int get_first_fibo();
int take_first_input();
int take_second_input();
int *create_fibo(int *n);
int *search_fibo(int *fibo_arr);
int n_term;

int main() {
    pthread_t t1, t2;
    
    
    int *fibo_arr;
    

    n_term = take_first_input();
    if (n_term < 0 || n_term > 40) {
        printf("Term must be between 0 and 40 and it must be and integer.\n");
        return 1;
    }


    //printf("&n: %d\n", *(&n));
    pthread_create(&t1, NULL, create_fibo, &n_term);  //by &n we are passing the address n
    pthread_join(t1, &fibo_arr);
    //printf("%p\n", fibo_arr);
    //printf("%d\n", *fibo_arr);
    //printf("GG\n");
    //printf("%d", fibo_arr[0]);
    // Further logic would go here...
    pthread_create(&t2, NULL, search_fibo, fibo_arr);
    pthread_join(t2, NULL);
    

    return 0;
}

int take_first_input() {
    char input[4];  // as we are taking two digit inputs
    int value;
    char *endptr;

    printf("Enter the term of fibonacci sequence:\n");
    fgets(input, sizeof(input), stdin); // read input as a string

    value = strtol(input, &endptr, 10); // convert to int

    if (endptr == input || *endptr != '\n') {
        printf("Invalid input.\n");
        return -1;
    }

    return value;
}

int take_second_input() {
    char input[10];  // depending on our first input naturally this len works fine
    int value;
    char *endptr;

    printf("How many numbers are you willing to search?:\n");
    fgets(input, sizeof(input), stdin); // read input as a string

    value = strtol(input, &endptr, 10); // convert to int (base 10)

    if (endptr == input || *endptr != '\n') {
        printf("Invalid input.\n");
        return -1;
    }

    return value;
}

int get_zeroth_fibo() {
    return 0;
}

int get_first_fibo() {
    return 1;
}


int *create_fibo(int *n) {
    // as we passed &n in the call of this function now we will use *n to dereference and access value of n
    int *a = malloc(sizeof(int) * (*n) + 1);
    for (int i = 0; i < (*n) + 1; i++) {
        if (i == 0) {
            a[i] = get_zeroth_fibo();
            printf("a[%d] = %d\n", i, a[i]);
        }
        else if (i == 1) {
            a[i] = get_first_fibo();
            printf("a[%d] = %d\n", i, a[i]);
        }
        else {
            a[i] = a[i - 1] + a[i - 2];
            printf("a[%d] = %d\n", i, a[i]);
        }
    }
    return a;
}

int *search_fibo(int *arg) {
    int *fibo_arr = (int *)arg;  // Cast back to int*

    int s = take_second_input();
    if (s <= 0) {
        printf("Search count must be greater than zero and an integer.\n");
        pthread_exit(NULL);
    }

    for (int i = 0; i < s; i++) {
        char input[20]; // large enough for safe reading
        char *endptr;
        int index;
        
        printf("Enter search #%d index: ", i + 1);
        fgets(input, sizeof(input), stdin);
        
        input[strcspn(input, "\n")] = '\0'; // Remove newline if present

        index = strtol(input, &endptr, 10);
        
        // Validate input
        if (endptr == input || *endptr != '\0') {
            printf("Invalid input! Please enter an integer.\n");
            return NULL;
        }

        if (index < 0 || index > n_term) {
            printf("result of search #%d = -1\n", i + 1);
        } else {
            printf("result of search #%d = %d\n", i + 1, fibo_arr[index]);
        }
    }

    pthread_exit(NULL);
}

