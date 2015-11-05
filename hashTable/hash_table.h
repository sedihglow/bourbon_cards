/*
    General purpose hash table for my projects and things. Shouldnt need too
    much teaking to fit into other projects.

    Full implementation.

    filename: hash_table.h
*/
#ifndef __SED_NUM__
#define __SED_NUM__
#endif
#ifndef __SED_ERR__
#define __SED_ERR__    /* error functions */
#endif
#include "../sedhead.h"


/* change the size of the hash table to fit the project */
#define _TBL_SIZE_    5

typedef struct Node
{
    char *whiskName;    /* holds character data */
    struct Node *next;  /* points to the next struct in the chain */
    int32_t whiskNum;   /* number corresponding to the whisk name */
} whiskey_s;

typedef struct Hashtable
{
    struct Node **table; /* hash table */
} whiskTable_s;

/* sets all the pointers in a hash table to NULL. */
#define null_table(tablePntr)                                                \
{                                                                            \
    int32_t ___Z_;                                                           \
    for((___Z_) = 0; (___Z_) < _TBL_SIZE_; ++(___Z_)){                       \
        (tablePntr) -> table[(___Z_)] = NULL;}                               \
} /* end null_table */

/* Allocats the table itself if not allready done, then calls null_table to
   set all the pointers in the table to NULL */
#define table_init(tablePntr)                                                  \
{                                                                              \
    if((tablePntr) == NULL)                                                    \
    {                                                                          \
        (tablePntr) = (whiskTable_s*) malloc(sizeof(whiskTable_s));            \
        (tablePntr) -> table = NULL;                                           \
    }                                                                          \
                                                                               \
    if((tablePntr) -> table == NULL){                                          \
        (tablePntr) -> table = (whiskey_s**) malloc(sizeof(whiskey_s*) * _TBL_SIZE_);}\
                                                                               \
    null_table((tablePntr));                                                   \
} /* end table_init */

/* insert a new node into the hash table 
   Return: 0 on error 1 on success */
int32_t table_insert(whiskTable_s *Restrict hTable, char *Restrict toAdd, int32_t wnum);

/* remove a node from the hash table */
int32_t hashNode_remove(whiskTable_s *Restrict hTable, char *Restrict toRemove);

/* retrieves the address of the node in question. 
   returns: NULL if match was not found. whiskey_s if match was found. 
   errors: */
whiskey_s* whisk_match(whiskTable_s *Restrict hTable, char *Restrict toFind);

/* deallocate the entire hash table */
void dealloc_table(whiskTable_s *Restrict hTable);

/* display all the contents of the hash table */
void hashtable_disp(whiskTable_s *Restrict hTable);

/* display chain */
void chain_disp(whiskey_s *Restrict chain);
