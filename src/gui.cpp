#include "gui.hpp"

GUI::GUI(file_ &f) {
	int parent_x, parent_y; 

	initscr(); 
	noecho(); 
  nodelay(stdscr, true);
	curs_set(FALSE); 

	// get our maximum window dimensions 
	getmaxyx(stdscr, parent_y, parent_x); 

  screen_size.first = parent_x;
  screen_size.second = parent_y;

	// set up initial windows 
	txt_field = newwin(parent_y - bar_size, parent_x, 0, 0); 
	bar_field = newwin(bar_size, parent_x, parent_y - bar_size, 0); 

  file_man = &f;
}

GUI::~GUI() {
  // clean up 
	delwin(bar_field); 
	delwin(txt_field);
	endwin(); 

}

void GUI::draw_border() {
  int x, y, i;
  WINDOW* screen;

  for (int curr_win = 0;curr_win < 2;curr_win++) {
    screen = (curr_win ? txt_field : bar_field);
    getmaxyx(screen, y, x);

    // 4 corners
    mvwprintw(screen, 0, 0, "+");
    mvwprintw(screen, y - 1, 0, "+");
    mvwprintw(screen, 0, x - 1, "+");
    mvwprintw(screen, y - 1, x - 1, "+"); 
    
    // sides
    for (i = 1; i < (y - 1); i++) { 
      mvwprintw(screen, i, 0, "|");
      mvwprintw(screen, i, x - 1, "|");
    }
    
    // top and bottom
    for (i = 1; i < (x - 1); i++) {
      mvwprintw(screen, 0, i, "-");
      mvwprintw(screen, y - 1, i, "-");
    }
  }

  // // refresh each window 
  // wrefresh(txt_field); 
  // wrefresh(bar_field); 
 
}

void GUI::put_current_txt() {
  txt_content txt = file_man->get_current_txt();

  // The first line is the border
  int current_line = 1;

  for (auto line : txt) {
    auto formated_line = line ;
    mvwprintw(txt_field, current_line, 2, formated_line.c_str());
    current_line += 1;
  }
}

void GUI::run() {
  int parent_x, parent_y, new_x, new_y;
  draw_border();

  while(1) {
    getmaxyx(stdscr, new_y, new_x);

    if (new_y != parent_y || new_x != parent_x) {
      parent_x = new_x;
      parent_y = new_y;

      wresize(txt_field, new_y - bar_size, new_x);
      wresize(bar_field, bar_size, new_x);

      mvwin(bar_field, new_y - bar_size, 0);

      wclear(stdscr);
      wclear(txt_field);
      wclear(bar_field);

      draw_border();
    }

    // draw to our windows 
    put_current_txt();
    mvwprintw(bar_field, 1, 1, "IOE Igor's Own Editor --- %s", file_man->file_name.c_str()); 

    // refresh each window 
    wrefresh(txt_field); 
    wrefresh(bar_field);
  }
}