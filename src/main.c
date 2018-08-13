#include "ui.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf ("Usage : %s <input file>\n", argv[0]);
    return ERROR;
  }

  file_io io = file_io_;
  gui g = gui_;
  syntax_engine stx = syntax_engine_;

  file *f = (file*) malloc(sizeof(file));

  char *s = argv[1];

  if (io.load_txt(f, s) == ERROR) {
    return ERROR;
  }

  if (io.write_default_color() == ERROR) {
    return ERROR;
  }

  if (stx.init(&stx, strcat(DEFAULT_CONFIG_FOLDER(), "/default_color.ioc"), NULL) == ERROR) {
    return ERROR;
  }

  g.run(f, &stx);

  io.destroy_file(f);
  g.clean();
  
  return 0;
}
