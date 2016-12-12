
/* mpasswdsort.c
 *  Tommie Lindberg (c15tlgn@cs.umu.se)
 *
 *  Sorterar användare efter UID givet en passwd-fil
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define BUF_SIZE 1<<10

typedef struct node {
    unsigned int uid;
    char *uname;
    struct node *next;
} node;

typedef node * list;

node *createStruct(void);
list readFile(FILE *fp);
bool ifDuplicate(list, int, char *);
node *create_node(void);
void listBubbleSort(list);
void printList(list);
void freeList(list);





/*
 * Main entry point.
 */
int main(int argc, const char *argv[]) {


    FILE* fp;


    if (argc == 2 ) {
        fp = fopen(argv[1], "r");

        if(fp == NULL){
            perror(argv[1]);
            exit(EXIT_FAILURE);

        }else if (argc < 2) {
            fp = stdin;

        } else {
            fprintf(stderr, "Too many input arguments: %d.\n", argc);
            return EXIT_FAILURE;

            }

    }

    list l = readFile(fp);
    fclose(fp);
    listBubbleSort(l);
    printList(l);
    freeList(l);

    return EXIT_SUCCESS;
}

/*
 * Create new node and return it.
 */
node *createStruct(void) {
    node *n = malloc(sizeof(node));

    if (!n) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    n->next = NULL;
    n->uname = NULL;

    return n;
}

/*
 * Find duplicate in list.
 * Return true if node with uid and uname is found, false otherwise.
 */
bool ifDuplicate(list l, int uid, char *uname) {
    for (node *n = l; n->next; n = n->next) {
        if (n->uid == uid || strcmp(n->uname, uname) == 0)
            return true;
    }

    return false;
}
/*
 * Read uid and uname from file pointer to list.
 */
list readFile(FILE *fp) {
    char buffer[10];
    node *a = createStruct();
    node *b = a;

    for (int l = 1; fgets(buffer, sizeof(buffer), fp); l++) {
        char *uname = strtok(buffer, ":");
        strtok(NULL, ":");  // Skip to the next separator
        char *uid = strtok(NULL, ":");

        if (!uname || !uid) {
            fprintf(stderr, "Error on line %d: wrong in the input\n", l);
            continue;
        }

        // Try to parse uid to number
        // Special case: uid 0 is noId
        int id = atoi(uid);
        bool noId = strcmp(uid, "0") == 0;

        if (id == 0 && !noId) {
            fprintf(stderr, "Error on line %d: Could not parse uid from input\n", l);
            continue;
        }

        // Unames and uids are unique.
        // Don't add duplicate lines.
        if (ifDuplicate(a, id, uname)) {
            fprintf(stderr, "Error on line %d: A duplicate of a user has been found\n", l);
            continue;
        }

        b->uid = id;
        b->uname = malloc(strlen(uname) + 1);

        if (!b->uname) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        strcpy(b->uname, uname);
        b->next = createStruct();
        b = b->next;
    }

    return a;
}
/*
 * Print list.
 */
void printList(list l) {
    for (node *n = l; n->next; n = n->next) {
        printf("%d:%s\n", n->uid, n->uname);
    }
}

/*
 * Sort list with bubble sort.
 */
void listBubbleSort(list l) {
    for (node *a = l; a->next; a = a->next) {
        for (node *b = a->next; b->next; b = b->next) {
            if (a->uid < b->uid) continue;
            node *temp = createStruct();
            temp->uname = a->uname;
            temp->uid = a->uid;
            a->uname = b->uname;
            a->uid = b->uid;
            b->uname = temp->uname;
            b->uid = temp->uid;
            free(temp);
        }
    }
}

/*
 * Free all nodes.
 */
void freeList(list l) {
    while (l->next) {
        node *temp = l->next;
        l->next = l->next->next;
        free(temp->uname);
        free(temp);
    }

    free(l->uname);
    free(l);
}
