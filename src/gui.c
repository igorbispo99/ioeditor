#include "gui.h"

// OP functions
void _op_up(int* x, int* y) {*x=*x; *y=*y-1;}
void _op_down(int* x, int* y) {*x=*x; *y=*y+1;}
void _op_right(int* x, int* y) {*x=*x+1; *y=*y;}
void _op_left(int* x, int* y) {*x=*x-1; *y=*y;}

op_func eval_arrow(int k) {
  switch (k) {
    case KEY_UP:
      return _op_up;
      break;
    case KEY_DOWN:
      return _op_down;
      break;
    case KEY_LEFT:
      return _op_left;
      break;
    case KEY_RIGHT:
      return _op_right;
      break;
    default:
      return NULL;
  }
}

bool _can_move_cursor (int k, text* txt,
    text_slice txt_slc) {
  int cursor_x, next_x;
  int cursor_y, next_y;

  // Getting current cursor position
  getyx(stdscr, cursor_y, cursor_x); 
  next_x = cursor_x;
  next_y = cursor_y;

  //Getting current screen size
  int screen_x, screen_y;
  getmaxyx(stdscr, screen_y, screen_x);

  // Evaluating operator function
  op_func op = eval_arrow(k);
  op(&next_x, &next_y);

  /* --------- Testing y_coord --------- */
  //Out of screen frame
  if (next_y < 1 || next_y > screen_y - 1) {
    return false;
  }

  //Out of txt bounds
  if (next_y > txt_slc.to_y + 1) {
    return false;
  }

  /* --------- Testing x_coord --------- */
  //Out of screen frame
  if (next_x < 1 || next_x > screen_x - 1) {
    return false;
  }

  //Out of txt bounds
  if (next_x > strlen(txt->lines[next_y-1]) + 1) {
    return false;
  }

  return true;
}

int _move_cursor (int k) {
  int cursor_x;
  int cursor_y;

  getyx(stdscr, cursor_y, cursor_x);
  op_func op = eval_arrow(k);   

  if (op) {
    op(&cursor_x, &cursor_y);
    move(cursor_y, cursor_x);
    refresh();

    return SUCCESS;   
  } else {
    return ERROR;
  }
}

int _display_txt (text* txt, text_slice txt_slc) {
  if (!txt->initialized)
    return ERROR;

  // TODO Modularize txt_slice functions
  // Text slice out of bounds
  if (txt_slc.from_y >= txt->num_of_lines ||
      txt_slc.from_y > txt_slc.to_y) {
      return ERROR;
  }

  int size_x, size_y;
  getmaxyx(stdscr, size_y, size_x);
 
  size_t scr_line = 1;
  size_t to_txt_line = (txt_slc.to_y >= size_y ? size_y : txt_slc.to_y);
  size_t l;
  for (l = txt_slc.from_y ;l <= to_txt_line ;l++) {
    mvprintw(scr_line, 1, txt->lines[l]);
    scr_line += 1;
  }

  if (l < size_y) {
    for (size_t i = l+1; i < size_y; i++)
      mvprintw(i, 1, "-");
  }
  
  move(1, 1);

  refresh();
  return SUCCESS;
}

int _run (file* f) {
  if (!f->initialized) {
    return ERROR;
  }
  int size_x, size_y;

  // Init routines
  initscr();
  //noecho();
  nonl();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, size_y, size_x);

  text_slice txt_slc;
  txt_slc.from_y = 0;
  txt_slc.to_y = f->txt->num_of_lines - 1;
  //nodelay(stdscr, true);
  if(_display_txt(f->txt, txt_slc) == ERROR) {
    return ERROR;
  }

  int k;
  bool insert_mode = true;

  while (1) {
    k = getch();

    // TODO Refactorize GUI execution flow
    if (k == KEY_END) {
      insert_mode = !insert_mode;
    } else if (insert_mode) {
      if (is_arrow(k))
        if (_can_move_cursor(k, f->txt, txt_slc))
          _move_cursor(k);
    } else if (k == 'q') {
      break;
    }
    usleep(500);
  }
  
  return SUCCESS;
}

int _clean () {
  endwin();
  return SUCCESS;
}

// End of class gui

// Misc functions
bool is_arrow(int k) {
  int arrow_types[] = {KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT};
  size_t num_of_arr = 4;
  for (size_t i = 0;i < num_of_arr;i++) {
    if (k == arrow_types[i])
      return true;
  }
  return false;
} 