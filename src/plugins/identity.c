#include <stdio.h>

static void identity(FILE * output, char * input, size_t inputSize) {
    fputs(input, output);  
}

void init(void (*registrar)(char *, void (*func)(FILE *, char *, size_t))) {
     registrar("identity", &identity);
}
