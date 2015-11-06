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
#define CD_AW       0x1    /* -aw */
#define CD_ANW      0x2    /* -anw */
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

/* identifies a whiskey, returning its unique identification number
   returns: whiskey identification number, -1 when not found.
   errors : EINVAL - char* is NULL.
*/
int32_t identify_whisk(whiskTable_s *whiskData, char *Restrict whisk);

/* -aw, -anw based on flag. see man.txt for information.
   Returns:  
   Errors: 
*/
int32_t add_opt(cardStack_s *Restrict cards, int32_t flags, char **args);
#endif
