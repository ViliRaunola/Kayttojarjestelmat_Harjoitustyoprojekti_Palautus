/* my-zip.c */
/* Jesse Pasanen 0545937 */
/* Vili Raunola 0543366 */

/* Source: https://stackoverflow.com/questions/59014090/warning-implicit-declaration-of-function-getline */
/* error with getline() solved with '#define  _POSIX_C_SOURCE 200809L' */
#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_Tempfile(char *file_name, FILE *fp_tempFile);
void compress_File(FILE *file);

int main(int argc, char *argv[]){

    //If no arguments are given, a message is shown to the user.
    if(argc == 1){
        fprintf(stderr, "my-zip: file1 [file2 ...]\n");   //The error message differs between assigment and tests
        // fprintf(stdout, "wzip: file1 [file2 ...]\n"); //Use this for testing
        exit(1);
    }

    //If there is only one input file that needs to be compressed it will be sent to compress_File()-function.
    //If there are multiple files to be compressed they are read one by one to a temporary file after which the 
    //temporary file will get compressed.
    if(argc < 3){
        FILE *fp;
        if ((fp = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr,"my-zip: cannot open file\n"); //The error message differs between assigment and tests
            // fprintf(stdout,"wzip: cannot open file\n"); //Use this for testing
            exit(1);
        }
        //Reads and zips the given file
        compress_File(fp);
    }else{
        //How to initialize temporary file in c: https://www.geeksforgeeks.org/tmpfile-function-c/
        FILE *fp_tempFile;
        if((fp_tempFile = tmpfile()) == NULL) {
            perror("my-zip: cannot create temp file");
        }

        //Reading all the given files to a temporary file
        for(int i = 1; i < argc;i++){
            create_Tempfile(argv[i], fp_tempFile);
        }

        //Placing the pointer back to the beginning of the temporaryfile: https://www.geeksforgeeks.org/tmpfile-function-c/
        rewind(fp_tempFile);

        //Reads and zips the temporary file
        compress_File(fp_tempFile);
    }

    return(0);
}

//This function copies the contents of a given file to a temporary file using getline.
void create_Tempfile(char *file_name, FILE *fp_tempFile){
    char *line = NULL;
    ssize_t line_size;
    size_t buffer_size = 0;
    FILE *file;

    if ((file = fopen(file_name, "r")) == NULL) {
        fprintf(stderr,"my-zip: cannot open file\n"); //The error message differs between assigment and tests
        // fprintf(stdout,"wzip: cannot open file\n"); //Use this for testing
        exit(1);
    }

    //getline() automatically updates the size of 'line' and 'n' to
    //fit the line that is beign read. They must be set to NULL and 0 before calling.
    //returns -1 when exiting.
    //Source: man getline
    while((line_size = getline(&line, &buffer_size, file)) != -1){ 
        fprintf(fp_tempFile, "%s", line);
    }

    free(line);
    fclose(file);
}

//This function does the compression by comparing the characters one by one.
void compress_File(FILE *file){
    char c; //Current character
    char pre_char;
    int counter = 0;    //Counter that keeps track of the repeating characters

    //Checks if the file is empty or not.
    if(feof(file)){
        perror("wzip: the file is empty"); 
        exit(1);
    }

    //Reads the file character by character using fgetc. Source: https://www.tutorialspoint.com/c_standard_library/c_function_fgetc.htm
    c = fgetc(file);
    pre_char = c;

    while(1){ 
        //If the end of the file is reached the remaining variables aka counter and current character are printed to standard output.
        if(feof(file)){
            fwrite(&counter, 4, 1,stdout);
            fprintf(stdout,"%c", pre_char);
            break;
        }
        //If the previous and current character match, a counter that keeps tracks of the repeating characters gets increased by one.
        if(pre_char == c){
            counter++;
        }else{ //If the current and previous character don't match, the counter and previous character will be printed aka the compressed information.
            fwrite(&counter, 4, 1,stdout);
            fprintf(stdout,"%c", pre_char);
            pre_char = c;
            counter = 1;
        }
        //Gets the next character from the file.
        c = fgetc(file);
    }

    fclose(file);
}

/*******************EOF*******************/