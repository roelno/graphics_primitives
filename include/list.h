/*
	Ordered list data structure

	The list is arbitrary size, with nodes created as needed.  The list
	is sorted according to a user-defined comparison function.

	The comparison function must return value < 0 if A < B, 0 if A = B
	and value > 0 if A > B
*/

// Node data structure
typedef struct {
	void *data;
	void *next;
} Node;

// List data structure
typedef struct {
	Node *root;
	Node *current;
} LinkedList;

// allocate and return a new linked list
LinkedList *ll_new( void );

// delete all of the memory used by the list
void ll_delete( LinkedList *ll, void (*freefunc)(const void *) );

// insert an item into the list
void ll_insert( LinkedList *ll, void *item, int (*comp)(const void *, const void *) );

// return a pointer to the top item in the list
void *ll_peek( LinkedList *ll );

// returns a pointer to the data in the first element or null, resets the current pointer
void *ll_head( LinkedList *ll );

// moves the current pointer forward and returns the next data element or null
void *ll_next( LinkedList *ll );

// remove the first item from the list
void *ll_pop( LinkedList *ll );

// returns true if the list is empty
int ll_empty( LinkedList *ll );
