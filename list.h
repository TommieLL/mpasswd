#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef __VALUE
#define __VALUE
typedef void *value;
#endif

#ifndef __MEMFREEVALUEFUNC
#define __MEMFREEVALUEFUNC
typedef void memFreeFunc(value);
#endif

#ifndef LLIST_H
#define LLIST_H

typedef struct node {
	void *value;
	struct node* next;
} node;

typedef struct llist {
	node *head;
	memFreeFunc *freeFunc;
} LinkedList;

/*
Purpose:    Creates a new empty list.
Returns:    A pointer to the new list.
Comments:	List needs to be free'd when finished using.
*/
LinkedList *list_empty(void);

/*
Purpose:    Install's a memory handler to take over the responsibility to
			free memory for values that don't exist anymore.
Parameters: *l - list & *m - a function-pointer to a function.
*/
void list_setMemHandler(LinkedList *l, memFreeFunc *m);

/*
Purpose:    Return's list first position.
Parameters: *l - list.
Returns:    First position of the list.
*/
node *list_first(LinkedList *l);

/*
Purpose:    Return's list next position.
Parameters: *l - list.
Returns:    Next position of the list.
*/
node *list_next(node *n);

/*
Purpose:    Check's if list is empty.
Parameters: *l - list.
Returns:    True if list is empty; otherwise false.
*/
bool list_isEmpty(LinkedList *l);

/*
Purpose:    Enter's value in list
Parameters: *l - list.
Returns:    Value
*/
node *list_insert(node *n, void *v);

node *list_remove(node *n, LinkedList *l);

void *list_inspect(node *n);

bool list_isEnd(node *n);

void list_free(LinkedList *l);
#endif
