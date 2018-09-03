#ifndef CONSOLE_H
#define CONSOLE_H

#include "NormalMode.h"
#include "createList.h"
#include "FileSystem.h"
#include "CommandMode.h"
#include "console.h"

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

#define BACKSPACE 127
#define COLON 58

#define UP      0x0105
#define DOWN    0x0106
#define LEFT    0x0107
#define RIGHT   0x0108
#define ESCAPE  0x001b
#define ENTER   0x000a

int get_sigle_character(void);

int get_the_escape_key_pressed(void);

void start_normal_mode();

#endif