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
#define gui_ {&_run, &_can_move_cursor,\
&_move_cursor, &_display_txt, &_clean}

typedef struct gui gui;

struct gui {
  int (*run) (file*);
  bool (*can_move_cursor) (int, text*, text_slice);
  int (*move_cursor) (int);
  int (*display_txt) (text*, text_slice);
  int (*clean) (void);
};

int _run(file*);
bool _can_move_cursor(int, text*, text_slice);
int _move_cursor(int);
int _display_txt(text*, text_slice);
int _clean (void);
// End of class gui

// Misc functions
bool is_arrow(int);

// Operator functions
typedef void (*op_func) (int* x, int* y);
void _op_up(int* x, int* y);
void _op_down(int* x, int* y);
void _op_right(int* x, int* y);
void _op_left(int* x, int* y);
op_func eval_arrow(int);


#endif