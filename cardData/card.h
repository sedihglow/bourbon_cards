/*
   Handles all the card information
   filename: card.h
*/

#ifndef _CARD_H_
#define _CARD_H_

#include "rbTree/red_black.h"

#define D_PTR_MX    5   /* number of pointers to use in drank array, card_s */
#define H_SHOE     50   /* number of bourbons for a horseshoe. Represents 1 int **drank index */

/* Allocates and fills all the card data, returning a pointer to all the
   card data information.
   Return: returns a allocated, filled, cardDeck_s pointer. NULL on error
   Errors: 
*/
cardDeck_s* obtain_cardData(char *path);

/* Allocates and initializes a cardDeck_s, returning the allocated table
   Returns: allocated, initialize cardDeck_s*
   Errors: Returns 0 on error, 1 on success (if rbTree_init's assert is diabled) */
cardDeck_s* alloc_cardDeck();

/* Adds a new card into the deck, without saving it to the database */
int32_t insertIntoDeck(cardDeck_s *Restrict cardData, card_s *Restrict toAdd);

/* Fills the table with the card information found in cards.txt
   Returns:
   Errors:
*/
void fill_cardDeck(cardDeck_s *Restrict cardData, char *path);

/* Saves a card into data which contains card information.
   - data, FILE*  , file to append the card data to.
   - card, card_s*, card to be added into the data */
inline void saveNewCard(FILE *Restrict data, card_s *Restrict card)
{/*#{{{*/
    int *position = NULL;
    int i = 0;
    int j = 0;

    if(fseek(data, 0, SEEK_END) == -1){                                        
        errExit("saveNewCard: fseek to SEEK_END");}                            
    fprintf(data, "%s\n" /* card holder name */                                
                  "%d\n" /* pin number */                                      
                  "%d\n" /* number of whiskeys drank */                        
                  , card -> name                                          
                  , card -> pinNum                                             
                  , card -> whiskCount);                                       
    /* whiskey numbers */
    for(i = 0; card -> drank[i] != NULL && i < D_PTR_MX; ++i)
    {
        position = card -> drank[i];
        for(j = 0; *position != 0 && j < H_SHOE; ++j)
        {
            fprintf(data, "%d ", *position);
            ++position;
        }
    }

    /* terminate whiskey numbers with a newline. Seek to ' ' after last whisk
       num print, and replace it with a newline */
    if(fseek(data, -1, SEEK_END) == -1){                                        
        errExit("saveNewCard: fseek to SEEK_END");}                            
    fprintf(data,"\n");
} /* end saveNewCard #}}} */

/* Remove all information from the cardDeck_s passed.
   Returns:
   Errors:
*/
void empty_cardDeck(cardDeck_s *Restrict cardData);

/* Take in a pin numbe, get the information from the tree and return the
   identification number of the whiskey 
   Returns: whiskNum, or -1 if no whiskey was found.
   Errors:
*/
card_s* card_find(cardDeck_s *Restrict cardData, int32_t pin);

/* Checks if the pinNum is allready in use or not.
   Returns: 1 if the pinNum allready exists. 0 if the pin number does not, and
            -1 if multiple 
   Errors: returns -1;
*/
int32_t check_pinNum(cardDeck_s *Restrict whiskData, int32_t pinNum);
#endif
