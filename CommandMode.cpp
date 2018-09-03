#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"
#include "console.h"

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

bool search_activated=false;
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

int create_directory(string source,string destination){
	string path=destination+"/"+get_directory_name_from_path(source);
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
	if(!status){  //created sucessfully
		return 1;
	}
	else{    //not created sucessfull
		return 0;
	}
}

int create_file(string file_path,string destination){
	string path=destination;
	path=path+"/"+get_directory_name_from_path(file_path);
	
	fstream file;
	file.open(path.c_str(), ios_base::out | ios_base::in);  // will not create file
	if (file.is_open()){
	    file.close();
	    return 0;
	}
	else{
	    file.clear();
	    file.open(path.c_str(), ios_base::out);
	    return 1;
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
    FileSystem direct(st,directory_name,path,parent,directory_name);
    direct.type=is_directory;	    
	initialize(dp,directory_name,direct,add_to_traversal_list);
	initiate_command_mode();
}

int copy_file(string file_path, string destination){
	FILE *src, *dest;
    src = fopen(file_path.c_str(), "r");
    fstream file;
	string path=destination+"/"+get_directory_name_from_path(file_path);
	file.open(path.c_str(), ios_base::out | ios_base::in);  // will not create file
	if (file.is_open()){
		file.close();
		return -1;
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
	return 1;
}


int copy_directory(string directory_path,string destination){
	create_directory(directory_path,destination);
	destination = destination+"/"+get_directory_name_from_path(directory_path);
	char cwd[directory_path.size() + 1];
	directory_path.copy(cwd, directory_path.size() + 1);
	cwd[directory_path.size()] = '\0';
	DIR* dp;
	if ((dp = opendir(cwd)) == NULL){
		return 0;
	}

	struct dirent *dirp;
      
      
      while ((dirp = readdir(dp)) != NULL){
            string file_name=dirp->d_name;
            string path_name=directory_path+"/"+file_name;
            struct stat st;
            //file_name.c_str()

            if(stat(path_name.c_str(), &st) != 0) {
            	return -1;
            }
            
            if(S_ISDIR(st.st_mode)){

                 if(file_name == "." || file_name == ".."){
                 	continue;
                 }
                 copy_directory(path_name,destination);
            }
            else{
            	create_file(path_name,destination);
            }
    }
	return 1;
}

int copy_file_directory(){
	string destination=actual_command[actual_command.size()-1];
	if(check_file_or_directory(destination) == does_not_exist){
		return 0;
	}
	for(int i=1;i<=actual_command.size()-2;i++){
	   int status=check_file_or_directory(actual_command[i]);
	   if(status == does_not_exist){
	   	return 0;
	   }
	   else if(status == is_file){
		   copy_file(actual_command[i],destination);
	   }

	   else if(status == is_directory){
	   	copy_directory(actual_command[i],destination);
	   }	
	}
	return 1;
}

int rename_file(string old_name_path,string new_name_path){
	int value = rename(old_name_path.c_str(), new_name_path.c_str());
    if(!value){
    	return 1;
    }
    else{
        return 0;
    }
}

int move_file_directory(){
	string destination=actual_command[actual_command.size()-1];
	for(int i=1;i<=actual_command.size()-2;i++){
		int status=check_file_or_directory(actual_command[i]);
		if(status == does_not_exist){
			return 0;
		}
		else{
			rename_file(actual_command[i],destination+"/"+get_directory_name_from_path(actual_command[i]));
		}
	}
	return 1;
}

int delete_file(string file_path){
	if (remove(file_path.c_str()) == 0)
		return 1;
   	else
   		return 0;

}
int delete_directory(string directory_path){
	char cwd[directory_path.size() + 1];
	directory_path.copy(cwd, directory_path.size() + 1);
	cwd[directory_path.size()] = '\0';
	DIR* dp;
	if ((dp = opendir(cwd)) == NULL){
		return 0;
	}

	struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL){
            string file_name=dirp->d_name;
            string path_name=directory_path+"/"+file_name;
            struct stat st;
            if(stat(path_name.c_str(), &st) != 0) {
            	return -1;
            }
            if(S_ISDIR(st.st_mode)){
                 if(file_name == "." || file_name == ".."){
                 	continue;
                 }
                 delete_directory(path_name);
            }
            else{
            	delete_file(path_name);
            }
    }
    rmdir(directory_path.c_str());
}

void display_search_list(vector<FileSystem> &search_list){
	clear_screen();
	cout<<"Following are the search results . Press left arrow key to go to previous folder\n";
	cout<<"\n";
	for(int i=0;i<search_list.size();i++){
		cout<<i+1<<". "<<search_list[i].directory_path<<"\n";
	}
	cout<<"\npress left arrow key to go to previous folder";
	return;
}

int search_file(string path){
	string parent_path=find_parent_path(path);
	string file_name=get_directory_name_from_path(path);
	vector<FileSystem> search_list;
	search_recur(parent_path,file_name,search_list);
	if(search_list.size() == 0){
		return 0;
	}
	search_activated=true;
	display_search_list(search_list);
	return 1;
}

int search_recur(string directory_path,string search_name,vector<FileSystem> &search_list){
	char cwd[directory_path.size() + 1];
	directory_path.copy(cwd, directory_path.size() + 1);
	cwd[directory_path.size()] = '\0';
	DIR* dp;
	if ((dp = opendir(cwd)) == NULL){
		return 0;
	}

	struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL){
            string file_name=dirp->d_name;
            string path_name=directory_path+"/"+file_name;
            struct stat st;
            if(stat(path_name.c_str(), &st) != 0) {
            	return -1;
            }
            if(S_ISDIR(st.st_mode)){
                 if(file_name == "." || file_name == ".."){
                 	continue;
                 }
                 search_recur(path_name,search_name,search_list);
            }
            else{
            	if(strstr(file_name.c_str(),search_name.c_str())){
            		FileSystem obj(st,file_name,path_name,directory_path,file_name);
                  	obj.type=is_file;
                  	search_list.push_back(obj);
				}
            }
    }
}

int generate_snapshot(string folder_path,string directory_path,string file_path){
	fstream fp;
	fp.open(file_path.c_str(), ios_base::out | ios_base::in);  // create file if does not exist
	if (fp.is_open()){
	    fp.close();
	}
	else{
	    fp.clear();
	    fp.open(file_path.c_str(), ios_base::out);
	}
	FILE * file;
	file = fopen(file_path.c_str(), "w");
	generate_recur(folder_path,directory_path,file);
	fclose(file);
	return 1;	
}

int generate_recur(string folder_path,string directory_path,FILE *file){
	string display_path="." + directory_path.substr(folder_path.length()) + ":";
	bool flag=false;
	fprintf(file,"%s\n", display_path.c_str());

	char cwd[directory_path.size() + 1];
	directory_path.copy(cwd, directory_path.size() + 1);
	cwd[directory_path.size()] = '\0';
	DIR* dp;
	if ((dp = opendir(cwd)) == NULL){
		return 0;
	}

	vector<string> child_paths;
	struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL){
            string file_name=dirp->d_name;
            string path_name=directory_path+"/"+file_name;
            struct stat st;
            if(stat(path_name.c_str(), &st) != 0) {
            	return -1;
            }
            if(S_ISDIR(st.st_mode)){
                 if(file_name == "." || file_name == ".."){
                 	continue;
                 }
                 fprintf(file,"%s\t", file_name.c_str());
                 child_paths.push_back(path_name);
                 flag=true;
            }
            else{
            	fprintf(file,"%s\t", file_name.c_str());
            	flag=true;
            }
    }
    fprintf(file,"\n");
    if(flag == true)
    	fprintf(file,"\n");
    for(int i=0;i<child_paths.size();i++){
    	generate_recur(folder_path,child_paths[i],file);
    }
    return 1;
}

void decide_command(){
	string cmd=actual_command[0];
	make_absolute_paths();
	if(cmd == "copy"){
		copy_file_directory();
	}
	else if(cmd == "move"){
		move_file_directory();
	}
	else if(cmd == "rename"){
		rename_file(actual_command[1],actual_command[2]);
	}
	else if(cmd == "create_dir"){
		create_directory(actual_command[1],actual_command[2]);
	}
	else if(cmd == "create_file"){
		create_file(actual_command[1],actual_command[2]);
	}
	else if(cmd == "delete_file"){
		delete_file(actual_command[1]);
	}
	else if(cmd == "delete_dir"){
		delete_directory(actual_command[1]);
	}
	else if(cmd == "goto"){
		move_to_directory();
	}
	else if(cmd == "search"){
		search_file(actual_command[1]);
	}
	else if(cmd == "snapshot"){
		generate_snapshot(actual_command[1],actual_command[1],actual_command[2]);
	}
	else{
		print_error("command not found");
	}
}


void print_error(string error){
	initiate_command_mode();
	cout<<error<<" ";
	cout<<"Press any character key to continue";

	while(true){
		int c=get_sigle_character();
		break;
	}
	initiate_command_mode();
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