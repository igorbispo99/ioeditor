#ifndef GUI_H_
#define GUI_H_

#include "file_manager.h"
#include <curses.h>
#include <unistd.h>

enum KEY {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

// Class gui
#define gui_ {&_run, &_move_cursor, &_display_txt}

typedef struct gui gui;

struct gui {
  int (*run) (file*);
  int (*move_cursor) (int);
  int (*display_txt) (text*);
};

int _run(file*);
int _move_cursor(int);
int _display_txt(text*);

#endif