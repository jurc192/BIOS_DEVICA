/*
    BIOS_device
    hacks.h

    BIOS_device hacks screen includes
*/
#ifndef HACKS_H
#define HACKS_H


// NCurses libs
#include <ncurses.h>
#include <menu.h>

// Deps
#include <stdlib.h>         /* malloc/calloc stuff */
#include <unistd.h>         /* sleep function */
#include <string.h>

#include <dirent.h>         /* Directory stuff */
#define _SVID_SOURCE        /* to rabim neki za scandir funkcijo */

// Configuration macros
#include "config.h"


/* Function definitions ******************************************************/

void print_info(const char *str);
void print_legend(void);

/* Actions */
int burn_image(ITEM *cur);
void open_repo(ITEM *cur);
char *get_path(ITEM *itm);

/* Menu stuff */
int dir_filter(const struct dirent *d);
int hacks_menu_handler(ITEM *cur, WINDOW *rmw);
MENU *create_hacks_menu(void);

/* Readme window stuff */
WINDOW *create_rmw(ITEM* cur);
WINDOW *draw_readme(ITEM *item, WINDOW *parent);
void scroll_readme(WINDOW *readme, unsigned long int y, unsigned long int x);

#endif