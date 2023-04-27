#include "main.h"
/**
* free_arr - frees array memory
* @arr: array to free
* return: void
*/
void free_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i = i + 1;
	}
	free(arr);
}
