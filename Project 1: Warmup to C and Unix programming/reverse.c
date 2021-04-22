/* Project 1: Warmup to C and Unix programming */
/* Jesse Pasanen 0545937 */
/* Vili Raunola 0543366 */

/* Source: https://stackoverflow.com/questions/59014090/warning-implicit-declaration-of-function-getline */
/* error with getline() solved with '#define  _POSIX_C_SOURCE 200809L' */
#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> /* for comparing file inode */

/* struct for doubly linked list */
struct NODE {
	char *line;
	struct NODE *pNext, *pPrevious;
};
typedef struct NODE Node;

/* reads given input file and adds lines to a doubly linked list */
Node *readFile(char *filename, Node *pStart, Node *pEnd) {
	FILE* file;
	/* If *buffer is set to NULL and buffer_size is set 0 before the call, then
       getline() will allocate a buffer for storing the line. */
	/* source: https://man7.org/linux/man-pages/man3/getline.3.html */
	char *buffer = NULL; /* buffer for incoming texts */
	size_t buffer_size = 0;
	ssize_t line_size;	/* amount of characters in the given line/buffer */
	Node *ptr; /* pointer for the linked list */

	if ((file = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
		exit(1);
	}
	
	/* getline returns -1 on failure to read (including EOF) */
	while ((line_size = getline(&buffer, &buffer_size, file)) != -1) {
		/* Malloc struct pointer */
		if ((ptr=(Node*)malloc(sizeof(Node))) == NULL) {
			fprintf(stderr, "malloc failed\n");
			exit(1);
		}
		/* Allocated sizeof(char) times line_size of memory for the line */
		if ((ptr->line=(char*)malloc(sizeof(char)*line_size)) == NULL) {
			fprintf(stderr, "malloc failed\n");
			exit(1);
		}
		/* Source: https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input */
		/* removing \n from buffer */
		buffer[strcspn(buffer, "\n")] = 0;

		/* Filling the linked list and forming links */
		strcpy(ptr->line, buffer);
		ptr->pNext = NULL;
		if (pStart == NULL) {
			ptr->pPrevious = NULL;
			pStart = ptr;
			pEnd = ptr;
		} else {
			ptr->pPrevious = pEnd;
			pEnd->pNext = ptr;
			pEnd = ptr;
		}
	}
	free(buffer);
	fclose(file);
	return pEnd;
}

/* Reads user input from stdin and adds lines to a doubly linked list */
/* very similar function as readFile() which has more in-depth comments */
/* In this function getline() uses stdin instead of a file */
Node *readStdin(Node *pStart, Node *pEnd) {
	Node *ptr;
	char *buffer = NULL; 
	size_t buffer_size = 0;
	ssize_t line_size;
	while ((line_size = getline(&buffer, &buffer_size, stdin)) != -1) {

		/* There weren't any guidelines on how to end the user input and commence printing */
		/* In this version if an empty line is passed, the loop ends and the input is printed in reverse */
		/* We also had a version with signal handling where the user presses ctrl+d to end input and print in reverse */
		if(line_size - 1 == 0) {
			break;
		}
		if ((ptr=(Node*)malloc(sizeof(Node))) == NULL) {
			fprintf(stderr, "malloc failed\n");
			exit(1);
		}
		if ((ptr->line=(char*)malloc(sizeof(char)*line_size)) == NULL) {
			fprintf(stderr, "malloc failed\n");
			exit(1);
		}
		buffer[strcspn(buffer, "\n")] = 0;
		strcpy(ptr->line, buffer);
		ptr->pNext = NULL;
		if (pStart == NULL) {
			ptr->pPrevious = NULL;
			pStart = ptr;
			pEnd = ptr;
		} else {
			ptr->pPrevious = pEnd;
			pEnd->pNext = ptr;
			pEnd = ptr;
		}
	}
	free(buffer);
	return pEnd;
}

/* prints the linked list in reverse to stdout (starting from end pointer) */
void printReverse(Node *pEnd) {
	Node *ptr = pEnd;
	while(ptr != NULL) {
		fprintf(stdout, "%s\n", ptr->line);
		ptr = ptr->pPrevious;
	}
}

/* writes the linked list in reverse to a file (starting from end pointer) */
void writeToFileReverse(char *filename, Node *pEnd) {
	Node *ptr = pEnd;
	FILE* file;
	if ((file = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "error: cannot open file '%s'\n", filename);
		exit(1);
	}
	while(ptr != NULL) {
		fprintf(file, "%s\n", ptr->line);
		ptr = ptr->pPrevious;
	}
	fclose(file);
}

/* frees memory allocated by malloc */
void freeMemory(Node *pEnd) {
	Node *ptr = pEnd;
	while (ptr != NULL) {
		pEnd = ptr->pPrevious;
		free(ptr->line);
		free(ptr);
		ptr = pEnd;		
	}
}


/* Source: https://man7.org/linux/man-pages/man2/lstat.2.html */
/* https://stackoverflow.com/questions/37049845/using-stat2-to-compare-to-files-if-the-same-dont-copy */
/* compares the file inode instead of just the file name to confirm that files are different */
void compareFileInode(char *input_filename, char *output_filename) {
	struct stat src, dst;
	if (stat(input_filename, &src) == -1) {
		// can't stat input file because it doesn't exits
		//perror("stat");
		fprintf(stderr, "reverse: cannot open file '%s'\n", input_filename);
		exit(1);
    	}
	if (stat(output_filename, &dst) == -1) {
		// can't stat output file because it doesn't exits
		return;
    	}
	if (src.st_ino == dst.st_ino) {
		// comparing input and output file inode number
		fprintf(stderr, "reverse: input and output file must differ\n");
		exit(1);
	}
}

int main(int argc, char* argv[]) {
	/* too many arguments */
	if (argc > 3) {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		exit(1);
	}
	/* Start and end pointers for the linked list */
	Node *pStart = NULL, *pEnd = NULL;
	/* Functions that form the linked list return pEnd (end pointer) to the linked list */
	if (argc == 2) {	
		pEnd = readFile(argv[1], pStart, pEnd);
		printReverse(pEnd);
	}
	else if (argc == 3) {
		compareFileInode(argv[1], argv[2]);
		pEnd = readFile(argv[1], pStart, pEnd);
		writeToFileReverse(argv[2], pEnd);
	} else {
		pEnd = readStdin(pStart, pEnd);
		printReverse(pEnd);
	}
	freeMemory(pEnd);
	return 0;
}


/*****************************EOF*****************************/