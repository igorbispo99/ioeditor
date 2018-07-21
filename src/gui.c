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

  // TODO Case when txt is larger than screen x size

  /* --------- Testing y_coord --------- */
  if (next_y < 0 || next_y > txt_slc.to_y - txt_slc.from_y) {
    return false;
  }

  /* --------- Testing x_coord --------- */
  if (next_x < 0 || next_x > strlen(txt->lines[txt_slc.from_y + next_y])-1) {
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

int _write_at_cursor (int k, text* txt, text_slice* txt_slc) {
  int cursor_x, new_x;
  int cursor_y, new_y;
  int size_x, size_y;

  getyx(stdscr, cursor_y, cursor_x);
  getmaxyx(stdscr, size_y, size_x);

  // Auxiliary vars
  size_t current_line = txt_slc->from_y + cursor_y;
  size_t line_size = strlen(txt->lines[current_line]);

  // Aux line
  char* new_line = NULL;

  switch (k) {
    // Case DELETE
    case KEY_DC:
      // Move elements after cursor to left
      for (size_t i = cursor_x;i < line_size;i++) {
        txt->lines[current_line][i] = txt->lines[current_line][i+1];
      }
      txt->lines[current_line][line_size] = '\0';

      new_x = cursor_x;
      new_y = cursor_y;

      break;
    
    // Case ENTER (new line)
    case '\n':
      // Allocate the new line
      new_line = calloc(line_size-cursor_x+1, sizeof(char));

      // Copy remain caracters after the cursor
      strcpy(new_line, txt->lines[current_line] + cursor_x);

      // Change current line size
      // TODO Refactorize
      txt->lines[current_line] = realloc(txt->lines[current_line], cursor_x+2);
      txt->lines[current_line][cursor_x] = '\n';
      txt->lines[current_line][cursor_x+1] = '\0';

      // Insert new line and move all lines 1 position down
      txt->lines = realloc(txt->lines, (txt->num_of_lines + 1) * sizeof(char*));
      txt->lines[txt->num_of_lines] = new_line;

      for (size_t i = txt->num_of_lines-1; i > cursor_y; i--) {
        swap_chr_ptr(&(txt->lines[i]), &(txt->lines[i+1]));
      }

      // Increase num of lines
      txt->num_of_lines += 1;

      if (txt_slc->to_y < size_y) {
        txt_slc->to_y += 1;
      }

      new_x = 0;
      new_y = cursor_y + 1;

      break;

    // Case common char (letter/num)
    default:
      // Increase size of line
      txt->lines[current_line] = realloc(txt->lines[current_line], line_size+2);
      txt->lines[current_line][line_size] = (char) k;

      // Move all elements one position to right
      for (size_t i = line_size - 1; i > cursor_x; i--) {
        swap_chr(&(txt->lines[current_line][i]), &(txt->lines[current_line][i+1]));
      }

      new_x = cursor_x+1;
      new_y = cursor_y;

      txt->lines[current_line][line_size+1] = '\0';

  } //end switch key

  //Writing on screen buffer
  if (_display_txt(txt, *txt_slc) == ERROR) {
    return ERROR;
  }

  //Move cursor to correct place
  move(new_y, new_x);
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
  wclear(stdscr);

  size_t scr_line = 0;
  size_t to_txt_line = (txt_slc.to_y >= size_y ? size_y : txt_slc.to_y);
  size_t l;
  for (l = txt_slc.from_y ;l <= to_txt_line ;l++) {
    mvprintw(scr_line, 0, txt->lines[l]);
    scr_line += 1;
  }

  if (l < size_y) {
    for (size_t i = l; i < size_y; i++)
      mvprintw(i, 0, "*");
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
  //nonl();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, size_y, size_x);

  text_slice txt_slc;
  txt_slc.from_y = 0;
  txt_slc.to_y = f->txt->num_of_lines - 1;

  if(_display_txt(f->txt, txt_slc) == ERROR) {
    return ERROR;
  }

  move(0, 0);

  int k;
  bool insert_mode = true;

  while (1) {
    k = getch();

    // TODO Refactorize GUI execution flow
    if (k == KEY_END) {
      insert_mode = !insert_mode;
    } else if (insert_mode) {
      if (is_arrow(k)) { 
        if (_can_move_cursor(k, f->txt, txt_slc))
          _move_cursor(k);
      } else { 
          _write_at_cursor(k, f->txt, &txt_slc);
      }
    } else if (k == 'q') {
      break;
    }
    usleep(500);
  }

  mvprintw(size_y-1, 0, "Deseja salvar o buffer modificado? (s/N) => ");
  k = getch();
  
  return (k == 's' ? _write_file(f) : SUCCESS);
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

void swap_chr_ptr(char** a, char** b) {
  char *a_ = *a;
  *a = *b;
  *b = a_;
}

void swap_chr(char* a, char* b) {
  char a_ = *a;
  *a = *b;
  *b = a_;
}