#ifndef GUI_
#define GUI_

#include <ncurses.h>
#include <unistd.h>

#include "file_manager.hpp"

class GUI {
  public:
    GUI();
    ~GUI();
    void run();
  private:
    WINDOW* bar_field;
    WINDOW* txt_field; 

    std::pair<int, int> screen_size;

    void draw_border();
};

#endif
