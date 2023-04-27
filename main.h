#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

/**
 * struct list_s - singly linked list
 * @str: string - (malloc'ed string)
 * @len: length of the string
 * @next: points to the next node
 *
 * Description: singly linked list node structure
 */
typedef struct list_s
{
	char *str;
	unsigned int len;
	struct list_s *next;
} list_t;

/**
 * struct built_in - Struct built_in
 *
 * @built_in_cmd: The cmd
 * @fn: The function associated
 */
typedef struct built_in
{
	char *built_in_cmd;
	void (*fn)(void);
} built_in_t;

extern char **environ;

/* free_memory */
void free_arr(char **arr);

/* linked_list */
size_t list_len(const list_t *h);
list_t *add_node_end(list_t **head, const char *str);
size_t print_list(const list_t *h);
char **list_to_array(list_t *head);
void free_list(list_t *head);

/* shell_cmd */
void interactive_shell(void);
char *user_getline(void);
char *trim_whitespace(char *str);
char **string_to_arr(char *str, char *delim, char *cmd, int switch_on_path);

/* path_child */
int execute_cmd(char **cmd_arr);
char *find_path(char* cmd);
char **check_cmd_arr(char **cmd_arr);
void create_child(char **cmd_arr);

/* built_in */
void built_in_exit(void);
void built_in_env(void);
void (*get_built_in_fn(char *str))(void);
int built_in_checker(char **cmd_arr);

#endif
