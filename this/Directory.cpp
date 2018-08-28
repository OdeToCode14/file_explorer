#include "createList.h"
#include "Directory.h"

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

/*
	public:
		string file_name;
		off_t file_size;
        struct passwd *user;
        struct group  *group;
        time_t modified_time;
        struct stat* st;*/
    
    Directory::Directory(struct stat ini,string name){
    	st=ini;
    	file_name=name;
    }

    void Directory::display(){
    	file_size = st.st_size;   
	    modified_time=st.st_mtime;

	    user = getpwuid(st.st_uid);
	    group = getgrgid(st.st_gid);
	            
	    char date[20];
	    strftime(date, 20, "%b %d %H:%M", gmtime(&(modified_time)));

	            // handling permissions https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
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
	    cout<< file_name << " --- ";
	    cout<< file_size << " --- ";
	            
	    cout<< date << " --- "<<"\n";

    }
    

