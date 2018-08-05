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

bool _can_move_cursor (int k, text* txt) {
  int cursor_x, next_x;
  int cursor_y, next_y;

  // Getting current cursor position
  getyx(stdscr, cursor_y, cursor_x); 
  next_x = cursor_x;
  next_y = cursor_y;

  // Evaluating operator function
  op_func op = eval_arrow(k);
  op(&next_x, &next_y);

  // Evaluating cursor movement

  // TODO Case when txt is larger than screen x size
  if (next_x < 0) return false;

  // Cursor go to next line case
  if (next_y > cursor_y) {
    // Current Line is the last line
    if (!txt->next_line)
      return false;
    // Next line is smaller than target x
    if (next_x > strlen(txt->next_line->content) - 1)
      return false;
  
  // Cursor go to previous line
  } else if (next_y < cursor_y) {
    // Current line is the first line
    if (!txt->prev_line)
      return false;
    // Previous line is smaller than target x
    if (next_x > strlen(txt->prev_line->content) - 1)
      return false;

  // Cursor stays at the same line
  } else {
    if (next_x > strlen(txt->content) - 1)
      return false;
  }

  return true;
}

int _move_cursor (int k, text_head* head, text_slice* txt_slc) {
  int cursor_x, new_x, size_x;
  int cursor_y, new_y, size_y;
  bool scroll = false;

  getyx(stdscr, cursor_y, cursor_x);
  getmaxyx(stdscr, size_y, size_x);

  // Command options

  // End of line
  if (k == END_LINE) {
    move(cursor_y, strlen(txt_slc->current_line_ptr->content)-1);

    refresh();
    return SUCCESS;   
  } 

  // Begin of line
  if (k == BEGIN_LINE) {
    move(cursor_y , 0);

    refresh();
    return SUCCESS;      
  }

  // Go to end of next line 
  if (k == NEXT_LINE) {
    // Last linext
    if (!txt_slc->current_line_ptr->next_line) return ERROR;
    txt_slc->current_line_num += 1;

    // Scroll case
    if (cursor_y == size_y - 2) {
      text* new_first = _scroll_txt(1, txt_slc->first_scr_line);
      _display_txt(new_first);
      txt_slc->first_scr_line = new_first;

      move(cursor_y, strlen(txt_slc->current_line_ptr->next_line->content) - 1);
      txt_slc->current_line_ptr = txt_slc->current_line_ptr->next_line;

      refresh();
      return SUCCESS;
    }

    // Regular case
    move(cursor_y+1, strlen(txt_slc->current_line_ptr->next_line->content) - 1);
    txt_slc->current_line_ptr = txt_slc->current_line_ptr->next_line;

    refresh();
    return SUCCESS;
  }

  // Go to end of prev line
  if (k == PREV_LINE) {
    // First line
    if (!txt_slc->current_line_ptr->prev_line) return ERROR;
    txt_slc->current_line_num -= 1;

    // Scroll case
    if (cursor_y == 0) {
      text* new_first = _scroll_txt(-1, txt_slc->first_scr_line);
      _display_txt(new_first);
      txt_slc->first_scr_line = new_first;

      move(cursor_y, strlen(txt_slc->current_line_ptr->prev_line->content) - 1);
      txt_slc->current_line_ptr = (txt_slc->current_line_ptr->prev_line);

      refresh();
      return SUCCESS;
    }

    // Regular case
    move(cursor_y-1, strlen(txt_slc->current_line_ptr->prev_line->content) - 1);
    txt_slc->current_line_ptr = (txt_slc->current_line_ptr->prev_line);

    refresh();
    return SUCCESS;
  }

  // Go to begin of file
  if (k == BEGIN_FILE) {
    _display_txt(head->first_line);

    txt_slc->current_line_num = 0;
    txt_slc->current_line_ptr = head->first_line;
    txt_slc->first_scr_line = head->first_line;

    move(0, 0);
    refresh();
    return SUCCESS;
  }

  // Go to EOF
  // TODO Implement go to EOF
  if (k == END_FILE) {
    // size_t scroll_offset = (txt->num_of_lines-size_y) - txt_slc->from_y + 1;
    // _scroll_txt(scroll_offset, txt, txt_slc); 
    // _display_txt(txt, *txt_slc);

    // move(txt->num_of_lines > size_y ? size_y-2 : txt->num_of_lines-1, 0);
    // refresh();
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

    if (k == KEY_DOWN) {
      txt_slc->current_line_ptr = txt_slc->current_line_ptr->next_line;
      txt_slc->current_line_num += 1;

      if (new_y == size_y-1) {
        text* new_first = _scroll_txt(1, txt_slc->first_scr_line);
        _display_txt(new_first);
        txt_slc->first_scr_line = new_first;

        scroll = true;
      }
    } else if (k == KEY_UP) {
      txt_slc->current_line_ptr = txt_slc->current_line_ptr->prev_line;
      txt_slc->current_line_num -= 1;

      if (new_y == -1) {
        text* new_first = _scroll_txt(-1, txt_slc->first_scr_line);   
        _display_txt(new_first);
        txt_slc->first_scr_line = new_first;

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

int _write_at_cursor (int k,text_head* head, text_slice* txt_slc) {
  int cursor_x, new_x;
  int cursor_y, new_y;
  int size_x, size_y;

  getyx(stdscr, cursor_y, cursor_x);
  getmaxyx(stdscr, size_y, size_x);

  // Auxiliary vars
  size_t line_size = strlen(txt_slc->current_line_ptr->content);

  //text aux_cell;
  text* aux_cell_ptr = NULL;
  text* aux_cell_ptr_ = NULL;

  // TODO Refactorize screen buffer handling

  switch (k) {
    // Case DELETE
    case KEY_DC:
      //TODO Fix bug delete

      // Case when char removal causes line colapse
      if (cursor_x == line_size - 1) {

        // TODO Implement last char removal
        // Delete last line (workaround)
        if (!txt_slc->current_line_ptr->next_line) {
          new_x = cursor_x;
          new_y = cursor_y;
          break;
        } 

        // Increase current line size
        size_t next_line_size = strlen(txt_slc->current_line_ptr->next_line->content);
        txt_slc->current_line_ptr->content = realloc(txt_slc->current_line_ptr->content, line_size + next_line_size);

        // Concatenate current line with next line
        strcpy(txt_slc->current_line_ptr->content + line_size-1, txt_slc->current_line_ptr->next_line->content);
        // Remove next line cell
        aux_cell_ptr = txt_slc->current_line_ptr->next_line;

        txt_slc->current_line_ptr->next_line = txt_slc->current_line_ptr->next_line->next_line;
        txt_slc->current_line_ptr->next_line->prev_line = txt_slc->current_line_ptr;

        free(aux_cell_ptr);

        head->num_of_lines -= 1;

        //Writing on screen buffer
        if (_display_txt(txt_slc->first_scr_line) == ERROR) {
         return ERROR;
        }

      // Regular char deletion
      } else {
        memmove(&(txt_slc->current_line_ptr->content[cursor_x]), 
                &(txt_slc->current_line_ptr->content[cursor_x+1]),
                line_size - cursor_x);

        txt_slc->current_line_ptr->content[line_size-1] = '\0';

        // Write at screen buffer
        delch();
      }

      new_x = cursor_x;
      new_y = cursor_y;

      break;
    
    // Case BACKSPACE
    case 127:
    case KEY_BACKSPACE:
      if (txt_slc->current_line_num == 0 && cursor_x == 0) {
        new_x = cursor_x;
        new_y = cursor_y;
        break;
      } 

      // Case when char removal causes line colapse
      if (cursor_x == 0) {
        // Realloc previous line
        size_t prev_line_size = strlen(txt_slc->current_line_ptr->prev_line->content);

        txt_slc->current_line_ptr->prev_line->content = realloc(
          txt_slc->current_line_ptr->prev_line->content,
          line_size + prev_line_size + 1);

        // Create concatenate previous line with current line
        strcpy(txt_slc->current_line_ptr->prev_line->content + prev_line_size-1,
          txt_slc->current_line_ptr->content);

        // Remove current line cell
        aux_cell_ptr = txt_slc->current_line_ptr->next_line; // It will be used later
        aux_cell_ptr_ = txt_slc->current_line_ptr->prev_line; // ''

        txt_slc->current_line_ptr->prev_line->next_line = txt_slc->current_line_ptr->next_line;

        // Corner case last line
        if (aux_cell_ptr)
          txt_slc->current_line_ptr->next_line->prev_line = txt_slc->current_line_ptr->prev_line;

        free(txt_slc->current_line_ptr);

        // Deletion first line
        if (!txt_slc->current_line_num) {
          new_x = cursor_x;
          new_y = cursor_y;

          txt_slc->first_scr_line = aux_cell_ptr;
          txt_slc->current_line_ptr = aux_cell_ptr;

          // Current line was freed, so we cant access current_line->next_line
          if (_display_txt(txt_slc->first_scr_line) == ERROR) return ERROR;

        } else {
          new_x = prev_line_size-1;
          new_y = cursor_y-1;

          txt_slc->current_line_ptr = aux_cell_ptr_;

          if (_display_txt(txt_slc->first_scr_line) == ERROR) return ERROR;          
        }
        head->num_of_lines -= 1;
        txt_slc->current_line_num -= 1;

      // Regular Backspace deletion
      } else {
        memmove(&(txt_slc->current_line_ptr->content[cursor_x-1]), 
          &(txt_slc->current_line_ptr->content[cursor_x]),
          line_size - cursor_x);

        txt_slc->current_line_ptr->content[line_size-1] = '\0';

        clrtoeol();
        mvaddstr(cursor_y, 0, txt_slc->current_line_ptr->content);

        new_x = cursor_x-1;
        new_y = cursor_y;     
      }

      break;

    // Case ENTER (new line)
    case '\n':
      // Allocate new line txt cell
      aux_cell_ptr = new_txt_cell(txt_slc->current_line_ptr->content + cursor_x);
      // Change current line size
      // TODO Refactorize
      txt_slc->current_line_ptr->content = realloc(txt_slc->current_line_ptr->content, cursor_x+2);
      txt_slc->current_line_ptr->content[cursor_x] = '\n';
      txt_slc->current_line_ptr->content[cursor_x+1] = '\0';

      // Insert new line txt cell
      aux_cell_ptr_ = txt_slc->current_line_ptr->next_line;

      txt_slc->current_line_ptr->next_line = aux_cell_ptr;
      aux_cell_ptr->prev_line = txt_slc->current_line_ptr;

      aux_cell_ptr->next_line = aux_cell_ptr_;

      // Last line corner case
      if (aux_cell_ptr_)
        aux_cell_ptr_->prev_line = aux_cell_ptr;

      // Increase num of lines
      head->num_of_lines += 1;

      // Move current line pointer
      txt_slc->current_line_ptr = txt_slc->current_line_ptr->next_line;
      txt_slc->current_line_num += 1;

      new_x = 0;
      new_y = cursor_y + 1;

      //Writing on screen buffer
      if (_display_txt(txt_slc->first_scr_line) == ERROR) {
        return ERROR;
      }

      break;

    // Case common char (letter/num)
    default:
      // Increase size of line
      txt_slc->current_line_ptr->content = realloc(txt_slc->current_line_ptr->content, line_size+2);
      txt_slc->current_line_ptr->content[line_size] = '\0';

      // Shift elements
      memmove(&(txt_slc->current_line_ptr->content[cursor_x+1]),
        &(txt_slc->current_line_ptr->content[cursor_x]),
        line_size - cursor_x+1);

      // Insert new char
      txt_slc->current_line_ptr->content[cursor_x] = (char) k;

      new_x = cursor_x+1;
      new_y = cursor_y;

      // Updating line screen buffer
      clrtoeol();
      mvaddstr(cursor_y, 0, txt_slc->current_line_ptr->content);

  } //end switch key

  //Move cursor to correct place
  move(new_y, new_x);
  refresh();
  return SUCCESS;
}

int _display_txt (text* txt) {
  // Failsafe
  if (!txt) return ERROR;

  int size_x, size_y;
  getmaxyx(stdscr, size_y, size_x);

  text* txt_crawler;
  int scr_line = 0;
  int scr_limit = size_y - 2;

  for (txt_crawler = txt;
      txt_crawler && scr_line <= scr_limit;
      txt_crawler = txt_crawler->next_line) {

    clrtoeol();
    mvaddstr(scr_line, 0, txt_crawler->content);
    scr_line += 1;
  }

  for (size_t i = scr_line; i <= scr_limit; i++) {
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

  if(_display_txt(f->txt_head->first_line) == ERROR) {
    return ERROR;
  }

  text_slice* txt_slc = calloc(1, sizeof(text_slice));
  txt_slc->current_line_ptr = f->txt_head->first_line;
  txt_slc->current_line_num = 0;
  txt_slc->first_scr_line = f->txt_head->first_line;

  // Initializing bar
  b.init(&b, f, txt_slc);
  b.display_bar(&b);

  move(0, 0);

  int k;
  text_head* cpy_buffer = NULL;

  while (1) {
    k = getch();
    // TODO Refactorize GUI execution flow

    // Command cases
    if (k == CTRL('q')) {
      break;
    } else if (k ==  CTRL('v')) {
      _move_cursor(END_LINE, f->txt_head, txt_slc);
    } else if (k == CTRL('t')) {
      _move_cursor(BEGIN_LINE, f->txt_head, txt_slc);
    } else if (k == CTRL('c')) {
      _move_cursor(NEXT_LINE, f->txt_head, txt_slc);
    } else if (k == CTRL('x')) {
      _move_cursor(PREV_LINE, f->txt_head, txt_slc);
    } else if (k == CTRL('[')) {
      _move_cursor(BEGIN_FILE, f->txt_head, txt_slc);
    } else if (k == CTRL(']')) {
      _move_cursor(END_FILE, f->txt_head, txt_slc);
    } else if (k == CTRL('u')) {
      _change_to_select_mode(txt_slc, cpy_buffer);
    
    // Insertion cases
    } else {
      if (is_arrow(k)) { 
        if (_can_move_cursor(k, txt_slc->current_line_ptr))
          _move_cursor(k, f->txt_head, txt_slc);
      } else { 
          _write_at_cursor(k, f->txt_head, txt_slc);
      }
    }

    usleep(250);
    b.display_lines_count(&b);
  }

  _destroy_txt(cpy_buffer);
  free(txt_slc);

  attron(A_REVERSE);
  mvprintw(size_y-1, 0, "Salvar o buffer modificado? (s/N) => ");
  k = getch();

  if (k == 's') {
    char filename[256];
    memset(filename, '\0', 256);
    strcpy(filename, f->filename);

    echo();

    // TODO Implement bottom bar
    move(size_y-1, 0);
    clrtoeol();

    printw("Nome do arquivo => ");
    printw(filename);

    k = getch();

    if (k != '\n') {
      memset(filename+strlen(filename), k, 1);
      getstr(filename+strlen(filename));
    }

    attroff(A_REVERSE);
    return  _write_file(f, filename);
  }
  
  return SUCCESS;
}

text* _scroll_txt(int n_lines, text* txt) {
  text* txt_crawler = txt;
  // Positive scroll case
  if (n_lines > 0) {
    size_t i = 0;
    while(txt_crawler && i < n_lines) {
      txt_crawler = txt_crawler->next_line;
      i += 1;
    } 
  // Negative scroll case
  } else if (n_lines < 0) {
    size_t i = 0;
    size_t limit = -n_lines;
    while(txt_crawler && i < limit) {
      txt_crawler = txt_crawler->prev_line;
      i += 1;
    }
  // No scroll case
  } else {
    return txt;
  }
  //if (txt_crawler) *txt = *txt_crawler;
  return txt_crawler;
}

int _clean () {
  endwin();
  return SUCCESS;
}

int _change_to_select_mode (text_slice* txt_slc, text_head* txt_buffer) {

  // Destroy old txt_buffer and create a new one
  if (txt_buffer) _destroy_txt(txt_buffer);

  txt_buffer = calloc(1, sizeof(text_head));
  txt_buffer->current_line = 0;
  txt_buffer->num_of_lines = 1;
  txt_buffer->initialized = true;

  int size_x, size_y;
  int cursor_x, cursor_y;

  getmaxyx(stdscr, size_y, size_x);
  getyx(stdscr, cursor_y, cursor_x);

  // Select current line on screen
  attron(A_REVERSE);
  mvprintw(cursor_y, 0, txt_slc->current_line_ptr->content);

  if (cursor_y != size_y-2) move(cursor_y+1, 0);

  int k = 0;
  size_t lines_offset = 0;
  text* current_line_cpy = txt_slc->current_line_ptr;

  while (k != CTRL('u')) {
    move(cursor_y, 0);
    k = getch();

    // TODO scroll case

    if (k == KEY_UP || k == CTRL('x')) {
      // "From" > "to" case not implemented (yet?)
      if(!lines_offset) continue; 

      // Scroll case, not implemented yet
      if(cursor_y == 0) continue;

      // Out of bounds
      if(!txt_slc->current_line_ptr->prev_line) continue;
      
      // Unselect current line on screen
      attroff(A_REVERSE);
      mvprintw(cursor_y, 0, txt_slc->current_line_ptr->content);
      attron(A_REVERSE);

      txt_slc->current_line_ptr = txt_slc->current_line_ptr->prev_line;
      txt_buffer->current_line -= 1;
      txt_buffer->num_of_lines -= 1;
      cursor_y -= 1;
      lines_offset -= 1;
    } else if (k == KEY_DOWN || k == CTRL('c')) {
      // Out of bounds
      if (!txt_slc->current_line_ptr->next_line) continue;

      // Scroll case, not implemented yet
      if (cursor_y == size_y-2) continue;

      // Select next line on screen
      mvprintw(cursor_y+1, 0, txt_slc->current_line_ptr->next_line->content);

      txt_slc->current_line_ptr = txt_slc->current_line_ptr->next_line;
      txt_buffer->current_line += 1;
      cursor_y += 1;
      lines_offset += 1;
    }
  }

  // Write on txt buffer
  txt_buffer->first_line = new_txt_cell(current_line_cpy->content);

  text* prev_txt = txt_buffer->first_line;
  text* new_cell = NULL;
  text** prev_next_addr = &(prev_txt->next_line);

  for(size_t i = 1;i < lines_offset;i++) {
    current_line_cpy = current_line_cpy->next_line;

    new_cell = new_txt_cell(current_line_cpy->content);
    new_cell->prev_line = prev_txt;
    *prev_next_addr = new_cell;

    prev_txt = new_cell;
    prev_next_addr = &(prev_txt->next_line);
  }

  return SUCCESS;
}

int paste_from_txt(int line, text* txt, text_head* txt_buffer) {

  // Shift all lines after current line txt_buffer->num_of_lines-1 positions down
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
