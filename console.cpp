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

struct termios new_terminal_state, previous_terminal_state;

int get_sigle_character(void);
int check_for_second_character_of_escape_sequence(void);
int get_the_escape_key_pressed(void);



void start_normal_mode();
void start_command_mode();

int get_sigle_character(void)
{
    int c = 0;

    tcgetattr(0, &previous_terminal_state);
    memcpy(&new_terminal_state, &previous_terminal_state, sizeof(new_terminal_state));
    new_terminal_state.c_cc[VMIN] = 1;

    new_terminal_state.c_lflag &= ~(ICANON | ECHO);
    
    new_terminal_state.c_cc[VTIME] = 0;
    
    tcsetattr(0, TCSANOW, &new_terminal_state);
    c = getchar();
    
    tcsetattr(0, TCSANOW, &previous_terminal_state);
    
    return c;
}

int check_for_second_character_of_escape_sequence(void)
{
    int c = 0;

    tcgetattr(0, &previous_terminal_state);
    memcpy(&new_terminal_state, &previous_terminal_state, sizeof(new_terminal_state));
    new_terminal_state.c_lflag &= ~(ICANON | ECHO);
    new_terminal_state.c_cc[VMIN] = 0;
    new_terminal_state.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &new_terminal_state);
    c = getchar();
    tcsetattr(0, TCSANOW, &previous_terminal_state);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

int get_the_escape_key_pressed(void)
{
    int c=0;

    if (!check_for_second_character_of_escape_sequence()) return KEY_ESCAPE;
    c = get_sigle_character();
    if (c == '[') {
        switch (get_sigle_character()) {
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
    } 
    if (c == 0){ 
      while (check_for_second_character_of_escape_sequence()) {
        get_sigle_character();
      }
    }
    return c;
}




void start_normal_mode(){
  int c;
  movecursor(cursor_position,1);
    while (1) {
        c = get_sigle_character();
        if(c == KEY_ESCAPE){
          c=get_the_escape_key_pressed();
        }
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
    exit(0); // this to avoid looping between normal mode and command mode
}

void start_command_mode(){
  initiate_command_mode();
  int c;

    while (1) {
        c = get_sigle_character();
        if(c == KEY_ESCAPE){
          c=get_the_escape_key_pressed();
        }
        if (c == KEY_ESCAPE) {
            start_normal_mode();
        }

        else if(c==KEY_ENTER){  // for clearing the screen https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
          //clear_screen();
          enter_function_of_command_mode();
        }
        else if(c==BACKSPACE){
          backspace_pressed();
        }
        else if (c == KEY_UP) {
           move_cursor_up_in_command_mode();
          //move_cursor_up();
        }
        else if (c == KEY_DOWN) {
            move_cursor_down_in_command_mode();
            //cursordown(1);
          //move_cursor_down();
        } 
        else if (c == KEY_RIGHT) {
            move_cursor_right_in_command_mode();
            //move_right();
        }
        else if (c == KEY_LEFT) {
          move_cursor_left_in_command_mode();
            //move_left();
        } 
        else{
          add_character_to_command(c);
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
