#include "lists.h"
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/**
 *
 *
 */
void free_arr(char **arr)
{
    int i;

    for (i = 0; arr[i] != NULL; i++)
    {
        free(arr[i]);
    }

    free(arr);
}

/**
 * interactive_shell - check if it an interactive or not
 *
 * Description: check if it an interactive or not
 * Return: nothing
 **/
void interactive_shell(void)
{
	if (isatty(STDIN_FILENO) == 1)
	{
		printf("#cisfun$ ");
	}

	return;
}


/**
 * execute_cmd - execute the command
 * @cmd: command
 *
 * Description: execute the command
 * Return: On success, no return, on erro, return -1
 **/
int execute_cmd(char **cmd_arr)
{
	extern char **environ;
	if (cmd_arr == NULL || cmd_arr[0] == NULL)
	{
		return (-1);
	}

	if (execve(cmd_arr[0], cmd_arr, environ) == -1)
	{
		perror(cmd_arr[0]);
		_exit(1);
	}

	return (0);
}


/**
 * user_getline - get the user input
 *
 * Description: get the user input
 * Return: On success return the buf, on error return NULL
 **/
char *user_getline(void)
{
	char *buf;
	size_t bufsize;
	ssize_t getline_return;

	buf = NULL;
	bufsize = 0;

	getline_return = getline(&buf, &bufsize, stdin);
	if (getline_return == EOF)
	{
		free(buf);
		return (NULL);
	}
	return (buf);
}

/**
 * trim_whitespace - remove delimiters from the input
 * @str: user input
 *
 * Description - remove delimiters from the input
 * Return: trimed user input
 **/
char *trim_whitespace(char *str)
{
	char *endp;

	while (isspace((char) *str) != 0)
	{
		str = str + 1;
	}

	if (*str == '\0')
	{
		return (str);
	}

	endp = str + strlen(str) -1;
	while (endp > str && (isspace((char) *endp) != 0))
	{
		endp = endp - 1;
	}
	endp[1] = '\0';

	return (str);
}

int count_token(char *str, char *delim )
{
	char *tokenized_str;
	char *token;
	int i;

	tokenized_str = strdup(str);

	i = 0;
	token = strtok(tokenized_str, delim);
	while (token != NULL)
	{
		token = strtok(NULL, delim);
		i = i + 1;
	}

	free(tokenized_str);
	return (i);
}
void *xmalloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	printf("malloc return is : %p\n", ptr);
	return (ptr);
}
void xfree(void *ptr)
{
	printf("free address: %p\n", ptr);
	free(ptr);
}


char **list_to_array(list_t *head)
{
	char **arr;
	int i;

	arr = malloc(sizeof(*arr) * (list_len(head) + 1));
	i = 0;
	while (head != NULL)
	{
		arr[i] = strdup(head->str);
		i = i + 1;
		head = head->next;
	}
	arr[i] = NULL;
	return(arr);
}


/**
 * string_to_arr - split the user input and make an array of strings, 
 * optional string concat
 * @str: user input
 * @delim: the delimiter
 * @cmd: command to concat onto each element
 * @switch_on_path: 1 turns on command string concat. 0 turns concat off
 * Description - split the user input and make an array of strings
 * Return: an array of tokens
 **/
char **string_to_arr(char *str, char *delim, char *cmd, int switch_on_path)
{
	char **token_arr;
	int i;
	char *token;
	char *path_cmd;
	list_t *head;

	head = NULL;

	if (switch_on_path == 1 && cmd == NULL)
	{
		return(NULL);
	}

	if (switch_on_path == 0)
	{
		i = 0;
		token = strtok(str, delim);
		while (token != NULL)
		{
			add_node_end(&head, token);
			token = strtok(NULL, delim);
			i = i + 1;
		}
	}
	else if (switch_on_path == 1)
	{
		token = strtok(str, delim);
		while (token != NULL)
		{
			path_cmd = malloc(sizeof(*path_cmd) * (strlen(token) + strlen(cmd) + 2));
			if (path_cmd == NULL)
			{
				return (NULL);
			}
			path_cmd[0] = '\0';
			path_cmd = strcat(path_cmd, token);
			path_cmd = strcat(path_cmd, "/");
			path_cmd = strcat(path_cmd, cmd);

			add_node_end(&head, path_cmd);
			free(path_cmd);
			token = strtok(NULL, delim);
		}
	}
	token_arr = list_to_array(head);
	free_list(head);
	return (token_arr);
}


/**
 * find_path - find the full path of a command
 * @cmd: the command that need to be searched
 *
 * Description - find the full path of a command
 * Return: on success, return command, on failure, return NULL
 **/
char *find_path(char* cmd)
{

	char *original_path;
	char *path;
	char **path_arr;
	char *found_path;
	int i;
	struct stat st;

	original_path = getenv("PATH");
	path = strdup(original_path);
	path_arr = string_to_arr(path, ":", cmd, 1);
	i = 0;
	while (path_arr[i] != NULL)
	{
		if (stat(path_arr[i], &st) == 0)
		{
			found_path = strdup(path_arr[i]);
			return(found_path);
		}
		i = i + 1;
	}
	free_arr(path_arr);
	return (NULL);
}

char **check_cmd_arr(char **cmd_arr)
{
	int exist_in_current;
	struct stat st;
	char *path_cmd;

	if (cmd_arr[0] == NULL)
	{
		return(NULL);
	}
	exist_in_current = stat(cmd_arr[0], &st);
	if (exist_in_current == 0)
	{
		return(cmd_arr);
	}
	else
	{
		path_cmd = find_path(cmd_arr[0]);
		if (path_cmd == NULL)
		{
			return(NULL);
		}
		free(cmd_arr[0]);
		cmd_arr[0] = path_cmd;
		return(cmd_arr);
	}
}

/**
 * create_child - fork a child process and execute program
 * @cmd_arr: an array of commands
 *
 * Description - fork a child process and execute program
 * Return: nothing
 **/
void create_child(char **cmd_arr)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Error: ");
		return;
	}

	if (pid == 0)
	{
		execute_cmd(cmd_arr);
	}
	else
	{
		wait(&status);
	}
}


/*build in relative functions begin*/
typedef struct built_in
{
	char *built_in_cmd;
	void (*fn)(void);
} built_in_t;

/*exits the shell*/
void built_in_exit(void)
{
	_exit(1);
}

/*prints the current environment*/
void built_in_env(void)
{
	extern char **environ;
	int i;

	i = 0;
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i = i + 1;
	}
}

/*get the right function to use*/
void (*get_built_in_fn(char *str))(void)
{
    built_in_t built_ins[] = {
        {"exit", built_in_exit},
        {"env", built_in_env},
        {NULL, NULL}
    };
	int i;

	i = 0;
	while (built_ins[i].built_in_cmd != NULL)
	{
		if (strcmp(built_ins[i].built_in_cmd, str) == 0)
		{
			return (built_ins[i].fn);
		}
		i = i + 1;
	}

    return (NULL);
}

/*create a built in cmd checker function*/
int built_in_checker(char **cmd_arr)
{
    void (*built_in_fn)(void);

	built_in_fn = get_built_in_fn(cmd_arr[0]);
	if (built_in_fn == NULL)
	{
		return (-1);
	}
	built_in_fn();
	return(0);
}
/*build in relative functions end*/



/**
 * main - entry point
 *
 * Return: Always 0.
 */
int main(void)
{
	char *buf;
	char *trimed_buf;
	char **cmd_arr;
	int bcmd_checker;

	while (1)
	{
		interactive_shell();
		buf = user_getline();
		if (buf == NULL)
		{
			return (0);
		}
		trimed_buf = trim_whitespace(buf);
		cmd_arr = string_to_arr(trimed_buf, " ", NULL, 0);

		/* is the cmd a built in cmd? yes 0, no -1 */
		bcmd_checker = built_in_checker(cmd_arr);
		if (bcmd_checker == -1)
		{
			cmd_arr = check_cmd_arr(cmd_arr);
			if (cmd_arr != NULL)
			{
				create_child(cmd_arr);
				free_arr(cmd_arr);
			}
		}
		free(buf);
	}

	return (0);
}
