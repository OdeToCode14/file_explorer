#include "createList.h"
#include "FileSystem.h"

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


#include <sys/ioctl.h> //for terminal dimensions

//#include <time.h>

#define movecursor(x,y) printf("\033[%d;%dH", x,y) // move the cursor to x,y

using namespace std;



//struct winsize terminal_size;  // for terminal dimensions

vector<FileSystem> create_list(DIR *dp){
      vector<FileSystem> file_system_list;
      struct dirent *dirp;
      
      
      while ((dirp = readdir(dp)) != NULL){
            //cout<<"here\n";
            string file_name=dirp->d_name;
            struct stat st;
            if(stat(file_name.c_str(), &st) != 0) {
              cout<<"ERROR FILE DOES NOT EXIST\n";
              return file_system_list;
            }
            
            if(S_ISDIR(st.st_mode)){
                  FileSystem obj(st,file_name);
                  obj.type=is_directory;
                  file_system_list.push_back(obj);
                  //obj.display();
            }
            else{
                  FileSystem obj(st,file_name);
                  obj.type=is_file;
                  file_system_list.push_back(obj);
                  //obj.display();
            }
    }
    return file_system_list;
}



/*void terminal_dimensions(void){  // https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
    ioctl(0, TIOCGWINSZ, &terminal_size);

    
}
*/