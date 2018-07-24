#include "gui.h"
#include "bot_bar.h"

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
  if ((next_y < 0 && txt_slc.from_y == 0) || (cursor_y > (txt_slc.from_y+screen_y)+2)) {
    return false;
  }

  if (next_y + txt_slc.from_y >= txt->num_of_lines) {
    return false;
  }

  /* --------- Testing x_coord --------- */
  if (next_x < 0 || next_x > strlen(txt->lines[txt_slc.from_y + next_y])-1) {
    return false;
  }

  return true;
}

int _move_cursor (int k, text* txt, text_slice* txt_slc) {
  int cursor_x, new_x, size_x;
  int cursor_y, new_y, size_y;
  bool scroll = false;

  getyx(stdscr, cursor_y, cursor_x);
  getmaxyx(stdscr, size_y, size_x);

  // Command options

  // End of line
  if (k == END_LINE) {
    size_t end_of_line_pos;
    end_of_line_pos = strlen(txt->lines[txt_slc->from_y+cursor_y]);

    move(cursor_y, end_of_line_pos-1);

    refresh();
    return SUCCESS;   
  } 

  // Begin of line
  if (k == BEGIN_LINE) {
    move(cursor_y , 0);

    refresh();
    return SUCCESS;      
  }

  // Go to EOF
  if (k == END_FILE) {
    size_t scroll_offset = (txt->num_of_lines-size_y) - txt_slc->from_y + 1;
    _scroll_txt(scroll_offset, txt, txt_slc);

    _display_txt(txt, *txt_slc);
    refresh();
    return SUCCESS;     
  }

  // End of command options

  op_func op = eval_arrow(k);
  new_x = cursor_x;
  new_y = cursor_y;   

  if (op) {
    op(&new_x, &new_y);

    // Case when cursor mov causes an scroll in txt
    // TODO Refactorize scroll handling

    if (new_y == size_y-1) {
      if (k == KEY_DOWN) {
        _scroll_txt(1, txt, txt_slc);
        _display_txt(txt, *txt_slc);
        scroll = true;
      }
    } else if (new_y == -1) {
      if (k == KEY_UP) {
        _scroll_txt(-1, txt, txt_slc);   
        _display_txt(txt, *txt_slc);
        scroll = true;
      }
    } 

    if (!scroll) {
      move(new_y, new_x);
    } else {
      move(cursor_y, cursor_x);
    }

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

  // TODO Refactorize screen buffer handling

  switch (k) {
    // Case DELETE
    case KEY_DC:
      //TODO Fix bug delete

      // Case when char removal causes line colapse
      if (cursor_x == line_size - 1) {
        if (current_line == txt->num_of_lines-1 ) {
          new_x = cursor_x;
          new_y = cursor_y;
          break;
        } 
        // Realloc current line
        size_t next_line_size = strlen(txt->lines[current_line+1]);
        txt->lines[current_line] = realloc(txt->lines[current_line], line_size + next_line_size);

        // Create concatenate current line with next line
        strcpy(txt->lines[current_line]+line_size-1, txt->lines[current_line+1]);

        // Move all lines after current line 1 position up
        for (size_t i = current_line + 1;i < txt->num_of_lines-1;i++) {
          txt->lines[i] = txt->lines[i+1];
        }

        // Realloc lines array
        txt->lines = realloc(txt->lines, (txt->num_of_lines-1) * sizeof(char*));

        txt->num_of_lines -= 1;

        //Writing on screen buffer
        if (_display_txt(txt, *txt_slc) == ERROR) {
         return ERROR;
        }

      } else {
        for (size_t i = cursor_x;i < line_size;i++) {
          txt->lines[current_line][i] = txt->lines[current_line][i+1];
        }
        txt->lines[current_line][line_size] = '\0';

         // Write at screen buffer
        delch();
      }

      new_x = cursor_x;
      new_y = cursor_y;

      break;
    
    // Case BACKSPACE
    case 127:
    case KEY_BACKSPACE:
      if (current_line == 0 && cursor_x == 0) {
        new_x = cursor_x;
        new_y = cursor_y;
        break;
      } 

      // Case when char removal causes line colapse
      if (cursor_x == 0) {
        // Realloc previous line
        size_t prev_line_size = strlen(txt->lines[current_line-1]);
        txt->lines[current_line-1] = realloc(txt->lines[current_line-1],
                                    line_size + prev_line_size);

        // Create concatenate previous line with current line
        strcpy(txt->lines[current_line-1]+prev_line_size-1, txt->lines[current_line]);

        // Move all lines after current line 1 position up
        for (size_t i = current_line;i < txt->num_of_lines-1;i++) {
          txt->lines[i] = txt->lines[i+1];
        }


        // Realloc lines array
        txt->lines = realloc(txt->lines, (txt->num_of_lines-1) * sizeof(char*));

        txt->num_of_lines -= 1;

        new_x = prev_line_size-1 +line_size-1;
        new_y = cursor_y-1;

        //Writing on screen buffer
        if (_display_txt(txt, *txt_slc) == ERROR) {
          return ERROR;
        }
      } else {
        for (size_t i = cursor_x-1;i < line_size;i++) {
          txt->lines[current_line][i] = txt->lines[current_line][i+1];
        }
        txt->lines[current_line][line_size] = '\0';  

        clrtoeol();
        mvaddstr(cursor_y, 0, txt->lines[current_line]);

        new_x = cursor_x-1;
        new_y = cursor_y;     
      }

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

      for (size_t i = txt->num_of_lines-1; i > current_line; i--) {
        swap_chr_ptr(&(txt->lines[i]), &(txt->lines[i+1]));
      }

      // Increase num of lines
      txt->num_of_lines += 1;

      new_x = 0;
      new_y = cursor_y + 1;

      //Writing on screen buffer
      if (_display_txt(txt, *txt_slc) == ERROR) {
        return ERROR;
      }

      break;

    // Case common char (letter/num)
    default:
      // Increase size of line
      txt->lines[current_line] = realloc(txt->lines[current_line], line_size+2);
      txt->lines[current_line][line_size] = (char) k;

      // Move all elements one position to right
      for (int i = line_size - 1; i >= cursor_x && i >= 0; i--) {
        swap_chr(&(txt->lines[current_line][i]), &(txt->lines[current_line][i+1]));
      }

      new_x = cursor_x+1;
      new_y = cursor_y;

      txt->lines[current_line][line_size+1] = '\0';

      // Updating line screen buffer
      clrtoeol();
      mvaddstr(cursor_y, 0, txt->lines[current_line]);

  } //end switch key

  //Move cursor to correct place
  move(new_y, new_x);
  refresh();
  return SUCCESS;
}

int _display_txt (text* txt, text_slice txt_slc) {
  if (!txt->initialized)
    return ERROR;

  int size_x, size_y;
  getmaxyx(stdscr, size_y, size_x);

  size_t scr_line = 0;
  size_t scr_limit = size_y + txt_slc.from_y-1;
  size_t to_txt_line = (txt->num_of_lines > scr_limit ? scr_limit : txt->num_of_lines) ;
  size_t l;

  for (l = txt_slc.from_y ;l < to_txt_line ;l++) {
    clrtoeol();
    mvaddstr(scr_line, 0, txt->lines[l]);
    scr_line += 1;
  }

  for (size_t i = l; i < scr_limit; i++) {
    clrtoeol();
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
  bar b = bar_;

  // Init routines
  initscr();
  noecho();
  raw();
  setlocale(LC_CTYPE, "");
  //nonl();
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, size_y, size_x);

  text_slice txt_slc;
  txt_slc.from_y = 0;
  txt_slc.to_y = (f->txt->num_of_lines > size_y ? size_y-1 : f->txt->num_of_lines-1);

  if(_display_txt(f->txt, txt_slc) == ERROR) {
    return ERROR;
  }

  // Initializing bar
  b.init(&b, f, &txt_slc);
  b.display_bar(&b);

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
          _move_cursor(k, f->txt, &txt_slc);
      } else { 
          _write_at_cursor(k, f->txt, &txt_slc);
      }
    } else if (k == 'q') {
      break;
    } else if (k == 't') {
      _move_cursor(END_LINE, f->txt, &txt_slc);
    } else if (k == 'v') {
      _move_cursor(BEGIN_LINE, f->txt, &txt_slc);
    } else if (k == ']') {
      _move_cursor(END_FILE, f->txt, &txt_slc);
    }
    usleep(250);
    b.display_lines_count(&b);
  }

  mvprintw(size_y-1, 0, "Deseja salvar o buffer modificado? (s/N) => ");
  k = getch();

  if (k == 's') {
    char filename[256];
    echo();

    // TODO Implement bottom bar
    mvprintw(size_y-1, 0, "                                              ");
    mvprintw(size_y-1, 0, "Nome do arquivo => ");
    getstr(filename);

    return  _write_file(f, filename);
  }
  
  return SUCCESS;
}

int _scroll_txt(int n_lines, text* txt, text_slice* txt_slc) {
  // Check if the scroll is valid
  if (txt_slc->to_y + n_lines > txt->num_of_lines ||
      txt_slc->from_y + n_lines > txt->num_of_lines) {
    return ERROR;
  }

  txt_slc->from_y += n_lines;
  txt_slc->to_y += n_lines;

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
