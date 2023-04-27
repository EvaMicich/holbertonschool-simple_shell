#include "main.h"
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

	endp = str + strlen(str) - 1;
	while (endp > str && (isspace((char) *endp) != 0))
	{
		endp = endp - 1;
	}
	endp[1] = '\0';

	return (str);
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
		return (NULL);
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
