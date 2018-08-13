#ifndef BOT_BAR_H_
#define BOT_BAR_H_

#include "file_manager.h"
#include "gui.h"

// Class bar
#define bar_ {0, 0, 0, 0, NULL, NULL, \
&_init_bar, &_display_splash, \
&_display_lines_count, &_display_bar, \
&_destroy_bar}

#define VER_SPLASH "Igor's Own Editor v0.2.5"

typedef struct bar bar;

struct bar {
  // Data
  size_t splash_size;
  size_t lines_count_size;
  size_t filename_size;
  size_t current_line;

  file* bar_f;
  text_slice* txt_slc;

  // Functions
  int (*init) (bar*, file*, text_slice*);
  int (*display_splash) (bar*);
  int (*display_lines_count) (bar*);
  int (*display_bar) (bar*);
  int (*destroy_bar) (bar*);
};

int _init_bar (bar*, file*, text_slice*);
int _display_splash(bar* b);
int _display_lines_count(bar *b);
int _display_bar(bar *b);
int _destroy_bar(bar *b);

#endif
