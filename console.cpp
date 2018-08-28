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
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
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
    
    initialize(dp,current);
    
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


    int c;

    while (1) {
        c = kbget();
        if (c == KEY_ESCAPE) {
            break;
        }
        else if(c==KEY_ENTER){  // for clearing the screen https://stackoverflow.com/questions/37774983/clearing-the-screen-by-printing-a-character
          clear_screen();
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
            cursorbackward(1);
        }
        else if (c == KEY_LEFT) {
            cursorforward(1);
        } 
        else {
            //putchar(c);
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


    return 0;
}