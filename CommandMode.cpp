#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"

#include <dirent.h>
#include <termios.h>

#include <sys/ioctl.h> 

#include <iostream>
#include <fstream>


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
		ind=get_command_token(ind);
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
	command_string="";

	decide_command();
}


void make_absolute_paths(){
	for(int i=1;i<actual_command.size();i++){  // find absolute path for strings starting from 1 since first is command 
		if(actual_command[i]=="."){
			actual_command[i]=current_directory.directory_path;
		}
		else if(actual_command[i]==".."){
			actual_command[i]=current_directory.parent_path;
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

void create_directory(){
	string path=actual_command[2];

	
	
	
	path=path+"/"+get_directory_name_from_path(actual_command[1]);
	/*char arr[4096];
	getcwd(arr,4096);

	movecursor(command_mode_top,command_mode_left);
	cout<<space_string;
	movecursor(command_mode_top,command_mode_left);
	cout<<arr;
	int x;
	cin>>x;
*/
	int status=mkdir(path.c_str(),0777);
	if(!status){
		cout<<"Directory created";
	}
	else{
		cout<<"Directory not created";
	}
}

void create_file(){
	string path=actual_command[2];
	path=path+"/"+get_directory_name_from_path(actual_command[1]);
	
	fstream file;
	file.open(path.c_str(), ios_base::out | ios_base::in);  // will not create file
	if (file.is_open()){
	    cout << "Warning, file already exists, proceed?";
	    file.close();
	}
	else{
	    file.clear();
	    file.open(path.c_str(), ios_base::out);  // will create if necessary
	    cout<<"file created";
	}
}

void move_to_directory(){
	string path=actual_command[1];

	char cwd[path.size() + 1];
	path.copy(cwd, path.size() + 1);
	cwd[path.size()] = '\0';
	DIR* dp;
	if ((dp = opendir(cwd)) == NULL){
	    //printf("can’t open %s\n", cwd);
		cout<<"can’t open "<<path<<"\n";
		return;
	}

	struct stat st;
    string directory_name=get_directory_name_from_path(path);
    string parent=find_parent_path(path);
    FileSystem dir(st,directory_name,path,parent,directory_name);	    
	initialize(dp,directory_name,dir,add_to_traversal_list);
	initiate_command_mode();
}

void copy_file(string file_path, string destination){
	FILE *src, *dest;
    src = fopen(file_path.c_str(), "r");
    fstream file;
	string path=destination+"/"+get_directory_name_from_path(file_path);
	file.open(path.c_str(), ios_base::out | ios_base::in);  // will not create file
	if (file.is_open()){
		cout <<get_directory_name_from_path(file_path) <<" file already exists";
		file.close();
		return;
	}
	else{
		file.clear();
	    file.open(path.c_str(), ios_base::out);  // will create if necessary
	}

    dest = fopen(path.c_str(), "w");
	char ch;
	while((ch = fgetc(src)) != EOF )
		fputc(ch, dest);
	fclose(src);
	fclose(dest);
}

void copy_file_directory(){
	string destination=actual_command[actual_command.size()-1];
	if(check_file_or_directory(destination) == does_not_exist){
		cout<<"destination does not exist";
		return;
	}
	for(int i=1;i<=actual_command.size()-2;i++){
	   int status=check_file_or_directory(actual_command[i]);
	   if(status == does_not_exist){
	   	cout<<actual_command[i]<<"does_not_exist";
	   }
	   else if(status == is_file){
		   copy_file(actual_command[i],destination);
	   }

	   else if(status == is_directory){

	   }	
	}
}

void decide_command(){
	string cmd=actual_command[0];
	make_absolute_paths();
	if(cmd == "copy"){
		copy_file_directory();
	}
	else if(cmd == "move"){

	}
	else if(cmd == "rename"){

	}
	else if(cmd == "create_dir"){
		create_directory();
	}
	else if(cmd == "create_file"){
		create_file();
	}
	else if(cmd == "goto"){
		move_to_directory();
	}
}

int get_command_token(int ind){
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