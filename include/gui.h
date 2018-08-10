#ifndef GUI_H_
#define GUI_H_

#ifndef CTRL
#define CTRL(c) ((c) & 0x1f)
#endif 

#define EXP_ENTER 10

#include "file_manager.h"
#include <curses.h>
#include <unistd.h>
#include <locale.h>

enum COMMAND_KEYS {
  END_LINE,
  BEGIN_LINE,
  NEXT_LINE,
  PREV_LINE,
  BEGIN_FILE,
  END_FILE
};

// Class text_slice
typedef struct text_slice {
  //int from_x, to_x;
  text* current_line_ptr;
  text* first_scr_line;
  size_t current_line_num;
} text_slice;

// Class gui
#define gui_ {&_run, &_can_move_cursor,\
&_move_cursor, &_write_at_cursor, &_display_txt,\
&_scroll_txt, &_clean, &_change_to_select_mode}

typedef struct gui gui;
typedef struct syntax_engine syntax_engine;

struct gui {
  int (*run) (file*, syntax_engine*);
  bool (*can_move_cursor) (int, text*);
  int (*move_cursor) (int, text_head*, text_slice*, syntax_engine*);
  int (*write_at_cursor) (int, text_head*, text_slice*, syntax_engine*);
  int (*display_txt) (text*, syntax_engine*);
  text* (*scroll_txt) (int, text*);
  int (*clean) (void);
  text_head* (*change_to_select_mode) (text_slice*);
};

int _run(file*, syntax_engine*);
bool _can_move_cursor(int, text*);
int _move_cursor(int, text_head*, text_slice*, syntax_engine*);
int _write_at_cursor(int, text_head*, text_slice*, syntax_engine*);
int _display_txt(text*, syntax_engine*);
int _clean (void);
text* _scroll_txt(int, text*);
text_head* _change_to_select_mode(text_slice*);
// End of class gui

// Misc functions
bool is_arrow(int);
void swap_chr_ptr(char**, char**);
void swap_chr(char* a, char* b);
int paste_from_txt(text_head*, text_slice*, text_head*, syntax_engine*);

// Operator functions
typedef void (*op_func) (int* x, int* y);
void _op_up(int* x, int* y);
void _op_down(int* x, int* y);
void _op_right(int* x, int* y);
void _op_left(int* x, int* y);
op_func eval_arrow(int);


#endif
