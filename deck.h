//***********************************************
//      DECK CLASS                              *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a stack structure  *
//  of card objects.  This is the main source   *
//  card objects that players will draw from    *
//  when playing the game.                      *
//                                              *
//***********************************************
//  Version 0.0.4.1 pushed by David Coleman III *
//  on  01 - 24 - 23                            *
//***********************************************



//***********************************************

#ifndef DECK_H
#define DECK_H

#include "card.h"
#include "discard.h"
#include <stack>
#include <random>

//***********************************************

const int MAX_SUITS = 4;                        //The total number of suits in a deck
const int MAX_RANKS = 13;                       //The total number of number values in a deck
const int DECK_SIZE = MAX_SUITS + MAX_RANKS;    //The total number of cards in a deck
const int DECK_STATE = 1;                       //The int version of the enumerated CARD_STATE value IN_DECK

//***********************************************

class Deck {
public:
    Deck();             //Default constructor - Generates a new deck of cards

//******
    Deck(int/*in*/);    //Alternative constructor - Generates a deck made of X decks of cards (TO BE IMPLEMENTED LATER);

//******
    void Shuffle();              //Shuffles the deck

//******
    Card DealCard(int/*in*/);    //Peeks a card from the top of the deck and distributes it to a player/river

//******
    int GetSize();               //Returns the current size of the deck to the calling function

//******
    void Reshuffle();            //Shuffles the discard pile back into the deck

//******
    void ToDiscard(Card/*in*/);        //Sends the card passed as argument to the discard pile

//******
    int DiscardSize();          //Returns current size of the discard pile to the calling function
//******
   // ~Deck();            //Destructor - currently does nothing


private:
    std::stack<Card> stDeck;
    Discard disPile;
};

#endif // DECK_H




