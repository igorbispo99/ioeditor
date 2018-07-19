#include "gui.h"
#include "file_manager.h"
#include <stdio.h>

int main() {
  file_io io = file_io_;
  gui g = gui_;

  file *f = (file*) malloc(sizeof(file));
  char *s = "example.c";  

  io.load_txt(f, s);
  g.run(f);

  io.destroy_file(f);
  return 0;
}