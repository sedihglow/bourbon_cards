/*
    author: James Ross
    Functions implemented to handle the pope house card drama yo.

    filename: card.c
    implements card.h functions */

/* 1 function will handle all -a functions. (-aw, -anw) using diff flags. 
   1 functions will handle all -r functions. (-rw, -rn) using diff flags.
   etc. etc... for -ds, -d, -c, -n */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "card.h"


int32_t add_opt(uint32_t flags, char **args)/*#{{{*/
{
    char *name = NULL;    /* name on card */
    char *whisk = NULL;   /* name of whiskey */
    uint32_t pin = 0;     /* pin number on card */

    if(args == NULL){
        errnumExit(EINVAL, "add_opt: args was null, nothing to do.");}
    
    name = args[0]; 
    whisk = args[1];
    pin = getu32_t(args[2], 0, "setting pin");



    return 1;
} /* end add_opt #}}} */
