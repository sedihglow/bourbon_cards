/*
    written by: James Ross

    non-integrated solution to the card crap
    - have them take their card home and bring it back. If they lose it, its
      all recorded. FUCK EM

    TODO: NOTE: when taking in input from the command line from the server, make
                sure to toupper. Also make everything after a space in a name
                a capital letter..... These are tiny important bullshits :D

                Making all the names all cap would be a easy out for this stuff
                but meh.... figure it out

                All lower case letters is good too, only uppercase the name
                in the display functions that get presented to the servers.

                The main issue to take into consideration is the fact that 
                the servers will be entering things into the command line
                quickly while they are slammed. Since this is not integerated
                into the POS system at the restaraunt, making everything passed
                into argv, and making the dictionary of whiskeys, toupper into
                all capital letters. This will allow the server to screw up
                capital and lower case letters and still get the input they
                want
*/

#include "pope.h"     /* card information and related functions */

/* OR's a flag integer with the appropriate CD_FLAG basd on cmd line args */
#define opt_arg(flags)                                                         \
{ /*#{{{*/                                                                     \
    while(*optarg != '\0')                                                     \
    {                                                                          \
        (flags) |= (*optarg == 'n') ? CD_N :                                   \
                   (*optarg == 'w') ? CD_W :                                   \
                   (*optarg == 'c') ? CD_C :                                   \
                   (*optarg == 's') ? CD_S : 0;                                \
        ++optarg;                                                              \
    }                                                                          \
} /* end opt_arg #}}} */

#define execute_flag(cards, flag)                                              \
{ /*#{{{*/                                                                     \
    (flag & CD_AC)    ? ((flag & CD_W && flag & CD_N) ? add_opt(cards, CD_ANW, argv+optind)  \
                                                      : add_opt(cards, CD_AW, argv+optind)) :\
    (flag & CD_RC)    ? ((flag & CD_W) ? 0/* -rw */                 \
                                       : 0/* -rn */)  :             \
    (flag & CD_S)     ? ((flag & CD_W) ? 0/* -dsw */                \
                                       : 0/* -dsn */) :             \
    (flag & CD_C)     ? ((flag & CD_D) ? 0/* -dc */                 \
                                       : 0/* -c */)   :             \
    (flag & CD_D)     ? ((flag & CD_W) ? 0/* -dw */                 \
                                       : 0/* -d */)   :             \
    (flag & CD_NDATA) ? ((flag & CD_W) ? 0/* -nw */                 \
                                       : 0/* -nn */)  :             \
    errExit("Flags is zero, something unexplicable happened.");\
} /* end execute_flag #}}} */

/* TODO: This is currently designed for name to be passed into main with no
         white space. It does not account for a first and last name, which
         should be required. */
int32_t main(int32_t argc, char *argv[])
{
    int32_t opt = 0;     /* awpt Kappa */
    int32_t flags = 0;   /* shows what options were chosen */

    while((opt = getopt(argc, argv, "a:r:d::n:c")) != -1)
    {
        switch(opt)
        {           
            case 'a': flags |= CD_AC;    opt_arg(flags);  break;
            case 'r': flags |= CD_RC;    opt_arg(flags);  break;
            case 'd': flags |= CD_D;     opt_arg(flags);  break;
            case 'n': flags |= CD_NDATA; opt_arg(flags);  break;
            case 'c': flags |= CD_C_;    opt_arg(flags);  break;
            default: noerrExit("Arguments. Check options. $card [opt]"
                               " [name] [whiskey] [number]"); 
        } 
    } 

    /* if no option arguments are found, argc must be 3, else arg error */
    if(flags == 0 && argc != 4){
        errnumExit(EINVAL, "[options] is optional. $card [opt] [name] [whiskey]"
                           " [number]");}

    if(flags == 0){
        flags |= (CD_AC | CD_W);}

    /* main loads in all the previous data from the saved file or whatever.
       It will fill the hashTable with whiskey information.
       It will fill the red black tree with card information.

       Then we operate on it after this is done. This means we will have
       a table and a tree made in main, and they will be sent into the
       appropriate functions */

    execute_flag(NULL, flags);

    return 0;
} /* end main */

