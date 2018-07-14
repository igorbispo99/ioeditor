#ifndef GUI_
#define GUI_

#include <ncurses.h>
#include <unistd.h>

#include "file_manager.hpp"

class GUI {
  public:
    GUI(file_ &f);
    ~GUI();
    void run();
    void put_current_txt();
  private:
    WINDOW* bar_field;
    WINDOW* txt_field; 
    file_* file_man;

    const int bar_size = 3;
    std::pair<int, int> screen_size;

    void draw_border();
};

#endif
