#ifndef READ_IN_HEADER
#define READ_IN_HEADER

#include <stdio.h>
#define READ_OK 0
#define ERROR_WHILE_READING 1

char * readInFrom(FILE * input, size_t* outputSize, int* error);

#endif
