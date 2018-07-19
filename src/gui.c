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

int _run (file* f) {
  if (!f->initialized) {
    return ERROR;
  }

  initscr();
  noecho();
  keypad(stdscr, TRUE);
  
  //nodelay(stdscr, true);
  mvprintw(1, 1, f->txt);
  refresh();

  int k;

  while ((k = getch()) != 'q') {
    _move_cursor(k);
    usleep(500);
  }

  return SUCCESS;
}