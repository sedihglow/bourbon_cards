void fill_whiskTable(whiskTable_s *Restrict whiskData, char *path)/*#{{{*/
{
/* buffer size for a single line in the file. whiskname,num */
#define __BUF_S    64
    FILE *infoWhisk = NULL;   /* file containing whisky info */
    char *buffer = NULL;      /* 1 line from infoWhisk */
    char *whiskName = NULL;   /* name of whiskey from line */
    char *chPos = NULL;       /* character position when copying from buffer */
    ssize_t inpLen = 0;       /* length line input from infoWhisk */
    int32_t whiskNum = 0;     /* number corresponding to whiskName */
    int32_t i = 0;
    int32_t eofRet = 0;       /* saves feof return */

    /* open access to file */
    infoWhisk = fopen(path,"r+"); 

    do
    {
        /* TODO: Replace the getLineInput functin with fscanf(). It will make the
                 for loop worthless */
        getLineInput(buffer, __BUF_S, infoWhisk, inpLen);
    
        /* TODO: make sure that feof doesnt return EOF untill after the
                 fgets that starts by reading nothing, rather than just getting
                 to the EOF location. The difference is 1 read */
        if((eofRet = feof(infoWhisk)) > 0)
        {
            /* This does not get free'd here. It is stored into the hashtable.
               it gets removed when the hash table gets removed */
            whiskName = (char*) malloc(sizeof(char)*inpLen+1);
            
            /* copy name from buffer into whiskName */
            /* the name will be at least 1 character... */
            chPos = buffer;
            for(i = 0; i < inpLen && *chPos != ','; ++i)
            {
                /* TODO: Once this is fully tested, try using postfix ++. *arr++ */
                whiskName[i] = *chPos;
                ++chPos;
            }
            ++chPos; /* increment position to place after ',' */
            /* place number with whiskey inside whiskNum */
            whiskNum = get32_t(chPos, 0, "whiskNum");

            /* insert each whiskData Node into the table */
            table_insert(whiskData, whiskName, whiskNum);
        }
    }while(eofRet > 0);

    fclose(infoWhisk);
    free(buffer);
#undef __BUF_S
} /* end fill_whiskTable #}}} */

