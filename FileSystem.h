#ifndef FILESYSTEM_H
#define FILESYSTEM_H

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
//#include <time.h>

using namespace std;


static const int is_directory =0;
static const int is_file= 1;



class FileSystem
{
public:
  string file_name;
  off_t file_size;
  struct passwd *user;
  struct group  *group;
  time_t modified_time;
  struct stat st;

  int type;
  string directory_path;
  string parent_path;
  string display_name;

public:
  FileSystem();
  FileSystem(struct stat st,string name,string directory_path,string parent_path,string display_name);
  void display();
  

};



#endif