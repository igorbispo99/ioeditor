#include "gui.h"
#include "file_manager.h"
#include <stdio.h>

int main() {
  file_io io = file_io_;
  file *f = (file*) malloc(sizeof(file));

  char *s = "example.c";  

  io.load_txt(f, s);
  io.show_txt(f);

  return 0;
}