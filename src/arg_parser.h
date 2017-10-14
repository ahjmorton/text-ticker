#ifndef ARGUMENT_PARSING_HEADER
#define ARGUMENT_PARSING_HEADER

#define NO_ERROR 0
#define UNKNOWN_ARGUMENT 1
#define TOO_MANY_ARGUMENTS_PASSED 2
#define NO_PLUGIN_SPECIFIED 3
#define NO_PLUGIN_DIR_SPECIFIED 4

#define MODE_LIST_PLUGINS 1
#define MODE_HELP 2
#define MODE_RUN_CERTAIN_PLUGIN 3
#define MODE_ASK_FOR_PLUGIN 4

typedef struct parsed_args {
   char mode;
   char * pluginName;
   char * pluginDir;
} args;

args * parse_arguments(int argc, char ** argv, int* error);

#endif
