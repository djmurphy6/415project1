#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include<fcntl.h>
#include "command.h"

/*for the ls command*/
void listDir() {
    DIR *dir = opendir(".");
    struct dirent *entry;

    if(dir == NULL){
        printf("opendir() error");
    }

    // read each file in dir
    while((entry = readdir(dir)) != NULL) {
        printf("%s ", entry->d_name);  // Print the name of each entry
    }

    closedir(dir);
    printf(" \n");
}

/*for the pwd command*/
void showCurrentDir() {
    char *cwd = getcwd(NULL, 0);  // Let getcwd allocate the buffer

    if (cwd != NULL) {
        printf("%s\n", cwd);
        free(cwd); 
    } else {
        printf("getcwd() error");
    }
}

/*for the mkdir command*/
void makeDir(char *dirName) { 
    int error = mkdir(dirName, 0777);
    if (error == -1) {
        printf("Directory already exists!");
    }
}

/*for the cd command*/
void changeDir(char *dirName) {
    chdir(dirName);
}

/*for the cp command*/
void copyFile(char *sourcePath, char *destinationPath) {
    // Open the source file for reading
    int src_fd = open(sourcePath, O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return;
    }

    // Open (or create) the destination file for writing, with appropriate permissions
    int dest_fd = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        close(src_fd);  // Make sure to close the source file if destination fails
        return;
    }

    // Buffer to hold the file content as it is copied
    char buffer[1024];
    ssize_t bytesRead;

    // Read from the source and write to the destination until reaching the end of the source file
    while ((bytesRead = read(src_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytesWritten = write(dest_fd, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            return;
        }
    }

    // Check if there was an error during reading
    if (bytesRead == -1) {
        perror("Error reading source file");
    }

    // Close the file descriptors
    close(src_fd);
    close(dest_fd);
}

/*for the mv command*/
void moveFile(char *sourcePath, char *destinationPath) {
    // copy file over

    // delete file from source path
    remove(sourcePath);
}

/*for the rm command*/
void deleteFile(char *filename) {
    remove(filename);
}

/*for the cat command*/
void displayFile(char *filename) {
	// open file
    int fd;
	fd = open(filename, O_RDONLY);
	if (fd == -1) {
        perror("open() error");
        return;
    }		
			
	/* Read in each line using getline() */
	char buffer[1024];
    ssize_t bytes;
	
    while ((bytes = read(fd, buffer, sizeof(buffer))) != 0){
	    write(STDOUT_FILENO, buffer, bytes);
	}

	
    // Handle read error
    if (bytes == -1) {
        printf("read() error");
    }

    close(fd);
			
	/* Write the line to stdout */
			
    /* write 80 "-" characters to stdout */
			
	//for(int i = 0; i < 80; i ++){
	//	write(STDOUT_FILENO, "-", strlen("-"));
	//}
	/* close the read file and free/null assign your line buffer */
	write(STDOUT_FILENO, "\n", 1);
}