#include "file_manager.h"

// Class file_io
int _show_txt(file* f) {
  for (size_t i = 0;i < f->txt->num_of_lines;i++) {
    puts(f->txt->lines[i]);
  }
  return SUCCESS;
}

int _load_txt(file* f, char* filename) {

  // Initializing text
  // TODO Code refactoring
  f->txt = (text*) calloc(1, sizeof(text));
  f->txt->lines = (char**) malloc(sizeof(char*));
  f->txt->num_of_lines = 0;


  // Initializing file
  f->current_file = fopen(filename, "r");
  f->filename = filename;
  
  // File dont exists
  if (!f->current_file) {
    f->current_file = fopen(filename, "w+");
    
    if(!f->current_file) {
      _destroy_txt(f->txt);
      return ERROR;
    }

    f->txt->num_of_lines = 1;
    f->txt->lines[0] = calloc(2, sizeof(char));
    f->txt->initialized = true;

    f->initialized = true;
    return SUCCESS;
  }

  f->initialized = true;
  // end of Initialization

  char* line_buffer = NULL;
  // if (!line_buffer) {
  //   _destroy_file(f);
  //   return ERROR;
  // }

  size_t char_num_dump = 0;
  size_t* l_num = &f->txt->num_of_lines;
  size_t line_size = 0;

  while((line_size = getline(&line_buffer,
        &char_num_dump, f->current_file)) != -1) {

    f->txt->lines[*l_num] = calloc(sizeof(char), line_size + 1);
    memcpy(f->txt->lines[*l_num], line_buffer, line_size);

    // if (f->txt->lines[*l_num][line_size-1] == '\n')  
    //   f->txt->lines[*l_num][line_size-1] = '\0';
      
    *l_num = *l_num + 1;
    f->txt->lines = realloc(f->txt->lines, sizeof(char*)*(*l_num + 1));
  }

  f->txt->initialized = true;
  free(line_buffer);
  return SUCCESS;
}

int _destroy_txt(text* txt) {
  for (size_t i = 0; i < txt->num_of_lines; i++) {
    free(txt->lines[i]);
  }
  free(txt->lines);
  free(txt);

  return SUCCESS;
}

int _destroy_file(file* f) {
  if (f) {
    if (f->txt) {
      _destroy_txt(f->txt);
    }
    fclose(f->current_file);
    free(f);
  }
  return SUCCESS;
}

int _write_file(file* f, char* filename) {
  // For now, the original file will not be replace
  FILE* out_file;

  char* out_file_name = filename;
  // char* out_file_name = calloc(256, sizeof(char));
  // if (!out_file_name){
  //   return ERROR;
  // }
  // strcpy(out_file_name, strcat(f->filename, ".out"));

  out_file = fopen(out_file_name, "w+");
  if (!out_file) {
    //free(out_file_name);
    return ERROR;
  }

  size_t len_line;

  for (size_t i = 0; i < f->txt->num_of_lines; i++) {
    len_line = strlen(f->txt->lines[i]);
    // TODO Refactorize code
    if (f->txt->lines[i][len_line-1] == '\n')
      fprintf(out_file, "%s", f->txt->lines[i]);
    else
      fprintf(out_file, "%s\n", f->txt->lines[i]);
  }

  fclose(out_file);
  //free(out_file_name);
  return SUCCESS;
}