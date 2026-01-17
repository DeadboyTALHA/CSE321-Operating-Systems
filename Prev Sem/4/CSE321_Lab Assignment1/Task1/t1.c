#include <stdio.h>
#include <string.h>


// char *arr[] is an array of pointers to strings, where each pointer refers to a string literal or dynamically allocated memory.
int main(int arg_count, char *arg_vector[]) {
    // Define the maximum length of input
    int max_len_input = 256;

    // Print all command-line arguments (for debugging)
    /*for (int i = 0; i < arg_count; i++) {
        printf("Argument %d: %s\n", i, arg_vector[i]);
    }*/

    // Check if exactly 2 arguments are given (program name + filename)
    if (arg_count != 2) {
        printf("File name not given!!\n");
        return 1;  // Exit with an error code
    }

    // Declare a file pointer
    FILE *file;

    // Open the file in append mode
    file = fopen(arg_vector[1], "a");

    printf("File '%s' opened successfully.\n", arg_vector[1]);
    printf("Enter strings to write to the file (enter -1 to stop):\n");

    // Declare an input buffer
    char input[max_len_input];

    while (1) {
        printf("Input: ");
        fgets(input, max_len_input, stdin);  // Read input from user

        //printf("What you entered: %s", input);  // (for debugging)

        // Stop if the user enters "-1"
        // strcmp is compare the strings lexicographically
        if (strcmp(input, "-1\n") == 0)  // why compare -1\n cuz fgets() adds the new character line
	  {
	      break;
	  }

        // Write input to file
        fprintf(file, "%s", input);
    }

    // Close the file after writing
    fclose(file);

    printf("Data successfully written to %s\n", arg_vector[1]);
    return 0;
}

