/*
    Implements the functions declared in whiskList.h

    filename: whiskList.c
*/


#include "whiskList.h"

#define CHECK_SIZE    20 /* the ammount of possible matches check_spelling is
                            allowed to find */

                    /* static functions */
/* Gets called when a whiskey was not found in the database.
   Horrible spellchecker. :D Searches the entire table for names that are 
   similar to the passed whiskName. Then displays the resulting options.
   Returns: 
   Errors:
*/
static int32_t check_spelling(whiskTable_s *Restrict whiskData, 
                              char *Restrict whiskName, char* posNames[])
{
    static int32_t position = 0;
    int32_t i = 0;
    
    /* end of table reached */
    if(++position == _TBL_SIZE_)
    {

    }

    /* TODO: check the string, if there are less than X different characters, it is
       a potential match. If it is an exact match, place the match in
       posNames[0], return 1 */


    check_spelling(whiskData, whiskName, posNames);
} /* end check_spelling */


                    /* header functions */
void fill_whiskTable(whiskTable_s *Restrict whiskData)
{
    /* loops */
    /* TODO */
    /* gather a BUFF ammount of whiskey information from a file */

    /* create a whiskData Node for each whiskey info in BUFF */

    /* insert each whiskData Node into the table */

    /* end loop -- after all data is pulled from the file */
}

void empty_whiskTable(whiskTable_s *Restrict whiskData)
{
    /* call the destructor for the table */
}

char* check_whiskeyName(whiskTable_s *Restrict whiskData, char *Restrict whiskName)
{
    char* posNames[CHECK_SIZE] = {NULL};
    int32_t match = 0;

    assert(whiskName != NULL);

    match = check_spelling(whiskData, whiskName, posNames);
    
    /* no match, no corrections */
    if(match == -1){
        return NULL;}
    
    /* exact match */
    if(match == 1){
        return *posNames;}

    /* TODO: display and suggest corrections */

} /* end check_whiskeyName */

/* TODO: This function will call the spell check when the whiskey searched for was
   not found. */
int32_t id_whiskey(whiskTable_s *Restrict whiskData, char *Restrict whiskName)
{
    /* find the node in the hash table */

    /* TODO: if the node does not exist, double check the spelling with the user
       using whatever spell check i come up with */
}

