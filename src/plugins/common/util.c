#include <time.h>
#include <ctype.h>
#include "util.h"

char rotateChar(char original, char rotation) {
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

int sleepFor(unsigned long millis) {
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
            return 1;
        }
    } while(res != 0);
    return 0;
}
