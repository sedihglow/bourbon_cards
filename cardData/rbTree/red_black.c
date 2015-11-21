/* 
    author: James Ross

    not for distribution, permittion to use needs to be approved beforehand.
*/

#include <sys/types.h>
#define NDEBUG
#include <assert.h>
#include "red_black.h"

#define BLACK 0  /* black color flag */
#define RED   1  /* red color flag */
#define LEFT  0  /* left child */
#define RIGHT 1  /* right child */

                   /* static prototypes for tree operation */
                   /* static functions #{{{ */

                /* insertion */
/* enter in a data value, return 1 if success, 0 if failure */
static uint32_t place_data(cardStack_s *tree, card_s *toAdd);

                /* insertion cases */
/* checks all the cases conforming to the red-black tree algorithm for insert */
static void check_insert_cases(cardStack_s *tree, rbNode_s *current);
/* case 1: N is the root node */
static uint32_t insert_case_1(rbNode_s *node, rbNode_s *parent);
/* case 2: N's parent is black */
static uint32_t insert_case_2(rbNode_s *parent);
/* case 3: N's parent and uncle are red */
static uint32_t insert_case_3(cardStack_s *tree, rbNode_s *parent, rbNode_s *grand, rbNode_s *uncle);
/* case 4: N is added to the to grandparent -> child[LEFT] -> child[RIGHT]
                            OR
           N is added to the grandparent -> child[RIGHT] -> child[LEFT] */
static uint32_t insert_case_4(cardStack_s *tree, rbNode_s *node, rbNode_s *parent, rbNode_s *grand);
/* case 5: N is added to grandparent -> child[LEFT] -> child[LEFT]
                            OR
          N is added to grandparent -> child[RIGHT] -> child[RIGHT] */
static void insert_case_5(cardStack_s *tree, rbNode_s *node, rbNode_s *parent, rbNode_s *grand);

                /* deletion */
/* remove the given node from the tree */
static uint32_t remove_node(cardStack_s *tree, rbNode_s *match);

/* removes everything from the tree. returns 1 if success, 0 if failure */
static uint32_t remove_rbTree(rbNode_s **node);

                /* removal cases */
/* case 1: replacement node N is the nwew root */
static void removal_case_1(cardStack_s *tree, rbNode_s *node);
/* case 2: S is red */
static void removal_case_2(cardStack_s *tree, rbNode_s *node);
/* case 3: parent, sibling, and the siblings children are all black */
static void removal_case_3(cardStack_s *tree, rbNode_s *node);
/* case 4: S & S's children are black, parent is red */
static void removal_case_4(cardStack_s *tree, rbNode_s *node);
/* case 5: S is black, S's left child is red, right child is black, N is left
   child of its parent. */
static void removal_case_5(cardStack_s *tree, rbNode_s *node);
/* case 6: S is black, S's right child is red, N is left child of its parent */
static void removal_case_6(cardStack_s *tree, rbNode_s *node);

                /* display */
/* display every nodes data, and its color/flag */
static uint32_t display_rbTree(rbNode_s *node);

                /* utility functions */
/* count every instance of a piece of data, return count */
static uint32_t count_data(rbNode_s *node, int32_t toCount);
/* return the total ammount of nodes in the cardStack_s */
static uint32_t count_total(rbNode_s *node);

               /* red black tree utility */
/* rotate a node left, parent = node -> parent */
static void left_rotate(cardStack_s *tree, rbNode_s *node);
/* rotate a node right */
static void right_rotate(cardStack_s *tree, rbNode_s *node);
/* retrieve the node that has a match */
static rbNode_s* rb_match(rbNode_s *node, const int32_t pin);
/* find in order predecessor, replace data with node, return predecessor */
static rbNode_s* replace_predecessor(rbNode_s *node);

                /* end static functions #}}} */


                   /* initializations */

void rbTree_init(cardStack_s *tree)/*#{{{*/
{
    /* check if tree is allready made for the pointer */
    assert(tree != NULL);
    
    /* initialize root */
    tree -> root = NULL;
} /* end rb_init #}}} */

                   /* insertion */

uint32_t give_data(cardStack_s *tree, card_s *toAdd)/*#{{{*/
{
    if(!tree)
    {
        noerrMsg("give_data: Tree is NULL");
        return 0;
    }

    return place_data(tree, toAdd);
} /* end give_data #}}} */

uint32_t place_data(cardStack_s *tree, card_s *toAdd)/*#{{{*/
{
    uint32_t lastTurn = 0;
    rbNode_s *newNode = NULL;   /* holds the located of newest node for checks */
    rbNode_s *current = NULL;   /* current pointer for traversal */
    rbNode_s *previous = NULL;  /* previous pointer for traversal */

    current = tree -> root;

    /* traverse to insertion position, previous is parent */
    while(current != NULL)
    {
        previous = current;
        if(toAdd -> pinNum < current -> data -> pinNum)
        {
            /* go to left child */
            current = current -> child[LEFT];
            lastTurn = LEFT;
        } /* end if */
        else
        {
            /* go to right child */
            current = current -> child[RIGHT];
            lastTurn = RIGHT;
        } /* end else */
    } /* end while */
    
    /* allocate and set the new node to be added */
    newNode = (rbNode_s*) malloc(sizeof(rbNode_s));
    if(newNode == NULL)
    {
        errMsg("give_data: new node failed to alloc");
        return 0;
    } 
    
    /* Since a card_s is passed, and data gets set to a pointer to card_s,
       There is no need to allocate this data. 
    newNode -> data = (card_s*) malloc(sizeof(card_s*));
    if(newNode -> data == NULL)
    {
        errMsg("give_data: data failed to alloc");
        return 0;
    } */

    newNode -> data        = toAdd;
    newNode -> parent      = previous;
    newNode -> child[LEFT] = newNode -> child[RIGHT] = NULL;
    newNode -> cflag       = RED;

    /* insert the new node into the red black tree data structure */
    /* if previous exists, root must also, else set root */
    (previous != NULL) ? (previous -> child[lastTurn] = newNode)
                       : (tree -> root = newNode);

    /* checks integrity of the tree at the inserted node */
    check_insert_cases(tree, newNode);

    return 1;
} /* end give_data #}}} */

void check_insert_cases(cardStack_s *tree, rbNode_s *current)/*#{{{*/
{
    rbNode_s *grand = NULL;  /* grand of current node */
    rbNode_s *uncle = NULL;  /* uncle of current node */
    rbNode_s *parent = NULL; /* parent of current node */
    
    /* initialization */
    parent = current -> parent; /* current will always exist in this implementation */
    grand = find_parent(parent);
    uncle = find_uncle(current, grand);
    
    /* case 1: N is the root node */
    if(insert_case_1(current, parent)){
        return;}

    /* case 2: N's parent is black */
    if(insert_case_2(parent)){
        return;}

    /* case 3: N's parent and uncle are red */
    if(insert_case_3(tree, parent, grand, uncle)){
        return;}

    /* case 4: N is added to the to grandparent -> child[LEFT] -> child[RIGHT]
               N is added to the grandparent -> child[RIGHT] -> child[LEFT] */
    if(insert_case_4(tree, current, parent, grand)){
        return;}

    /* case 5: N is added to grandparent -> child[LEFT] -> child[LEFT]
               N is added to grandparent -> child[RIGHT] -> child[RIGHT] */
    insert_case_5(tree, current, parent, grand);
} /* end check_insert_cases #}}} */

uint32_t insert_case_1(rbNode_s *node, rbNode_s *parent)/*#{{{*/
{
    /* if node is top of tree (no parent) */
    if(!parent)
    {
        node -> cflag = BLACK;
        return 1;
    } /* end if */

    /* case was not valid */
    return 0;
} /* end insert_case_1 #}}} */

uint32_t insert_case_2(rbNode_s *parent)/*#{{{*/
{
    /* if parent is black */
    if(parent && !parent -> cflag){
        return 1;}
    
    /* parent was red */
    return 0;
} /* end insert_case_2 #}}} */

uint32_t insert_case_3(cardStack_s *tree, rbNode_s *parent, rbNode_s *grand, rbNode_s *uncle)/*#{{{*/
{
    /* if the grand and  uncle exists and its color is red */
    if(uncle && uncle -> cflag)
    {
        parent -> cflag = BLACK;
        uncle -> cflag = BLACK;

        /* if theres an uncle theres a grandparent */
        grand -> cflag = RED;

        check_insert_cases(tree, grand);
        return 1;
    } /* end if */

    return 0;
} /* end insert_case_3 #}}} */

uint32_t insert_case_4(cardStack_s *tree, rbNode_s *node, rbNode_s *parent, rbNode_s *grand)/*#{{{*/
{
    if(node == parent -> child[RIGHT] && parent == grand -> child[LEFT])
    {
        /* rotate parent left */
        left_rotate(tree, parent);
        
        /* fix result with case 5 */
        node = node -> child[LEFT];
        insert_case_5(tree, node, node -> parent, node -> parent -> parent);
        return 1;
    } /* end if */

    if(node == parent -> child[LEFT] && parent == grand -> child[RIGHT])
    {
        /* rotate parent right */
        right_rotate(tree, parent);

        /* fix result with case 5 */
        node = node -> child[RIGHT];
        insert_case_5(tree, node, node -> parent, node -> parent -> parent);
        return 1;
    } /* end if */
   
    return 0;
} /* end insert_case_4 #}}} */

void insert_case_5(cardStack_s *tree, rbNode_s *node, rbNode_s *parent, rbNode_s *grand)/*#{{{*/
{
    if(node == parent -> child[LEFT] && parent == grand -> child[LEFT])
    {
        /* adjust colors */
        parent -> cflag = BLACK;
        grand -> cflag = RED;

        right_rotate(tree, grand);
    } /* end if */
    else
    {
        assert(node == parent -> child[RIGHT] && parent == grand -> child[RIGHT]);
        /* adjust colors */
        parent -> cflag = BLACK;
        grand -> cflag = RED;

        left_rotate(tree, grand);
   } /* end else */
} /* end insert_case_5 #}}} */


                   /* deletion */

uint32_t remove_first(cardStack_s *tree, const char *toRemove, const int32_t pin)/*#{{{*/
{
    rbNode_s *match = NULL;  /* node that matched toRemove */

    if(!tree)
    {
        noerrMsg("remove_first: tree not initialized, nothing to remove.");
        return 0;
    }

    /* find the node that has matching data, match will be parent -> child */
    match = rb_match(tree -> root, pin);

    if(remove_node(tree, match)){
        return 1;}

    return 0;
} /* end first_remove #}}} */

/* OPT NOTE: this function starts from root each time it searchs for the item
             to be removed. Much of the tree changes with removal, therefore
             this may be a required lag as appose to doing it in 1 search. 
             More analysis is required for me to be 100% certin */
uint32_t remove_each(cardStack_s *tree, const char *toRemove, const int32_t pin)/*#{{{*/
{   
    uint32_t removals = 0;
    
    /* while something gets removed, removes all instances of toRemove */
    while(remove_first(tree, toRemove, pin)){
          ++removals;}

    return removals;
} /* end remove_each #}}} */

void remove_allRB(cardStack_s *tree)/*#{{{*/
{
    if(!tree && !tree -> root)
    {
        noerrMsg("remove_allRB: tree not initialized, nothing to remove.");
        return;
    } /* end if */

    remove_rbTree(&tree -> root);
} /* end remove_allRB #}}} */

uint32_t remove_rbTree(rbNode_s **node)/*#{{{*/
{
    if(*node == NULL){
        return 0;}
    
    remove_rbTree(&(*node) -> child[LEFT]);
    remove_rbTree(&(*node) -> child[RIGHT]);

    /* if we are at a leaf, deallocate it */
    if(!(*node) -> child[LEFT] && !(*node) -> child[RIGHT])
    {
        /* free data */
        delete_card((*node) -> data);
        /* free node */
        free(*node);
        /* set to NULL for parent node to become a leaf */
        *node = NULL;
    } /* end if */

    return 1;
} /* end remove_rbTree #}}} */


                  /* removal cases */

uint32_t remove_node(cardStack_s *tree, rbNode_s *match) /*#{{{*/
{
    rbNode_s *child = NULL; /* childs of node to be free */
    rbNode_s *pred = NULL;  /* predecessor of node - match */

    if(match == NULL){
        return 0;} /* no match to find */

    if(match -> child[LEFT] != NULL && match -> child[RIGHT] != NULL)
    {
        /* find in order predecessor and replace the data in node, gets nodes
           address */
        pred = replace_predecessor(match);
        match = pred;
    } /* end if */

    assert(match -> child[LEFT] == NULL || match -> child[RIGHT] == NULL);
    child = (match -> child[RIGHT] == NULL) ? match -> child[LEFT] : match -> child[RIGHT];

    if(match -> cflag == BLACK)
    {
        match -> cflag = (child == NULL) ? BLACK : child -> cflag;
        removal_case_1(tree, match);
    } /* end if */
    
    /* replace match (to free) with its child */
    if(match -> parent == NULL){
        tree -> root = child;} /* end if */
    else
    {
        if(match == match -> parent -> child[LEFT]){
            match -> parent -> child[LEFT] = child;}  /* end if */
        else{
            match -> parent -> child[RIGHT] = child;} /* end else */
    } /* end else */

    if(child != NULL){
        child -> parent = match -> parent;}
    
    /* ensure root is black */
    if(match -> parent == NULL && child != NULL){
        child -> cflag = BLACK;}
    
    delete_card(match -> data);
    free(match);

    return 1;
} /* end remove_node #}}} */

void removal_case_1(cardStack_s *tree, rbNode_s *node)/*#{{{*/ 
{
    /* nodes parent is NULL */
    if(node -> parent != NULL){
        removal_case_2(tree, node);}
} /* end removal_case_1 #}}} */

void removal_case_2(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    rbNode_s *sibling = NULL;
    sibling = find_sibling(node);
    
    /* if sibling is red */
    if(sibling -> cflag == RED)
    {
        node -> parent -> cflag = RED;
        sibling -> cflag = BLACK;
        if(node == node -> parent -> child[LEFT]){
            left_rotate(tree, node -> parent);}  /* end if */
        else{
            right_rotate(tree, node -> parent);} /* end else */
    } /* end if */

    removal_case_3(tree, node);
} /* end removal_case_2 #}}} */

void removal_case_3(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    rbNode_s *sibling = find_sibling(node);
    int32_t cLeft = find_rbcolor(sibling -> child[LEFT]);
    int32_t cRight = find_rbcolor(sibling -> child[RIGHT]);

    if( (node -> parent -> cflag == BLACK) &&
        (sibling -> cflag == BLACK) &&
        (cLeft == BLACK) &&
        (cRight == BLACK))
    { /* start if */
        sibling -> cflag = RED;
        removal_case_1(tree, node -> parent);
    } /* end if */
    else
    {
        removal_case_4(tree, node);
    } /* end else */
} /* end removal_case_3 #}}} */

void removal_case_4(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    rbNode_s *sibling = find_sibling(node);
    int32_t cLeft = find_rbcolor(sibling -> child[LEFT]);
    int32_t cRight = find_rbcolor(sibling -> child[RIGHT]);
    
    if( (node -> parent -> cflag == RED) &&
        (sibling -> cflag == BLACK) &&
        (cLeft == BLACK) &&
        (cRight == BLACK))
    { /* start if */
        sibling -> cflag = RED;
        node -> parent -> cflag = BLACK;
    } /* end if */
    else
    {
        removal_case_5(tree, node);
    } /* end else */
} /* end removal_case_4 #}}} */

void removal_case_5(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    rbNode_s *sibling = find_sibling(node);
    int32_t cLeft = find_rbcolor(sibling -> child[LEFT]);
    int32_t cRight = find_rbcolor(sibling -> child[RIGHT]);

    if(sibling -> cflag == BLACK)
    {
        if( (node == node -> parent -> child[LEFT]) &&
            (cRight == BLACK) &&
            (cLeft == RED)) 
        { /* start if */
            sibling -> cflag = RED;
            sibling -> child[LEFT] -> cflag = BLACK;
            right_rotate(tree, sibling);
        } /* end if */
        else
        {
            if( (node == node -> parent -> child[RIGHT]) &&
                (cLeft == BLACK) &&
                (cRight == RED))
            { /* start if */
                sibling -> cflag = RED;
                sibling -> child[RIGHT] -> cflag = BLACK;
                left_rotate(tree, sibling);
            } /* end if */
        } /* end else */
    } /* end if */

    removal_case_6(tree, node);
} /* end removal_case_5 #}}} */

void removal_case_6(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    rbNode_s *sibling = find_sibling(node);

    sibling -> cflag = node -> parent -> cflag;
    node -> parent -> cflag = BLACK;

    if(node == node -> parent -> child[LEFT])
    {
        assert(sibling -> child[RIGHT] -> cflag == RED);

        sibling -> child[RIGHT] -> cflag = BLACK;
        left_rotate(tree, node -> parent);
    }
    else
    {
        assert(sibling -> child[LEFT] -> cflag == RED);

        sibling -> child[LEFT] -> cflag = BLACK;
        right_rotate(tree, node -> parent);
    } /* end else */
} /* end removal_case_6 #}}} */


                 /* utility */

uint32_t data_count(cardStack_s *tree, int32_t pin)/*#{{{*/
{
    return count_data(tree -> root , pin);
} /* end data_count #}}} */

uint32_t count_data(rbNode_s *node, int32_t toCount)/*#{{{*/
{
    uint32_t match = 0; /* total times match was found */

    if(node == NULL){
        return 0;}
    
    /* check if current nodes data is a match */
    match = (toCount == node -> data -> pinNum) ? 0 : 1; 

    return count_data(node -> child[RIGHT], toCount) 
           + count_data(node -> child[LEFT], toCount) + match;
} /* end count_data #}}} */

uint32_t total_data_count(cardStack_s *tree)/*#{{{*/
{
    if(!tree || !tree -> root){
        return 0;}

    return count_total(tree -> root);
} /* end total_data_count #}}} */

uint32_t count_total(rbNode_s *node)/*#{{{*/
{
    if(!node){
        return 0;}

    return count_total(node -> child[LEFT]) + count_total(node -> child[RIGHT]) 
           + 1;
} /* end count_total #}}} */

inline void left_rotate(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    /* save nodes right, and nodes parent */
    rbNode_s *sRight = node -> child[RIGHT];
    rbNode_s *nParent = node -> parent;

    /* fix root */
    if(node -> parent == NULL){
        tree -> root = sRight;}
    
    /* rotate */
    node -> child[RIGHT] = sRight -> child[LEFT];
    sRight -> child[LEFT] = node;

    /* adjust nparent accordingly */
    if(nParent)
    {
        if(nParent -> child[LEFT] == node){
            nParent -> child[LEFT] = sRight;}  /* end if */
        else{
            nParent -> child[RIGHT] = sRight;} /* end else */
    } /* end if */

    /* fix parents */
    sRight -> parent = nParent;
    node -> parent = sRight;

    if(node -> child[RIGHT] != NULL){
        node -> child[RIGHT] -> parent = node;}
} /* end left_rotate #}}} */

inline void right_rotate(cardStack_s *tree, rbNode_s *node)/*#{{{*/
{
    /* save nodes left, and nodes parent */
    rbNode_s *sLeft = node -> child[LEFT];
    rbNode_s *nParent = node -> parent;
    
    /* fix root */
    if(node -> parent == NULL){
        tree -> root = sLeft;} 

    /* rotate */
    node -> child[LEFT] = sLeft -> child[RIGHT];
    sLeft -> child[RIGHT] = node;

    /* adjust nparent accordingly */
    if(nParent)
    {
        if(nParent -> child[LEFT] == node){
            nParent -> child[LEFT] = sLeft;}  /* end if */
        else{
            nParent -> child[RIGHT] = sLeft;} /* end else */
    } /* end if */

    /* fix parents */
    sLeft -> parent = nParent;
    node -> parent = sLeft;
    if(node -> child[LEFT] != NULL){
        node -> child[LEFT] -> parent = node;}
} /* end right_rotate #}}} */

struct Card* rb_find(cardStack_s *tree, const int32_t pin)/*#{{{*/
{
    rbNode_s *temp = NULL;
    temp = rb_match(tree -> root, pin);
    return temp -> data;
} /* end find_place #}}} */

rbNode_s* rb_match(rbNode_s *node, const int32_t pin)/*#{{{*/
{
    if(node == NULL){
        return NULL;}

    if(pin == node -> data -> pinNum){
        return node;}

    if(pin < node -> data -> pinNum)
    {
        return rb_match(node -> child[LEFT], pin);
    } /* end if **/
    else
    {
        assert(pin > node -> data -> pinNum);
        return rb_match(node -> child[RIGHT], pin);
    } /* end else */
} /* end find_place #}}} */
                
rbNode_s* replace_predecessor(rbNode_s *node)/*#{{{*/
{
    rbNode_s *current = NULL;

    assert(node != NULL);

    current = node -> child[LEFT];

    if(current)
    {
        /* get the predecessor */
        while(current -> child[RIGHT] != NULL){
            current = current -> child[RIGHT];}
        
        /* replace the card information from current into node */
        copy_card(node -> data, current -> data);

        return current;
    }

    return node;
} /* end replace_predecessor #}}} */
           
                /* display */
/* TODO: test the display tree function. This is probably only going to
               be used for debugging since we will only be displaying a couple
               cards, not the entire effing tree. */
uint32_t display_all(cardStack_s *tree)/*#{{{*/
{
    if(!tree || !tree -> root)
   {
        noerrMsg("display_all: tree is empty, nothing to display");
        return 0;
    }

    return display_rbTree(tree -> root);
} /* end display_all #}}} */

uint32_t display_rbTree(rbNode_s *node)/*#{{{*/
{
    if(node == NULL){
        return 0;}

    display_rbTree(node -> child[LEFT]);

    printf("data: %s, flag: %d, parent: %s\n",node -> data -> name, node -> cflag,
                            (node -> parent) ? node -> parent -> data -> name : 0);

    display_rbTree(node -> child[RIGHT]);

    return 1;
} /* end display_all #}}} */

/* undef's to clean up namespace #{{{ */
#undef BLACK
#undef RED
#undef LEFT
#undef RIGHT /*#}}}*/
