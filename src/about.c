/*
    BIOS_device
    about.c

    About screen
*/
#include <ncurses.h>
#include "config.h"

void print_info(const char *str);

int about_screen(void)
{
    WINDOW *about_screen = newwin(0, 0, 0, 0);
    wrefresh(about_screen);


    print_info("Q = back");

    int about_h = 25;
    int about_w = 80;
    WINDOW *about_window = newwin(about_h, about_w, (LINES - about_h)/2, (COLS - about_w)/2);
    wborder(about_window, '0', '0', '#', '#', '#', '#', '#', '#');
    wrefresh(about_window);

    WINDOW *text_window = derwin(about_window, about_h-4, about_w-4, 2, 2);
    wprintw(text_window, "This is a program for reading and writing BIOS assembly programs.\nIt was developed and never finished by Jure Vidmar. \nFaculty of Informatics and Computer Science, Ljubljana.\n Made in FRIžider");
    wrefresh(text_window);


    int c;
    while ((c = wgetch(about_screen)) != 'q')
    {
       ;
    }

    return 0;
}