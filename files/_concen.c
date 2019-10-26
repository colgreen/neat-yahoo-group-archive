/* concen.C by M. Timin, October, 2003

   usage:
        concen [kinds] [cardsPerKind]
   examples:
        concen
        concen 5
        concen 5 4     
   If only one number is given it is taken to be how many kinds.
   Default values are used when required.     
*/

/* miscellaneous rules & conditions:

Players A & B play the game of concentration with some number of different kinds of cards,
and some number of each card.  A normal deck would have 4 each of 13 kinds.
Our default deck has 2 each of 3 kinds so the deck only has six cards.
Normally, there are an even number of each kind of card, so that there need be no leftover
cards.  There are no jokers.
The cards are represented by integers.  The normal deck would consist of 1111222233334444....
The default deck is 112233.
0 will represent the absence of a card, i.e., it's been removed, and
-1 represents a face down card. 
A linear arrangement of cards is used.  Since the common rectangular arrangement has no
significance within the rules of the game, any arrangment is equivalent to any other.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXDECK 52                       /* the largest deck of cards that this program can use */
#define LOOP(i,N) for(i=0; i<(N); i++)   /* be careful using this! (not parenthesized!) */

/* default values - can be overidden by command line */
#define KINDS          3               /* how many different kinds of cards */
#define CARDSPERKIND   2               /* how many cards of each kind */

/* global variables */
int Ascore = 0, Bscore = 0;    /* how many cards collected */

/* places all card randomly in the destination array */
void deal(int* destination, int kinds, int cardsPerKind);

/* Player A plays randomly, with no memory.  It turns over one card. */
int PlayerA(int* showing, int kinds, int cardsPerKind);

/* PlayerB is just as skillful as player A. :)  */
int PlayerB(int* showing, int kinds, int cardsPerKind);

/* prints what's showing. 0 for no card, -1 for face down. */
void show(int *showing, int decksize);

/* determines if target matches anything showing.  If so, returns
   the position of the match found.  Otherwise, returns -1.  */
int match(int *showing, int target, int decksize);
    
int main(int argc, char *argv[])  {

    int kinds        = KINDS;           /* how many different kinds of cards */
    int cardsPerKind = CARDSPERKIND;    /* how many cards of each kind */
    int decksize;           /* total number of cards (product of above two quantities) */
    int theDeal[MAXDECK];   /* array where the cards are "dealt" */
    int showing[MAXDECK];   /* array as seen by players */
    int AtoMove;            /* set when it's player A's turn */
    int moves = 0;          /* counts moves by one player */
    int i, j, choice;
    
    if(argc > 1)
        kinds = atoi(argv[1]);
    if(argc > 2)
        cardsPerKind = atoi(argv[2]);
        
    decksize = kinds * cardsPerKind;

    srand(time(NULL));      /* pseudo-randomize the pseudo-random generator */

    deal(theDeal, kinds, cardsPerKind);
    
    LOOP(i, decksize)      /* initialize this array to show all cards face down */
        showing[i] = -1;   
    
    printf("Concentration 1.0 with a deck of %d kinds of cards, %d of each kind.\n\n", kinds, cardsPerKind);
    LOOP(i, decksize)               /* output the results of the deal  */
        printf("%2d ", theDeal[i]);  
    printf("  is the deal.\n");  
    show(showing, decksize);    /* now show them all as seen by players, face down */
    printf("\n");  
 
    AtoMove = rand()%2;     /* flip a coin for first move */
    printf("player %s's turn\n", AtoMove ? "A" : "B");    
    
    /* Play begins here. */ 
    while(Ascore+Bscore < decksize)  {                  /* play while cards remain */
        if(Ascore > decksize/2 || Bscore > decksize/2)
            break;                  /* but quit if a player has more than half the deck */
        if(AtoMove)
            choice = PlayerA(showing, kinds, cardsPerKind);
        else    
            choice = PlayerB(showing, kinds, cardsPerKind);
        showing[choice] = theDeal[choice];
        show(showing, decksize);    /* show the cards as seen by players */
        moves += 1;
        j = match(showing, choice, decksize);
        if(j >= 0)  {                            /* if there's a match, then handle it. */
            showing[choice] = showing[j] = 0;
            theDeal[choice] = theDeal[j] = 0;
            if(AtoMove)
                Ascore += 2;
            else
                Bscore += 2;
            moves = 0;          
            show(showing, decksize);    /* show the cards as seen by players */
            continue;    
        }
        
        if(moves <= 1)                  /* Each player gets at least 2 moves. */
            continue;
        /* All cards are turned face down when its the other player's turn. */
        LOOP(i, decksize)
            if(showing[i] != 0)
                showing[i] = -1;   
        AtoMove = !AtoMove;             /* toggle the flag for whose turn it is */
        moves = 0;    
        printf("player %s's turn\n", AtoMove ? "A" : "B");    
        show(showing, decksize);    /* show the cards as seen by players */
    }
    
    if(Ascore > Bscore)
        printf("A won.\n");
    else if(Ascore < Bscore)   
        printf("B won.\n");
    else    
        printf("The game was a draw.\n");
      
    return 0;
}
