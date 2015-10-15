/*
    author: James Ross
    Functions implemented to handle the pope house card drama yo.

    filename: card.c
    implements card.h functions */

/* 1 function will handle all -a functions. (-aw, -anw) using diff flags. 
   1 functions will handle all -r functions. (-rw, -rn) using diff flags.
   etc. etc... for -ds, -d, -c, -n */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "card.h"

/* 
   returns a pointer to the card that needs to be manipulated
   returns: 1 on success, 0 on error.
   errors:
*/
static card_s *get_card(char *name, uint32_t pin);

/* 
   adds a whiskey to an existing card
   returns: 1 on success, 0 on error.
   errors:
*/
static int32_t add_whiskey_to_card(char *name, uint32_t whiskNum, uint32_t pin)/*#{{{*/
{
    card_s *toEdit = NULL;
    uint32_t *current = NULL;
    uint32_t newTotal = NULL;

    /* get the card that the whiskey needs to be added into */
    toEdit = get_card(name, pin);


    /* go to the end of the drank array and fill in whisk while making sure
       the whiskey does not allready exist */
    current = toEdit -> drank;
    do
    {
        if(whiskNum == *current){
            return -1;}
        ++current;
        ++newTotal;
    }while(*current != 0 && newTotal != 0);
    
    *current = whiskNum;
    ++newTotal;
    toEdit -> whiskCount;
    return 1;
} /* end add_whiskey_to_card #}}} */

/*
   adds a new card into the pile.
   returns:
   errors:
*/
static int32_t add_card(card_s *newCard)/*#{{{*/
{

    
    return 1;
} /* end add_card #}}} */


int32_t add_opt(uint32_t flags, char **args)/*#{{{*/
{
    card_s *newCard = NULL; /* card to be inserted if -anw */
    char *name = NULL;      /* name on card */
    uint32_t whisk = NULL;  /* name of whiskey */
    uint32_t pin = 0;       /* pin number on card */
    char input = '\0';      /* input from user */

    if(args == NULL){
        errnumExit(EINVAL, "add_opt: args was null, nothing to do.");}
    
    /* set components of the card culled from args */
    name = args[0]; 
    pin = getu32_t(args[2], 0, "setting pin");
    
    if(whisk = identify_whisk(args[1]) == -1)
    {
        /* if identify whisk returns no whiskey found, display what was typed in,
           aswell as similar names found in the database. Spell check it and ask
           if any of the similar ones found are corrects. Y/N */
    } 

    /* search for location of card, add a new whiskey */
    if(flags & CD_AW &&
       add_whiskey_to_card(name, whisk, pin) == -1)
    {   /* error occured, whiskey existed on the card */
        do
        {
            printf("\n%s is allready drank on %s's card.\n"
                   "Would you like to enter a different whiskey?\n"
                    "(Y/N)===> ", args[1], name);
            getChar(input);
            input = toupper(input);
        }while(input != 'Y' && input != 'N');
    } /* end if */
    else
    {   /* using a new card, insert a new card into the tree */
        newCard = create_card(name, pin);
        add_card(newCard);
    }

    printf("\nflag: %d name: %s whisk: %s pin: %u\n", flags, name, whisk, pin); 
    return 1;
} /* end add_opt #}}} */
