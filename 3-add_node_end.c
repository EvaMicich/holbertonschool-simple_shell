#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lists.h"

/**
 * add_node_end - new node to end of list_t
 *
 * @head: pointer to pointer to head of the linked list
 * @str: string to add to node
 * Return: pointer to linked list
 */
list_t *add_node_end(list_t **head, const char *str)
{
	list_t *ptr;
	list_t *temp;

	if (str != NULL)
	{
		temp = malloc(sizeof(*temp));
		if (temp == NULL)
		{
			return (NULL);
		}
		temp->str = strdup(str);
		temp->len = strlen(str);
		temp->next = NULL;
		if (*head == NULL)
		{
			*head = temp;
		}
		else
		{
			ptr = *head;
			while (ptr->next != NULL)
			{
				ptr = ptr->next;
			}
			ptr->next = temp;
		}
		return (*head);

	}
	return (NULL);
}
