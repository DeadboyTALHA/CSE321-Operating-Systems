#include <stdio.h>
#include <stdlib.h>  // Needed for atoi()


// Bubble sort implementation
void sort(int arr[], int n) {
   for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] < arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


int main(int arg_count, char *arg_vector[]) {
    int len_num_arr = arg_count - 1;
    
    // Integer array to store the converted values
    int arr[len_num_arr];

    if (arg_count == 1) {
        printf("No numbers given!\n");
        return 1;
    }

    for (int i = 1; i < arg_count; i++) {
        //printf("Argument %d: %s\n", i, arg_vector[i]);  for debugging

        // Convert argument from string to integer and store in arr using atoi() -> ascii to int it returns 0 if non numeric value is passed
        arr[i - 1] = atoi(arg_vector[i]);
    }

    printf("Odd-Even Status:\n");
    
    sort(arr, len_num_arr);
    
    // Print the integer array
    for (int i = 0; i < arg_count - 1; i++) {
      if (arr[i] % 2 == 1){
        printf("Odd:%d ", arr[i]);
      }
      else if (arr[i] % 2 == -1){
        printf("Odd:%d ", arr[i]);
      }
      else {
        printf("Even:%d ", arr[i]);
      }
    }
    printf("\n");

    return 0;
}
