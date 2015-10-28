/*
    General purpose hash table for my projects and things. Shouldnt need too
    much teaking to fit into other projects.

    Full implementation.

    filename: hash_table.h
*/

#define __SED_ERR__    /* error functions */
#include "../sedhead.h"


/* change the size of the hash table to fit the project */
#define _TBL_SIZE_    5

typedef struct Node
{
    char *data;        /* holds character data */
    struct Node *next; /* points to the next struct in the chain */
} node_s;

typedef struct Hashtable
{
    struct Node **table; /* hash table */
} hashTable_s;

/* sets all the pointers in a hash table to NULL. */
#define null_table(hashPntr)                                                \
{                                                                           \
    int32_t ___Z_;                                                          \
    for((___Z_) = 0; (___Z_) < _TBL_SIZE_; ++(___Z_)){                      \
        (hashPntr) -> table[(___Z_)] = NULL;}                               \
} /* end null_table */

/* Allocats the table itself if not allready done, then calls null_table to
   set all the pointers in the table to NULL */
#define table_init(hashPntr)                                                  \
{                                                                             \
    if((hashPntr) == NULL)                                                    \
    {                                                                         \
        (hashPntr) = (hashTable_s*) malloc(sizeof(hashTable_s));              \
        (hashPntr) -> table = (node_s**) malloc(sizeof(node_s*) * _TBL_SIZE_);\
    }                                                                         \
    null_table((hashPntr));                                                   \
} /* end table_init */

/* insert a new node into the hash table */
int32_t table_insert(hashTable_s *Restrict hTable, char *Restrict toAdd);

/* remove a node from the hash table */
int32_t hashNode_remove(hashTable_s *Restrict hTable, char *Restrict toRemove);

/* retrieves the address of the node in question. 
   returns: NULL if match was not found. node_s if match was found. 
   errors: */
node_s* retrieve_match(hashTable_s *Restrict hTable, char *Restrict toFind);

/* deallocate the entire hash table */
void dealloc_table(hashTable_s *Restrict hTable);

/* display all the contents of the hash table */
void hashtable_disp(hashTable_s *Restrict hTable);

/* display chain */
void chain_disp(node_s *Restrict chain);
