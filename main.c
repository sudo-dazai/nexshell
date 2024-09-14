#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#define ctrl(x) ((x) & 0x1f)
#define SHELL "[fnsh]$ "
#define ENTER 10

int main() {
  initscr();		
  raw();
  noecho();

  bool QUIT = false;
  int in;

  char command[1024] = {0};
  size_t command_s = 0;
  size_t line = 0;

  while(!QUIT) {
    mvprintw(line, 0 , SHELL);
    mvprintw(line, 0+sizeof(SHELL)-1,command);
    
    in = getch();
    switch(in) {
      case ctrl('q'): 
        QUIT = true;
        break;
      case KEY_ENTER:
      case ENTER:
        line++;
        memset(command, 0, sizeof(char)* command_s);
        command_s = 0;
        break;
      default: 
        command[command_s++] = in;
        break;
    }
  }
  

	refresh();			;		
	endwin();	

  return 0;
}
