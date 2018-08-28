
#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"

#include <dirent.h>
#include <termios.h>

#include <sys/ioctl.h> // for terminal_dimensions

#define movecursor(x,y) printf("\033[%d;%dH", x,y);

int list_size;

int rows;
              // leave two lines above one for application name other empty
              // leave three lines below one for cmd one above it one below
int top;
int bottom; //since first line for application display and last 2 lines for comd mode
int cursor_position;
int display_index;
struct winsize terminal_size;

string root;

vector<FileSystem> file_system_list;


void move_cursor_down(){
	cursor_position++;
	if(cursor_position!=bottom+1){
		movecursor(cursor_position,0);
	}
	else{
		scroll_down();
	}
}
void move_cursor_up(){
	cursor_position--;
	if(cursor_position != top-1){
		movecursor(cursor_position,0);
	}
	else{
		scroll_up();
	}
}

void clear_screen(){
    printf("\033[H");
    printf("\033[J");
}

void initialize(DIR *dp,string name){
    root=name;	
	terminal_dimensions();
	//list_size=file_system_list.size();

	rows=terminal_size.ws_row;
              // leave two lines above one for application name other empty
              // leave three lines below one for cmd one above it one below
	clear_screen();
	file_system_list=create_list(dp);

    cout<<"Current working dir: "<<root<<"\n";
	top=3;
	bottom=rows-3; //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;
	
	display_index=0;

	display_list(file_system_list);
	movecursor(top,1);
	cursor_position=top;
}

void scroll_down(){
	if(display_index+bottom-top >= file_system_list.size()){
		movecursor(top,1);
		cursor_position=top;
		return;
	}
	clear_screen();
    cout<<"Current working dir: "<<root<<"\n";
	 //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;
	
	display_index++;
	if(display_index < file_system_list.size()){
		display_list(file_system_list);
	}
	movecursor(top,1);
	cursor_position=top;
}

void scroll_up(){
	if(display_index == 0){
		movecursor(top,1);
		cursor_position=top;
		return;
	}
	clear_screen();
    cout<<"Current working dir: "<<root<<"\n";
	 //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;
	
	display_index--;
	if(display_index>=0){
		display_list(file_system_list);
	}
	movecursor(top,1);
	cursor_position=top;
}


void display_list(vector<FileSystem> file_system_list){
	  int size=file_system_list.size();
      for(int i=display_index; i<size && cursor_position<=bottom ;i++){
      	movecursor(cursor_position,1);
      	file_system_list[i].display();
      	cursor_position++;
      }
}

void open_folder(DIR *dp){
	create_list(dp);
}

/*
void display_list(){      

}
*/
void terminal_dimensions(void){  // https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
    ioctl(0, TIOCGWINSZ, &terminal_size);

    printf ("lines %d\n", terminal_size.ws_row);
    printf ("columns %d\n", terminal_size.ws_col);
    
}