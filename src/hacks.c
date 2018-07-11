/*
    HACKS screen
    hacks.c
*/
#include "hacks.h"

int hacks_screen(void)
{

    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_MAGENTA);

    // Create new fullscreen window
    WINDOW *hacks_screen = newwin(0, 0, 0, 0);
    // wbkgd(hacks_screen, COLOR_PAIR(1));
    wrefresh(hacks_screen);

    print_legend();
    print_info("This is a dummy description of an item!");


    MENU *menu_hacks = create_hacks_menu();

    /* MENU WINDOW W/ BORDER STUFF */
    int mw_height = LINES-STATUSBAR_HEIGHT;
    int mw_width = MENU_WIDTH;              // TODO: dynamic width
    int mw_x = GLOBAL_PADDING;
    int mw_y = GLOBAL_PADDING;

    WINDOW *menu_window = newwin(mw_height, mw_width, mw_y, mw_x);
    if (menu_window == NULL) {
        printw("Menu window ERROR!\n");
        refresh();
        while(1) {}
    }
    wborder(menu_window, '0', '0', '#', '#', '#', '#', '#', '#');

    WINDOW *menu_subw = derwin(menu_window, mw_height - 4, mw_width - 4, 2, 2);     // HARDCODED PADDING TODO FIX
    // wbkgd(menu_subw, COLOR_PAIR(2));
    wrefresh(menu_subw);
    if (menu_window == NULL)
    {
        printw("Menu SUBwindow ERROR!\n");
        refresh();
        while (1) {}
    }

    menu_opts_off(menu_hacks, O_SHOWDESC);
    set_menu_mark(menu_hacks, NULL);
    set_menu_win(menu_hacks, menu_window);
    set_menu_sub(menu_hacks, menu_subw);

    keypad(menu_window, TRUE);
    post_menu(menu_hacks);
    wrefresh(menu_window);
    refresh();


    /* README WINDOW W/ BORDER STUFF */
    int rmw_height  = mw_height;
    int rmw_width   = COLS - (mw_width+6);
    int rmw_y       = mw_y;
    int rmw_x       = mw_width+4;

    WINDOW *readme_parent = newwin(rmw_height, rmw_width, rmw_y, rmw_x);
    if(readme_parent == NULL) {
        printw("Error at readmeparent\n");
        refresh();
    }
    wborder(readme_parent, 'I', 'I', '#', '#', '#', '#', '#', '#');
    wrefresh(readme_parent);


    /* HACKS MENU MAIN LOOP */
    ITEM *curr_item = current_item(menu_hacks);

    WINDOW *readme_window = draw_readme(curr_item, readme_parent);
    scroll_readme(readme_window, 0, 0);

    unsigned long int rmw_cur_col = 0;
    unsigned long int rmw_cur_row = 0;
    int c;
    while((c = wgetch(menu_window)) != 'q') {

        switch (c)
        {
            case KEY_DOWN:
                menu_driver(menu_hacks, REQ_DOWN_ITEM);
                rmw_cur_row = 0;
                rmw_cur_col = 0;
                curr_item = current_item(menu_hacks);
                readme_window = draw_readme(curr_item, readme_parent);
                break;

            case KEY_UP:
                menu_driver(menu_hacks, REQ_UP_ITEM);
                rmw_cur_row = 0;
                rmw_cur_col = 0;
                curr_item = current_item(menu_hacks);
                readme_window = draw_readme(curr_item, readme_parent);
                break;

            case 'w':   /* Scroll up */
                if (rmw_cur_row > 0) rmw_cur_row--;
                scroll_readme(readme_window, rmw_cur_row, rmw_cur_col);
                break;

            case 's':   /* Scroll down */
                rmw_cur_row++;
                // TODO: FIX ker zdej lahko skrolaš neskončno dol semizdi :S
                scroll_readme(readme_window, rmw_cur_row, rmw_cur_col);
                break;

            case 'a':   /* Scroll left */
                if (rmw_cur_col > 0) rmw_cur_col--;
                scroll_readme(readme_window, rmw_cur_row, rmw_cur_col);
                break;

            case 'd':   /* Scroll right */
                rmw_cur_col++;
                scroll_readme(readme_window, rmw_cur_row, rmw_cur_col);
                break;

            case 'o':   /* Open repository/folder */
                open_repo(curr_item);
                break;

            case 'b':   /* Burn image to floppy disk */
                burn_image(curr_item);
                break;

            /* Regular enter; KEY_ENTER works only for the numpad key */
            case 10:
                hacks_menu_handler(curr_item, readme_window);
                break;
        }
    }


    /* Cleaning up */
    unpost_menu(menu_hacks);
    wrefresh(menu_subw);        // Delete menu and refresh

    wclear(menu_subw);          // Clear menu subwindow
    wrefresh(menu_subw);
    delwin(menu_subw);          // Delete menu subwindow
    
    wclear(menu_window);        // Clear menu window
    wrefresh(menu_window);
    delwin(menu_window);        // Delete menu window

    // SEM S TEM SPLOH KAJ NAREDIL? LOST POINTER?
    wclear(readme_window);      // Clear readme window
    delwin(readme_window);      // Delete readme window

    wclear(readme_parent);      // Clear menu window
    wrefresh(readme_parent);
    delwin(readme_parent);      // Delete readme parent

    wclear(hacks_screen);
    wrefresh(hacks_screen);
    delwin(hacks_screen);

    return 0;
}


/* Burns image to floppy */
int burn_image(ITEM *itm)
{

    /* Glue together path to image file */
    const char *a = get_path(itm);
    const char *b = "builds/";
    const char *c = item_name(itm);
    const char *d = ".img";

    char *path = (char *) malloc(strlen(a) + strlen(b) + strlen(c) + strlen(d) + 1);
    if (path == NULL) {
        print_info("Can't malloc filepath string!");
        return -1;
    }
    strcpy(path, a);
    strcat(path, b);
    strcat(path, c);
    strcat(path, d);

    print_info(path);

    /* Check if image file exists */
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        print_info("Can't open image file. Does it exist?");
        return -1;
    }

    /* Glue together dd command */
    char *cmd_a = "sudo dd if=";
    char *cmd_b = " of=/dev/sdb";
    char *cmd = (char *) malloc(strlen(cmd_a) + strlen(cmd_b) + strlen(path) + 1);
    strcpy(cmd, cmd_a);
    strcat(cmd, path);
    strcat(cmd, cmd_b);

    print_info(cmd);
    
    // Execute command
    // system(cmd);
    FILE *p = popen(cmd, "r");
    if (p == NULL) {
        print_info("Can't execute command to burn the image");
        return -1;
    }

    // Check & print process output to window
    // Create window for console output
    WINDOW *cmdwin = newwin(
            LINES-(2*GLOBAL_PADDING + STATUSBAR_HEIGHT),    // nLINES
            COLS - (2*GLOBAL_PADDING + MENU_WIDTH + 6),     // nCOLS
            GLOBAL_PADDING + 2,                             // begin Y
            MENU_WIDTH + GLOBAL_PADDING + 4                 // begin X
        );

    // wbkgd(cmdwin, COLOR_PAIR(1));
    wrefresh(cmdwin);
    int x, y;
    int ch;
    while((ch = fgetc(p)) != EOF) {
        if(ch == '\n') {
            getyx(cmdwin, y, x);
            wmove(cmdwin, y, 0);
            wechochar(cmdwin, '\r');
        }
        wechochar(cmdwin, ch);
    }
    return 0;
}


/* Opens folder of a currently selected hack */
void open_repo(ITEM *cur)
{
    // Get hack filepath
    char *path = get_path(cur);

    char *a = "nautilus --browser ";
    char *b = " > /dev/null 2>&1";

    char *cmd = (char *) malloc(strlen(a) + strlen(path) + strlen(b) + 1);
    if (cmd == NULL) print_info("Error at cmd malloc!");

    strcpy(cmd, a);
    strcat(cmd, path);
    strcat(cmd, b);

    // Execute command:
    // nautilus --browser $PATH > /dev/null 2>&1
    system(cmd);
}

/* Returns path to an item/hack folder (with trailing slash)*/
char *get_path(ITEM *itm)
{
    const char *name = item_name(itm);              // Name of the current selected hack

    char *fname = (char *) malloc(strlen(HACKS_DIR) + strlen(name) +3);     // +2 is for NULL terminator and '/'
    if (fname == NULL)
    {
        printw("Can't malloc filename!\n");
        refresh();
    }

    strcpy(fname, HACKS_DIR);
    strcat(fname, "/");
    strcat(fname, name);
    strcat(fname, "/");

    return fname;
}


void scroll_readme(WINDOW *readme, unsigned long int y, unsigned long int x)
{   
    if (prefresh(readme, y, x, GLOBAL_PADDING + 2, MENU_WIDTH + GLOBAL_PADDING + 4, LINES - (STATUSBAR_HEIGHT + 1), COLS - (GLOBAL_PADDING + 3)) == ERR)
    {
        printw("Error refreshing!\n");
        refresh();
    }
}

WINDOW *draw_readme(ITEM *item, WINDOW *parent)
{
    WINDOW *rmw = create_rmw(item);

    wclear(parent);
    wborder(parent, 'I', 'I', '#', '#', '#', '#', '#', '#');
    wrefresh(parent);

    /*
    int prefresh(
        WINDOW *pad,
        int pminrow,    // Upper left corner of the pad content to show - row
        int pmincol,    // Upper left corner of the pad content to show - column
        int sminrow,    // Begin window ROW
        int smincol,    // Begin window COL
        int smaxrow,    // End window ROW [0:ROWS-1]
        int smaxcol     // End window COL [0:COLS-1]
        );
    // smaxrow in smacol sta absolutni koordinati (0-index pazi!)   
    */

    if (prefresh(rmw, 0, 0, GLOBAL_PADDING + 2, MENU_WIDTH + GLOBAL_PADDING + 4, LINES- (STATUSBAR_HEIGHT + 1), COLS - (GLOBAL_PADDING+3)) == ERR)
    {
        printw("Error refreshing!\n");
        refresh();
    }

    return rmw;
}

WINDOW *create_rmw(ITEM *item)
{
    /* Generate README.md file pathname */
    const char *name = item_name(item);           // Name of the current selected hack
    char *fname = (char *)malloc(strlen(HACKS_DIR) + strlen(name) + strlen("README.md") + 3);
    if (fname == NULL)
    {
        printw("Can't malloc filename!\n");
        refresh();
    }
    strcpy(fname, HACKS_DIR);
    strcat(fname, "/");
    strcat(fname, name);
    strcat(fname, "/");
    strcat(fname, "README.md");

    /* Open a file */
    FILE *f = fopen(fname, "r");
    if (f==NULL) {
        printw("Can't open file!\n");
        refresh();
        while(1) {}
    }

    /* Allocate memory for the file contents */
    unsigned long int size;
    char *fbuf;

    fseek(f, 0, SEEK_END);      // seek to end of file
    size = ftell(f);            // get current file pointer
    rewind(f);                  // rewind to the beginning of the file

    fbuf = (char *)malloc(size);    // zakaj je v examplu tu +1? Kaj je z nullterminatorjem?
    if (fbuf == NULL) {
        printw("File malloc failed!\n");
        refresh();
    }

    /* Read file into the filebuffer */
    size_t nread = fread(fbuf, sizeof(char), size*sizeof(char), f);

    if (nread != size) {    // Check if number of successfully read bytes equals size of file
        printw("\nDidn't manage to read file contents!\n");
        refresh();
    }
    
    fclose(f);

    
    /* Find width and height */
    /* Count the number of lines - UGLY, TODO FIX */
    unsigned int cols = 180;
    unsigned int lines = 0;

    for(int i=0; i<size; i++) {
        if(fbuf[i] == '\n') lines++;
    }

    /* Check for empty files */
    if (lines == 0) {
        WINDOW *padwin = newpad(2, cols);
        wprintw(padwin, "Empty file!\n");
        return padwin;
    }

    /* Create pad (window) */
    WINDOW *padwin = newpad(lines + 2, cols);   // +2 lines for safety. Check log.md
    if (padwin == NULL) {
        printf("Can't create pad window!\n");
        refresh();
        while(1) {}
    }

    /* Read file contents into the pad */
    if (wprintw(padwin, fbuf) != OK) {
        printw("Can't write file contents into pad!\n");
        refresh();
    }


    return padwin;
}


/*
    Returns a menu with entries based on HACK_DIR subdirs
*/
MENU *create_hacks_menu()
{

    /* HACK_DIRECTORY PARSING -----------------------------------------------*/
    struct dirent **subdirs;

    int n = scandir(        // number of hacks/subdirs
            HACKS_DIR,      // path to directory
            &subdirs,       // pointer where to store directory entries
            dir_filter,     // pointer to a custom selector function
            alphasort       // pointer to a sorting function
        );

    if (n == 0) {           // Check for fails
        move(0, 0);
        clear();
        refresh();
        printw("No subdirs found :(\n");
        refresh();

        // TODO: TO IZPIŠI NEKAM DA BO VIDNO + POSKRBI DA NE CRASHA VSEENO
    }


    /* MENU ITEMS CREATION --------------------------------------------------*/
    ITEM **items = (ITEM**) calloc(n+1, sizeof(ITEM *));
    if (items == NULL)
    {
        printw("Memory allocation err");
        refresh();
    }

    for(int i=0; i<n; i++) {
        items[i] = new_item(subdirs[i]->d_name, "");
    }

    // Return MENU* 
    return new_menu(items);
}


/*
    Checks if current subdir is a HACK_repo or not
*/
int dir_filter(const struct dirent *d)
{
    char has_readme = 0;
    char is_hidden = 0;

    // Check if the directory contains README.md
    // First concatenate correct filepath
    char *fpath = (char *)malloc(strlen(HACKS_DIR) + strlen(d->d_name) + 2);
    if (fpath == NULL) {
        printw("Can't malloc filename!\n");
        refresh();
    }
    strcpy(fpath, HACKS_DIR);
    strcat(fpath, "/");
    strcat(fpath, d->d_name);

    // Open current hacks directory
    DIR *dir = opendir(fpath);
    if (dir == NULL) {
        printw("Can't open directory!\n");
        refresh();
    }

    // Scan the subdir for README.md
    struct dirent *dd;
    while((dd = readdir(dir)) != NULL) {
        if (strcmp(dd->d_name, "README.md") == 0) has_readme = 1;
    }

    // Skip hidden files/dirs
    if (d->d_name[0] == '.') is_hidden = 1;
    
    if (!has_readme || is_hidden) return 0;     // return 0 = skip folder
    else return 1;                              // return 1 = add folder to menu
}

int hacks_menu_handler(ITEM *cur, WINDOW *rmw)
{
    // TODO: Popup dialog
    return 0;
}

/* Print text on the bottom of the screen */
void print_info(const char *str)
{
    move(LINES - STATUSBAR_HEIGHT + STATUSBAR_PADDING + 2, 0);
    clrtoeol();
    mvprintw(LINES - STATUSBAR_HEIGHT + STATUSBAR_PADDING + 2, GLOBAL_PADDING, str);
    refresh();
}

void print_legend(void)
{
    char str[] = "Q = back \t W,A,S,D = scroll text \t\t O = open folder \t B = burn to floppy disc";

    move(LINES - STATUSBAR_HEIGHT + STATUSBAR_PADDING, 0);
    clrtoeol();
    mvprintw(LINES - STATUSBAR_HEIGHT + STATUSBAR_PADDING, GLOBAL_PADDING, str);
    refresh();
}