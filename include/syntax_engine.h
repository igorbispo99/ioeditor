#ifndef SYNTAX_ENGINE_H_
#define SYNTAX_ENGINE_H_

#include <stdlib.h>
#include <curses.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define syntax_engine_ {NULL, NULL, NULL, 0, 0, \
0, 0, 0, &_init_syntax_engine, &_print_line_color, \
&_init_color_pairs, &_destroy_syntax_engine}

enum COLOR_TYPES {
  WORDS = 1,
  FUNCTION,
  QUOTES,
  OPERATOR,
  BACKGROUND,
  NUMBERS,
  DEFAULT_COLOR
};

// Class word_list member of syntax_engine
typedef struct word_list {
  char** words;
  size_t num_of_words;
  short color;
}word_list;

// Class syntax_engine
typedef struct text_slice text_slice;
typedef struct syntax_engine syntax_engine;

struct syntax_engine {
  // Data
  FILE* dict_file;
  text_slice* scr_txt;

  // Color data
  word_list* wrd_lst;
  short op_color;
  short func_color;
  short quotes_color;
  short back_color;
  short num_color;

  // Functions
  int (*init) (syntax_engine*, char*, text_slice*);
  int (*print_line_color) (syntax_engine*, char*, int, int);
  int (*init_color) (syntax_engine*);
  int (*destroy) (syntax_engine*);
};

int _init_syntax_engine (syntax_engine*, char*, text_slice*);
int _print_line_color (syntax_engine*, char*, int, int);
int _init_color_pairs (syntax_engine*);
int _destroy_syntax_engine(syntax_engine*);
// End of class syntax_engine

char* return_char_pos(char*, char);
short eval_color(char*);
int eval_color_type(char*);
char** extract_words(char*, size_t*);
bool is_a_special_word(char*, syntax_engine*);
void print_char_color(char, int, int, int);
void error_color_file(char*);

#endif
