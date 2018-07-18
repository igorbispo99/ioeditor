#include "file_manager.h"

// Class file_io
int _show_txt(file* f) {
  puts(f->txt);
  return SUCCESS;
}

int _load_txt(file* f, char* filename) {

  // Initializing file
  f->txt = malloc(1);
  f->txt[0] = '\0';
  f->txt_size = 1;
  f->current_file = fopen(filename, "r+");
  
  if (!f->current_file) {
    free(f->txt);
    return ERROR;
  }

  f->initialized = true;
  // end of Initialization

  char* line_buffer = malloc(256);
  if (!line_buffer) {
    _destroy_file(f);
    return ERROR;
  }

  size_t char_num_dump = 0;
  size_t total_size = 0;
  size_t line_size = 0;

  while((line_size = getline(&line_buffer,
        &char_num_dump, f->current_file)) != -1) {

    total_size += line_size;
    f->txt = realloc(f->txt, total_size + 1);
    f->txt = strcat(f->txt, line_buffer);
  }
  
  return SUCCESS;
}

int _destroy_file(file* f) {
  if (f->initialized) {
    free(f->txt);
    fclose(f->current_file);
  }
  return SUCCESS;
}