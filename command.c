#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h>
#include<fcntl.h>
#include "command.h"

//error handling to send perror to stdout
void perror_stdout(const char *msg) {
    // If there's a message, print it first
    if (msg != NULL && strlen(msg) > 0) {
        write(STDOUT_FILENO, msg, strlen(msg));
        write(STDOUT_FILENO, ": ", 2);
    }

    // Get the error message string corresponding to errno
    const char *error_msg = strerror(errno);
    
    // Write the error message to stdout
    write(STDOUT_FILENO, error_msg, strlen(error_msg));
    write(STDOUT_FILENO, "\n", 1);
}

/*for the ls command*/
void listDir() {
    DIR *dir = opendir(".");
    struct dirent *entry;

    if(dir == NULL){
        perror_stdout("opendir() error");
    }

    // read each file in dir
    while((entry = readdir(dir)) != NULL) {
        write(STDOUT_FILENO, entry->d_name, strlen(entry->d_name));
        write(STDOUT_FILENO, " ", 1);  // Print a space between filenames
    }

    closedir(dir);
    write(STDOUT_FILENO, "\n", 1);
}

/*for the pwd command*/
void showCurrentDir() {
    char *cwd = getcwd(NULL, 0);  // Let getcwd allocate the buffer

    if (cwd != NULL) {
        write(STDOUT_FILENO, cwd, strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
        free(cwd); 
    } else {
        perror_stdout("getcwd() error");
    }
}

/*for the mkdir command*/
void makeDir(char *dirName) { 
    int error = mkdir(dirName, 0777);
    if (error == -1) {
        perror_stdout("mkdir error");
    }
}

/*for the cd command*/
void changeDir(char *dirName) {
    int result = chdir(dirName);
    if (result == -1) {
        perror_stdout("chdir error");
    } 
}

/*for the cp command*/
void copyFile(char *sourcePath, char *destinationPath) {
    struct stat dest_stat;

    // Check if the destination path is a directory
    if (stat(destinationPath, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        // If destinationPath is a directory, append the file name to the destination
        char *fileName = basename(sourcePath);  // Extract the file name from sourcePath
        char fullDestPath[1024];  // Buffer to hold the new destination path
        snprintf(fullDestPath, sizeof(fullDestPath), "%s/%s", destinationPath, fileName);
        destinationPath = fullDestPath;
    }


    // Open the source file for reading
    int src_fd = open(sourcePath, O_RDONLY);
    if (src_fd == -1) {
        perror_stdout("Error opening source file");
        return;
    }

    // Open/create destination file for writing
    int dest_fd = open(destinationPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror_stdout("Error opening destination file");
        close(src_fd);  //close the source file if destination fails
        return;
    }

    // Buffer to hold the file content as it is copied
    char buffer[1024];
    ssize_t bytesRead;

    // Read from the source and write to the destination until reaching the end of the source file
    while ((bytesRead = read(src_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytesWritten = write(dest_fd, buffer, bytesRead);
        if (bytesWritten == -1) {
            perror_stdout("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            return;
        }
    }

    // Check if there was an error during reading
    if (bytesRead == -1) {
        perror_stdout("Error reading source file");
    }

    // Close the file descriptors
    close(src_fd);
    close(dest_fd);
}

/*for the mv command*/
void moveFile(char *sourcePath, char *destinationPath) {
    copyFile(sourcePath, destinationPath);

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
        perror_stdout("Error opening file to display!");
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
        perror_stdout("Error reading file!");
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