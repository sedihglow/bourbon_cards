/*
    author: James Ross
    Functions implemented to handle the pope house card drama yo.

    filename: card.c
    implements card.h functions */

/* 1 function will handle all -a functions. (-aw, -anw) using diff flags. 
   1 functions will handle all -r functions. (-rw, -rn) using diff flags.
   etc. etc... for -ds, -d, -c, -n */

#include "card.h"
                    /* static functions */
/* 
   Adds a whiskey to an existing card
   Returns: 1 on success, -1 on error. 0 when not found.
   Errors: EINVAL - null pointer was passed.
*/
static int32_t add_whiskey_to_card(rbTree_s *Restrict cards, char *Restrict name,/*#{{{*/
                                   int32_t whiskNum, int32_t pin)
{
    card_s *toEdit = NULL;
    int32_t *current = NULL;
    int32_t newTotal = NULL;

    assert(cards != NULL && name != NULL);

    /* get the card that the whiskey needs to be added into */
    toEdit = rb_find(cards, name, pin);

    /* go to the end of the drank array and fill in whisk while making sure
       the whiskey does not allready exist */
    current = toEdit -> drank;
    do
    {
        if(whiskNum == *current){
            return 0;}
        ++current;
        ++newTotal;
    }while(*current != 0 && newTotal != 0);
    
    *current = whiskNum;
    ++newTotal;
    ++(toEdit -> whiskCount);
    return 1;
} /* end add_whiskey_to_card #}}} */

/*
   adds a new card into the pile.
   returns:
   errors:
*/
static int32_t add_card(card_s *Restrict newCard)/*#{{{*/
{

    return 1;
} /* end add_card #}}} */

                    /* header functions */
int32_t identify_whisk(whiskTable_s *whiskData, char *Restrict whisk)/*#{{{*/
{
    assert(whisk != NULL && whiskData != NULL);
    return id_whiskey(whiskData, whisk);
} /* end identify_whisk #}}} */

int32_t add_opt(rbTree_s *Restrict cards, int32_t flags, char **args)/*#{{{*/
{
    card_s *newCard = NULL;           /* card to be inserted if -anw */
    whiskTable_s *whiskData = NULL;    /* whiskey data */
    char *name = NULL;                /* name on card args[0] */
    int32_t whisk = NULL;             /* identity of whiskey name args[2] */
    int32_t pin = 0;                  /* pin number on card args[1]*/
    char input = '\0';                /* input from user */

    if(args == NULL){
        errnumExit(EINVAL, "add_opt: args was null, nothing to do.");}

    whiskData = obtain_whiskData();

    /* set components of the card culled from args */
    name = args[0]; 
    pin = get32_t(args[2], 0, "setting pin");

    /* identify the whiskey entered,  */
    if((whisk = identify_whisk(whiskData, args[1])) == -1)
    {
        /* TODO:
           if identify whisk returns no whiskey found, display what was typed in,
           aswell as similar names found in the database. Spell check it and ask
           if any of the similar ones found are corrects. Y/N, 

           The spell check may be its own function, it might be called
           inside identify whiskey.*/
    } 

    /* search for location of card, add a new whiskey */
    if(flags & CD_AW &&
       add_whiskey_to_card(cards, name, whisk, pin) == 0)
    {   
        /* error occured, whiskey existed on the card */
        yesNo(input, "\n%s is allready drank on %s's card.\n"
                     "Would you like to enter a different whiskey?\n"
                     "(Y/N)===> ", args[1], name);
        /* TODO: implement calling add_whiskey_to_card again if the
                 response was yes. */
    } 
    else
    {  
        /* using a new card, insert a new card into the tree */
        newCard = create_card(name, pin);
        add_card(newCard);
    }

    printf("\nflag: %"PRId32" name: %s whisk: %s pin: %"PRId32"\n", flags, name, args[1], pin);
    return 1;
} /* end add_opt #}}} */

