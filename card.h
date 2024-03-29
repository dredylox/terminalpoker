//***********************************************
//      CARD CLASS                              *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as the building block *
//  of a card game - such as poker, spades,     *
//  etc. - consisting of a class for the cards  *
//  themselves, and the definition for critical *
//  enums related to cards in a card game       *
//                                              *
//***********************************************

#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <iomanip>
#include <string>

//***********************************************

enum CardSuit {CLUBS, DIAMONDS, HEARTS, SPADES};            //The suit of each card object
enum CardNum {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,    //The rank of each card object
              NINE, TEN, JACK, QUEEN, KING, ACE};
enum CardState {IN_DECK, IN_HAND, IN_RIVER, IN_DISCARD};

const std::string SUIT_SYM[4] = {u8"♣", u8"♦", u8"♥", u8"♠"};           //String versions of suit...
const std::string SUIT_SYM_COMP[4] = {"C", "D", "H", "S"};              //String suits - compatible version in case u8 symbols are not compatible
const std::string SUIT_STR[4] = {" CLUB", " DMND", "HEART", "SPADE"};
const std::string NUM_STR[13] = {" 2", " 3", " 4", " 5", " 6",     //Str version of rank.  Translate directly with enum values
                                 " 7", " 8", " 9", "10", " J",
                                 " Q", " K", " A"};

const int SUIT_COUNT = (sizeof(SUIT_STR) / sizeof(SUIT_STR[0]));         //Holds total number of suits for each card
const int NUM_COUNT = (sizeof(NUM_STR) / sizeof(NUM_STR[0]));           //Holds total number of possible ranks for each card

//***********************************************

class Card {
public:
    Card();                         //Default constructor - Does nothing as of yet

//******
    Card(const Card& cOther);       //Copy constructor - Creates a new object by copying the cOther

//******
    Card(int /*in*/, int/*in*/);    //Non-default constructor - Creates the card, uses the input parameter
                                    //...to generate suit and rank value.

//******
    Card(int /*in*/, int/*in*/, bool);    //Non-default constructor - Creates the card, uses the input parameter
                                    //...to generate suit and rank value.  Bool for compatibility mode

//******
    CardSuit GetSuit() const;             //Returns the card's suit to the calling function

//******
    CardNum GetNum() const;               //Returns the card's num to the calling function

//******
    std::string GetSuitStr() const;       //Returns a str version of suit to calling function

//******
    std::string GetNumStr() const;        //Returns a str version of rank to the calling function

//******
    void SetSuit(int/*in*/);        //Sets the suit to the input parameter

//******
    void SetNum(int/*in*/);         //Sets the rank to the input parameter

//******
    void SetState(CardState/*in*/); //Sets the state of the card

//******
    void SetComp(bool);

//******
    bool GetComp() const;

//******
    CardState GetState() const;           //Gets the state of the card

//******
    std::string GetSuitSym() const;       //Gets the symbol string for the card's suit

//******
    bool operator==(const Card& cOther) const;  //Overload == operator; allows card equivilency checking with ==


private:
    CardSuit suit;              //The card's suit - spade, diamond, heart, and club
    CardNum num;                //The card's rank - two through ten, jack, queen, king, and ace
    std::string suitStr;        //string representation of the card's suit
    std::string numStr;         //string representation of the card's rank
    std::string suitSym;        //string represetnation of the card's suit symbol
    CardState state;            //The state of the card - in hand, in deck, in discard, etc.
                                    //****THIS ELEMENT/MECHANIC CURRENTLY DOES NOTHING****
    bool compMode;
};



#endif // CARD_H



