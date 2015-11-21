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
    cardStack_s *newDeck = (cardStack_s*) malloc(sizeof(cardStack_s));
    rbTree_init(newDeck);
    return NULL; /* newDeck will be null if malloc failed */
} /* end alloc_cardStack #}}} */

void fill_cardStack(cardStack_s *Restrict cardData, char *path)/*#{{{*/
{


} /* end fill_cardStack #}}} */

void save_cardStack(FILE *path)/*#{{{*/
{

} /* end save_cardStack #}}} */

void empty_cardStack(cardStack_s *Restrict cardData)/*#{{{*/
{
    remove_allRB(cardData);
} /* end empty_cardStacl #}}} */

card_s* card_find(cardStack_s *Restrict cardData, int32_t pin)/*#{{{*/
{
    return rb_find(cardData, pin);
} /* end card_find #}}} */

int32_t check_pinNum(cardStack_s *Restrict cardData, int32_t pinNum)/*#{{{*/
{
    uint32_t totalCount = 0;

    totalCount = data_count(cardData, pinNum);

    if(totalCount > 1)
    {
        noerrMsg("check_pinNum: there are %u of the same pin number was found "
                 "allready\ninside of the data. This should be resolved.", totalCount);
        return -1;
    }

    return totalCount;
} /* end check_pinNum #}}} */
