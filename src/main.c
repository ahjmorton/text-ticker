#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "plugin_manager.h"

#define BUFFER_SIZE 1024
#define OK 0
#define KO 1

static char * readInFrom(FILE * input, size_t* outputSize, int* error) {
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

static const char TICKER_CHARS[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char TICKER_CHARS_LENGTH = sizeof(TICKER_CHARS);

static char rotateChar(char original, char rotation) {
    if(rotation == 0 || !isalnum(original)) {
        return original;
    } 

    const size_t location = strcspn(TICKER_CHARS, &original);
    return TICKER_CHARS[(location + rotation) % TICKER_CHARS_LENGTH];
}

static int sleepFor(unsigned long millis) {
    struct timespec request, remainder;
    request.tv_sec = (int)(millis / 1000);
    millis = millis - (request.tv_sec * 1000);
    request.tv_nsec = millis * 1000000L;
    int res = 0;
    do {
        res = nanosleep(&request, &remainder);
        if(res == -1) {
            request = remainder;
            remainder.tv_sec = 0;
            remainder.tv_nsec = 0;
        } else if(res != 0) {
            return KO;
        }
    } while(res != 0);
    return OK;
}

int main(void) {
    ttpm_init();
    ttpm_load_plugins(".", stderr);

    size_t inputSize;
    int status;
    char * input = readInFrom(stdin, &inputSize, &status);
    if(status != OK) {
       return 1;
    }

    size_t effectCount;
    effect * effectSelection = effects(&effectCount);
    int i;
    for(i = 0; i < effectCount; i++) {
        (*effectSelection->func)(stdout, input, inputSize);
    }

    free(effectSelection);
    ttpm_teardown();
    free(input);

    return 0;
}
