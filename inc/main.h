/*
    BIOS_device
    main.h

    BIOS_DEVICE main header file
*/
#ifndef MAIN_H
#define MAIN_H


// NCurses libs
#include <ncurses.h>
#include <menu.h>

// Deps
#include <stdlib.h>         /* malloc/calloc stuff */
#include <unistd.h>         /* sleep function */
#include <string.h>

// Configuration macros
#include "config.h"


/* Function definitions ******************************************************/
// Menu selections
int hacks_screen(void);

// Main menu defines
void draw_mainscr(MENU *menu, WINDOW *mw);
void clear_mainscr(MENU *menu, WINDOW *mw, WINDOW *msw);

void print_info(const char *str);
int menu_handler(ITEM *cur);


#endif