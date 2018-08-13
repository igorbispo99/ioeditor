#include "default_config.h"
#include "file_manager.h"

// Class text
text* new_txt_cell(char* txt_content) {
  text* new_cell = calloc(1, sizeof(text));

  new_cell->next_line = NULL;
  new_cell->prev_line = NULL;
  new_cell->content = calloc(strlen(txt_content)+1, sizeof(char));
  strcpy(new_cell->content, txt_content);

  return new_cell;
}

// Class file_io
int _show_txt(file* f) {
  text* txt_crawler = f->txt_head->first_line;

  while(txt_crawler) {
    puts(txt_crawler->content);
    txt_crawler = txt_crawler->next_line;
  }

  return SUCCESS;
}

int _load_txt(file* f, char* filename) {
  // Initializing text_head
  // TODO Code refactoring
  f->txt_head = (text_head*) calloc(1, sizeof(text_head));
  f->txt_head->first_line = NULL;
  f->txt_head->num_of_lines = 0;
  f->txt_head->initialized = false;

  // Initializing file
  f->current_file = fopen(filename, "r");
  f->filename = calloc(256, sizeof(char));
  strcpy(f->filename, filename);
  
  // File dont exists
  if (!f->current_file) {
    f->current_file = fopen(filename, "w+");
    
    if(!f->current_file) {
      _destroy_txt(f->txt_head);
      return ERROR;
    }

    f->txt_head->first_line = new_txt_cell(" ");
    f->txt_head->current_line = 0;
    f->txt_head->num_of_lines = 1;

    f->txt_head->initialized = true;

    f->initialized = true;
    return SUCCESS;
  }

  f->initialized = true;
  // end of Initialization

  char* line_buffer = NULL;

  size_t char_num_dump = 0;
  size_t line_size = 0;

  // Case when the file only contains the '\0'
  if (getline(&line_buffer, &char_num_dump, f->current_file) == -1) {
    f->txt_head->num_of_lines = 1;
    f->txt_head->first_line  = new_txt_cell(" ");

  } else {
    f->txt_head->num_of_lines = 1;
    f->txt_head->first_line = new_txt_cell(line_buffer);
    f->txt_head->current_line = 0;
    
    text* prev_txt = f->txt_head->first_line;
    text* new_cell = NULL;
    text** prev_next_addr = &(prev_txt->next_line);

    while((line_size = getline(&line_buffer,
          &char_num_dump, f->current_file)) != -1) {
      
      new_cell = new_txt_cell(line_buffer);
      new_cell->prev_line = prev_txt;
      *prev_next_addr = new_cell;    

      prev_txt = new_cell;
      prev_next_addr = &(prev_txt->next_line);

      f->txt_head->num_of_lines += 1;
    }
    f->txt_head->last_line = new_cell;
  }

  f->txt_head->initialized = true;
  free(line_buffer);
  return SUCCESS;
}

int _destroy_txt(text_head* txt_head) {
  if (!txt_head) return SUCCESS;

  text* txt_crawler = txt_head->first_line;
  text* aux_txt;

  while (txt_crawler) {
    aux_txt = txt_crawler->next_line;
    free(txt_crawler->content);
    free(txt_crawler);

    txt_crawler = aux_txt;
  }

  free(txt_head);
  return SUCCESS;
}

int _destroy_file(file* f) {
  if (f) {
    _destroy_txt(f->txt_head);
    fclose(f->current_file);
    free(f->filename);
    free(f);
  }
  return SUCCESS;
}

int _write_file(file* f, char* filename) {
  FILE* out_file;

  out_file = fopen(filename, "w+");
  if (!out_file) return ERROR;

  size_t len_line;
  text* txt_crawler = f->txt_head->first_line;

  while (txt_crawler) {
    len_line = strlen(txt_crawler->content);

    if (txt_crawler->content[len_line-1] == '\n')
      fprintf(out_file, "%s", txt_crawler->content);
    else
      fprintf(out_file, "%s", txt_crawler->content);
    
    txt_crawler = txt_crawler->next_line;
  }

  fclose(out_file);
  return SUCCESS;
}

int _default_color(void) {
  char* user_home_folder = getenv("HOME");
  if (!user_home_folder) return ERROR;

  char* config_folder_name = malloc(
    strlen(user_home_folder) + strlen("/.ioeditor") + 1
  );
  
  strcpy(config_folder_name, user_home_folder);
  strcat(config_folder_name, "/.ioeditor");

  char* config_file_name = malloc(
    strlen(config_folder_name) + strlen("/default_color.ioc") + 1
  );

  strcpy(config_file_name, config_folder_name);
  strcat(config_file_name, "/default_color.ioc");
  
  struct stat st = {0};

  FILE* out_config; 
  
  if (stat(config_folder_name, &st) == -1) {
    mkdir(config_folder_name, 0700);
  } else {
    if ( (out_config = fopen(config_file_name, "r")) ) {
      fclose(out_config);
      return SUCCESS;
    }
  }
  
  // Copying default color scheme to $HOME/.ioeditor/default_color.ioc
  out_config = fopen(config_file_name, "w+");
  
  fprintf(out_config, DEFAULT_COLOR_IOC_);

  free(config_file_name);
  free(config_folder_name);
  fclose(out_config);

  return SUCCESS;
}
