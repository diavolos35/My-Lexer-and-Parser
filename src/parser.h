#ifndef PARSER_H
#define PARSER_H

#endif

typedef struct var_t {
    char name[MAX_LEX_SIZE];
    int type;
    int value_int;
    double value_float;
    char value_bool[7];
    char value_char;
    char value_string[MAX_LEX_SIZE];
} var_t;

typedef struct list_var_t {
    size_t size;
    size_t length;
    var_t *data;
} list_var_t;

typedef struct arr_t {
    char name[MAX_LEX_SIZE];
    int type;
    size_t size;
    var_t *data;
} arr_t;

typedef struct list_arr_t {
    size_t size;
    size_t length;
    arr_t *data;
} list_arr_t;

extern list_lexeme_t *lex_list;

extern void run_lexer();

double arithmetic_operation(size_t *current_ptr);

var_t define(size_t *current_ptr);

arr_t *define_arr(size_t *current_ptr);

list_var_t *list_var_t_create(size_t s);

list_var_t *list_var_t_enlarge(list_var_t *list1);

void list_var_t_add(list_var_t *list, var_t item);

int if_arithmetic_operation(size_t *current_ptr);

int do_if_define(size_t *current_ptr);

int do_if_define_arr(size_t *current_ptr);

int do_if_assignment(size_t *current_ptr);

void do_assignment(size_t *current_ptr);

int get_index_of_var(char *name);

char *do_bool_operation(size_t *current_ptr);

int if_bool_operation(size_t *current_ptr);

int print(size_t *current_ptr);

arr_t *arr_t_create(enum token_types t, char *name, size_t s);

list_arr_t *list_arr_t_create(size_t s);

void list_arr_t_add(list_arr_t *list1, arr_t item);

list_arr_t *list_arr_t_enlarge(list_arr_t *list1);

int get_index_of_arr(char *name);

var_t *get_arr_element(size_t *current_ptr);

char *do_catenation(size_t *current_ptr);

int if_catenation(size_t *current_ptr);

int statement(size_t *current_ptr);

char *is_greater(size_t *current_ptr);

char *is_less(size_t *current_ptr);

char *is_equal(size_t *current_ptr);

int is_comparison(size_t *current_ptr);

int do_if_if(size_t *current_ptr);

int do_if_while(size_t *current_ptr);
