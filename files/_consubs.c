/* consubs.C by M. Timin, October, 2003
   functions used by concen.c
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOOP(i,N) for(i=0; i<(N); i++)   /* be careful using this! (i not parenthesized!) */
#define MAXDECK 52                       /* the largest deck of cards that this program can use */

extern int Ascore, Bscore;

/* places all card randomly in the destination array */
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

/* Player A plays randomly, with no memory.  It turns over one card. */
int PlayerA(int* showing, int kinds, int cardsPerKind)  {
    int decksize = kinds*cardsPerKind;
    int j;
        
    do {   /* turn over a face down card */ 
        j = rand()%decksize;    
    } while(showing[j] != -1);    
    
    return j;
}

/* PlayerB is just as skillful as player A. :)  */
int PlayerB(int* showing, int kinds, int cardsPerKind)  {
    int decksize = kinds*cardsPerKind;
    int j;
        
    do {   /* turn over a face down card */ 
        j = rand()%decksize;    
    } while(showing[j] != -1);    
    
    return j; 
}

/* prints what's showing. blank for no card, 'X' for face down. */
void show(int *showing, int decksize) {
    int i;
    
    LOOP(i, decksize) {
        if(showing[i] > 0)
            printf("%2d ", showing[i]);
        else if(showing[i] == -1)    
            printf(" X ");
        else    
            printf("   ");
    }    
    printf("  A %d   B %d\n", Ascore, Bscore);
}

/* determines if target matches anything showing.  If so, 
   returns the position of the match found.  Otherwise, 
   returns -1 (for negative result).  */
int match(int *showing, int which, int decksize)  {
    int i, target;
    target = showing[which];
    
    LOOP(i, decksize)
        if(target == showing[i] && i != which)
            return i;
   
    return -1;    
}
