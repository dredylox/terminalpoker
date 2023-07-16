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


#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "deck.h"
#include <vector>
#include <math.h>

//***********************************************

const int HAND_STATE = 1;         //Holds the numerical version of the enum CARD_STATE for IN_HAND
                                  //NOT FULLY IMPLEMENTED
                                  //Seems like a superfluous feature - possibly refactor w/ it being removed???

//This block consists of the multipliers for hands of cards
//  This block is very important for assuring that a superior hand will ALWAYS beat a weaker one
//******
const int MIN_STRENGTH = 10;                //Used specifically for hand judging

const int TWOPAIR_MULT = 30;                //Multiplier for two pairs
const int TRIPS_MULT = 720;                 //Multiplier for 3 of a kind
const int FULLHOUSE_PAIR_MULT = 1000;       //Multiplier for the pair section of a full house
const int STRAIGHT_MULT = 2400;             //Multiplier for a straight
const int FLUSH_MULT = 7200;                //Multiplier for a flush
const int FULLHOUSE_TRIPS_MULT = 300000;    //Multiplier for the 3 of a kind section of a full house
const int QUADS_MULT = 4000000;             //Multiplier for 4 of a kind
const int STRAIGHTFLUSH_MULT = 53000000;    //Multiplier for a straight flush
const float ROYAL_MULT = 9999999999.0;      //Multiplier for a royal flush - uses float as it's not being cast back into one like prev mults

//This block consists of the constant values used for actually printing the hand value
//  *****This is needlessly complex and can be simplified drastically!*****
//******
const int PAIR_OUT = 100;                   //Const value to make sure the game outputs pair when printing hand rank
const int TWOPAIR_OUT = 200;                //Const value to make sure the game outputs two pair when printing hand rank
const int TRIPS_OUT = 300;                  //Const value to make sure the game outputs three of a kind when printing hand rank
const int STRAIGHT_OUT = 400;               //Const value to make sure the game outputs straight when printing hand rank
const int FLUSH_OUT = 500;                  //Const value to make sure the game outputs flush when printing hand rank
const int FULLHOUSE_OUT = 600;              //Const value to make sure the game outputs full house when printing hand rank
const int QUADS_OUT = 800;                  //Const value to make sure the game outputs Four of a Kind when printing hand rank
const int ROYAL_OUT = 900;                  //Const value to make sure the game outputs royal flush when printing hand rank

const int ROYAL_THRESHOLD = 2914000000;     //Threshold before the game automatically makes the hand score a royal flush

//***********************************************

class Hand {
public:
    Hand(int, Deck& /*in&out*/);                                 //Constructor - Gets a hand of cards

//******
    void GetHand(int, Deck&/*in&out*/);                     //Function called by hand - generates a hand and returns it to the vector

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
    void PrintAces() const;

//******
    void PrintSelection(std::vector<Card>) const;           //Prints a selection of cards

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
    void SelectCards(std::vector<int>&);                //used by the CPU to select the cards they'd like to discard

//******
    void CPUDiscard(Deck&, std::vector<int>&);          //used by the CPU to discard their selected cards

private:
    std::vector<Card> vCards;                           //This vector of cards serves as the actual data for the hand of cards the player uses

};

#endif //HAND_H
