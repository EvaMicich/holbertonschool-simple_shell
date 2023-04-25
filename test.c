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

char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;


    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( frontp != str && endp == frontp )
            *str = '\0';
    else if( str + len - 1 != endp )
            *(endp + 1) = '\0';


    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}


/**
 * main - fork example
 *
 * Return: Always 0.
 */
int main(void)
{
	char *buf;
	pid_t pid;
	int status;
	char **cmd_array;
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
			trimed_buf = trim(buf);
			execute_cmd(trimed_buf);
		}
		else
		{
			wait(&status);
		}
	}

	free(buf);
	return (0);
}
