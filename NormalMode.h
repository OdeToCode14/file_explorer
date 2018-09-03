#ifndef NORMALMODE_H
#define NORMALMODE_H

#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"

#include <dirent.h>
#include <termios.h>

#include <sys/ioctl.h> // for terminal_dimensions

#define add_to_traversal_list 1
#define movecursor(x,y) printf("\033[%d;%dH", x,y);

extern int bottom;
extern int cursor_position;
extern int list_size;

extern int rows;
extern int cols;
              // leave two lines above one for application name other empty
              // leave three lines below one for cmd one above it one below
extern int top;

extern int display_index;

extern FileSystem current_directory;
extern vector<FileSystem> traversal_list;
extern int traversal_point;

void move_cursor_down();
void move_cursor_up();
void clear_screen();

void initialize(DIR *dp,string name,FileSystem current_dir,int add_or_not);

void display_list(vector<FileSystem> file_System_list);

void scroll_down();
void scroll_up();

//void display_list();

void terminal_dimensions(void);

void enter();

void move_right();

void move_left();

void move_to_parent();

void go_to_home();

void select_a_directory(FileSystem file_clicked,int add_or_not);
#endif