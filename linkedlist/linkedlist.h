struct Node {
    // the data that this node contains
    void *data;
    // the next node in the linked list
    struct Node *next;
};

struct List {
    // number of elements in list
    int size;
    // size of this list's type in bytes
    int type_size;
    // the name of this list's type
    char *type_name;
    // the function used to free this list's data type
    void (*free_func)(void *);
    // the first node of this list
    struct Node *head;
    // the last node of this list
    struct Node *tail;
};

/**
 * Allocate space for - and initialize - a new generic linked list
 * 
 * Args:
 *     - (void *) data: the data to be held in the list head (cannot be NULL)
 *     - (int) type_size: the size of this list's type in bytes
 *     - (char *) type_name: the name of this list's type
 *     - (void (*)(void *)) free_func: the function used to free this list's data type
 * 
 * Returns:
 *     - (struct List *) a pointer to a new linked list if memory allocation is successful,
 *                       or NULL if memory allocation fails
 */
struct List *createList(void *data, int type_size, char *type_name, void (*free_func)(void *));

/**
 * Deallocate the space used for a linked list.
 * 
 * Args: 
 *     - (struct List *) list: the list to delete
 * 
 * Returns:
 *     - (struct List *) a reference to the deleted list; will always be NULL
 */
struct List *deleteList(struct List *list);

/**
 * Add a new node to the linked list.
 * 
 * Args:
 *     - (struct List *) list: the list to add the node to
 *     - (void *) data: a pointer to the data which the node will contain
 * 
 * Returns:
 *     - (struct List *) the list that was passed as the "list" arg
 */
struct List *appendList(struct List *list, void *data);

/**
 * Removes a node from a linked list. The "data" arg is compared to each node in the list
 * using the "comparator" function arg. If the comparator returns 0, that node will be removed,
 * and removeNode() will return. If the "all" arg is 1, removeNode() will remove EVERY node 
 * for which "comparator" returns 0.
 * 
 * Args:
 *     - (struct List *) list: the list to remove the node from
 *     - (void *) data: a pointer to the data which will be removed from the list
 *     - (int (*)(void *, void *)) comparator: the function that will be used to compare
 *           the "data" arg to each node in the list; MUST return 0 if arguments match
 *     - (char) all: a flag used to determine whether to delete every node for which "comparator"
 *           returns 0 or not; if any other number is passed, removeNode() will only delete the
 *           first node specified by "comparator".
 * 
 * Returns:
 *     - (struct Node *) a reference to the deleted node; will always be NULL
 */
struct Node *removeNode(struct List *list, void *data, int (*comparator)(void *, void *), char all);

/**
 * Returns a node from a linked list. The "data" arg is compared to each node in the list 
 * using the "comparator" function arg. If the comparator returns 0, that node will be
 * returned by getNode(). If no matching node is found, getNode() returns NULL.
 * 
 * Args:
 *     - (struct List *) list: the list to search for the node matching "data"
 *     - (void *) data: a pointer to the data to be compared using "comparator"
 *     - (int (*)(void *, void *)) comparator: the function that will be used to compare
 *           the "data" arg to each node in the list; MUST return 0 if arguments match
 * 
 * Returns:
 *     - (struct Node *) a reference to the matching node; if no matching node is found,
 *           NULL is returned
 */
struct Node *getNode(struct List *list, void *data, int (*comparator)(void *, void *));

/**
 * Print out the contents of the list. The data in each node will be converted to a printable
 * string using the "toString" function arg. Note: this function will free every string returned
 * by "toString", so ensure that you are willing to have such data freed. 
 * 
 * Args:
 *     - (struct List *) list: the list whose contents will be printed
 *     - (char *(*)(void *)) toString: the function that converts the data in each node
 *           to a printable string.
 */
void printList(struct List *list, char *(*toString)(void *));

void msortList(struct List *list, int (*comparator)(void *, void *));