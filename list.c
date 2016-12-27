#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
* Purpose:    Creates a new empty list.
* Returns:    A pointer to the new list.
* Comments:   List needs to be free'd when finished using.
*/
LinkedList *list_empty(void)
{

	LinkedList *l = malloc(sizeof(struct llist));
	l->head = malloc(sizeof(struct node));
	l->head->next = NULL;
	l->freeFunc=NULL;

	return l;
}

/*
Purpose:    Install's a memory handler to take over the responsibility to
			free memory for values that don't exist anymore.
Parameters: *l - list & *m - a function-pointer to a function.
*/
void list_setMemHandler(LinkedList *l, memFreeFunc *m)
{
	l->freeFunc = m;
}

/*
Purpose:    Return's list first position.
Parameters: *l - list.
Returns:    First position of the list.
*/
node *list_first(LinkedList *l)
{
	return (l->head);
}

/*
Purpose:    Return's list next position.
Parameters: *l - list.
Returns:    Next position of the list.
*/
node *list_next(node *n)
{
	return n->next;
}

/*
Purpose:    Check's if list is empty.
Parameters: *l - list.
Returns:    True if list is empty; otherwise false.
*/
bool list_isEmpty(LinkedList *l)
{
	return ((l->head->next) == NULL);
}

/*
Purpose:    Enter's value in list
Parameters: *l - list.
Returns:    Value
*/
node *list_insert(node *n, void *v)
{
	node *n2 = malloc(sizeof(struct node));
	n2->next = n->next;
	n2->value = v;
	n->next = n2;

	return n2;
}

/*
Purpose: Remove the list.
Parameters: node pointer, LinkedList
Returns: node pointer.
*/
node *list_remove(node *n, LinkedList *l)
{
	node *temp = n->next->next;

	if(l->freeFunc != NULL){
	 l->freeFunc(n->next->value);
	 }

	free(n->next);

	if(temp != NULL) {
		n->next = temp;
	} else {
		n->next = NULL;
	}
	return n;
}

/*
Purpose: Inspect data at specific place in the list.
Parameters: node pointer
Returns: value.
*/
void *list_inspect(node *n)
{
	return (n->next->value);
}

/*
Purpose: Check if this value is last in the list.
Parameters: node pointer
Returns: boolean
*/
bool list_isEnd(node *n)
{
	return (n->next == NULL);
}

/*
Prupose: To free memory from the list.
Parameters: LinkedList
*/
void list_free(LinkedList *l)
{

	node *n = l->head;

	while(!list_isEnd(n)) n = list_remove(n, l);

	free(l->head);
	free(l);
}
