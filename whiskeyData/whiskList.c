/*
    Implements the functions declared in whiskList.h

    filename: whiskList.c
*/


#include "whiskList.h"

#define _PERC_ACPT_   0.6 /* 1.0 == 100%, accepted spellcheck matches */
#define CHECK_SIZE    20  /* the ammount of possible matches check_spelling is
                            allowed to find */

                    /* static functions */
/* Asks the user if any of the suggested corrections are the accurate whiskName
   that was supposed to be entered. If nothing matches, they butchers the name
   or it is not in the table, but there are things close to it.
   Return: NULL when no correction was correct
   Errors: */
static char* get_correction(char *posNames[])/*#{{{*/
{
/* 2 digits 1 '\n' 1 '\0', increase if num of names increments in digits */
#define BF_SZ_ 4 

    char *curName = NULL;
    size_t inLen = 0;  /* size of the string inputed */
    int32_t i = 0;
    char ans[BF_SZ_] = {0}; /* user input for correction */

    /* explain the screen they are seeing is the spell checker */
    printf("There was no match for the whiskey name you entered.\n"
           "There could have been a typo.\n"
           "These are the potential whiskey names you may have meant\n");

    /* display the corrections to the user */
    /* loop through and display everything in posNames */
    curName = posNames[0];
    for(i = 1; curName != NULL; ++i, ++curName){
        printf("%d : %s\n", i, curName);}
   
    do
    {
        /* get input from the user.
           0 means nothing was correct */
        printf("\nPlease enter the correct corresponding number.\n"
               "(0 = nothing is correct)\n>> ");

        fgetsInput(ans, BF_SZ_, stdin, inLen);
        i = get32_t(ans, GN_NOEXIT_, "ans"); 
        /* TODO: ensure behavior with GN_NOEXIT_ is
                 appropriate when i try to break it */

        /* i cannot be larger than CHECK_SIZE, or its an invalid input. */
    }while(errno > 0 || i > CHECK_SIZE);

    free(ans); /* finished with users answer */

    /* return the resulting string */
    return posNames[i];
#undef BF_SZ_
} /* end get_corrections #}}} */

/* Compares each whiskey name with whiskName for each whiskey_s in the chain,
   starting at head.
   Places similar names into posName.
   Return: 1 if exact match is found, 0 otherwise.
   Errors:
*/
static int32_t spellCheck_chain(whiskey_s *Restrict head, char *Restrict whiskName, /*#{{{*/
                                char *posNames[], int32_t posAmmt)
{
    char *toCmp = NULL;               /* current char to cmpare from */
    char *curChar = NULL;             /* current char from head */
    float percent = 0.0;              /* the percent matching */
    int32_t i = 0;
    int32_t  matchCnt = 0;            /* number of matches in the name */
    ssize_t len = strlen(whiskName);

    if(head == NULL){
        return 0;}

    curChar = head -> whiskName;
    toCmp = whiskName;
    /* compare the two names char by char */
    for(/*i=0*/; i < len; ++i)
    {
        /* if there is a match, increase count */
        if(curChar == toCmp){
            ++matchCnt;}

        /* if every byte matched, return 1 */
        if(matchCnt == len){
            return 1;}

        /* if _PERC_ACPT_ or more percent is reached, insert into posName */
        percent = (float)matchCnt/(float)len;
        /* TODO: Once tested, place the ++ in the posNames[] array, postscript,
                 if it does not cause complications */
        if(percent >= _PERC_ACPT_)
        {
            posNames[posAmmt] = head -> whiskName;
            ++posAmmt;
            if(posAmmt == CHECK_SIZE){
                errMsg("spellCheck_chain: Maximum ammount of spell check suggestions reached");}
            i = len; /* break the loop, this frame is complete */
        } 

        ++curChar;
        ++toCmp;
    }

    return spellCheck_chain(head -> next, whiskName, posNames, posAmmt);
} /* end spellCheck_chain #}}} */

/* Gets called when a whiskey was not found in the database.
   Horrible spellchecker. :D Searches the entire table for names that are 
   similar to the passed whiskName. Then displays the resulting options.
   Returns: 
   Errors:
*/
static int32_t check_spelling(whiskTable_s *Restrict whiskData, /*#{{{*/
                              char *Restrict whiskName, char* posNames[])
{
    int32_t pos = 0;       /* current position in the table */
    int32_t pnAmmt = 0;    /* ammount of names filled into posNames */
    for(/*pos=0*/; pos < _TBL_SIZE_; ++pos)
    {
        if(spellCheck_chain(whiskData -> table[pos], whiskName, posNames, pnAmmt) == 1){
            return 1;}
    }
   
    /* no match or close match was found */
    if(pnAmmt == 0){
        return -1;}
   
    /* some close match was found */
    return 0;
} /* end check_spelling #}}} */


                    /* header functions */
whiskTable_s* obtain_whiskData(char *path)/*#{{{*/
{
    whiskTable_s *newTable = NULL;

    newTable = alloc_whiskTable();
    
    fill_whiskTable(newTable, path); 

    return newTable;
} /* end obtain_whiskData #}}} */

whiskTable_s* alloc_whiskTable()/*#{{{*/
{
    whiskTable_s *newTable = NULL;
    table_init(newTable); 
    return newTable;
} /* end alloc_whiskTable #}}} */

void fill_whiskTable(whiskTable_s *Restrict whiskData, char *path)/*#{{{*/
{
#define _B_SIZE_    1024
#define _RES_SIZE_  64
    FILE *infoWhisk = NULL;           /* file containing whisky info */
    char *whiskName = NULL;           /* name of whiskey from line */
    char *bufPl     = NULL;           /* place in buffer */
    int32_t whiskNum   = 0;           /* number corresponding to whiskName */
    size_t size        = 0;           /* holds size of new whiskName */
    char buff[_B_SIZE_] = {'\0'};     /* buffer from file */
    char resStr[_RES_SIZE_] = {'\0'}; /* string pulled from buffer */

    /* open access to file */
    infoWhisk = fopen(path,"r+"); 

    /* gather the first buffer */
    freadInput(buff, sizeof(char), _B_SIZE_, infoWhisk);
    bufPl = buff;

    /* go through entire file, aquiring whiskNames and whiskNum */
    do
    {
        fread_nextFullFile(infoWhisk, buff, sizeof(char), _B_SIZE_, bufPl,
                           resStr, *bufPl != '\n' && *bufPl != ',');

        /* if, is a digit, get whiskName 
           else, get whiskNum, insert into table */
        if(isdigit(*resStr) == 0)
        {
            size = strlen(resStr)+1; /* size of whiskName array, with '\0' */
            whiskName = (char*) malloc(sizeof(char)*size);
            resize_string(resStr, size, whiskName);
        } /* end if */
        else
        {
            whiskNum = get32_t(resStr, 0, "fill_whiskTable: whiskNum");
            table_insert(whiskData, whiskName, whiskNum);
        } /* else */
    }while(feof(infoWhisk) == 0);

    /* close access to the file */
    fclose(infoWhisk);
#undef _B_SIZE_
#undef _RES_SIZE_
} /* end fill_whiskTable #}}} */

void empty_whiskTable(whiskTable_s *Restrict whiskData)/*#{{{*/
{
    dealloc_table(whiskData);
} /* empty_whiskTable #}}} */

char* check_whiskeyName(whiskTable_s *Restrict whiskData, char *Restrict whiskName)/*#{{{*/
{
    char* posNames[CHECK_SIZE] = {NULL};
    int32_t match = 0;

    assert(whiskData != NULL && whiskName != NULL);

    match = check_spelling(whiskData, whiskName, posNames);
    
    /* no match, no corrections */
    if(match == -1){
        return NULL;}
    
    /* exact match */
    if(match == 1){
        return whiskName;}

    return get_correction(posNames);
} /* end check_whiskeyName #}}} */

int32_t id_whiskey(whiskTable_s *Restrict whiskData, char *Restrict whiskName)/*#{{{*/
{
    whiskey_s *winfo = NULL;
    char *fixedName = NULL;     /* holds potential fixed whiskName */

    /* find the node in the hash table */
    winfo = whisk_match(whiskData, whiskName);

    /* if no match was found, check spelling. If no spelling results were
       correct, exit. The name is not in the database.
       TODO: Potentially add the option to get a manager pin and include it
             with a Y/N pin: question. */
    if(winfo == NULL)
    {
        fixedName = check_whiskeyName(whiskData, whiskName);/* check spelling */
        winfo = whisk_match(whiskData, whiskName);          /* get info */
        if(fixedName == NULL || winfo == NULL){             /* check validity */
            return -1;}
    }

    /* return the corresponding whiskey number value */
    return winfo -> whiskNum;
} /* end id_whiskey #}}} */

