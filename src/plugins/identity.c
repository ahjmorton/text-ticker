#include <stdio.h>

static void identity(FILE * output, char * input, size_t inputSize) {
    fputs(input, output);  
}

int init(int (*registrar)(char *, void (*func)(FILE *, char *, size_t))) {
     return registrar("identity", &identity);
}
