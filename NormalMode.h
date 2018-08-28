#ifndef NORMALMODE_H
#define NORMALMODE_H

#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"

#include <dirent.h>
#include <termios.h>

#include <sys/ioctl.h> // for terminal_dimensions
/*
int list_size;

int rows;
              // leave two lines above one for application name other empty
              // leave three lines below one for cmd one above it one below
int top;
int bottom; //since first line for application display and last 2 lines for comd mode
int cursor_position;
*/

void move_cursor_down();
void move_cursor_up();
void clear_screen();

void initialize(DIR *dp,string root);

void display_list(vector<FileSystem> file_System_list);

void open_folder(DIR *dp);
void scroll_down();
void scroll_up();

//void display_list();

void terminal_dimensions(void);


#endif