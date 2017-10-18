#ifndef PLUGIN_MANAGER_HEADER
#define PLUGIN_MANAGER_HEADER

#define TTPM_OK 0
#define CANNOT_READ_PLUGIN_DIR 1
#define ERROR_LOADING_PLUGIN 2
#define ERROR_READING_PLUGIN 3
#define CANNOT_FIND_INIT 4

#include <stdio.h>

typedef struct effect_struct {
   void (*func)(FILE *, char *, size_t);
   char * name;
} effect;

effect * effects(size_t * number);

int ttpm_init(void);
int ttpm_load_plugins(char * plugin_dir, FILE * error);
int ttpm_teardown(void);

#endif
