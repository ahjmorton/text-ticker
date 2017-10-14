#include <time.h>
#include <ctype.h>
#include <strings.h>
#include "util.h"

static char ALPHABET[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static size_t ALPHABET_LEN = sizeof(ALPHABET);

char rotateChar(char original, char rotation) {
    if(rotation == 0 || !isalnum(original)) {
        return original;
    } 
    size_t index = strcspn(ALPHABET, &original);
    return ALPHABET[(index + rotation) % ALPHABET_LEN];
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
