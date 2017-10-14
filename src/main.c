#include <stdio.h>

#include "readIn.h"
#include "plugin_manager.h"

int main(void) {
    ttpm_init();
    ttpm_load_plugins(".", stderr);

    size_t inputSize;
    int status;
    char * input = readInFrom(stdin, &inputSize, &status);
    if(status != READ_OK) {
       return status;
    }

    size_t effectCount;
    effect * effectSelection = effects(&effectCount);
    int i;
    for(i = 0; i < effectCount; i++) {
        effect * currentEffect = effectSelection + i;
        (*currentEffect->func)(stdout, input, inputSize);
    }

    free(effectSelection);
    ttpm_teardown();
    free(input);

    return 0;
}
