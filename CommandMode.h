#ifndef COMMANDMODE_H
#define COMMANDMODE_H

#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"

#include <dirent.h>
#include <termios.h>

#include <sys/ioctl.h> 


extern int command_mode_top;
extern int command_mode_bottom;

extern int command_mode_left;
extern int command_mode_right;

extern int max_cursor_col_position_in_command_mode;

extern string command_string;
extern string space_string;

extern int cursor_row_in_command_mode;
extern int cursor_col_in_command_mode;

void initiate_command_mode();
void add_character_to_command(int c);

int enter_function_of_command_mode();
void move_cursor_up_in_command_mode();
void move_cursor_down_in_command_mode();

void move_cursor_left_in_command_mode();
void move_cursor_right_in_command_mode();

void backspace_pressed();

void refresh_command_string(string changed);
int getCommandToken(int ind);

void decide_command();

void make_absolute_paths();

void createDirectory();


#endif