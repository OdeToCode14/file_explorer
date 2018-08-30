#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"

#include <dirent.h>
#include <termios.h>

#include <sys/ioctl.h> 


int command_mode_top=bottom+2;
int command_mode_bottom=rows;

int command_mode_left=1;
int command_mode_right=cols;

int cursor_row_in_command_mode=bottom+2;
int cursor_col_in_command_mode=1;

string space_string;

string command_string;
void initiate_command_mode(){
	command_mode_top=bottom+2;
    command_mode_bottom=rows;

    command_mode_left=1;
    command_mode_right=cols;
	cursor_row_in_command_mode=bottom+2;
	cursor_col_in_command_mode=1;
	movecursor(command_mode_top,command_mode_left);
	char ch=' ';
	space_string=string(cols,ch);
}
void move_cursor_up_in_command_mode(){
	if(cursor_row_in_command_mode == command_mode_top){
		return;
	}
	cursor_row_in_command_mode--;
	movecursor(cursor_row_in_command_mode,cursor_col_in_command_mode);
}
void move_cursor_down_in_command_mode(){
	if(cursor_row_in_command_mode == command_mode_bottom){
		return;
	}
	cursor_row_in_command_mode++;
	movecursor(cursor_row_in_command_mode,cursor_col_in_command_mode);
}

void move_cursor_left_in_command_mode(){
	if(cursor_col_in_command_mode == command_mode_left){
		return;
	}
	cursor_col_in_command_mode--;
	movecursor(cursor_row_in_command_mode,cursor_col_in_command_mode);
}
void move_cursor_right_in_command_mode(){
	if(cursor_col_in_command_mode == command_mode_right){
		return;
	}
	cursor_col_in_command_mode++;
	movecursor(cursor_row_in_command_mode,cursor_col_in_command_mode);
}

void add_character_to_command(int c){
	putchar(c);
	move_cursor_right_in_command_mode();
	command_string+=c;
}

void backspace_pressed(){
	if(cursor_col_in_command_mode == command_mode_left){
		return;
	}
	string changed=command_string.substr(0, cursor_col_in_command_mode-2) + command_string.substr(cursor_col_in_command_mode-1);
	command_string=changed;
	cursor_col_in_command_mode--;
	refresh_command_string(changed);
}

void refresh_command_string(string changed){
	movecursor(command_mode_top,command_mode_left);
	cout<<space_string;
	movecursor(command_mode_top,command_mode_left);
	cout<<changed;
	movecursor(command_mode_top,cursor_col_in_command_mode);
}

void enter_function_of_command_mode(){
	
}