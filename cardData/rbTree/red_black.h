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
#define find_sibling(node) (((node) == (node) -> parent -> child[LEFT])        \
                              ? (node) -> parent -> child[RIGHT]               \
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

/* card structure holding all the details in the card. The last pointer in
   drank will remain NULL in the pope house program */
/* DESIGN NOTE: **drank could be turned into a linear linked list. This
                would allow the list of whiskeys drank to go on forever. The
                way the program is written it is not hard to extend the **, and
                this using less pointers and dereferences is worth it in this
                specific implementation for pope_cards */
typedef struct Card 
{
    char *name;         /* name of the card owner */ 
    int32_t **drank;    /* whiskeys drank. Each full index is a horseshoe */
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
} cardDeck_s; 

/* frees all the data in a card, including the card itself */
#define delete_card(cardDel)                                                   \
{ /*#{{{*/                                                                     \
    free_all((cardDel) -> name, (cardDel) -> drank, (cardDel));                \
} /* end delete_card #}}}*/

/* creates and initializes a card */
#define init_card(newCard)                                                     \
{ /*#{{{*/                                                                     \
    int _I;                                                                    \
    if(newCard == NULL)                                                        \
    {                                                                          \
        newCard = (card_s*) malloc(sizeof(card_s));                            \
        if(newCard == NULL){                                                   \
            errExit("create_card: malloc failure.");}                          \
    }                                                                          \
    newCard -> name       = NULL;                                              \
    newCard -> pinNum     = 0;                                                 \
    newCard -> whiskCount = 0;                                                 \
                                                                               \
    newCard -> drank = (int32_t**) malloc(sizeof(int32_t*)*D_PTR_MX);          \
    if(newCard -> drank == NULL){                                              \
        errExit("init_card: malloc failure");}                                 \
                                                                               \
    for(_I = 0; _I < D_PTR_MX; ++_I){                                          \
        (newCard) -> drank[_I] = NULL;}                                        \
} /* end init_card #}}} */

                /* initializations */
/* initialize a rb tree */
int rbTree_init(cardDeck_s *tree);

                /* insertion */
/* give a piece of data to a tree, return 1 if success, 0 if error */
uint32_t give_data(cardDeck_s *tree, card_s *toAdd);


               /* removal */
/* removes the first instance of a piece of data in the tree, return 1 if
   success, 0 if toRemove was not in the tree */
/* wrapper for first_remove */
uint32_t remove_first(cardDeck_s *tree, const int32_t pin); 

/* removes each node that contains toRemove's data. Returns number of nodes
   removed */
uint32_t remove_each(cardDeck_s *tree, const int32_t pin);

/* removes everything from the tree. returns 1 if success, 0 if failure */
void remove_allRB(cardDeck_s *tree); /* wrapper for remove_rbTree() */
         

              /* utility functions */
/* count every instance of a piece of data, return count */
uint32_t data_count(cardDeck_s *tree, int32_t pin); /* count_data wrapper */

/* finds the first piece of data and returns the node's address */
struct Card* rb_find(cardDeck_s *tree, int32_t pin);

/* return the total ammount of nodes in the cardDeck_s */
uint32_t total_data_count(cardDeck_s *tree);

            
             /* display functions */
/* display every nodes data, and its color/flag */
uint32_t display_all(cardDeck_s *tree); /* wrapper for display_rbTree */
#endif
