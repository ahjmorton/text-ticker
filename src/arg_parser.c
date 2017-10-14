#include <stdlib.h>
#include <unistd.h>

#include "arg_parser.h"

#define MODE_UNSET -1

args * parse_arguments(int argc, char ** argv, int* error) {
    args * result = (args *)malloc(sizeof(args));
    result->pluginName = NULL;
    result->mode = MODE_ASK_FOR_PLUGIN;
    result->pluginDir = ".";
    int errorCode = NO_ERROR;
    
    int opt;

    while((opt = getopt(argc, argv, "lhd:p:")) != -1) {
       switch(opt) {
         case 'l' :
           result->mode = MODE_LIST_PLUGINS;
           break;
         case 'h' :
           result-> mode = MODE_HELP;
           break;
         case 'p' :
           result-> mode = MODE_RUN_CERTAIN_PLUGIN;
           result->pluginName = optarg;
           break;
         case 'd' :
           result->pluginDir = optarg;
           break;
         case '?' :
           if(optopt == 'p') {
               errorCode = NO_PLUGIN_SPECIFIED;
           } else if(optopt == 'd') {
               errorCode = NO_PLUGIN_DIR_SPECIFIED;
           } else {
               errorCode = UNKNOWN_ARGUMENT;
           }
           goto fail;
       }
    }

    goto success;

    fail :
    free(result);
    result = NULL;

    success :
    *error = errorCode;
    return result;
}
