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



using namespace std;




struct termios new_terminal_state, previous_terminal_state;

int get_sigle_character(void);
int check_for_second_character_of_escape_sequence(void);
int get_the_escape_key_pressed(void);

void disableEcho();

void start_normal_mode();
void start_command_mode();
void set_new_terminal_state();
void set_previous_terminal_state();

int get_sigle_character(void) // for getting single character input
{
    int c = 0;

    disableEcho();
    new_terminal_state.c_cc[VMIN] = 1;
    
    new_terminal_state.c_cc[VTIME] = 0;
    set_new_terminal_state();
    c = getchar();
    set_previous_terminal_state();
    return c;
}

void set_new_terminal_state(){  // setting terminal state to the new terminal state
  tcsetattr(0, TCSANOW, &new_terminal_state);
}
void set_previous_terminal_state(){  // setting terminal state to original
  tcsetattr(0, TCSANOW, &previous_terminal_state);
}
int check_for_second_character_of_escape_sequence(void){  // if the first character is escape get the second to find which escape sequence pressed
    int c = 0;

    disableEcho();
    new_terminal_state.c_cc[VMIN] = 0;
    new_terminal_state.c_cc[VTIME] = 1;
    set_new_terminal_state();
    c = getchar();
    set_previous_terminal_state();
    if (c != -1){
      ungetc(c, stdin);
      return 1;
    }
    return 0;
}

void disableEcho(){  // for disabling echo and putting in non canonical mode
    tcgetattr(0, &previous_terminal_state);
    memcpy(&new_terminal_state, &previous_terminal_state, sizeof(new_terminal_state));
    new_terminal_state.c_lflag &= ~(ICANON | ECHO);
}

int get_the_escape_key_pressed(void)  // for selecting the escape key pressed
{
    int c=0;

    if (!check_for_second_character_of_escape_sequence()) return ESCAPE;
    c = get_sigle_character();
    if (c == '[') {
        int ch=get_sigle_character();
        if(ch == 'A'){
          return UP;
        }
        else if(ch == 'B'){
          return DOWN;
        }
        else if(ch== 'C'){
          return RIGHT;
        }
        else if(ch == 'D'){
          return LEFT;
        }
        else{
          c=0;
        }
      }
      if (c == 0){ 
        while (check_for_second_character_of_escape_sequence()) {
        get_sigle_character();
        }
      }
        
    return c;
}




void start_normal_mode(){  // setting properties of normal mode and getting characters pressed
  int c;
  movecursor(cursor_position,1);
    while (true) {
        c = get_sigle_character();
        if(c == ESCAPE){
          c=get_the_escape_key_pressed();
        }

        if(c==ENTER){  
          enter();
        }
        else if(c==BACKSPACE){
          move_to_parent();
        }
        else if(c==COLON){
          start_command_mode();
        }
        else if (c == UP) {
          move_cursor_up();
        }
        else if (c == DOWN) {
          move_cursor_down();
        } 
        else if (c == RIGHT) {
            move_right();
        }
        else if (c == LEFT) {
            move_left();
        } 
        else if(c==104){ // go to home folder when h is pressed
            go_to_home();
        }
        else if(c== 113){
          printf("\e[?1049l");
          exit(0); // to avoid looping between command mode and normal mode
        }
    }
}

void start_command_mode(){  // setting properties of command mode
  initiate_command_mode();
  int c;

    while (true) {
        c = get_sigle_character();
        if(c == ESCAPE){
          c=get_the_escape_key_pressed();
        }

        if (c == ESCAPE) {
            start_normal_mode();
        }

        else if(c==ENTER){  
          enter_function_of_command_mode();
        }
        else if(c==BACKSPACE){
          backspace_pressed();
        }
        else if (c == UP) { // disabled
           //move_cursor_up_in_command_mode();
        }
        else if (c == DOWN) {  //disabled
            //move_cursor_down_in_command_mode();
        } 
        else if (c == RIGHT) {
            move_cursor_right_in_command_mode();
        }
        else if (c == LEFT) {
          move_cursor_left_in_command_mode();
        } 
        else{
          add_character_to_command(c);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[]) {  // program starts here
    clear_screen();
    printf("\e[?1049h");
    
    DIR *dp;
    struct dirent *dirp;
    
   char cwd[PATH_MAX];
   string current;
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       current=string(cwd);
       cout<<"Current working dir: "<<current<<"\n";
   } 
   else {
       perror("getcwd() error");
       return 1;
   }
  
   
    // alternate way use opendir(".")
    if ((dp = opendir(cwd)) == NULL){
      cout<<"can’t open "<<current<<"\n";
    }  


    struct stat st;
    string directory_name=get_directory_name_from_path(current);
    FileSystem dir(st,directory_name,current,current,directory_name);
    home_directory=dir;
    initialize(dp,directory_name,dir,add_to_traversal_list);

    start_normal_mode();

    return 0;
}
