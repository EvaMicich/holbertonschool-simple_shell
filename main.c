#include "main.h"

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
