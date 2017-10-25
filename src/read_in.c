#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define OK 0
#define KO 1

char * readInFrom(FILE * input, size_t* outputSize, int* error) {
    char buffer[BUFFER_SIZE];
    (*outputSize) = 1;

    char * result = malloc(sizeof(char) * BUFFER_SIZE);

    if(result == NULL) {
       goto fail;
    }
    result[0] = '\0';
    while(fgets(buffer, BUFFER_SIZE, input)) {
        char * oldPointer = result;
        (*outputSize) += strlen(buffer);
        result = realloc(result, (*outputSize));
        if(result == NULL) {
           result = oldPointer;
           goto fail;
        }
        strcat(result, buffer);
    }

    (*error) = OK;
    goto success;

    fail:
    (*error) = KO;
    (*outputSize) = 0;
    free(result);
    result = NULL;

    success:
    return result;
}

