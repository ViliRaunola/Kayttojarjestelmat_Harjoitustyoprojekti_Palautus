/* my-grep.c */
/* Jesse Pasanen 0545937 */
/* Vili Raunola 0543366 */

/* Source: https://stackoverflow.com/questions/59014090/warning-implicit-declaration-of-function-getline */
/* error with getline() solved with '#define  _POSIX_C_SOURCE 200809L' */
#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void read_File(char *searchTerm, char *file_name);
void readStdin(char *searchTerm);

int main(int argc, char *argv[]){
    /* no command-line arguments passed */
    if(argc <= 1){
        fprintf(stderr,"my-grep: searchterm [file ...]\n");    //The error message differs between assigment and tests
        //fprintf(stdout,"wgrep: searchterm [file ...]\n"); //Use this for testing
        exit(1);
    /* Only search term is passed */
    } else if (argc == 2) {
        readStdin(argv[1]);
        exit(0);
    }
    /* Search term and one or more files is passed */
    /* Reads files from the command line one by one */
    for(int i = 2; i < argc;i++) {
        read_File(argv[1], argv[i]);
    }
    return(0);
}


void read_File(char *searchTerm, char *file_name){
    /* If *lineptr (*line) is set to NULL and *n (buffer_size) is set 0 before the call, then
       getline() will allocate a buffer for storing the line. */
    /* Source: https://man7.org/linux/man-pages/man3/getdelim.3.html */
    char *line = NULL;
    size_t buffer_size = 0;
    FILE *file;

    if ((file = fopen(file_name, "r")) == NULL) {
        fprintf(stderr,"my-grep: cannot open file\n"); //The error message differs between assigment and tests
        // fprintf(stdout,"wgrep: cannot open file\n"); //Use this for testing
        exit(1);
    }
    /* getline returns -1 on failure to read  (including end-of-file condition)*/
    while(getline(&line, &buffer_size, file) != -1){ 
        /* Reads file line by line and prints it to stdout if the search term is present on the line */
        /*source: https://www.delftstack.com/howto/c/string-contains-in-c/ */
        if (strstr(line, searchTerm)) {
            fprintf(stdout, "%s", line);
        }
    }
    free(line);
    fclose(file);
}


void readStdin(char *searchTerm) {
    char *line = NULL;
    size_t buffer_size = 0;

    while(getline(&line, &buffer_size, stdin) != -1){ 
        /* Reads stdin line by line and prints it to stdout if the search term is present on the line */
        /*source: https://www.delftstack.com/howto/c/string-contains-in-c/ */
        if (strstr(line, searchTerm)) {
            fprintf(stdout, "%s", line);
        }
    }
    free(line);
}
/*******************EOF*******************/