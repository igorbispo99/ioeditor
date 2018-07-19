#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error handling
enum SIGNAL {
  SUCCESS,
  ERROR
};

//Class file
typedef struct file {
  char* txt;
  FILE* current_file;
  size_t txt_size;
  bool initialized;
}file;

// Class file_io
#define file_io_ {&_show_txt, &_load_txt, &_destroy_file}

typedef struct file_io file_io;

struct file_io {
  int (*show_txt) (file*);
  int (*load_txt) (file*, char*);
  int (*destroy_file) (file*);
};

int _show_txt(file*);
int _load_txt(file*, char*);
int _destroy_file(file*);

#endif