#include "main.h"
/**
 * list_to_array - turns linked list into an array
 * @head: head of the linked list
 *
 * Return: pointer to the new array
 */

char **list_to_array(list_t *head)
{
	char **arr;
	int i;

	arr = malloc(sizeof(*arr) * (list_len(head) + 1));
	i = 0;
	while (head != NULL)
	{
		arr[i] = strdup(head->str);
		i = i + 1;
		head = head->next;
	}
	arr[i] = NULL;
	return (arr);
}

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
		printf("%s\n", h->str);
		h = h->next;
		counter = counter + 1;
	}
	return (counter);
}

/**
 * list_len - returns the number of  elements of a list_t list
 *
 * @h: elements of the list
 *
 * Return: size_t number of nodes
 */
size_t list_len(const list_t *h)
{
	size_t counter;

	counter = 0;
	while (h != NULL)
	{
		h = h->next;
		counter = counter + 1;
	}
	return (counter);
}

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

/**
 * free_list - Realease the memory allocated for a list
 *
 * @head: A pointer to the first node of the list to free
 */
void free_list(list_t *head)
{
	if (head)
	{
		free_list(head->next);
		if (head->str)
			free(head->str);
		free(head);
	}
}
