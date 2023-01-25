#include <stdlib.h>

#include "config.h"

// Helper function that returns an environment variable as a double, and, if
// it doesn't exist, returns the default value given.
double getenv_double(char * env_var, double default_value) {
    if (env_var != NULL) {
        return strtod(env_var, NULL);
    } else {
        return default_value;
    }
};

// Helper function that returns an environment variable as a char pointer, and,
// if it doesn't exist, returns the default value given.
char * getenv_string(char * env_var, char * default_value) {
    if (env_var != NULL) {
        return env_var;
    } else {
        return default_value;
    }
};

void init_config(PolybarConfig * config) {
    
};
