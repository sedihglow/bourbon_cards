/*
    author: James Ross
    Functions implemented to handle the pope house card drama yo.

    filename: pope.c
    implements pope.h functions */

/* 1 function will handle all -a functions. (-aw, -anw) using diff flags. 
   1 functions will handle all -r functions. (-rw, -rn) using diff flags.
   etc. etc... for -ds, -d, -c, -n */

#include "pope.h"

/* saved a cards information into the file that holds alll card information.
   - data, FILE *, pointer to the filestream of card database.
   - toSave, card_s*, the new card to save into the file*/
#define saveNewCard_File(data, toSave) {saveNewCard(data, toSave);}

int32_t add_whiskey_to_card(cardDeck_s *Restrict cards, int32_t whiskNum, /*#{{{*/
                            int32_t pin)
{
    card_s *toEdit = NULL;
    int32_t *current = NULL;
    int32_t newTotal = 0;
    int32_t i = 0;

    assert(cards != NULL);

    /* get the card that the whiskey needs to be added into */
    toEdit = card_find(cards, pin);

    /* go to the end of the drank array and fill in whisk while making sure
       the whiskey does not allready exist */

    for(i = 0; toEdit -> drank[i] != NULL && i < D_PTR_MX; ++i)
    {
        current = toEdit -> drank[i];
        do
        {
            if(whiskNum == *current){
                return 0;}
            ++current;
            ++newTotal;
        }while(*current != 0 && newTotal <= H_SHOE);
    } /* end for */
   
    /* insert the whiskey at the next available place in drank, located at
       currents address */
    *current = whiskNum;
    ++(toEdit -> whiskCount);
    return 1;
} /* end add_whiskey_to_card #}}} */

int32_t addNewCard(cardDeck_s *Restrict cards, card_s *Restrict newCard)/*#{{{*/
{
    /* card should be full */
    assert(newCard != NULL && newCard -> name != NULL && newCard -> drank != NULL);

    /* place card into rbtree using card.h functions */
    insertIntoDeck(cards, newCard);
    
    /* TODO: save the new card into the database OR decide to do it outside
             of this function. */

    return 1;
} /* end add_card #}}} */

int32_t identify_whisk(whiskTable_s *whiskData, char *Restrict whisk)/*#{{{*/
{
    assert(whisk != NULL && whiskData != NULL);
    return id_whiskey(whiskData, whisk);
} /* end identify_whisk #}}} */

int32_t saveExistingCard(cardDeck_s *Restrict cards, card_s *Restrict toSave,/*#{{{*/
                         int pin)
{
    /* find where in the file existing card is. */

    /* delete the existing card from the file */

    /* save the new version of the card in the same location? (not required
       to be in the same location? Could use saveNewCard for this operation */
    
    return 1;
} /* end saveExistingCard #}}} */

