/*
    General purpose hash table for my projects and things. Shouldnt need too
    much teaking to fit into other projects.

    Full implementation.

    filename: hash_table.c
*/

#include "hash_table.h"
/*#define NDEBUG*/
#include <assert.h>

/* initialize a node */
static inline void init_node(node_s **Restrict toInit)/*#{{{*/
{
    if(*toInit == NULL)
    {
        *toInit = (node_s*) malloc(sizeof(node_s));
        if(*toInit == NULL){
            errExit("insert_table: malloc failure");}
    }

    (*toInit) -> data = NULL;
    (*toInit) -> next = NULL;
} /* end node_init #}}} */

/* this can be easily altered when conforming to a different project.
   This copy's a nodes data (whever char* or struct *) into the node */
static inline void give_data(node_s *Restrict to, char *Restrict newData)/*#{{{*/
{
    int32_t len = 0;

    if(newData == NULL){
        errnumExit(EINVAL, "give_data: data in from is NULL. cannot copy.");}

    len = strlen(newData) + 1;

    if(to -> data == NULL){
        to -> data = (char*) malloc(sizeof(char) * len);}

    strncpy(to -> data, newData, len);
} /* end give_data #}}} */

/* hash a string by adding up the total of the ascii characters and moding it
   by the size of the hash table to aquire the index.
   returns index on success, -1 on failure */
static int32_t hashString(char *Restrict keyString)/*#{{{*/
{
    int32_t i = 0;
    int32_t stringTotal = 0; /* all the characters in the string added up */
    int32_t keySize = strlen(keyString); /* size of keyString */

    if(keyString == NULL){
        return -1;}

    for(/*i=0*/; i < keySize; ++i){
        stringTotal += keyString[i];}
    
    return stringTotal % _TBL_SIZE_;
} /* end hashString #}}} */

/* Retrieves a matching node based on the given data toFind.
   Returns: Pointer to matching node. NULL if node was not found.
   Errors: calls errExit if no table was passed into function */
node_s* hash_match(hashTable_s *Restrict hTable, char *Restrict toFind)/*#{{{*/
{
    int32_t index = 0; /* index result of hash */
    node_s *current = NULL;

    if(hTable == NULL){
        errnumExit(EINVAL, "hash_match: Missing table");}

    /* no match found */
    if(toFind == NULL){
        return 0;}
    
    /* get index */
    index = hashString(toFind);

    /* set current to hTable index. the head ptr */
    current = hTable -> table[index];
    
    /* if the index the node is at does exist, neither does the node */
    if(current == NULL){
        return NULL;}
    
    /* NOTE: checks if string data is there. change this 
                     when altering for specific project */
    while(current != NULL)
    {
        if(strcmp(current -> data, toFind) == 0){
            return current;}
        current = current -> next;
    }

    return NULL;
} /* end hash_match #}}} */

/* insert a new node into the hash table, inserts at head of chain.
   Returns: 1 on success, -1 if there was not enough room to malloc
            0 if nothing to add, bad call values.
   Errors : EINVAL, malloced data was NULL. */
int32_t table_insert(hashTable_s *Restrict hTable, char *Restrict toAdd)/*#{{{*/
{
    node_s *temp = NULL;
    int32_t index = 0;

    if(toAdd == NULL || hTable == NULL)
    {
        noerrMsg("table_insert: Nothing to Add to/from");
        return 0;
    }

    init_node(&temp);
    
    /* turn key value into an index */
    index = hashString(toAdd);

    /* adds a new node into the table */
    give_data(temp, toAdd);

    temp -> next = hTable -> table[index];
    hTable -> table[index] = temp;
    
    return 1;
} /* end insert #}}} */

/* recursivly removes a node from the table based on toRemove from a given
   chain.
   Returns: 1 on successfull removal, 0 if nothing was removed.
   Errors : */
static int32_t dealloc_node(node_s *chain, node_s *prev, char *Restrict toRemove)/*#{{{*/
{
    if(chain == NULL){
        return 0;}

    if(strcmp(chain -> data, toRemove) == 0)
    {
        prev -> next = chain -> next;
        free(chain -> data);
        free(chain);
        return 1;
    }
    
    return dealloc_node(chain -> next, chain, toRemove);
} /* end dealloc_node #}}}*/

/* remove a node from the hash table.
   Returns: 1 on succes, 0 if node was not found.
   Errors: noerrExit if no table passed */
int32_t hashNode_remove(hashTable_s *Restrict hTable, char *Restrict toRemove)/*#{{{*/
{
    node_s *temp = NULL;
    int32_t index = 0;  /* index result of hash */

    if(hTable == NULL){
        noerrExit("hash_node_remove: Missing table");}
    
    if(toRemove == NULL){
        return 0;}

    index = hashString(toRemove);

    temp = hTable -> table[index];

    if(temp == NULL){
        return 0;}
    
    if(strcmp(temp -> data, toRemove) == 0)
    {
        hTable -> table[index] = temp -> next;
        free(temp -> data);
        free(temp);
        return 1;
    } 

    return dealloc_node(temp -> next, temp, toRemove);
} /* end remove #}}} */

/* deallocate the entire hash table from memory. */
void dealloc_table(hashTable_s *Restrict hTable) /*#{{{*/
{
    node_s *tmpHead = NULL; /* gets set to a pntr index in table */
    node_s *nxtNode = NULL; /* gets set to the next node in a chain */
    int32_t i = 0;

    if(hTable != NULL)
    {
        /* remove each index's contents. Remove the chain of each pointer in the
           array */
        for(/*i=0*/; i < _TBL_SIZE_; ++i)
        {
            tmpHead = hTable -> table[i];

            /* free chain */
            while(tmpHead != NULL)
            {
                nxtNode = tmpHead -> next;
                if(nxtNode != NULL)
                {
                   tmpHead -> next = nxtNode -> next;
                   /* NOTE: data will need to be freed differently 
                                  depening on implementation */
                   free(nxtNode -> data);
                   free(nxtNode);
                   nxtNode = NULL;
                }
                else
                {  
                    assert(tmpHead -> next == NULL);
                    free(tmpHead -> data);
                    free(tmpHead);
                    tmpHead = NULL;
                } /* end else */
            } /* end while */
        } /* end for */
    } /* end if */
    /* now that everything is free, free the table itself */
    free(hTable -> table);
    free(hTable);
    hTable = NULL;
} /* end dealloc_table #}}} */

/* display all the contents of the hash table */
void hashtable_disp(hashTable_s *Restrict hTable)/*#{{{*/
{
    int32_t i = 0;
    
    if(hTable == NULL){
        return;}

    for(/*i=0*/; i < _TBL_SIZE_; ++i)
    {
        printf("\nIndex number: %d",i);
        fflush(stdout);
        chain_disp(hTable -> table[i]);
        printf("\n");
    } 
} /* end hashtable_disp #}}} */

/* display chain */
void chain_disp(node_s *Restrict chain)/*#{{{*/
{
    int32_t i = 0;

    if(chain == NULL){
        return;}

    while(chain != NULL)
    {
        ++i;
        printf("\nNode #%d\n%s\n", i, chain -> data);
        chain = chain -> next;
    }
} /* end chain_disp #}}} */
