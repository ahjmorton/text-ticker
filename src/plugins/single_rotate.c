#include <stdio.h>
#include <ctype.h>

#include "common/util.h"

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

int init(int (*registrar)(char *, void (*func)(FILE *, char *, size_t))) {
     return registrar("single rotate", &rotate);
}
