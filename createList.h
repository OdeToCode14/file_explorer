//Roll No. 2018201099
//Amit Tiwari

#ifndef CREATELIST_H
#define CREATELIST_H

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


//#include <time.h>

using namespace std;

extern FileSystem home_directory; // represents path to home of application

int check_file_or_directory(string path);

string get_directory_name_from_path(string current);

string find_directory_for_special_dot(string str);

string find_parent_path(string str);

vector<FileSystem> create_list(DIR*,FileSystem opened_dir);

void display_list();

void terminal_dimensions(void);

char* create_array(string str,int size);


#endif