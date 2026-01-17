#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main() {
    int fd;
    char buffer[100];
    static char message[] = "Hello, world"; 
    // Open file with O_CREAT flag to create if it doesn't exist
    // O_RDWR: Open for reading and writing
    // O_CREAT: Create file if it doesn't exist
    // 0644: File permissions (rw-r--r--)
    fd = open("myfile", O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        printf("myfile opened for read/write access\n");

        // Write to file
        ssize_t bytes_written = write(fd, message, strlen(message));
        printf("Written %ld bytes to file\n", bytes_written);
        
        // Go back to the beginning of the file
        lseek(fd, 0, SEEK_SET);
        
        // Read from file
        ssize_t bytes_read = read(fd, buffer, strlen(message));
        buffer[bytes_read] = '\0'; // Null-terminate the string
        
        printf("'%s' was written to myfile\n", buffer);
        
        // Close file
        close(fd);
        printf("File closed successfully\n");
    } else {
        printf("Error opening file!\n");
        return 1;}  
    return 0;}