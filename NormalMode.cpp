
#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"

#include <dirent.h>
#include <termios.h>
#include <stdlib.h>
#include <sys/ioctl.h> // for terminal_dimensions





int list_size;

int rows;
int cols;
              // leave two lines above one for application name other empty
              // leave three lines below one for cmd one above it one below
int top;
int bottom; //since first line for application display and last 2 lines for command mode
int cursor_position;
int display_index;  //stores start of the list from where files have been displayed
struct winsize terminal_size;  // storing dimensions of terminal

string root;

vector<FileSystem> file_system_list;
FileSystem current_directory;

vector<FileSystem> traversal_list;
int traversal_point=-1;

void move_cursor_down(){  // handling cursor down movement in normal mode and scrolling down
	if(display_index + cursor_position-top + 1 == file_system_list.size()){ // if there is nothing to scroll below
		return;
	}
	cursor_position++;
	if(cursor_position!=bottom+1){
		movecursor(cursor_position,0);
	}
	else{
		scroll_down();
	}
}
void move_cursor_up(){  // handling cursor up movement in normal mode and scrolling up
	cursor_position--;
	if(cursor_position != top-1){
		movecursor(cursor_position,0);
	}
	else{
		scroll_up();
	}
}

void clear_screen(){  // refreshing normal mode screen
    printf("\033[H");
    printf("\033[J");
    movecursor(bottom+1,1);
    char ch='-';
    string str=string(terminal_size.ws_col, ch);
    cout<<str;
    movecursor(1,1);
}


void reduce_list(){   // reducing traversal list when enter is pressed or backspace is pressed or goto command is used
	for(int i=traversal_list.size()-1;i>traversal_point;i--){
		traversal_list.pop_back();
	}
}

void initialize(DIR *dp,string name,FileSystem current_dir,int add_or_not){   // to create list of current folder
	if(add_or_not){
		reduce_list();
		traversal_list.push_back(current_dir); // to keep record of traversal sequence
		traversal_point++;
	}
    
    root=name;	
    current_directory=current_dir;
	terminal_dimensions();

	rows=terminal_size.ws_row;
	cols=terminal_size.ws_col;
	clear_screen();  //refresh screen everytime
	file_system_list=create_list(dp,current_directory);

    cout<<"Current directory: "<<root<<"\n";  // display name of current directory
	top=3;
	bottom=rows-4; //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;  // cursor position maintains record of where cursor is presently on screen
	
	display_index=0;

	display_list(file_system_list);
}

void scroll_down(){  //for scrolling down
	if(display_index+bottom-top >= file_system_list.size()){
		movecursor(top,1);
		cursor_position=top;
		return;
	}
	clear_screen();
    cout<<"Current directory: "<<root<<"\n";
	 //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;
	
	display_index++;
	if(display_index < file_system_list.size()){
		display_list(file_system_list);
	}
}

void scroll_up(){  //for scrolling up
	if(display_index == 0){
		movecursor(top,1);
		cursor_position=top;
		return;
	}
	clear_screen();
    cout<<"Current directory: "<<root<<"\n";
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


void display_list(vector<FileSystem> file_system_list){  //display current fitting list in terminal
	clear_screen();
	cout<<"Current working dir: "<<root<<"\n";
	 //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;
	  int size=file_system_list.size();
      bool displayed_something=false;
      for(int i=display_index; i<size && cursor_position<=bottom ;i++){
      	displayed_something=true;
      	movecursor(cursor_position,1);
      	file_system_list[i].display();
      	cursor_position++;
      }
      if(displayed_something == true){
      	cursor_position--;
      	movecursor(cursor_position,1);
      }
}

void terminal_dimensions(void){  
    ioctl(0, TIOCGWINSZ, &terminal_size);
}


void select_a_directory(FileSystem file_clicked,int add_or_not){ //code for entering into directory 
	if(file_clicked.type == is_directory){
		if(file_clicked.directory_path.length() < home_directory.directory_path.length()){
			return;
		}
		string path=file_clicked.directory_path;
		string real_name=file_clicked.file_name;

		if(file_clicked.file_name == "."){  // ignore if this is entered
			return;
		}
		else if(file_clicked.file_name == ".."){  // find absolute path of parent
			string real_path=find_directory_for_special_dot(path);
			path=real_path;
			real_name=get_directory_name_from_path(real_path);
			file_clicked.file_name=real_name;
			file_clicked.directory_path=real_path;
			file_clicked.parent_path=find_parent_path(real_path);
		}
        
		char cwd[path.size() + 1];
		path.copy(cwd, path.size() + 1);
		cwd[path.size()] = '\0';
		DIR* dp;
		if ((dp = opendir(cwd)) == NULL){
		    cout<<"can’t open "<<path<<"\n";
		    return;
		}   
		initialize(dp,real_name,file_clicked,add_or_not);
	}
}



void enter(){  // called when enter is pressed in normal mode opens directory and file
	int pos=display_index+cursor_position-top;
	FileSystem file_clicked=file_system_list[pos];
	if(file_clicked.type == is_directory){
		select_a_directory(file_clicked,add_to_traversal_list);
	}
	else{ 
		int pid = fork();
		if (pid == 0) {
			execl("/usr/bin/xdg-open", "xdg-open", file_clicked.directory_path.c_str(), (char *)NULL);
			exit(1);
		}		
	}
}


void move_right(){  // handling next traversal
	if(traversal_point == traversal_list.size()-1){
		return;
	}
	traversal_point++;
	select_a_directory(traversal_list[traversal_point],0);
}

void move_left(){  // handling previous traversal
	if(traversal_point == 0){
		return;
	}
	traversal_point--;
	select_a_directory(traversal_list[traversal_point],0);
}

void move_to_parent(){  // traverse to parent of current folder
	for(FileSystem obj : traversal_list){
		if(obj.directory_path == current_directory.parent_path){
			select_a_directory(obj,add_to_traversal_list);
			break;
		}
	}
}
void go_to_home(){  // jump to home directory
	select_a_directory(home_directory,add_to_traversal_list);
}