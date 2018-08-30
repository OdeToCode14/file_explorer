
#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"

#include <dirent.h>
#include <termios.h>
#include <stdlib.h>
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
FileSystem current_directory;

vector<FileSystem> traversal_list;
int traversal_point=-1;

void move_cursor_down(){
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
    movecursor(bottom+1,1);
    char ch='-';
    string str=string(terminal_size.ws_col, ch);
    cout<<str;
    movecursor(1,1);
}


void reduce_list(){
	for(int i=traversal_list.size()-1;i>traversal_point;i--){
		traversal_list.pop_back();
	}
}

void initialize(DIR *dp,string name,FileSystem current_dir,int add_or_not){
	if(add_or_not){
		reduce_list();
		traversal_list.push_back(current_dir); // to keep record of traversal sequence
		traversal_point++;
	}
    
    root=name;	
    current_directory=current_dir;
	terminal_dimensions();
	//list_size=file_system_list.size();

	rows=terminal_size.ws_row;
              // leave two lines above one for application name other empty
              // leave three lines below one for cmd one above it one below
	clear_screen();
	file_system_list=create_list(dp,current_directory);

    cout<<"Current working dir: "<<root<<"\n";
	top=3;
	bottom=rows-3; //since first line for application display and last 2 lines for comd mode
	movecursor(top,1);
	cursor_position=top;
	
	display_index=0;

	display_list(file_system_list);
	//movecursor(top,1);   //change this
	//cursor_position=top;  //this also
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
	//movecursor(top,1);
	//cursor_position=top;
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

/*
void display_list(){      

}
*/
void terminal_dimensions(void){  // https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
    ioctl(0, TIOCGWINSZ, &terminal_size);

    printf ("lines %d\n", terminal_size.ws_row);
    printf ("columns %d\n", terminal_size.ws_col);
    
}


//code for entering into directory


void select_a_directory(FileSystem file_clicked,int add_or_not){
	if(file_clicked.type == is_directory){
		if(file_clicked.directory_path.length() < home_directory.directory_path.length()){
			return;
		}
		string path=file_clicked.directory_path;
		string real_name=file_clicked.file_name;

		if(file_clicked.file_name == "."){
			return;
		}
		else if(file_clicked.file_name == ".."){
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
		    //printf("can’t open %s\n", cwd);
		    cout<<"can’t open "<<path<<"\n";
		    return;
		}
		    
		initialize(dp,real_name,file_clicked,add_or_not);
	}
}



void enter(){

	
	int pos=display_index+cursor_position-top;
	FileSystem file_clicked=file_system_list[pos];
	if(file_clicked.type == is_directory){
		select_a_directory(file_clicked,add_to_traversal_list);
	}
	else{ // code for opening file
		 string inp="​xdg-open "+file_clicked.directory_path;
		  char t1[1000];
		  strcpy(t1,inp.c_str());
		  system(t1);
		  
              /*
		       int pid = fork();
				if (pid == 0) {
				  execl("/usr/bin/xdg-open", "xdg-open", file_clicked.directory_path, (char *)0);
				  exit(1);
				}
				*/
	}
}


void move_right(){
	if(traversal_point == traversal_list.size()-1){
		return;
	}
	traversal_point++;
	select_a_directory(traversal_list[traversal_point],0);
}

void move_left(){
	if(traversal_point == 0){
		return;
	}
	traversal_point--;
	select_a_directory(traversal_list[traversal_point],0);	
}

void move_to_parent(){
	for(FileSystem obj : traversal_list){
		if(obj.directory_path == current_directory.parent_path){
			select_a_directory(obj,add_to_traversal_list);
			break;
		}
	}
}
void go_to_home(){
	select_a_directory(home_directory,add_to_traversal_list);
}