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


/* Styling and sizing ********************************************************/
// Main menu
#define M_MENU_WIDTH            23
#define M_MENU_HEIGHT           8
#define M_MENU_PADDING          2      


/* Function definitions ******************************************************/
// Menu selections
int hacks_screen(void);
int write_screen(void);
int about_screen(void);

// Main menu defines
void draw_mainscr(MENU *menu, WINDOW *mw);
void clear_mainscr(MENU *menu, WINDOW *mw, WINDOW *msw);
void recalc_mainscr(MENU *menu, WINDOW *mw, WINDOW *msw);

void print_info(const char *str);
int menu_handler(ITEM *cur);


#endif