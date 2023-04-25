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


/**
 * main - entry point
 *
 * Return: Always 0.
 */
int main(void)
{
	char *buf;
	pid_t pid;
	int status;
	char *trimed_buf;

	
	while (1)
	{
		interactive_shell();

		buf = user_getline();
		if (buf == NULL)
		{
			return (0);
		}

		pid = fork();
		if (pid == -1)
		{
			perror("Error: ");
			return (0);
		}

		if (pid == 0)
		{
			trimed_buf = trim_whitespace(buf);
			execute_cmd(trimed_buf);
			free(trimed_buf);
		}
		else
		{
			wait(&status);
		}
		free(buf);
	}

	return (0);
}
