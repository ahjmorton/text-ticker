#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg_parser.h"
#include "read_in.h"
#include "plugin_manager.h"

static effect * load_plugins(char * pluginDir, FILE* errorFile, size_t* effectCount, int * resultCode) {
    int errorCode;
    errorCode = ttpm_init();
    if(errorCode != TTPM_OK) {
        *resultCode = 1;
        return NULL;
    }

    errorCode = ttpm_load_plugins(pluginDir, errorFile);
    if(errorCode != TTPM_OK) {
        *resultCode = 2;
        return NULL;
    }
    *resultCode = 0;
    return effects(effectCount);
}

static void teardown_plugins(effect * effects) {
    free(effects);
    ttpm_teardown();
}

static int list_plugins(char * pluginDir, FILE * outputFile, FILE * errorFile) {
    size_t effectCount;
    int errorCode;
    effect * effects = load_plugins(pluginDir, errorFile, &effectCount, &errorCode);
    if(errorCode != 0) {
        return errorCode;
    }

    int i;
    for(i = 0; i < effectCount; i++) {
        fprintf(outputFile, "%s\n", (effects + i)->name);
    }
    teardown_plugins(effects);
    return 0;
}

static int print_help(FILE * outputFile) {
    return 0;
}

static int applyTransformation(char * pluginDir, 
                               char * pluginName,
                               FILE * inputFile, 
                               FILE * outputFile, 
                               FILE * errorFile) {
    size_t effectCount;
    int errorCode;
    effect * effects = load_plugins(pluginDir, errorFile, &effectCount, &errorCode);
    if(errorCode != 0) {
        return errorCode;
    }

    int i;
    for(i = 0; i < effectCount; i++) {
        if(strcmp(pluginName, (effects + i)->name) == 0) {
            break;
        }
    }
    int resultCode = 0;
    if(i >= effectCount) {
        resultCode = 1;
    } else {
        size_t inputSize;
        char * input = readInFrom(inputFile, &inputSize, &errorCode);
        if(errorCode != READ_OK) {
            resultCode = 1;
        } else {
            ((effects + i)->func)(outputFile, input, inputSize);
        }
    }
    teardown_plugins(effects);
    return resultCode;

}

int go(int argc, 
       char** argv, 
       FILE * inputFile, 
       FILE * outputFile, 
       FILE * errorFile) {

    int errorCode;
    args * args = parse_arguments(argc, argv, &errorCode);
    if(errorCode != NO_ERROR) {
        return errorCode;
    }
    int resultCode;
    switch(args->mode) {
       case MODE_LIST_PLUGINS :
          resultCode = list_plugins(args->pluginDir, outputFile, errorFile);
          break;
       case MODE_HELP : 
          resultCode = print_help(outputFile);
          break;
       case MODE_RUN_CERTAIN_PLUGIN :
          resultCode = applyTransformation(args->pluginDir, args->pluginName, inputFile, outputFile, errorFile);
          break;
       case MODE_ASK_FOR_PLUGIN :
          break;
    }
    free(args);
    return resultCode;
}
