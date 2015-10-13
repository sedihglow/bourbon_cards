/*
    written by: James Ross

    non-integrated solution to the card crap
    - have them take their card home and bring it back. If they lose it, its
      all recorded. FUCK EM
*/

#include <sys/types.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "rbTree/red_black.h"         /* red black tree data structure */
#include "get_num/get_num.h"          /* convert strings to numbers */
#include "err_handle/err_handle.h"    /* error handling functions */
#include "card.h"                     /* card information and related functions */

/* OR's a flag integer with the appropriate CD_FLAG basd on cmd line args */
#define opt_arg(flags)/*#{{{*/{                \
    while(optarg != '\0')                      \
    {                                          \
        (flags) |= (*optarg == 'n') ? CD_N :   \
                   (*optarg == 'w') ? CD_W :   \
                   (*optarg == 'c') ? CD_C :   \
                   (*optarg == 's') ? CD_S : 0;\
    }                                          \
} /* end opt_arg #}}} */

#define execute_flag(flag)/*#{{{*/                                       \
{                                                                        \
    (flag & CD_AC)    ? ((flag & CD_W) ? add_opt(CD_AW, argv+optind)     \
                                       : add_opt(CD_ANW, argv+optind))  :\
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

int32_t main(int32_t argc, char *argv[])
{
    int32_t opt = 0;    /* awpt Kappa */
    int32_t flags = 0;  /* shows what options were chosen */

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
        flags |= (CD_AC | CD_N | CD_W);}

    execute_flag(flags);

    return 0;
} /* end main */

