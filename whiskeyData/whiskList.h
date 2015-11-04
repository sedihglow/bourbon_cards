/* 
    Handles the whiskey database information.

    I think card will need to have functions that call the functions 
    defined here. Make it so cards.h is the only header required for the
    execution of the project. 

    filename: whiskList.h
*/

#include "../sedhead.h"
#include "../hashTable/hash_table.h"

#define _MAX_WNAME    100 /* Max length of a name of a whiskey */

/* Fills the table with the whiskey information found in whiskData.txt
   Returns:
   Errors:
*/
void fill_whiskTable(whiskTable_s *Restrict whiskData);

/* Remove all information from the whiskTable passed.
   Returns:
   Errors:
*/
void empty_whiskTable(whiskTable_s *Restrict whiskData);

/* Take in a whiskey name, get the information from the table and return the
   identification number of the whiskey 
   Returns:
   Errors:
*/
int32_t id_whiskey(whiskTable_s *Restrict whiskData, char *Restrict whiskName);

/* Spell checks the name against the whiskeys in the database. Prompts the user
   with the potential matches if no exact match was found and returns the
   correct choice.
   Returns: NULL if non of the options were correct. char* correct string
   Errors:
*/
char* check_whiskeyName(whiskTable_s *Restrict whiskData, char *Restrict whiskName);
