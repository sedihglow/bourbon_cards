/* 
    Handles the whiskey database information.
    filename: whiskList.h
*/
#include "hashTable/hash_table.h"

#define _MAX_WNAME    100    /* Max length of a name of a whiskey */

/* Allocates and fills all the whiskey data, returning a pointer to all the
   whiskey data information.
   Return: returns a allocated, filled, table pointer. NULL on error
   Errors: 
*/
whiskTable_s* obtain_whiskData(char *path);

/* Allocates and initializes a whiskyTable_s, returning the allocated table
   Returns: allocated, initialize whiskTable_s*
   Errors: Errors not handled, only potential error is not enough memory 
*/
whiskTable_s* alloc_whiskTable();

/* Fills the table with the whiskey information found in whiskData.txt
   Returns:
   Errors:
*/
void fill_whiskTable(whiskTable_s *Restrict whiskData, char *path);

/* Saves the contents of the table to the file. Overwrites the entire file if
   any whiskey got deleted from the table. If only things were added to the
   table, the file will be appended.
   Returns:
   Errors: 
*/
void save_whiskTable(FILE *path);

/* Appened a new whiskey and whiskey number into the whiskey database.
   - data, FILE*    , holds the whiskey names and corresponding numbers
   - whiskName char*, string of the whiskey to be saved's name.
   - wnum, int      , whiskey number corresponding to the name.
   Errors:*/
#define save_newWhisk(data, whiskName, wnum)                                   \
{ /*#{{{*/                                                                     \
    if(fseek(data, 0, SEEK_END) == -1){ /* ensures placement */                \
        errExit("save_newWhisk: fseek to SEEK_END");                           \
    fprintf(data, "%s,%d\n", whiskName, wnum);                                 \
} /* end save_newWhisk #}}} */

/* Remove all information from the whiskTable passed.
   Returns:
   Errors:
*/
void empty_whiskTable(whiskTable_s *Restrict whiskData);

/* Take in a whiskey name, get the information from the table and return the
   identification number of the whiskey 
   Returns: whiskNum, or -1 if no whiskey was found.
   Errors:
*/
int32_t id_whiskey(whiskTable_s *Restrict whiskData, char *Restrict whiskName);

/* Spell checks the name against the whiskeys in the database. Prompts the user
   with the potential matches if no exact match was found and returns the
   correct choice.
   Returns: NULL if non of the options were correct. char* correct string.
   Errors:
*/
char* check_whiskeyName(whiskTable_s *Restrict whiskData, char *Restrict whiskName);
