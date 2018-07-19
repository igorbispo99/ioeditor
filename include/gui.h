#ifndef GUI_H_
#define GUI_H_

#ifndef CTRL
#define CTRL(c) ((c) & 0x1f)
#endif 

#include "file_manager.h"
#include <curses.h>
#include <unistd.h>

enum KEY {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

// Class text_slice
typedef struct text_slice {
  //int from_x, to_x;
  int from_y, to_y;
} text_slice;

// Class gui
#define gui_ {&_run, &_move_cursor, &_display_txt, &_clean}

typedef struct gui gui;

struct gui {
  int (*run) (file*);
  int (*move_cursor) (int);
  int (*display_txt) (text*, text_slice);
  int (*clean) (void);
};

int _run(file*);
int _move_cursor(int);
int _display_txt(text*, text_slice);
int _clean (void);

#endif