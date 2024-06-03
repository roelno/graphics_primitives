/*
	Bruce A. Maxwell
	Fall 2014
	CS 351 Computer Graphics

	Ordered list data structure

	The list is arbitrary size, with nodes created as needed.  The list
	is sorted according to a user-defined comparison function.

	The comparison function must return value < 0 if A < B, 0 if A = B
	and value > 0 if A > B
*/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
	Creates a new linked list and returns a pointer to it.
*/
LinkedList *ll_new( void ) {
	LinkedList *ll;

	ll = malloc(sizeof(LinkedList));
	if( !ll ) {
		printf("ll_new(): unable to allocate list memory\n");
		return(NULL);
	}

	ll->root = NULL;
	ll->current = ll->root;

	return(ll);
}

/* 
	 Inserts the item into the list using the specified comparison
	 function.
*/
void ll_insert( LinkedList *ll, void *item, int (*comp)(const void *, const void *) ) {
	Node *p, *q, *n;

	// see if the list is empty or the item replaces the starting element
	if( ll->root == NULL || comp(item, ll->root->data) <= 0 ) {
		n = malloc(sizeof(Node));
		n->next = ll->root;
		n->data = item;
		ll->root = n;
		return;
	}

	// deal with the rest of the list, it doesn't go in front
	q = ll->root;
	p = q->next;
	while( p ) {
		// see if it comes between p and q
		if( comp(item, p->data) <= 0 ) {
			n = malloc(sizeof(Node));
			n->data = item;
			n->next = p;
			q->next = n;
			return;
		}
		q = p;
		p = q->next;
	}
	
	// goes at the end
	n = malloc(sizeof(Node));
	n->data = item;
	n->next = NULL;
	q->next = n;

	return;
}

/*
	Returns true if the list is empty
*/
int ll_empty( LinkedList *ll ) {
	if( ll->root == NULL )
		return(1);
	return(0);
}

/* 
	 Return a pointer to the top item in the list
*/
void *ll_peek( LinkedList *ll ) {
	if( ll->root == NULL )
		return(NULL);

	return(ll->root->data);
}

/*
	Sets the current iterator to the head of the list and returns the
	data from the first node.
 */
void *ll_head( LinkedList *ll ) {
	ll->current = ll->root;
	if( ll->current != NULL )
		return( ll->current->data );

	return(NULL);
}

/*
	Returns the data from the next element in the list as specified by
	the iterator.  Returns NULL if the end of the list has been reached.
 */
void *ll_next( LinkedList *ll ) {
	if( ll->current != NULL )
		ll->current = ll->current->next;

	if( ll->current != NULL )
		return( ll->current->data );

	return(NULL);
}

/* 
	 Removes the top element from the list and returns it.
 */
void *ll_pop( LinkedList *ll ) {
	Node *n;
	void *node_data;
	
	if( ll->root == NULL )
		return(NULL);

	n = ll->root;
	ll->root = ll->root->next;

	node_data = n->data;
	free(n);

	return( node_data );
}

/*
	Deletes all elements of the list and the list itself, freeing each
	node's content using the specified function, which could be NULL.
 */
void ll_delete( LinkedList *ll, void (*freefunc)(const void *) ) {
	Node *p, *q;

	p = ll->root;
	while( p != NULL ) {
		q = p->next;
		if(freefunc && p->data)
			freefunc( p->data );
		free(p);
		p = q;
	}
	free( ll );

	return;

}


