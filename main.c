#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

#define ctrl(x) ((x) & 0x1f)
#define SHELL "[fnsh]$ "
#define ENTER 10
#define DATA_START_CAPACITY 128

  #define ASSERT(cond, ...) \
        do { \
            if (!(cond)) { \
                endwin();   \
                fprintf(stderr, "%s:%d: ASSERTION FAILED: ", __FILE__, __LINE__); \
                fprintf(stderr, __VA_ARGS__); \
                fprintf(stderr, "\n"); \
                exit(1); \
            } \
        } while (0)

    #define DA_APPEND(da, item) do {                                                       \
        if ((da)->count >= (da)->capacity) {                                               \
            (da)->capacity = (da)->capacity == 0 ? DATA_START_CAPACITY : (da)->capacity*2; \
            void *new = calloc(((da)->capacity+1), sizeof(*(da)->data));                   \
            ASSERT(new,"out of ram");                                                       \
            if ((da)->data != NULL)                                                        \
                memcpy(new, (da)->data, (da)->count);                                      \
            free((da)->data);                                                              \
            (da)->data = new;                                                              \
        }                                                                                  \
        (da)->data[(da)->count++] = (item);                                                \
    } while (0)

typedef struct {
  char *data;
  size_t count;
  size_t capacity;
} String;

typedef struct {
  String *data;
  size_t count;
  size_t capacity;
} Strings;

int main() {
  initscr();		
  raw();
  noecho();

  bool QUIT = false;
  int in;

  String command = {0};
  Strings command_history = {0};
  size_t line = 0;

  while(!QUIT) {
    mvprintw(line, 0 , SHELL);
    mvprintw(line, 0+sizeof(SHELL)-1,"%.*s",(int)command.count, command.data);
    
    in = getch();
    switch(in) {
      case ctrl('q'): 
        QUIT = true;
        break;
      //case KEY_ENTER:
      case ENTER:
        line++;
        DA_APPEND(&command_history, command);
        command = (String){0};
        break;
      default:
        DA_APPEND(&command, in);
        break;
    }
  }
  

	refresh();			;		
	endwin();	
  for(size_t i = 0; i < command_history.count; i++) {
    printf("%.*s\n", (int)command_history.data[i].count, command_history.data[i].data);
    free(command_history.data[i].data);
  }
  return 0;
}
