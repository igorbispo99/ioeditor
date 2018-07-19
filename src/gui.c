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

int _display_txt (text* txt) {
  if (!txt->initialized) {
    return ERROR;
  }
  for (size_t i = 0;i < txt->num_of_lines;i++) {
    mvprintw(i+1, 1, txt->lines[i]);
  }
  refresh();
  return SUCCESS;
}

int _run (file* f) {
  if (!f->initialized) {
    return ERROR;
  }

  // Init routines
  int size_x, size_y;

  initscr();
  noecho();
  nonl();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, size_y, size_x);
  //nodelay(stdscr, true);
  if(_display_txt(f->txt) == ERROR) {
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