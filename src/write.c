/*
    BIOS_device
    write.c

    BIOS_DEVICE write a hack screen
*/
#include "write.h"


int write_screen(void)
{

    WINDOW *write_screen = newwin(0, 0, 0, 0);
    mvwprintw(write_screen, LINES-2, 2, "Q = back");
    wrefresh(write_screen);

    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    init_pair(2, COLOR_RED, COLOR_GREEN);


    /* WINDOW INITIALIZATION */
    int fw_h = 20;
    int fw_w = 60;
    int fw_y = (LINES - fw_h) / 2;
    int fw_x = (COLS - fw_w) / 2;
    WINDOW *formwin = newwin(fw_h, fw_w, fw_y, fw_x);
    // wbkgd(formwin, COLOR_PAIR(1));
    wborder(formwin, '0', '0', '#', '#', '#', '#', '#', '#');
    wrefresh(formwin);

    int fs_w = fw_w - 20;
    int fs_h = fw_h - 12;
    WINDOW *formsub = derwin(formwin, fs_h, fs_w, (fw_h - fs_h)/2, (fw_w - fs_w)/2);   // nlines, ncols, y, x
    // wbkgd(formsub, COLOR_PAIR(2));

    // Title
    char *title = "WRITE A HACK!";
    mvwprintw(formwin, 3, (fw_w - strlen(title)) / 2, title);
    wrefresh(formsub);


    /* FORM INITIALIZATION */
    int n_flds = 2;
    FIELD **flds = (FIELD **) malloc(n_flds+1 * sizeof(FIELD *));
    flds[n_flds] = NULL;                                            /* Array must be null terminated */

    // Label 1      (height, width, y, x, 0, 0)
    flds[0] = new_field(1, fs_w, 2, 0, 0, 0);
    set_field_buffer(flds[0], 0, "Enter your hack name: ");
    field_opts_off(flds[0], O_ACTIVE);
    set_field_just(flds[0], JUSTIFY_CENTER);                        /* Center Justification */

    // Input 1 (hack name)
    int fld_width = 20;
    flds[1] = new_field(1, fld_width, 4, (fs_w-fld_width)/2, 0, 0);
    set_field_back(flds[1], A_UNDERLINE);                           /* Print a line for the option 	*/
    

    FORM *form = new_form(flds);
    if (form == NULL) {
        printw("newform failed!");
        refresh();
        sleep(2);
    }

    // Should I error check any of theese?
    int min_h, min_w;
    scale_form(form, &min_h, &min_w);   // h=3, w=22
    set_form_win(form, formwin);
    set_form_sub(form, formsub);
    post_form(form);
    
    curs_set(1);
    wrefresh(formwin);


    /* MAIN LOOP */
    char *name_buff = (char *)malloc(30 * sizeof(char));
    int c;
    while((c = wgetch(formwin)) != 'q') {
        switch(c) {

            case 10:    /* Enter */
                // Read form input into name_buff
                form_driver(form, REQ_END_LINE);                        // field_buffer syncs only when leaving the field or requesting validation
                trimwhitespace(name_buff, 30, field_buffer(flds[1], 0));

                // Create new project
                new_project(name_buff, formwin);

                // Clear screen
                unpost_form(form);
                wclear(formwin);
                wrefresh(formwin);
                clear();

                // Open editor @project folder
                open_project(name_buff);

                // Cleanup & exit
                unpost_form(form);
                free_field(flds[0]);
                free_field(flds[1]);
                free(flds);
                delwin(formsub);
                delwin(formwin);
                delwin(write_screen);
                curs_set(0);

                return 0;

            case 127:   /* Backspace */
                form_driver(form, REQ_DEL_PREV);
                break;

            default:    /* Echo to screen */
                form_driver(form, c);
                break;
        }
        wrefresh(formwin);
    }

    // Shouldn't reach this
    return -1;
}


/* Start a scritp to create new project skeleton and run editor */
int new_project(char *name, WINDOW *win)
{

    // Poslat morem to komando:
    // /bin/bash $SKELETON_DIR/scriptname.sh name $HACKS_DIR $SKELETON_DIR

    char *a = "/bin/bash ";
    char *b = "new_project.sh ";
    char *cmd = (char *) malloc(strlen(a)+strlen(SKELETON_DIR)+ 1 +strlen(b)+ 1 +strlen(name)+ 1 +strlen(HACKS_DIR)+ 1 +strlen(SKELETON_DIR)+ 1);
    strcpy(cmd, a);
    strcat(cmd, SKELETON_DIR);
    strcat(cmd, "/");
    strcat(cmd, b);
    strcat(cmd, " ");
    strcat(cmd, name);
    strcat(cmd, " ");
    strcat(cmd, HACKS_DIR);
    strcat(cmd, " ");
    strcat(cmd, SKELETON_DIR);

    // mvprintw(0, 0, cmd);// PRINT THIS @statusbar
    // refresh();

    /* CHECKING SCRIPT RETURN CODE */
    // system() return some special kind of encoded exit status, so this macro gives the thing we need (actual return code)
    int r = WEXITSTATUS(system(cmd));
    switch(r) {

        case 0:
            // OK
            break;

        case 1:
            // Invalid arguments -> error message
            mvwprintw(win, 0, 0, "Invalid arguments!");
            wrefresh(win);
            break;
        
        case 2:
            // Can't open HACKS_DIR
            mvwprintw(win, 0, 0, "Can't open HACKS_DIR");
            wrefresh(win);
            break;

        case 3:
            // Can't create new directory
            mvwprintw(win, 0, 0, "Can't create new directory");
            wrefresh(win);
            break;

        default:
            mvwprintw(win, 0, 0, "Dafuck just happened?");
            wrefresh(win);
            break;
    }
    
    return 0;
}



/* Opens an editor within newly created project */
int open_project(char *name)
{

    // Print loading message
    char *editor_msg = "Loading editor!";
    mvprintw(LINES / 2, (COLS - strlen(editor_msg)) / 2, editor_msg);
    refresh();

    // Poslat morem to komando:
    // atom HACKS_DIR/name HACKS_DIR/name/README.md
    char *a = "atom ";
    char *b = "README.md";
    char *cmd = (char *) malloc(strlen(a) + strlen(HACKS_DIR) + 1 + strlen(name) + strlen(HACKS_DIR) + 1 + strlen(name) + 1 + strlen(b) + 1);
    strcpy(cmd, a);
    strcat(cmd, HACKS_DIR);
    strcat(cmd, "/");
    strcat(cmd, name);
    strcat(cmd, " ");
    strcat(cmd, HACKS_DIR);
    strcat(cmd, "/");
    strcat(cmd, name);
    strcat(cmd, "/");
    strcat(cmd, b);

    // Execute command & check for failure
    int r = system(cmd);
    if (r != 0) {
        printw("Can't open editor");
        refresh();
        sleep(5);
        return 1;
    }

    sleep(5);       // Sleep to be able to read the loading screen

    return 0;
}


/* Copied from stackoverflow */
// Stores the trimmed input string into the given output buffer, which must be
// large enough to store the result.  If it is too small, the output is
// truncated.
size_t trimwhitespace(char *out, size_t len, const char *str)
{
    if (len == 0)
        return 0;

    const char *end;
    size_t out_size;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
    {
        *out = 0;
        return 1;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end++;

    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (end - str) < len - 1 ? (end - str) : len - 1;

    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
}