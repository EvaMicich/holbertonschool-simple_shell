#include "main.h"
/**
 * execute_cmd - execute the command
 * @cmd_arr: array containing command
 *
 * Description: execute the command
 * Return: On success, no return, on erro, return -1
 **/
int execute_cmd(char **cmd_arr)
{
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
 * find_path - find the full path of a command
 * @cmd: the command that need to be searched
 *
 * Description - find the full path of a command
 * Return: on success, return command, on failure, return NULL
 **/
char *find_path(char *cmd)
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
			return (found_path);
		}
		i = i + 1;
	}
	free_arr(path_arr);
	return (NULL);
}
/**
 * check_cmd_arr - checks that the command is valid, and find path if needed
 * @cmd_arr: array of strings from the
 *
 * return: array on success, NULL on failure
 */
char **check_cmd_arr(char **cmd_arr)
{
	int exist_in_current;
	struct stat st;
	char *path_cmd;

	if (cmd_arr[0] == NULL)
	{
		return (NULL);
	}
	exist_in_current = stat(cmd_arr[0], &st);
	if (exist_in_current == 0)
	{
		return (cmd_arr);
	}
	else
	{
		path_cmd = find_path(cmd_arr[0]);
		if (path_cmd == NULL)
		{
			return (NULL);
		}
		free(cmd_arr[0]);
		cmd_arr[0] = path_cmd;
		return (cmd_arr);
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
