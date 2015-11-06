/*
    implements all the functions declared in card.h
    filename: card.c
*/


#include "card.h"


                    /* static functions */




                    /* header functions */
cardStack_s* obtain_cardData(char *path)/*#{{{*/
{
    cardStack_s *newStack = NULL;
    newStack = alloc_cardStack();
    fill_cardStack(newStack, path);
    return NULL;
} /* end obtain_cardData #}}} */

cardStack_s* alloc_cardStack()/*#{{{*/
{
    return NULL;
} /* end alloc_cardStack #}}} */

void fill_cardStack(cardStack_s *Restrict cardData, char *path)/*#{{{*/
{

} /* end fill_cardStack #}}} */

void save_cardStack(FILE *path)/*#{{{*/
{

} /* end save_cardStack #}}} */

void empty_cardStack(cardStack_s *Restrict cardData)/*#{{{*/
{

} /* end empty_cardStacl #}}} */

card_s* card_find(cardStack_s *Restrict cardData, char *name, int32_t pin)/*#{{{*/
{
    return 0;
} /* end card_find #}}} */

int32_t check_pinNum(cardStack_s *Restrict whiskData, int32_t pinNum)/*#{{{*/
{
    return 0;
} /* end check_pinNum #}}} */
