/*
   Handles all the card information
   filename: card.h
*/

#ifndef _CARD_H_
#define _CARD_H_

#include "rbTree/red_black.h"

/* Allocates and fills all the card data, returning a pointer to all the
   card data information.
   Return: returns a allocated, filled, cardStack_s pointer. NULL on error
   Errors: 
*/
cardStack_s* obtain_cardData(char *path);

/* Allocates and initializes a cardStack_s, returning the allocated table
   Returns: allocated, initialize cardStack_s*
   Errors: Returns null on error. 
*/
cardStack_s* alloc_cardStack();

/* Fills the table with the card information found in cards.txt
   Returns:
   Errors:
*/
void fill_cardStack(cardStack_s *Restrict cardData, char *path);

/* Saves the contents of the cardStack to the file. Overwrites the entire file if
   any cards got deleted from the table. If only things were added to the
   table, the file will be appended.
   TODO:NOTE: not sure how the red black tree will act over time. One concern i would
         have is that all the newer people with cards, who actualy stick with it,
         will always be toward the bottom of the tree. It would be far more
         benefitial if that happens less often than not when the tree gets
         built.
   Returns:
   Errors: 
*/
void save_cardStack(FILE *path);

/* Remove all information from the cardStack_s passed.
   Returns:
   Errors:
*/
void empty_cardStack(cardStack_s *Restrict cardData);

/* Take in a pin numbe, get the information from the tree and return the
   identification number of the whiskey 
   Returns: whiskNum, or -1 if no whiskey was found.
   Errors:
*/
card_s* card_find(cardStack_s *Restrict cardData, int32_t pin);

/* Checks if the pinNum is allready in use or not.
   Returns: 1 if the pinNum allready exists. 0 if the pin number does not, and
            -1 if multiple 
   Errors: returns -1;
*/
int32_t check_pinNum(cardStack_s *Restrict whiskData, int32_t pinNum);
#endif
