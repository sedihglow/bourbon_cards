/* 
   Specifically for testing the red black tree structure, ensureing cards
   are being inserted and removed properly. This file is based off the rbTest.c
   i made for the "generic" red black tree which only used integer data.

   testRB.c
*/


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "red_black.h" /* what we testing */
#include "../card.h"   /* card information, also included in red_black.h --; */
#include "../err_handle/err_handle.h"/* error handling functions */
#include "../get_num/get_num.h"      /* string conversions */

/* arg1 = # of inserts, arg 2 = # of removals arg3 = mod */
int32_t main(int32_t argc, char *argv[])
{
    /* rb tree to test */
    rbTree rbTest;

    uint32_t insertions = 0;   /* argv[1] */
    uint32_t removals = 0;     /* argv[2] */
    uint32_t modVal = 0;       /* argv[3] */
    uint32_t pin = 0;          /* number to add/remove */
    uint32_t resultCheck = 0;  /* proper ammount of nodes remaining */
    uint32_t totalData = 0;    /* total data count */
    uint32_t i = 0;
    card_s *cardInsert = NULL; /* card to be inserted into the RB tree */
    char *testName = (char*) malloc(sizeof(char) * MAX_NAME);
    sprintf(testName, "name1212");
    printf("testName: %s",testName);

    /* seed random for insertion/removal testing. Using time for convenience */
    srandom(1);

    /* check main args */
    if(argc != 4){
        noerrExit("Usage: $ program #inserts #removals modValue\n\n");}
    
    /* convert main args */
    insertions = getu32_t(argv[1], 0, (char*)NULL); /* # of insertions */
    removals   = getu32_t(argv[2], 0, (char*)NULL); /* # of removals */
    modVal     = getu32_t(argv[3], 0, (char*)NULL); /* mod value */
    
    if(insertions < removals){
        noerr_msg("Warning, more removals than insertions");}

    /* initialize rbTree for testing */
    rbTree_init(&rbTest);
        
    /* insert */
    for(/*i=0*/; i < insertions; ++i)
    {
        pin = random() % modVal;
        cardInsert = create_card(pin, testName);

        if(give_data(&rbTest, cardInsert) == 0){
            noerrExit("inserting data into tree");}
    } 

    /* remove */
    for(i = 0; i < removals; ++i)
    {
        pin = random() % modVal;
        cardInsert = create_card(pin, testName);

        resultCheck += remove_first(&rbTest, testName, pin);
    } 

    /* check results */
    totalData = total_data_count(&rbTest);

    printf("\nResulting node count: %d\n"
           "Resulting node count should be: %d\n"
           "Remove found no value %d times.\n"
           "Remove was attempted %d times.\n",
           totalData, (insertions - resultCheck), resultCheck, removals);

    remove_all(&rbTest);

    exit(EXIT_SUCCESS);
}  /* end main */
