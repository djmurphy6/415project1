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
        perror("opendir() error");
    }

    // read each file in dir
    while((entry = readdir(dir)) != NULL) {
        printf("%s ", entry->d_name);  // Print the name of each entry
    }

    closedir(dir);
    printf("\n");
}

/*for the pwd command*/
void showCurrentDir() {
    char *cwd = getcwd(NULL, 0);  // Let getcwd allocate the buffer

    if (cwd != NULL) {
        printf("%s\n", cwd);
        free(cwd); 
    } else {
        perror("getcwd() error");
    }
}

/*for the mkdir command*/
void makeDir(char *dirName) { 
    mkdir(dirName, 0777);
}

/*for the cd command*/
void changeDir(char *dirName) {
    chdir(dirName);
}

/*for the cp command*/
void copyFile(char *sourcePath, char *destinationPath) {
    
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
			
			
	/* Read in each line using getline() */
	char buffer[1024];

	size_t bytes;
	while ((bytes = read(fd, buffer, sizeof(buffer))) != 0){
	    write(STDOUT_FILENO, buffer, bytes);
	}

	close(fd);
			
	/* Write the line to stdout */
			
    /* write 80 "-" characters to stdout */
			
	for(int i = 0; i < 80; i ++){
		write(STDOUT_FILENO, "-", strlen("-"));
	}
	/* close the read file and free/null assign your line buffer */
	write(STDOUT_FILENO, "\n", 1);
}