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
int get_command_token(int ind);

void decide_command();

void make_absolute_paths();

int create_directory(string source,string destination);
int create_file(string file_path,string destination);

int move_to_directory();

int copy_file_directory();

int copy_file(string file_path, string destination);
int copy_directory(string directory_path,string destination);
int move_file_directory();

int rename_file(string old_name_path,string new_name_path);

int delete_file(string file_path);

int delete_directory(string directory_path);

int search_file(string path);

int search_recur(string directory_path,string search_name,vector<FileSystem> &search_list);
void display_search_list(vector<FileSystem> &search_list);

int generate_snapshot(string folder_path,string directory_path,string file_path);
int generate_recur(string folder_path,string directory_path,FILE *file);

void print_error(string error);

#endif