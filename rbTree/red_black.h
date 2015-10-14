/*
   author: James Ross

   header file for the red_black tree data structure,

   red_black.h
*/


                /* headers */

#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <sys/types.h>
#include <inttypes.h>
#include "../err_handle/err_handle.h"
#include "../sedhead.h"
#include "../card.h"   /* holds card data. node -> data points to */

#define CHILD 2  /* ammout of children per node */
#define BLACK 0  /* black color flag */
#define RED   1  /* red color flag */
#define LEFT  0  /* left child */
#define RIGHT 1  /* right child */



/* retreive the color of the given rbNode */
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

/* node the tree contains */
typedef struct RBnode
{
    card_s *data;               /* change data pointer based on client */
    uint32_t cflag;             /* black = 0, red = 1 */

    struct RBnode *parent;      /* parent of current node */
    struct RBnode *child[CHILD];/* child 1/2 (index 0/1) */
} rbNode;

/* holds a root pointer to an entire RB tree */
typedef struct RBtree
{
    struct RBnode *root;
} rbTree; 

                /* initializations */
/* initialize a rb tree */
void rbTree_init(rbTree *tree);

                /* insertion */
/* give a piece of data to a tree, return 1 if success, 0 if error */
uint32_t give_data(rbTree *tree, card_s *toAdd);


               /* removal */
/* removes the first instance of a piece of data in the tree, return 1 if
   success, 0 if toRemove was not in the tree */
/* wrapper for first_remove */
uint32_t remove_first(rbTree *tree, const char *toRemove, const uint32_t pin); 

/* removes each node that contains toRemove's data. Returns number of nodes
   removed */
uint32_t remove_each(rbTree *tree, const char *toRemove, const uint32_t pin);

/* removes everything from the tree. returns 1 if success, 0 if failure */
void remove_all(rbTree *tree); /* wrapper for remove_rbTree() */
         

              /* utility functions */
/* count every instance of a piece of data, return count */
uint32_t data_count(rbTree *tree, const char *toCount); /* count_data wrapper */

/* finds the first piece of data and returns the node's address */
rbNode* find_match(rbTree *tree, const char *toFind, const uint32_t pin);

/* return the total ammount of nodes in the rbTree */
uint32_t total_data_count(rbTree *tree);

            
             /* display functions */
/* display every nodes data, and its color/flag */
uint32_t display_all(rbTree *tree); /* wrapper for display_rbTree */
#endif
