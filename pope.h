/*
    author: James Ross
    Functions implemented to handle the pope house card drama yo.
    
    filename: pope.h 
*/

/* OR(|) these together to make a command in flags. Example:  CD_AC | CD_W, add 
   whiskey to existing card */


#ifndef _POPE_H_
#define _POPE_H_

#include "whiskeyData/whiskList.h"
#include "cardData/card.h"

/* flags used in main, from getopt. Passed to execute_flag */
#define CD_AC       0x1   /* Add new card information */
#define CD_RC       0x2   /* Remove card information */
#define CD_NDATA    0x4   /* New Database information */
#define CD_D        0x8   /* Display */
#define CD_C        0x10  /* Comment */
#define CD_W        0x20  /* Whiskey */
#define CD_N        0x40  /* Name */
#define CD_S        0x80  /* search (used with CD_NDATA) */

/* flags used in execute_flag to inform called functions on what to execute.
   Definitions match [options] from the command line. see man.txt */
#define CD_AW       0x1    /* -aw , add whiskey */
#define CD_ANW      0x2    /* -anw, add name whiskey */
#define CD_RN       0x4    /* -rn */
#define CD_RW       0x10   /* -rw */
#define CD_D_       0x20   /* -d */
#define CD_DW       0x40   /* -dw */
#define CD_DC       0x80   /* -dc */
#define CD_DSW      0x100  /* -dsw */
#define CD_DSN      0x200  /* -dsn */
#define CD_NW       0x400  /* -nw */
#define CD_NN       0x800  /* -nn */
#define CD_C_       0x1000 /* -c */

                /* card functions */

/* Adds newCard into the deck. Not saved into database.
   Returns:
   Errors: */
int32_t addNewCard(cardDeck_s *Restrict cards, card_s *Restrict newCard);

/* Replaces a cards information with the new information in toSave.
   Returns:
   Errors: */
int32_t saveExistingCard(cardDeck_s *Restrict cards, card_s *Restrict toSave,
                         int pin);

/* saved a cards information into the file that holds alll card information.
   - data, FILE *, pointer to the filestream of card database.
   - toSave, card_s*, the new card to save into the file. */
/* TODO: check saveNewCard()'s implementation for placement in file. Should
         data allready be at the proper location? Probably not. */
#define saveNewCard_File(data, toSave) {saveNewCard(data, toSave);}

/* Remove a card from the cardDeck.
   Returns:
   Errors: */
int32_t cardFromDeck();

/* Remove a card from the data file.
   Returns:
   Errors: */
int32_t cardFromFile(cardDeck_s *Restrict cards, int32_t pin);


                /* whisk functions */

/* Identifies a whiskey, returning its unique identification number
   returns: whiskey identification number, -1 when not found.
   errors : EINVAL - char* is NULL.*/
int32_t identify_whisk(whiskTable_s *whiskData, char *Restrict whisk);

/* Adds a whiskey to an existing card
   Does NOT save to the data file
   Returns: 1 on success, -1 on error. 0 when not found.
   Errors: EINVAL - null pointer was passed. */
int32_t add_whiskey_to_card(cardDeck_s *Restrict cards, int32_t whiskNum, 
                            int32_t pin);

/* Remove whiskey from table and database.
   Returns:
   Errors : */
int32_t killWhiskData();

/* Removes a whiskey from a card in the deck
   Returns:
   Errors: */
int32_t whiskDataFromCard();

/* Add a new whiskey to the whiskey table
   Returns:
   Errors: */
int32_t whiskToTable();

/* Save a whiskey into the whiskey data file.
   Retruns:
   Errors:  */
int32_t whiskToFile();



                    /* msc. */

/* TODO: See how the spellcheck for the whiskeys should be finalized. Check it
         on user input? Let the whiskey header handle it? Look for efficiency */

#endif
