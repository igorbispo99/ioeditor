#include "syntax_engine.h"
#include "gui.h"

void print_char_color(char c, int pos_y, int pos_x, int color_pair_n){
  attron(COLOR_PAIR(color_pair_n));
  mvaddch(pos_y, pos_x, c);
  attroff(COLOR_PAIR(color_pair_n)); 
}

bool is_operator(char c) {
  char operators[] = {'+','-','=','*','!','<','>','|','&'};
  size_t num_of_operators = strlen(operators);

  for (size_t i = 0;i < num_of_operators;i++) {
    if (operators[i] == c)
      return true;
  }

  return false;
}

bool is_a_special_word(char* word, syntax_engine* sytx) {
  if(!word || *word == '\n') return false;
  size_t limit = sytx->wrd_lst->num_of_words;
  for (size_t i = 0;i < limit;i++) {
    if(!strcmp(word, sytx->wrd_lst->words[i]))
      return true;
  }
  return false;
}

char** extract_words(char* str_in, size_t* num_of_words) {
  if(!str_in) return NULL;
  char** wrds_list = calloc (256, sizeof(char*));
  char** wrd_crawler;

  for(wrd_crawler = wrds_list;
      (*wrd_crawler = strsep(&str_in, " ")) != NULL;) {
    if(**wrd_crawler != '\0')
      if(++wrd_crawler >= &wrds_list[256])
        break;
    *num_of_words += 1;
  }

  return wrds_list;
}

char* return_char_pos (char* str, char c) {
  size_t limit = strlen(str);

  for (size_t i = 0;i < limit;i++) 
    if (str[i] == c)
      return &(str[i]);

    return NULL;
}
 
short eval_color (char* color_name) {
  if (!strcmp(color_name, "RED")) return COLOR_RED;
  if (!strcmp(color_name, "BLUE")) return COLOR_BLUE;
  if (!strcmp(color_name, "YELLOW")) return COLOR_YELLOW;
  if (!strcmp(color_name, "WHITE")) return COLOR_WHITE;
  if (!strcmp(color_name, "CYAN")) return COLOR_CYAN;
  if (!strcmp(color_name, "GREEN")) return COLOR_GREEN;
  if (!strcmp(color_name, "MAGENTA")) return COLOR_MAGENTA;
  if (!strcmp(color_name, "BLACK")) return COLOR_BLACK;
  return -1;
}

int eval_color_type (char* color_type) {
  if (*color_type == ';') return WORDS;
  if (!strcmp(color_type, "function\n")) return FUNCTION;
  if (!strcmp(color_type, "quotes\n")) return QUOTES;
  if (!strcmp(color_type, "operator\n")) return OPERATOR;
  if (!strcmp(color_type, "background\n")) return BACKGROUND;
  if (!strcmp(color_type, "numbers\n")) return NUMBERS;
  return -2;
}

void error_color_file(char* filename) {
  fprintf(stderr, "Erro abrindo o arquivo de color scheme \"%s\" . \n", filename);
}

int _init_syntax_engine (syntax_engine* sytx, char* dict_name,
    text_slice* txt_slc) {

  FILE* dict = fopen(dict_name, "r");
  if(!dict) return ERROR;

  //syntax_engine* sytx = calloc(1, sizeof(syntax_engine));

  char* line_buffer = NULL;
  char* separator_pos = NULL;

  size_t char_num_dump = 0;
  size_t num_of_words = 0;

  while(getline(&line_buffer, &char_num_dump, dict) != -1) {
    separator_pos = return_char_pos(line_buffer, ':');
    if (!separator_pos || *(separator_pos+1) == '\0')  {
      fclose(dict);
      error_color_file(line_buffer);
      return ERROR;     
    }

    *separator_pos = '\0';
    short current_color = eval_color(line_buffer);
    if (current_color == -1) {
      fclose(dict);
      error_color_file(line_buffer);
      return ERROR;
    }

    int color_type = eval_color_type(separator_pos+1);
    if (color_type == -2) {
      fclose(dict);
      error_color_file(line_buffer);
      return ERROR;    
    }

    switch (color_type) {
      case FUNCTION:
        sytx->func_color = current_color;
        continue;
      case OPERATOR:
        sytx->op_color = current_color;
        continue;
      case QUOTES:
        sytx->quotes_color = current_color;
        continue;
      case BACKGROUND:
        sytx->back_color = current_color;
        continue;
      case NUMBERS:
        sytx->num_color = current_color;
        continue;
      default: //case WORDS
        if(sytx->wrd_lst) {
          fclose(dict);
          error_color_file(line_buffer);
          return ERROR;    
        } 
        sytx->wrd_lst = calloc(1, sizeof(word_list));
        sytx->wrd_lst->color = current_color;
        sytx->wrd_lst->words = extract_words(strndup(separator_pos+2,
                               strlen(separator_pos+2)-1), &num_of_words);
        sytx->wrd_lst->num_of_words = num_of_words;
    }
  } 

  sytx->dict_file = dict;
  sytx->scr_txt = txt_slc;
  
  return SUCCESS;
}

int _print_line_color (syntax_engine* sytx, char* str_line,
    int pos_y, int pos_x) {
    
  size_t num_of_words = 0;
  size_t str_size = strlen(str_line);
  char* str_copy = strdup(str_line);
  char** get_words_addr = extract_words(str_copy, &num_of_words);
  bool is_open_quotes = false;

  for (size_t i = pos_x, j = 0; i < str_size;i++) {
    if(str_copy + i == get_words_addr[j+1]) j += 1;

    if (str_copy[i] == '\'' || str_copy[i] == '\"') is_open_quotes = !is_open_quotes;
    if (is_open_quotes) attron(A_BOLD);

    if(isdigit(str_copy[i])) {
      print_char_color(str_line[i], pos_y, i, NUMBERS);
    } else if (is_a_special_word(get_words_addr[j], sytx)) {
      print_char_color(str_line[i], pos_y, i, WORDS); 
    } else if (is_operator(str_line[i])) {
      print_char_color(str_line[i], pos_y, i, OPERATOR);
    } else {
      print_char_color(str_line[i], pos_y, i, DEFAULT_COLOR); 
    }

    if (!is_open_quotes) attroff(A_BOLD);
  }
  return SUCCESS;
}

int _init_color_pairs(syntax_engine* sytx) {
  init_pair(WORDS, sytx->wrd_lst->color, sytx->back_color);
  init_pair(NUMBERS, sytx->num_color, sytx->back_color);
  init_pair(FUNCTION, sytx->func_color, sytx->back_color);
  init_pair(QUOTES, sytx->quotes_color, sytx->back_color);
  init_pair(OPERATOR, sytx->op_color, sytx->back_color);

  init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
  return SUCCESS;
}
