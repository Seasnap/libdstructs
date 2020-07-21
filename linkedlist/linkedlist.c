/* ---Pre-processor directives--- */

#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ---Auxiliary Function Definitions--- */

// required for deleteList() since C doesn't allow anonymous functions
int returnZero(void *a, void *b) {
    return 0;
}

/* ---Function Definitions--- */

struct List* createList(void *data, int type_size, char *type_name, void (*free_func)(void *)) {
    struct List *list = (struct List *)malloc(sizeof(struct List));

    // check for valid arguments
    if (list != NULL && data != NULL && type_size > 0 && type_name != NULL && free_func != NULL) {
        list->type_size = type_size;
        list->type_name = type_name;
        list->free_func = free_func;
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }

    return appendList(list, data);
}

struct List* deleteList(struct List *list) {
    if (list != NULL) {
        removeNode(list, NULL, (*returnZero), 1);

        list->type_name = NULL;
        list->free_func = NULL;
        list->head = NULL;
        list->tail = NULL;
        free(list);
        list = NULL;
    }

    return list;
}

struct List* appendList(struct List *list, void *data) {
    struct Node *node, *temp; 

    // exhaustive error checking 
    if (list != NULL && data != NULL) {
        node = (struct Node *)malloc(sizeof(struct Node));
        
        if (node != NULL) {
            node->data = data;
            node->next = NULL;

            // check if this is first element in list
            if (list->size != 0) {
                temp = list->tail;
                list->tail = node;
                temp->next = node;
            }
            else {
                list->head = node;
                list->tail = node;
            }

            list->size++;
        }
    }

    return list;
}

struct Node* removeNode(struct List *list, void *data, int (*comparator)(void *, void *), char all) {
    struct Node *mid, *temp, *prev = NULL;

    // more exhaustive error checking
    if (list != NULL && list->size > 0) {
        mid = list->head;

        while (mid != NULL) {
            // if this node's data matches "data"
            if (!((*comparator)(mid->data, data))) {
                if (mid == list->head) {
                    list->head = mid->next;
                }
                if (mid == list->tail) {
                    list->tail = NULL;
                }

                temp = mid->next;
                (*(list->free_func))(mid->data);
                free(mid);
                mid = temp;
                
                if (prev != NULL) {
                    prev->next = mid;
                }

                list->size--;

                // if all flag is 0, exit function
                if (!all) {
                    break;
                }
            }
            else {
                prev = mid;
                mid = mid->next;
            }
        }
    }
    
    return NULL;
}

struct Node* getNode(struct List *list, void *data, int (*comparator)(void *, void *)) {
    struct Node *mid = NULL;

    if (list != NULL) {
        mid = list->head;

        while (mid != NULL) {
            if (!((*comparator)(mid->data, data))) {
                break;
            }
            else {
                mid = mid->next;
            }
        }
    }

    return mid;
}

void printList(struct List *list, char *(*toString)(void *)) {
    struct Node *mid = NULL;
    char *temp = NULL;
    int i;

    if (list != NULL && toString != NULL) {
        mid = list->head;
        
        printf("List of type \'%s\' and of size \'%d\'\n", list->type_name, list->size);
        printf("========================================\n");
        printf("(Index): (Data)\n");
        printf("- - - - - - - - - - - - - - - - - - - - \n");

        for (i = 0; mid != NULL; i++) {
            temp = (*toString)(mid->data);
            printf("%d: %s", i, temp);
            if (temp != NULL) {
                free(temp);
            }            
            mid = mid->next;
            printf("\n");
        }

        printf("========================================\n");
    }

    return;
}

// Credit to Simon G Tatham for concept/structure
// ---his original code (with my formatting fixes + certain
//    annotations) can be found in listsort.c

// 1. divide list into smallest partitions
// 2. sort these partitions
// 3. connect tail of each partition to head of next partition
//     3a. maintain head and tail of list
// 4. increase in partition size
// 5. repeat until list is sorted (num_merges <= 1)
void msortList(struct List *list, int (*comparator)(void *, void *)) {
    // lo starts w/ the node at the beginning of the partition
    // mid starts w/ node at middle of partition
    // temp holds either lo or mid, pushed into temporary ""list""
    // aux_nodes is a ""list"" (not an actual list structure, just collection of nodes)
    //     which contains sorted nodes
    struct Node *lo, *mid, *temp, *aux_nodes;
    // part_size is size of partition / 2
    // lo size is first half of partition size
    // mid size is second half of partition size
    // num_merges tracks number of merges in each pass
    // i is loop variable
    int part_size, lo_size, mid_size, num_merges, i;

    if (list != NULL) {
        part_size = 1;

        for (;;) {
            lo = list->head;
            list->head = NULL;
            aux_nodes = NULL;
            num_merges = 0;

            // keep making part_size partitions until lo falls off
            while (lo != NULL) {
                num_merges++;
                mid = lo;
                lo_size = 0;

                // step "part_size" nodes into list
                for (i = 0; i < part_size; i++) {
                    lo_size++;

                    if (mid != NULL) {
                        mid = mid->next;
                    }
                    else {
                        break;
                    }
                }

                mid_size = part_size;

                // merge two lists
                while (lo_size > 0 || (mid_size > 0 && mid != NULL)) {
                    // if no more elements in lo partition, use next element of mid
                    if (lo_size == 0) {
                        if (mid != NULL) {
                            temp = mid; 
                            mid = mid->next; 
                        }

                        mid_size--;
                    }
                    // if no more elements in mid partition, use next element of lo 
                    else if (mid_size == 0 || mid == NULL) {
                        if (lo != NULL) {
                            temp = lo;
                            lo = lo->next;
                        }

                        lo_size--;
                    }
                    // both partitions have elements, compare one of each
                    // if lo comes first (or is same as mid), use lo
                    else if (lo != NULL && mid != NULL && (*comparator)(lo->data, mid->data) <= 0) {
                        if (lo != NULL) {
                            temp = lo;
                            lo = lo->next;
                        }

                        lo_size--; 
                    }
                    // if mid comes first, use mid
                    else {
                        if (mid != NULL) {
                            temp = mid;
                            mid = mid->next;
                        }

                        mid_size--;
                    }

                    // push mid or lo into temporary ""list""
                    if (aux_nodes != NULL) {
                        aux_nodes->next = temp;
                    }
                    // establish new head
                    else {
                        list->head = temp;
                    }                    

                    // make sure to update temporary ""list""
                    aux_nodes = temp;
                }

                // lo has now stepped "part_size" places along, and mid has too
                lo = mid;
            }

            // establish new tail once pass is done
            temp->next = NULL;
            list->tail = temp;

            // if only two lists had to be merged, list is sorted
            if (num_merges <= 1) {
                break;
            }

            part_size *= 2;
        }
    }

    return;
}