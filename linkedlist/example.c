/**
 * Example usage of linkedlist.h
 * Author: Conrad Smith
 * 
 */

// ---Pre-processor Directives---

#include "linkedlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct foo {
    int bar;
    char *baz;
    struct foo *fooProp;
};

// ---Function Definitions---

struct foo *createFoo(int bar, char *baz, struct foo *fooProp) {
    struct foo *foo = (struct foo *)malloc(sizeof(struct foo));

    if (foo != NULL) {
        foo->bar = bar;
        foo->baz = baz;
        foo->fooProp = fooProp;
    }

    return foo;
}

void freeFoo(void *fooObj) {
    struct foo *temp = (struct foo *)fooObj;

    if (temp != NULL) {
        free(temp);
    }

    fooObj = NULL;
    temp = NULL;
    return;
}

char *fooToString(void *fooObj) {
    struct foo *temp = (struct foo *)fooObj;
    int num;
    char *str = NULL;

    if (temp != NULL) {
        num = temp->bar;
        // get exact number of bytes needed for itoa on num
        str = (char *)calloc(snprintf(NULL, 0, "%d", num) + 1, sizeof(char));

        if (str != NULL) {
            sprintf(str, "%d", temp->bar);                
        }
    }

    return str;
}

// return 0 when equal
int compareFoo(void *fooObj1, void *fooObj2) {
    struct foo *temp1 = (struct foo *)fooObj1;
    struct foo *temp2 = (struct foo *)fooObj2;

    // see if temp1 is greater than temp2
    if (temp1 != NULL && temp2 != NULL) {
        return (temp1->bar - temp2->bar);
    }
    // temp1 is NULL so temp2 is greater
    else if (temp1 == NULL) {
        return -1;
    }
    // temp2 is NULL so temp1 is greater
    else {
        return 1;   
    }
}

int main(void) {
    struct foo *foobie = createFoo(1, "Test", NULL);
    struct List *list1 = createList(foobie, sizeof(struct foo), "foo", (*freeFoo));
    int i;

    srand(time(NULL));
    printf("Here is the basic list...\n");
    printList(list1, (*fooToString));

    for (i = 0; i < 20; i++) {
        appendList(list1, createFoo(rand() % 1000, "More test!", NULL));
    }

    printf("Here is a list with more elements...\n");
    printList(list1, (*fooToString));

    printf("Sorting in ascending order...\n");
    msortList(list1, compareFoo);
    printList(list1, (*fooToString));

    printf("Deleting list...\n");
    list1 = deleteList(list1);
    printList(list1, (*fooToString));

    printf("All done!\n");

    return 0;
}