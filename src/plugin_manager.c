#include <dlfcn.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plugin_manager.h"

typedef struct effect_des {
   void (*func)(FILE *, char *, size_t);
   char * name;
} effect_description;

static effect_description * effect_store;
static size_t effect_count;

int ttpm_init(void) {
    effect_count = 0;
    effect_store = (effect_description *)malloc(sizeof(effect_description) * 0);
    return TTPM_OK;
}

int ttpm_teardown(void) {
    effect_count = 0;
    free(effect_store);
    return TTPM_OK;
}

effect * effects(size_t* number) {
    effect * result = (effect *)malloc(sizeof(effect) * effect_count);
    int i;
    effect_description current;
    for(i = 0; i < effect_count; i++) {
        current = *(effect_store + i);
        result[i].func = current.func;
        result[i].name = current.name;
    }
    (*number) = effect_count;
    return result;
}

static int register_effect(char * name, void (*func)(FILE *, char *, size_t)) {
     size_t newCount = effect_count + 1;
     effect_store = (effect_description *)realloc(effect_store, sizeof(effect_description) * newCount);
     (effect_store + newCount - 1)->name = name;
     (effect_store + newCount - 1)->func = func;
     effect_count = newCount;
     return TTPM_OK;
}

static int load_plugin(char * libraryFile, FILE * error) {
    void * handle = dlopen(libraryFile, RTLD_LAZY | RTLD_LOCAL);
    if(!handle) {
        fputs(dlerror(), error);
        return ERROR_READING_PLUGIN; 
    }

    int (*registrar)(int (char *, void (*func)(FILE *, char *, size_t)));
    registrar = dlsym(handle, "init");
    char * errorText = dlerror();
    if(errorText) {
        fputs(errorText, error);
        return ERROR_READING_PLUGIN;
    }

    return registrar(&register_effect);
}

static int endsWith(const char * str, const char * ext) {
   if(!str || !ext) {
      return 0;
   }
   size_t strLength = strlen(str);
   size_t extLength = strlen(ext);
   if(extLength > strLength) {
      return 0;
   }
   return strncmp(str + (strLength - extLength), ext, extLength) == 0;
}

static char * buildFullPath(char * directory, char * file) {
    char * result = (char *)malloc(sizeof(char) * (strlen(directory) + 2 + strlen(file)));
    sprintf(result, "%s/%s", directory, file);
    return result;
}

int ttpm_load_plugins(char * plugin_dir, FILE * error) {
    DIR * directory = opendir(plugin_dir);
    if(directory == NULL) {
        return CANNOT_READ_PLUGIN_DIR;
    } 
    
    int result = TTPM_OK;
    struct dirent * ent;
    while((ent = readdir(directory)) != NULL) {
        if(ent->d_type == DT_REG && endsWith(ent->d_name, ".so")) {
            char * fullPath = buildFullPath(plugin_dir, ent->d_name);
            load_plugin(fullPath, error);
            free(fullPath);
        }
    }
   
    closedir(directory);
    return result;
}
