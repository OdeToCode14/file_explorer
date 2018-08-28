#ifndef DIRECTORY_H
#define DIRECTORY_H

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


#define is_directory 0;
#define is_file 1;



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

public:
  FileSystem(struct stat st,string name);
  void display();
  

};


#endif