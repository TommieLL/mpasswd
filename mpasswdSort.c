
/* mpasswdsort.c
 *  Tommie Lindberg (c15tlg@cs.umu.se)
 *
 *  Sorterar användare efter UID givet en passwd-fil
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mpasswdSort.h"

#define BUF_SIZE 1<<10

typedef struct node {
    unsigned int uid;
    char *uname;
    struct node *next;
} node;

typedef node * list;

bool isEmpty(FILE *file);
node *createNewNode(void);
char *strtok_single (char * str, char const * delims);
list readFile(FILE *, int *failed);
void bubbleSort(list);
void freeList(list);

//Global int
int failed;



/*
 * Main loop.
 */
int main(int argc, const char *argv[]) {


    FILE* fp = stdin;


    if (argc > 1) {
        fp = fopen(argv[1], "r");

        if(fp == NULL){
            perror(argv[1]);
            exit(EXIT_FAILURE);

        }else if (argc < 2) {
            fp = stdin;

        }
     }

    //If there's an empty file
    if(isEmpty(fp)){
        fclose(fp);
        return EXIT_SUCCESS;
    }
    
    // Read data from the file.
    list l = readFile(fp, &failed);
    //Close file.
    fclose(fp);
    
    //Sort list with bubbelsort.
    bubbleSort(l);
    
    //Print the list.
    for (node *n = l; n->next; n = n->next) {
        printf("%d:%s\n", n->uid, n->uname);
    }
    
    if(failed > 0 ){
        freeList(l);
        return EXIT_FAILURE;
    }
    //If there's a file with only errors
    if(l->next == NULL){
        freeList(l);
        return EXIT_FAILURE;
    }
    
    //If all succedded.
    freeList(l);
    return EXIT_SUCCESS;
}

/*
 * Purpose: To chech if a file is empty
 * Returns: True or false.
 */
bool isEmpty(FILE *file){
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0){
    
        return true;
    }

    fseek(file, savedOffset, SEEK_SET);
    return false;
}

/*
 * Create new node and return it.
 */
node *createNewNode(void) {
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
 * Purpose: To seperate a string with data.
 * Returns: A pointer to the data that has been found.
 */ 
char *strtok_single (char * str, char const * delims)
{
  static char  * src = NULL;
  char  *  p,  * ret = 0;

  if (str != NULL)
    src = str;

  if (src == NULL)
    return NULL;

  if ((p = strpbrk (src, delims)) != NULL) {
    *p  = 0;
    ret = src;
    src = ++p;

  } else if (*src) {
    ret = src;
    src = NULL;
  }

  return ret;
}

/*
 * Purpose: Read uid and uname from a file pointer to a list.
 * Returns: A list containing the uid and uname.
 */
list readFile(FILE *fp, int *failed) {
    char buffer[BUF_SIZE];
    node *a = createNewNode();
    node *b = a;

    for (int l = 1; fgets(buffer, sizeof(buffer), fp); l++) {
        char *uname = strtok_single(buffer, ":");
        strtok_single(NULL, ":");  // Skip to the next separator
        char *uid = strtok_single(NULL, ":");
        char *gid = strtok_single(NULL, ":");
        

        if (!uname || !uid || !gid) {
            fprintf(stderr, "Error on line %d: wrong in the input\n", l);
            *failed = *failed +1;
            continue;
        }

        // Try to parse uid to number
        // Special case: uid 0 is noId
        int id = atoi(uid);
        int gi = atoi(gid);
        
        
        bool noId = strcmp(uid, "0") == 0;
        bool noGid = strcmp(gid,"0") == 0;
        
        //Test UID
        if (id == 0 && !noId && !(id>0)) {
            fprintf(stderr, "Error on line %d: Could not parse uid from input\n", l);
            *failed = *failed +1;
            continue;
        }
        if(snprintf(0,0,"%s",uid) > snprintf(0,0,"%+d",id)){
            fprintf(stderr,"Error on line %d: UID can only contain numbers\n", l);
            *failed = *failed +1;
            continue;
        }
        
        if( id<0 ){
            fprintf(stderr,"Error on line %d: Uid can't be a negative number\n",l);
            *failed = *failed +1;
            continue;
        }
        
        //Test GID
        if(gi == 0 && !noGid && !(gi>0)){
            fprintf(stderr,"Error on line %d: Invalid format(no gid)\n", l);
            *failed = *failed +1;
            continue;
        }
        
        if(snprintf(0,0,"%s",gid) > snprintf(0,0,"%+d",gi)){
            fprintf(stderr,"Error on line %d: Gid can only contain numbers\n", l);
            *failed = *failed +1;
            continue;
        }
        
        if( gi<0 ){
            fprintf(stderr,"Error on line %d: Gid can't be a negative number\n",l);
            *failed = *failed +1;
            continue;
        }
        
      
        
        b->uid = id;
        b->uname = malloc(strlen(uname) + 1);

        if (!b->uname) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        strcpy(b->uname, uname);
        b->next = createNewNode();
        b = b->next;
    }

    return a;
}

/*
 * Purpose: To sort the list by uid.
 */
void bubbleSort(list l) {
    for (node *a = l; a->next; a = a->next) {
        for (node *b = a->next; b->next; b = b->next) {
            if (a->uid < b->uid) continue;
            node *temp = createNewNode();
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
 * Purpose: To free all memory the list has used.
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
