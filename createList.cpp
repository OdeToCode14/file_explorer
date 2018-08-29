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

FileSystem home_directory;

//struct winsize terminal_size;  // for terminal dimensions




string get_directory_name_from_path(string current){
  string directory_name;
  int ind=current.length()-1;
  while(ind >=0 && current[ind]!='/'){
    ind--;
  }
  directory_name=current.substr(ind + 1);
  return directory_name;
}


string find_directory_for_special_dot(string str){
  int count=0;
  int i;
  for(i=str.length()-1;i>=0;i--){
    if(str[i] == '/'){
      count++;
      if(count == 2){
        break;
      }
    }
  }
  string real_path=str.substr(0,i);
  return real_path;
}

string find_parent_path(string str){
   int count=0;
  int i;
  for(i=str.length()-1;i>=0;i--){
    if(str[i] == '/'){
      count++;
      if(count == 1){
        break;
      }
    }
  }
  string parent_path=str.substr(0,i);
  return parent_path; 
}



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

                 string parent_path=opened_dir.directory_path;
                 string real_name=file_name;
                  if(file_name == ".."){

                          string real_path=find_directory_for_special_dot(path_name);
                          path_name=real_path;
                          real_name=get_directory_name_from_path(real_path);
                          /*file_clicked.file_name=real_name;
                          file_clicked.directory_path=real_path;*/
                          parent_path=find_parent_path(real_path);
                  }

                  FileSystem obj(st,real_name,path_name,parent_path,file_name);
                  obj.type=is_directory;
                  file_system_list.push_back(obj);
                  //obj.display();
            }
            else{
                  FileSystem obj(st,file_name,path_name,opened_dir.directory_path,file_name);
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