
#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>

#include <dirent.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>

#include <pwd.h>
#include <grp.h>

using namespace std;

FileSystem::FileSystem(){

}

FileSystem::FileSystem(struct stat ini,string name,string dir_path,string par_path,string dis_name){ // a class for storing info. about files and directories
    	st=ini;
    	file_name=name;
    	directory_path=dir_path;
    	parent_path=par_path;
    	display_name=dis_name;
    }

void FileSystem::display(){ // for dispaying file and folder details
    	file_size = st.st_size;   
	    modified_time=st.st_mtime;

	    user = getpwuid(st.st_uid);
	    group = getgrgid(st.st_gid);
	            
	    char date[20];
	    strftime(date, 20, "%b %d %H:%M", gmtime(&(modified_time)));

	    printf( (S_ISDIR(st.st_mode)) ? "d " : "- ");
	    printf( (st.st_mode & S_IRUSR) ? "r" : "-");
	    printf( (st.st_mode & S_IWUSR) ? "w" : "-");
	    printf( (st.st_mode & S_IXUSR) ? "x" : "-");
	    printf( (st.st_mode & S_IRGRP) ? "r" : "-");
	    printf( (st.st_mode & S_IWGRP) ? "w" : "-");
	    printf( (st.st_mode & S_IXGRP) ? "x" : "-");
	    printf( (st.st_mode & S_IROTH) ? "r" : "-");
	    printf( (st.st_mode & S_IWOTH) ? "w" : "-");
	    printf( (st.st_mode & S_IXOTH) ? "x" : "-");
	    cout<<" ";
	    
	    cout<< user->pw_name<<" ";
	    cout<< group->gr_name<<" ";

	    std::stringstream ss;
		ss << file_size;
		std::string str = ss.str();
	    char *ch=create_array(string(str),10);
	    cout<<ch;
	    cout<< date <<"  ";

	    ch=create_array(display_name,30);
	    cout<<ch<<"\n";

    }

    