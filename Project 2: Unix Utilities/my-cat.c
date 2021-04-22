/* my-cat.c */
/* Jesse Pasanen 0545937 */
/* Vili Raunola 0543366 */

/* Source: https://stackoverflow.com/questions/59014090/warning-implicit-declaration-of-function-getline */
/* error with getline() solved with '#define  _POSIX_C_SOURCE 200809L' */
#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>

void read_File(char *file_name);

int main(int argc, char *argv[]){
    /* No files specified on the command line */
    if(argc <= 1){
        return(0);
    }
    /* Reads files from the command line one by one */
    for(int i = 1; i < argc;i++){
        read_File(argv[i]);
    }
    return(0);
}


void read_File(char *file_name){
    /* If *lineptr (*line) is set to NULL and *n (buffer_size) is set 0 before the call, then
       getline() will allocate a buffer for storing the line. */
    /* Source: https://man7.org/linux/man-pages/man3/getdelim.3.html */
    char *line = NULL;
    size_t buffer_size = 0;
    FILE *file;

    if ((file = fopen(file_name, "r")) == NULL) {
        fprintf(stderr ,"my-cat: cannot open file\n"); //The error message differs between assignment and tests
        //fprintf(stdout ,"wcat: cannot open file\n"); //Use this for tests
        exit(1);
    }
    /* getline returns -1 on failure to read  (including end-of-file condition)*/
    while(getline(&line, &buffer_size, file) != -1){ 
        /* Reads file line by line and prints it to stdout */
        fprintf(stdout, "%s", line);
    }
    free(line);
    fclose(file);
}
