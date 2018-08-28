#ifndef CREATELIST_H
#define CREATELIST_H

#include "NormalMode.h"
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
//#include <time.h>

using namespace std;


vector<FileSystem> create_list(DIR*,FileSystem opened_dir);

void display_list();

void terminal_dimensions(void);


#endif