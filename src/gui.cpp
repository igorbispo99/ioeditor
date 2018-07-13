#include "gui.hpp"

GUI::GUI() {
	int parent_x, parent_y; 
	int bar_size = 3; 

	initscr(); 
	noecho(); 
	curs_set(FALSE); 

	// get our maximum window dimensions 
	getmaxyx(stdscr, parent_y, parent_x); 

  screen_size.first = parent_x;
  screen_size.second = parent_y;

	// set up initial windows 
	txt_field = newwin(parent_y - bar_size, parent_x, 0, 0); 
	bar_field = newwin(bar_size, parent_x, parent_y - bar_size, 0); 

	// draw to our windows 
	mvwprintw(txt_field, 0, 0, "TXT Field");
	mvwprintw(bar_field, 0, 0, "Bar Field"); 

	// refresh each window 
	wrefresh(txt_field); 
	wrefresh(bar_field); 
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

  // refresh each window 
  wrefresh(txt_field); 
  wrefresh(bar_field); 
 
}

void GUI::run() {
  while(1) {
    draw_border();
  }
}