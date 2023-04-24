#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

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
int execute_cmd(char *cmd)
{ 
	char *user_input[2];
	extern char **environ;

	user_input[0] = cmd;
	user_input[1] = NULL;

	if (execve(user_input[0], user_input, environ) == -1)
	{
		perror("Error: ");
		return (-1);
	}

	return (0);
}


/**
 * user_getline - get the user input
 *
 * Description: get the user input
 * Return: On success return the buf, on error return NULL
 **/
char * user_getline(void)
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
 * count_token - counting the number of strings that have been split
 * @str: user input string
 * @delim: delimiter
 *
 * Description: counting the number of strings that have been split
 * Return: the number of strings
 **/
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
	
	return (i);
}


/**
 * trim_user_input - remove delimiters from the input
 * @str: user input
 *
 * Description - remove delimiters from the input
 * Return: trimed user input
 **/
char **trim_user_input(char *str, char *delim)
{
	char **token_array;
	int num_token;
	int i;
	char *token;

	num_token = count_token(str, delim);

	token_array = malloc(sizeof(*token_array) * num_token);
	if (token_array == NULL)
	{
		return (NULL);
	}
	
	i = 0;
	token = strtok(str, delim);
	while (token != NULL)
	{
		token_array[i] = token;
		token = strtok(NULL, delim);
		i = i + 1;
	}

	return (token_array);
}



/**
 * main - fork example
 *
 * Return: Always 0.
 */
int main(__attribute__ ((unused)) int argc, __attribute__ ((unused)) char **argv)
{
	char *buf;
	pid_t pid;
	int status;
	char **cmd_array;
	
	while (1)
	{
		interactive_shell();

		buf = user_getline();
		if (buf == NULL)
		{
			return (0);
		}

		cmd_array = trim_user_input(buf, "\n");


		pid = fork();
		if (pid == -1)
		{
			perror("Error: ");
			return (0);
		}

		if (pid == 0)
		{
			execute_cmd(cmd_array[0]);
		}
		else
		{
			wait(&status);
		}
	}

	free(buf);
	return (0);
}
