/*
    author: James Ross
    Functions implemented to handle the pope house card drama yo.
    
    filename: card.h */

/* OR(|) these together to make a command in flags. Example:  CD_AC | CD_W, add 
   whiskey to existing card */
#ifndef _CARD_H_
#define _CARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "err_handle/err_handle.h"

/* flags used in main, from getopt. Passed to execute_flag */
#define CD_AC       00    /* Add new card information */
#define CD_RC       01    /* Remove card information */
#define CD_NDATA    02    /* New Database information */
#define CD_D        04    /* Display */
#define CD_C        010   /* Comment */
#define CD_W        020   /* Whiskey */
#define CD_N        040   /* Name */
#define CD_S        0100  /* search (used with CD_NDATA) */

/* flags used in execute_flag to inform called functions on what to execute.
   Definitions match [options] from the command line. see man.txt */
#define CD_AW       00    /* -aw */
#define CD_ANW      01    /* -anw */
#define CD_RN       02    /* -rn */
#define CD_RW       04    /* -rw */
#define CD_D_       010   /* -d */
#define CD_DW       020   /* -dw */
#define CD_DC       040   /* -dc */
#define CD_DSW      0100  /* -dsw */
#define CD_DSN      0200  /* -dsn */
#define CD_NW       0400  /* -nw */
#define CD_NN       01000 /* -nn */
#define CD_C_       02000 /* -c */
/* octoBaller */

/* max name for a person on a card. big names are big. see google. */
#define MAX_WS    256   /* largest ammount a person can drink without failure */
#define MAX_NAME  512   /* largest a persons full name can be */

/* NOTE: it may be better to make drank **drank rather than *drank and only
         allocate 50 at a time to save memory since 50 is each milestone.
         This will require adjusting the add_whiskey function. Keep it in mind
         while continuing the project furthur with adding to tree */
typedef struct Card 
{
    char *name;          /* name of the card owner */ 
    uint32_t *drank;     /* whiskeys drank. allocates room for 250 whiskeys, 
                            recorded by number corresponding to database */
    uint32_t pinNum;     /* pin number used to identify the person */
    uint32_t whiskCount; /* current number of whiskeys drank */

} card_s;

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
static inline card_s* create_card(uint32_t pin, char *name)/*#{{{*/
{
    int32_t len = 0;
    card_s *newCard = NULL;

    if(name == NULL){
        noerrExit("Requires a name for insertion to card.");}
    
    newCard = (card_s*) malloc(sizeof(card_s));
    len = strlen(name) + 1;
    newCard -> name = (char*) malloc(sizeof(char) * len);
    strncpy(newCard -> name, name, len);

    newCard -> drank = (uint32_t*) malloc(sizeof(uint32_t) * MAX_WS);
    memset(newCard -> drank, 0, sizeof(uint32_t) * MAX_WS);
    newCard -> pinNum     = pin;
    newCard -> whiskCount = 0;

    return newCard;
} /* end create_card #}}} */

/* -aw, -anw based on flag. see man.txt for information.
   Returns:  
   Errors: 
*/
int32_t add_opt(uint32_t flags, char **args);
#endif
