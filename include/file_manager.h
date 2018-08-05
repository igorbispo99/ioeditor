#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//const size_t MAX_LINE_SIZE = 256;

// Error handling
enum SIGNAL {
  SUCCESS,
  ERROR
};

//Class text (linked list)
typedef struct text_head text_head;
typedef struct text text;

struct text_head {
  text* first_line;
  text* last_line;
  size_t num_of_lines;
  size_t current_line;
  bool initialized;
};

struct text {
  text* next_line;
  text* prev_line;
  char* content;
};

text* new_txt_cell(char*);

//Class file
typedef struct file {
  text_head* txt_head;
  FILE* current_file;
  bool initialized;
  char* filename;
}file;

// Class file_io
#define file_io_ {&_show_txt, &_load_txt, &_destroy_txt, &_destroy_file, &_write_file}

typedef struct file_io file_io;

struct file_io {
  int (*show_txt) (file*);
  int (*load_txt) (file*, char*);
  int (*destroy_txt) (text_head*);
  int (*destroy_file) (file*);
  int (*write_file) (file*, char*);
};

int _show_txt(file*);
int _load_txt(file*, char*);
int _destroy_txt (text_head*);
int _destroy_file(file*);
int _write_file(file*, char*);



#endif
