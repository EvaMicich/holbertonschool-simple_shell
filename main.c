#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

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
		return (-1);
	}

	free(cmd_arr);
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
	int num_token;
	int i;
	char *token;
	char *path_cmd;

	num_token = count_token(str, delim);

	token_arr = malloc(sizeof(*token_arr) * (num_token + 1));
	if (token_arr == NULL)
	{
		free(token_arr);
		return (NULL);
	}
	if (switch_on_path == 0)
	{
		i = 0;
		token = strtok(str, delim);
		while (token != NULL)
		{
			token_arr[i] = token;
			token = strtok(NULL, delim);
			i = i + 1;
		}
		token_arr[i] = NULL;
	}
	else if (switch_on_path == 1)
	{
		if (cmd == NULL)
		{
			return(NULL);
		}
		i = 0;
		token = strtok(str, delim);
		while (token != NULL)
		{
			path_cmd = malloc(sizeof(*path_cmd) * (strlen(token) + strlen(cmd) + 2));
			if (path_cmd == NULL)
			{
				free(path_cmd);
				return (NULL);
			}
			path_cmd[0] = '\0';
			path_cmd = strcat(path_cmd, token);
			path_cmd = strcat(path_cmd, "/");
			path_cmd = strcat(path_cmd, cmd);

			token_arr[i] = path_cmd;
			token = strtok(NULL, delim);
			i = i + 1;
		}
		token_arr[i] = NULL;
	}

	return (token_arr);
}

char *_getenv(const char *name)
{
	extern char **environ;
	int i;
	char *token;

	i = 0;
	while (environ[i] != NULL)
	{
		token = strtok(environ[i], "=");
		if (strcmp(token, name) == 0)
		{
			token = strtok(NULL, "=");
			return (token);
		}
		i = i + 1;
	}

	return (NULL);
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
	char *path;
	char **path_arr;
	int i;
	struct stat st;

	path = _getenv("PATH");
	path_arr = string_to_arr(path, ":", cmd, 1);
	i = 0;
	while (path_arr[i] != NULL)
	{
		if (stat(path_arr[i], &st) == 0)
		{
			return (path_arr[i]);
		}
		i = i + 1;
	}
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
 * main - entry point
 *
 * Return: Always 0.
 */
int main(void)
{
	char *buf;
	char *trimed_buf;
	char **cmd_arr;

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
		cmd_arr = check_cmd_arr(cmd_arr);
		if (cmd_arr != NULL)
		{
			create_child(cmd_arr);
		}
		free_arr(cmd_arr);
		free(buf);
	}

	return (0);
}
