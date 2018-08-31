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

int max_cursor_col_position_in_command_mode;

vector<string> actual_command;// clear it ever time

string space_string;

string command_string;
void initiate_command_mode(){
	command_mode_top=bottom+2;
    command_mode_bottom=rows;

    command_mode_left=1;
    command_mode_right=cols;
	cursor_row_in_command_mode=bottom+2;
	cursor_col_in_command_mode=1;
	max_cursor_col_position_in_command_mode=1;
	movecursor(command_mode_top,command_mode_left);
	char ch=' ';
	command_string="";
	space_string=string(cols,ch);
	refresh_command_string(command_string);
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
	if(cursor_col_in_command_mode == command_mode_right || cursor_col_in_command_mode == max_cursor_col_position_in_command_mode){
		return;
	}
	cursor_col_in_command_mode++;
	movecursor(cursor_row_in_command_mode,cursor_col_in_command_mode);
}

void add_character_to_command(int c){
	putchar(c);
	if(cursor_col_in_command_mode == max_cursor_col_position_in_command_mode){
		max_cursor_col_position_in_command_mode++;
	}
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
	max_cursor_col_position_in_command_mode--;
	refresh_command_string(changed);
}

void refresh_command_string(string changed){
	movecursor(command_mode_top,command_mode_left);
	cout<<space_string;
	movecursor(command_mode_top,command_mode_left);
	cout<<changed;
	movecursor(command_mode_top,cursor_col_in_command_mode);
}

int enter_function_of_command_mode(){
	actual_command.clear();
	int ind=0;
	while(ind<command_string.length()){
		ind=getCommandToken(ind);
		if(ind == -1){
			break;
		}
	}
	if(ind == -1 || actual_command.size()==0){
		movecursor(command_mode_top,command_mode_left);
		cout<<space_string;
		movecursor(command_mode_top,command_mode_left);
		cout<<"command format not correct";
		movecursor(command_mode_top,command_mode_left);
	}
	movecursor(command_mode_top,command_mode_left);
	cout<<space_string;
	movecursor(command_mode_top,command_mode_left);

	decide_command();
}


void make_absolute_paths(){
	for(int i=1;i<actual_command.size();i++){  // find absolute path for strings starting from 1 since first is command 
		if(actual_command[i]=="."){
			actual_command[i]=current_directory.directory_path;
		}
		
		else if(actual_command[i] == "/" || actual_command[i] == "~"){
			actual_command[i]=home_directory.directory_path;
		}
		else if(actual_command[i][0]=='/'){
			actual_command[i]=home_directory.directory_path+"/"+actual_command[i].substr(1);
		}
		else if(actual_command[i][0]=='~' && actual_command[i][1]=='/'){
			actual_command[i]=home_directory.directory_path+"/"+actual_command[i].substr(2);
		}
		else if(actual_command[i][0]=='.' && actual_command[i][1]=='/'){
		    actual_command[i]=current_directory.directory_path+"/"+actual_command[i].substr(2);	
		}
		else{
			actual_command[i]=current_directory.directory_path+"/"+actual_command[i];
		}
		
		if(actual_command[i][actual_command[i].length()-1]=='/'){
			actual_command[i]=actual_command[i].substr(0,actual_command[i].length()-1);
		}
	}
}

void createDirectory(){
	string path=actual_command[2];

	
	
	chdir(path.c_str());
	/*char arr[4096];
	getcwd(arr,4096);

	movecursor(command_mode_top,command_mode_left);
	cout<<space_string;
	movecursor(command_mode_top,command_mode_left);
	cout<<arr;
	int x;
	cin>>x;
*/
	int status=mkdir(get_directory_name_from_path(actual_command[1]).c_str(),0777);
	if(!status){
		cout<<"created";
	}
	else{
		cout<<"not created";
	}
}
void decide_command(){
	string cmd=actual_command[0];
	make_absolute_paths();
	if(cmd == "copy"){

	}
	else if(cmd == "move"){

	}
	else if(cmd == "rename"){

	}
	else if(cmd == "create_dir"){
		createDirectory();
	}
	else if(cmd == "create_file"){
		
	}
}

int getCommandToken(int ind){
	string res="";

	if(command_string[ind] == '\''){
			int i;
			for(i=ind+1;i<command_string.length();i++){
				if(command_string[i]=='\''){
					break;
				}
			}
			if(i==command_string.length()){
				return -1;
			}
			if(i+1 < command_string.length() && command_string[i+1]!=' '){
				return -1;
			}
			else{
				res=command_string.substr(ind+1,i-ind-1);
				actual_command.push_back(res);
			}
			ind=i+1;
			return ind;
    }
    if(command_string[ind] == '\"'){
			int i;
			for(i=ind+1;i<command_string.length();i++){
				if(command_string[i]=='\"'){
					break;
				}
			}
			if(i==command_string.length()){
				return -1;
			}
			if(i+1 < command_string.length() && command_string[i+1]!=' '){
				return -1;
			}
			else{
				res=command_string.substr(ind+1,i-ind-1);
				actual_command.push_back(res);
			}
			ind=i+1;
			return ind;
    }
	while(ind<command_string.length()){
		if(command_string[ind] == '\\'){
			if(ind+1 == command_string.length()){
				return -1;
			}
			else{
				res=res+command_string[ind+1];
				ind++;
			}
		}
		else if(command_string[ind] == ' '){
			ind++;
			break;
		}

		else{
			res+=command_string[ind];
		}
		ind++;
	}

	actual_command.push_back(res);
	/*movecursor(command_mode_top,command_mode_left);
	cout<<space_string;
	movecursor(command_mode_top,command_mode_left);
	cout<<"this is"<<actual_command[0];
	int x;
	cin>>x;*/
	return ind;
}