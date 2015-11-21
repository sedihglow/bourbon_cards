/*
   author: James Ross

   header file for the red_black tree data structure,

   red_black.h
*/


                /* headers */

#ifndef __RBTREE_H__
#define __RBTREE_H__

#ifndef __SED_ERR__
#define __SED_ERR__
#endif
#ifndef __SED_NUM__
#define __SED_NUM__
#endif
#include "../../sed/sedhead.h"

/* retreive the color of the given rbNode_s */
#define find_rbcolor(node) (((node) == NULL) ? BLACK : (node) -> cflag)

/* finds a nodes grandparent, pointer, or NULL if non existant */
#define find_parent(node)  (((node) != NULL) ? (node) -> parent : NULL)

/* finds the sibling of a node */
#define find_sibling(node) (((node) == (node) -> parent -> child[LEFT])\
                              ? (node) -> parent -> child[RIGHT]       \
                              : (node) -> parent -> child[LEFT])

/* finds a nodes uncle, pointer, or NULL if non existant */
#define find_uncle(node, grand) (((grand)) /* if grand exists get uncle */      \
                                ? (                                             \
                                    ((node) -> parent == (grand) -> child[LEFT])\
                                    ? (grand) -> child[RIGHT]                   \
                                    : (grand) -> child[LEFT])                   \
                                : NULL) /* else NULL */

/* max name for a person on a card. big names are big. see google. */
#define MAX_WS    256   /* Ammount a person can drink without liver failure */
#define MAX_NAME  512   /* largest a persons full name can be */

/* NOTE: it may be better to make drank **drank rather than *drank and only
         allocate 50 at a time to save memory since 50 is each milestone.
         This will require adjusting the add_whiskey function. Keep it in mind
         while continuing the project furthur with adding to tree */
typedef struct Card 
{
    char *name;         /* name of the card owner */ 
    int32_t *drank;     /* whiskeys drank. allocates room for 250 whiskeys, 
                           recorded by number corresponding to database */
    int32_t pinNum;     /* pin number used to identify the person */
    int32_t whiskCount; /* current number of whiskeys drank */
} card_s;

/* node the tree contains */
typedef struct RBnode
{
    struct Card *data;          /* change data pointer based on client */
    uint32_t cflag;             /* black = 0, red = 1 */

    struct RBnode *parent;      /* parent of current node */
    struct RBnode *child[2];    /* child 1/2 (index 0/1) */
} rbNode_s;

/* holds a root pointer to an entire RB tree */
typedef struct RBtree
{
    struct RBnode *root;
} cardStack_s; 

/* takes two card pointers, copys a card to another card */
#define copy_card(toPtr, fromPtr)/*#{{{*/                                    \
{                                                                            \
    int32_t __i__ = 0;                                                       \
    int32_t _toPtrLen_ = strlen((fromPtr) -> name) + 1;                      \
                                                                             \
    free((toPtr) -> name);                                                   \
    (toPtr) -> name = (char*) malloc(sizeof(char)*_toPtrLen_);               \
    strncpy((toPtr) -> name, (fromPtr) -> name, _toPtrLen_);                 \
                                                                             \
    memset((toPtr) -> name, 0, strlen((fromPtr) -> name) + 1);               \
    for(/*i=0*/; __i__ < MAX_WS ; ++__i__){                                  \
        (toPtr) -> drank[__i__] = (fromPtr) -> drank[__i__];}                \
                                                                             \
    (toPtr) -> pinNum = (fromPtr) -> pinNum;                                 \
    (toPtr) -> whiskCount = (fromPtr) -> whiskCount;                         \
} /* end copy_card #}}} */

/* frees all the data in a card, including the card itself */
#define delete_card(cardDel){ /*#{{{*/ \
    free_all((cardDel) -> name, (cardDel) -> drank, (cardDel));}/* end delete_card #}}}*/

/* creates and initializes a card.
   -Sets pinNum
   -Sets name
   Return: pointer to a new card */
static inline card_s* create_card(char *name, int32_t pin)/*#{{{*/
{
    int32_t len = 0;
    struct Card *newCard = NULL;

    if(name == NULL){
        noerrExit("Requires a name for insertion to card.");}
    
    newCard = (card_s*) malloc(sizeof(card_s));
    len = strlen(name) + 1;
    newCard -> name = (char*) malloc(sizeof(char) * len);
    strncpy(newCard -> name, name, len);

    newCard -> drank = (int32_t*) malloc(sizeof(int32_t) * MAX_WS);
    memset(newCard -> drank, 0, sizeof(int32_t) * MAX_WS);
    newCard -> pinNum     = pin;
    newCard -> whiskCount = 0;

    return newCard;
} /* end create_card #}}} */

                /* initializations */
/* initialize a rb tree */
void rbTree_init(cardStack_s *tree);

                /* insertion */
/* give a piece of data to a tree, return 1 if success, 0 if error */
uint32_t give_data(cardStack_s *tree, card_s *toAdd);


               /* removal */
/* removes the first instance of a piece of data in the tree, return 1 if
   success, 0 if toRemove was not in the tree */
/* wrapper for first_remove */
uint32_t remove_first(cardStack_s *tree, const char *toRemove, const int32_t pin); 

/* removes each node that contains toRemove's data. Returns number of nodes
   removed */
uint32_t remove_each(cardStack_s *tree, const char *toRemove, const int32_t pin);

/* removes everything from the tree. returns 1 if success, 0 if failure */
void remove_allRB(cardStack_s *tree); /* wrapper for remove_rbTree() */
         

              /* utility functions */
/* count every instance of a piece of data, return count */
uint32_t data_count(cardStack_s *tree, int32_t pin); /* count_data wrapper */

/* finds the first piece of data and returns the node's address */
struct Card* rb_find(cardStack_s *tree, int32_t pin);

/* return the total ammount of nodes in the cardStack_s */
uint32_t total_data_count(cardStack_s *tree);

            
             /* display functions */
/* display every nodes data, and its color/flag */
uint32_t display_all(cardStack_s *tree); /* wrapper for display_rbTree */
#endif
