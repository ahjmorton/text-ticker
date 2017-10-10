#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define OK 0
#define KO 1

typedef void (*effect)(FILE *, char *, size_t);

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

static void identity(FILE * output, char * string, size_t size) {
    fprintf(output, "%s", string);
}

static char rotateChar(char original, char rotation) {
    char base;
    int max;
    if(rotation == 0 || !isalnum(original)) {
        return original;
    } else if(isdigit(original)) {
        base = '0';
        max = 10;
    } else if(islower(original)) {
        base = 'a';
        max = 26;
    } else {
        base = 'A';
        max = 26;
    }

    return base + (((original - base) + rotation) % max);
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

#define ROTATION_MAX 3
#define NO_ROTATION -1
#define SLEEP_DURATION 25L

static void rotate(FILE * output, char * string, size_t size) {
    int toRotate = 0;
    char rotations[size];
    int i;
    for(i = 0; i < size; i++) {
        if(isalnum(string[i])) {
            toRotate += ROTATION_MAX;
            rotations[i] = ROTATION_MAX;
        } else {
            rotations[i] = NO_ROTATION;
        }
    }
    while(toRotate != 0) {
        for(i = 0; i < size; i++) {
            char original = string[i];
            char rotation = rotations[i];
            if(original == '\n' || rotation == NO_ROTATION) {
               continue;
            }
            char current = rotateChar(original, rotation);
            rotations[i] = rotation - 1;
            toRotate--;
            fputc(current, output);
            fflush(output);
            sleepFor(SLEEP_DURATION);
        }
        fprintf(output, "\033[%d;D", size);
    }
    for(i = 0; i < size; i++) {
      char original = string[i];
      fputc(original, output);
      fflush(output);
      sleepFor(SLEEP_DURATION);
    }
}

int main(void) {
    size_t inputSize;
    int status;
    char * input = readInFrom(stdin, &inputSize, &status);
    if(status != OK) {
       return 1;
    }

    effect action = &rotate;
    action(stdout, input, inputSize);
    free(input);
    return 0;
}
