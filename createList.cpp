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

vector<FileSystem> create_list(DIR *dp,FileSystem opened_dir){
      vector<FileSystem> file_system_list;
      FileSystem opened_directory=opened_dir;
      struct dirent *dirp;
      
      
      while ((dirp = readdir(dp)) != NULL){
            //cout<<"here\n";
            string file_name=dirp->d_name;
            string path_name=opened_dir.directory_path+"/"+file_name;
            struct stat st;
            //file_name.c_str()
            if(stat(path_name.c_str(), &st) != 0) {
              cout<<"ERROR FILE DOES NOT EXIST"<<file_name<<"\n";
              return file_system_list;
            }
            
            if(S_ISDIR(st.st_mode)){
                  FileSystem obj(st,file_name,path_name,opened_dir.directory_path);
                  obj.type=is_directory;
                  file_system_list.push_back(obj);
                  //obj.display();
            }
            else{
                  FileSystem obj(st,file_name,path_name,opened_dir.directory_path);
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