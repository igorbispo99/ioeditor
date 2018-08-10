#include "bot_bar.h"

int _destroy_bar (bar* b) {
  free(b);
  return SUCCESS;
}

int _init_bar (bar* b, file* f, text_slice* txt_slc_) {
  if (!b || !f) return ERROR;

  b->splash_size = strlen(VER_SPLASH);
  b->filename_size = strlen(f->filename);

  b->bar_f = f;
  b->txt_slc = txt_slc_;

  return SUCCESS;
}

int _display_splash(bar *b) {
  int size_y, size_x;
  getmaxyx(stdscr, size_y, size_x);
  attron(A_REVERSE);

  // Version splash
  mvprintw(size_y-1, 0, VER_SPLASH);
  attroff(A_REVERSE);

  return SUCCESS;
}

int _display_lines_count(bar *b) {
  if (!b) return ERROR;

  int size_x, size_y;
  int cursor_x, cursor_y;

  getmaxyx(stdscr, size_y, size_x);
  getyx(stdscr, cursor_y, cursor_x);
  attron(A_REVERSE);

  char* lines_count = calloc(128, sizeof(char));

  if (!lines_count) return ERROR;

  sprintf(lines_count, "%s | %ld/%ld",
  b->bar_f->filename, 
  b->txt_slc->current_line_num+1,
  b->bar_f->txt_head->num_of_lines);

  // Update size lines_count
  b->lines_count_size = strlen(lines_count);

  // Lines count
  mvprintw(size_y-1, size_x - b->lines_count_size -1, lines_count);
  mvaddch(size_y-1, size_x-1, ' ');
  mvaddch(size_y-1, size_x, ' ');

  attroff(A_REVERSE);

  move(cursor_y, cursor_x);
  free(lines_count);
  return SUCCESS;
}


int _display_bar(bar* b) {
  // Fill bar background with blank spaces
  int size_x, size_y;
  getmaxyx(stdscr, size_y, size_x);

  char* blank_line = calloc(size_x+1, sizeof(char));
  if (!blank_line) return ERROR;

  memset(blank_line, ' ', size_x) ;

  attron(A_REVERSE);
  mvaddstr(size_y-1, 0, blank_line);
  attroff(A_REVERSE);

  free(blank_line);

  _display_splash(b);
  _display_lines_count(b);

  return SUCCESS;
}
