#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lists.h"

/**
 * print_list - prints all the elements of a list_t list
 *
 * @h: elements of the list
 *
 * Return: size_t
 */
size_t print_list(const list_t *h)
{
	size_t counter;

	counter = 0;
	while (h != NULL)
	{
		if (h->str == NULL)
		{
			printf("[0] (nil)\n");
			h = h->next;
			counter = counter + 1;
		}
		printf("[%i] %s\n", h->len, h->str);
		h = h->next;
		counter = counter + 1;
	}
	return (counter);
}
