#include "createList.h"
#include "NormalMode.h"
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

#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))

#define cursorup(x) printf("\033[%dA", (x))
#define cursordown(x) printf("\033[%dB", (x))

#define BACKSPACE 127
#define COLON 58

#define KEY_ESCAPE  0x001b
#define KEY_ENTER   0x000a
#define KEY_UP      0x0105
#define KEY_DOWN    0x0106
#define KEY_LEFT    0x0107
#define KEY_RIGHT   0x0108

static struct termios term, oterm;

static int getch(void);
static int kbhit(void);
static int kbesc(void);
static int kbget(void);


void start_normal_mode();
void start_command_mode();
static int getch(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    return c;
}

static int kbhit(void)
{
    int c = 0;

    tcgetattr(0, &oterm);
    memcpy(&term, &oterm, sizeof(term));
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &term);
    c = getchar();
    tcsetattr(0, TCSANOW, &oterm);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

static int kbesc(void)
{
    int c;

    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_RIGHT;
                break;
            case 'D':
                c = KEY_LEFT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0) while (kbhit()) getch();
    return c;
}

static int kbget(void)
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}



void start_normal_mode(){
  int c;
  movecursor(cursor_position,1);
    while (1) {
        c = kbget();
        if (c == KEY_ESCAPE) {
            break;
        }
        else if(c==KEY_ENTER){  // for clearing the screen https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
          //clear_screen();
          enter();
        }
        else if(c==BACKSPACE){
          move_to_parent();
        }
        else if(c==COLON){
          start_command_mode();
        }
        else if (c == KEY_UP) {
            //cursorup(1);
          move_cursor_up();
        }
        else if (c == KEY_DOWN) {
            //cursordown(1);
          move_cursor_down();
        } 
        else if (c == KEY_RIGHT) {
            move_right();
        }
        else if (c == KEY_LEFT) {
            move_left();
        } 
        else if(c==104){
            go_to_home();
        }
    }
    printf("\n");



    cout<<"Press q to exit";
    char ch;
      do{
       
       cin>>ch;
       if(ch=='q'){
          printf("\e[?1049l");
       }
    }while(ch!='q');
    exit(0); // this to avoi looping between normal mode and command mode
}

void start_command_mode(){
  movecursor(bottom+2,1);
  int c;

    while (1) {
        c = kbget();
        if (c == KEY_ESCAPE) {
            start_normal_mode();
        }
        else if(c==KEY_ENTER){  // for clearing the screen https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
          //clear_screen();
          enter();
        }
        else if(c==BACKSPACE){
          move_to_parent();
        }
        else if(c==COLON){

        }
        else if (c == KEY_UP) {
            //cursorup(1);
          move_cursor_up();
        }
        else if (c == KEY_DOWN) {
            //cursordown(1);
          move_cursor_down();
        } 
        else if (c == KEY_RIGHT) {
            move_right();
        }
        else if (c == KEY_LEFT) {
            move_left();
        } 
        else if(c==104){
            go_to_home();
        }
    }
    printf("\n");



    cout<<"Press q to exit";
    char ch;
      do{
       
       cin>>ch;
       if(ch=='q'){
          printf("\e[?1049l");
       }
    }while(ch!='q');
}

int main(int argc, char *argv[]) {
    clear_screen();
    printf("\e[?1049h");
    
    DIR *dp;
    struct dirent *dirp;
    
   char cwd[PATH_MAX];
   string current;
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       current=string(cwd);
       cout<<"Current working dir: "<<current<<"\n";
       //printf(stdout, "Current working dir: %s\n", cwd);
   } 
   else {
       perror("getcwd() error");
       return 1;
   }

  
   
    // alternate way use opendir(".")
    if ((dp = opendir(cwd)) == NULL){
      //printf("can’t open %s\n", cwd);
      cout<<"can’t open "<<current<<"\n";
    }  


    struct stat st;
    string directory_name=get_directory_name_from_path(current);
    FileSystem dir(st,directory_name,current,current,directory_name);
    home_directory=dir;
    initialize(dp,directory_name,dir,add_to_traversal_list);
/*
    cout<<current.c_str()<<"\n";
    usleep(1000);
    if(stat(current.c_str(), &st) != 0){
      //home_directory.st=st;
      directory_name=get_directory_name_from_path(current);
      home_directory.file_name=directory_name;
      home_directory.directory_path=current;  // considering parent of home is home itself
      home_directory.parent_path=current;
      FileSystem dir(st,directory_name,current,current);
      home_directory=dir;
      initialize(dp,directory_name,home_directory);
    }
    else{
      cout<<"Error\n";
    }
   */
    
    
   /*char cwd[PATH_MAX];
   string current;
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       current=string(cwd);
       cout<<"Current working dir: "<<current<<"\n";
       //printf(stdout, "Current working dir: %s\n", cwd);
   } 
   else {
       perror("getcwd() error");
       return 1;
   }

    // alternate way use opendir(".")
    if ((dp = opendir(cwd)) == NULL){
      //printf("can’t open %s\n", cwd);
      cout<<"can’t open "<<current<<"\n";
    }
    else{
      create_list(dp);
      display_list();
      closedir(dp);
    }
    */

    start_normal_mode();

    return 0;
}
