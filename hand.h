//***********************************************
//      HAND CLASS                              *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a vector structure *
//  of card objects.  This is the main source   *
//  card objects that players will use as       *
//  their "hands" when playing the game.        *
//                                              *
//***********************************************
//  Version 0.0.4.1 pushed by David Coleman III *
//  on  01 - 24 - 23                            *
//***********************************************

#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "deck.h"
#include <vector>
#include <math.h>

//***********************************************

const int HAND_STATE = 1;         //Holds the numerical version of the enum CARD_STATE for IN_HAND

//***********************************************

class Hand {
public:
    Hand(Deck& /*in&out*/);                                 //Constructor - Gets a hand of cards

//******
    void GetHand(Deck&/*in&out*/);                          //Function called by hand - generates a hand and returns it to the vector

//******
    void SortHand();                                        //Function to sort the hand by value - suit sorting not necessary

//******
    void DrawCard(int,/*in*/ Deck&/*in&out*/);              //Interacts with the deck and draws cards from it

//******
    void Discard(std::vector<int>,/*in*/ Deck&/*in&out*/);  //Passes a list of all cards to be discarded and discards them before drawing more

//******
    void PrintHand() const;                                 //Prints every card in the players hand based on str values - FOR TESTING PURPOSES

//******
    void PrintCard(Card) const;                             //Prints only a single card - can be used to print similar card data structures to hands that are not hands
                                                            //...like all aces in a hand when deciding which to keep (when discarding 4 cards) in example

//******
    double CountPoints();                                   //Iterates multiple times through a sorted hand to determine how strong that hand is vs other players

//******
    double CountPoints(bool);                               //Iterates multiple times through a sorted hand to determine how strong that hand is vs other players - no printing

//******
    void DiscardCards(Deck&);                               //Allows the user to discard cards in order to draw new ones
                                                            //Up to 3 cards can be discarded normally, or 4 cards if the player is holding an
                                                            //...an ace.  Draws an appropriate amount of cards once finished
//******
    void DiscardHand(Deck&);                                //Discards the entire player's hand back into the discard pile

//******
    int CardCount();                                    //Returns the amount of cards in hand to the calling function

//******
    void ShowScore(int, double) const;                 //Prints out the specific hand the player has

//******
    ~Hand();

private:
    std::vector<Card> vCards;

};

#endif //HAND_H
