/* 4play.C by M. Timin, October, 2003
   functions used to play the 4 card puzzle based on the Concentration game
   
   Many of these function were written for a larger deck, but some have
   been changed so that "kinds" must be 2 and "cardsPerKind" must be 2.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   

#define LOOP(i,N) for(i=0; i<(N); i++)   /* be careful using this! (i not parenthesized!) */
#define MAXDECK 4                       /* the largest deck of cards that this program can use */

/* global variable */
int showStuff = 0;             /* extra output when this is set */

/* places all cards randomly in the destination array */
/* The kinds of cards are simply 1's or 2's. */
void deal(int* destination, int kinds, int cardsPerKind) {
    int decksize = kinds*cardsPerKind;
    int *ptr;
    int source[MAXDECK];
    int i, j;
    
    ptr = source;
    LOOP(i, kinds)             /* first make a perfectly ordered deck: */
        LOOP(j, cardsPerKind)
            *ptr++ = i+1;
        
    LOOP(i, decksize)
        destination[i] = 0;    /* 0 represents absence of any card in that position */ 
    
    ptr = source;
    LOOP(i, decksize) {
        do {
            j = rand()%decksize;
        } while(destination[j] != 0);
        destination[j] = source[i];
    }
}

/* The computer player looks at "showing", and its own memory.
   "which" is used to select which of a population of players will be used.
   "init" will be set to 1 at the start of each game, then it will be 0. */
int PlayerA(int* showing, int kinds, int cardsPerKind, float *which, int init)  {
    int decksize = kinds*cardsPerKind;
    int pick;
    
    /* YOUR COMPUTER PLAYER HERE                                    */
    /* It should set "pick" to 0, 1, 2 or 3 to pick which card.     */
    /* If it can't come up with something, then set it to 4.        */
    
    return pick;
}

/* prints what's showing. blank for no card, 'X' for face down. */
void show(int *showing, int decksize) {
    int i;
    
    LOOP(i, decksize) {
        if(showing[i] > 0)
            printf(" %2d  ", showing[i]);
        else if(showing[i] == -1)    
            printf("  X  ");
        else    
            printf("     ");
    }    
    printf("\n");
}                                                               

/* PlayerB plays randomly; it's used for the opponents first move.  */
int PlayerB(int* showing, int kinds, int cardsPerKind, float *which, int init)  {
    int decksize = kinds*cardsPerKind;
    int j;
        
    do {   /* turn over a face down card */ 
        j = rand()%decksize;    
    } while(showing[j] != -1);    
    
    return j; 
}

/* PlayerBprime will not make a match.  (This is for 4 card deck.)
   It's used for the opponents 2nd move. */
int PlayerBprime(int* showing, int* theDeal, int kinds, int cardsPerKind)  {
    int decksize = kinds*cardsPerKind;
    int j, target;
     
    LOOP(j, decksize)
        if(showing[j] != -1)    /* We assume there is exactly one card showing. */
            break;
    target = showing[j];
    do {            
        do {   /* turn over a face down card */ 
            j = rand()%decksize;    
        } while(showing[j] != -1);    
    } while(theDeal[j] == target);    
    
    return j; 
}

/* determines if target matches anything showing.  If so, 
   returns the position of the match found.  Otherwise, 
   returns -1 (for negative result).  */
int match(int *showing, int which, int decksize)  {
    int i, target;
    
    if(!(target = showing[which]))
        return -1;      /* this happens if an empty spot is chosen */
    
    LOOP(i, decksize)
        if(target == showing[i] && i != which)
            return i;
   
    return -1;    
}


/* This has a few specializations for decksize 4.
   A 1 is returned if A wins, a 0 is returned if B wins.
   Note that the first three calls to PlayerA() don't use it's returned value.
   These calls are just so that PlayerA can "see" the cards on the table. */
int playGame(int kinds, int cardsPerKind, float* Aptr, float* Bptr) {
    int decksize;
    int theDeal[MAXDECK];   /* array where the cards are "dealt" */
    int showing[MAXDECK];   /* array as seen by players */
    int i,j, choice;
    
    decksize = kinds * cardsPerKind;    /* has to be 4 for this code */
    
    /* Play begins here. */ 
    deal(theDeal, kinds, cardsPerKind);
    LOOP(i, decksize)      /* initialize this array to show all cards face down */
        showing[i] = -1; 
    PlayerA(showing, kinds, cardsPerKind, Aptr, 1);  /* just a look! (and init) */
    choice = PlayerB(showing, kinds, cardsPerKind, Bptr, 1);   /* B's 1st move */
    showing[choice] = theDeal[choice];
    if(showStuff)                           /* optional output */
        show(showing, decksize);    
    PlayerA(showing, kinds, cardsPerKind, Aptr, 0);  /* just a look! */
    choice = PlayerBprime(showing, theDeal, kinds, cardsPerKind);  /* B's 2nd move */
    showing[choice] = theDeal[choice];
    if(showStuff)
        show(showing, decksize);     
    PlayerA(showing, kinds, cardsPerKind, Aptr, 0);  /* just a look! */
    if(showStuff)
        printf("\n");
    /* All cards are turned face down when its the other player's turn. */
    LOOP(i, decksize)
        showing[i] = -1;    
    choice = PlayerA(showing, kinds, cardsPerKind, Aptr, 0);   /* A's 1st move */
    showing[choice] = theDeal[choice];
    if(showStuff)
        show(showing, decksize);   
    choice = PlayerA(showing, kinds, cardsPerKind, Aptr, 0);   /* A's 2nd move */
    showing[choice] = theDeal[choice];
    if(showStuff)
        show(showing, decksize);    
    j = match(showing, choice, decksize);
    if(j < 0)
        return 0;   /* this means that A lost */
    else
        return 1;   /* this means that A won */ 
}
