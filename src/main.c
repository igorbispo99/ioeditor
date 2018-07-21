#include "gui.h"
#include "file_manager.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf ("Usage : %s <input file>\n", argv[0]);
    return ERROR;
  }

  file_io io = file_io_;
  gui g = gui_;

  file *f = (file*) malloc(sizeof(file));
  char *s = argv[1];  

  io.load_txt(f, s);
  g.run(f);

  io.destroy_file(f);
  g.clean();
  
  return 0;
}