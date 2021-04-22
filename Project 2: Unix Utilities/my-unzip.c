/* my-unzip.c */
/* Jesse Pasanen 0545937 */
/* Vili Raunola 0543366 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//This struct is used to store temporarily the counter of how many times a character appeared in a row in the original file and the character itself.
struct compressed_char_struct{
    int binary_no;
    char character;
};

void read_Zipped_File(char *file_name);
void unZip(int amount, char character);

int main(int argc, char *argv[]){
    //If no arguments are given, a message is shown to the user.
    if(argc == 1){
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n"); //The error message differs between assignment and tests
        // fprintf(stdout, "wunzip: file1 [file2 ...]\n"); //Use this for testing
        exit(1);
    }

    //Each given file is unzipped one by one
    for(int i = 1; i < argc;i++){
        read_Zipped_File(argv[i]);
    }
    return(0);
}

//Opens the given file and performs unzipping to it
void read_Zipped_File(char *file_name){
    FILE *file;
    struct compressed_char_struct compressed_char;

    if ((file = fopen(file_name, "rb")) == NULL) {
        fprintf(stderr, "my-unzip: cannot open file\n"); //The error message differs between assignment and tests
        // fprintf(stdout, "wunzip: cannot open file\n"); //Use this for testing
        exit(1);
    }

    //The while loop uses fread to read 5 bytes at a time and store them to a struct.
    //How to read file to struct https://overiq.com/c-programming-101/fread-function-in-c/
    //The size of read bytes is 5 because char takes one byte and integer the remaining four.
    while (fread(&compressed_char, 5, 1, file) == 1) {
        unZip(compressed_char.binary_no, compressed_char.character); //The read binary number and character are then passed from the struct to the unZip function.
    }
    fclose(file);
}

//This function performes the uncrompession by printing the character as many times as the counter suggests.
void unZip(int amount, char character){
    for(int i = 0; i < amount; i++) {
        printf("%c", character);
    }
}

/*******************EOF*******************/