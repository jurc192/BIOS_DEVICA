/*
    BIOS_device
    write.h

    BIOS_DEVICE write screen includes
*/
#ifndef WRITE_H
#define WRITE_H

// NCurses libs
#include <ncurses.h>
#include <menu.h>
#include <form.h>

// Deps
#include <stdlib.h>         /* malloc/calloc stuff */
#include <unistd.h>         /* sleep function */
#include <string.h>
#include <ctype.h>          /* isspace ()*/

// Configuration macros
#include "config.h"


/* Function definitions ******************************************************/

int new_project(char *name, WINDOW *win);
int open_project(char *name);

void print_info(const char *str);
size_t trimwhitespace(char *out, size_t len, const char *str);

#endif