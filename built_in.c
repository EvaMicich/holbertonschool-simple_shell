#include "main.h"

/**
* built_in_exit -  exits
*
* returns: void
*/
void built_in_exit(void)
{
	_exit(0);
}

/**
* built_in_env - prints the current environment
*
* return: void
*/
void built_in_env(void)
{
	int i;

	i = 0;
	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i = i + 1;
	}
}

/**
* get_built_in_fn - find function to match built in cmd
* @str: command to check
*
* Return: success pointer to function, command not found: NULL
*/
void (*get_built_in_fn(char *str))(void)
{
	built_in_t built_ins[] = {
		{"exit", built_in_exit},
		{"env", built_in_env},
		{NULL, NULL}
	};
	int i;

	i = 0;
	while (built_ins[i].built_in_cmd != NULL && str != 0)
	{
		if (strcmp(built_ins[i].built_in_cmd, str) == 0)
		{
			return (built_ins[i].fn);
		}
		i = i + 1;
	}

	return (NULL);
}

/**
* built_in_checker - built in cmd checker function
* @cmd_arr: array containing command to check
*
* Return: 0 success, -1 fail.
*/
int built_in_checker(char **cmd_arr)
{
	void (*built_in_fn)(void);

	built_in_fn = get_built_in_fn(cmd_arr[0]);
	if (built_in_fn == NULL)
	{
		return (-1);
	}
	built_in_fn();
	return (0);
}
