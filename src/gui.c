#include "gui.h"

int _move_cursor (int k) {
  int cursor_x;
  int cursor_y;
  int return_value;
  getyx(stdscr, cursor_y, cursor_x);

  switch (k) {
    case KEY_UP:
      move(cursor_y - 1, cursor_x);
      break;
    case KEY_DOWN:
      move(cursor_y + 1, cursor_x);
      break;
    case KEY_LEFT:
      move(cursor_y, cursor_x - 1);
      break;
    case KEY_RIGHT:
      move(cursor_y, cursor_x + 1);
      break;
  }
  refresh();
  return SUCCESS;
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
  noecho();
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
    if (k == KEY_END) {
      insert_mode = !insert_mode;
    } else if (insert_mode) {
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