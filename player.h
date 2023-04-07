//***********************************************
//      PLAYER CLASS                            *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a structure for    *
//  each individual player at the game table    *
//                                              *
//***********************************************
//  Version 0.0.4.1 pushed by David Coleman III *
//  on  01 - 24 - 23                            *
//***********************************************

#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include <random>

//***********************************************

enum PlayerPerception {WEAK, SUBPAR, NO_ANALYSIS, AVERAGE, CANNOT_READ, DECENT, STRONG};                //The perception that a CPU player will have about another player.  These are held both by a player's output and other player's perceptions

//***********************************************

const int DICE_MOD = 10;

//***********************************************

class Player {
public:
    Player(Deck&/*in&out*/, float/*in*/);       //Generates a player, gives them a hand of 5 cards and a sum of cash

//******
    float AnteUp(int);                          //The player wagers the ante for the hand

//******
    virtual float BetCash(Deck&);               //The player chooses how much money to bet on a hand - Redefined if used by a CPU

//******
    virtual float CallBet(float, Deck&);       //The player can choose to call or raise an existing bet - Redefined if used by a CPU

//******
    float TakeWager(float);                     //The player's wagered money is given to the dealer and placed in the pot

//******
    bool FoldedHand();                          //Returns whether or not the player has folded their hand or not

//******
    void GetCash(float/*in*/);                  //The player gets cash in return from the pot when they win

//******
    float CheckCash();                          //Checks the amount of cash a player has

//******
    void Fold(Deck&);                           //The player can forfeit their hand until the end of the round and get a new hand

//******
    void PlayHand();                             //The player plays their sorted hand

//******
    void PlayHand(bool);                        //The player plays their sorted hand - NO PRINTING

//******
    void DiscardCards(Deck&/*in&out*/);         //Allows the player to discard a set number of cards and draw that many cards before playing hand

//******
    void DiscardHand(Deck& /*in&out*/);         //Places all of the player's cards into the discard pile

//******
    void DiscardHand(Deck&, /*in&out*/ bool);   //Discards player's entire hand - bool argument will make sure that a new hand is not drawn

//******
    void NewHand(Deck&/*in&out*/);              //Discards player's entire hand and draws a new one

//******
    void NewHand(Deck&,/*in&out*/ bool);        //Draws enough cards to give the player a new hand - bool = no discarding

//******
    void GetHand(Deck& /*in&out*/);             //Draws as many cards as necessary to get a full hand of cards

//******
    int GetWager();                             //Returns the player's current wager amount to the calling function

//******
    float HandPoints();                         //Returns the player's point values to the calling function for this hand

//******
    ~Player();                                  //Destructor - currently does nothing

protected:
    Hand* hCards;
    float fCash;
    bool bFolded;
    int currWager;
};

class CPU : public Player {
public:

//******
    CPU(Deck&/*in&out*/ myDeck, float/*in*/ myCash);                        //Generates a player, gives them a hand of 5 cards and a sum of cash

//******
    float BetCash(int, Deck&, vector<Perception>);                          //The CPU chooses how much money to bet on a hand - bets are made based on how well the CPU can
                                                                            //...bluff, how well they can call opponents tells, and how aggressive the player is
//******
    float CallBet(float, int, Deck&, vector<Perception>);                   //The CPU can choose to call or raise an existing bet - calls and raises are made based on how well the CPU can
                                                                            //...bluff, how well they can call opponents tells, and how aggressive the player is

//******
    ~CPU();

//******
private:
    int perceptionStat;                 //This stat will affect the CPU's ability to read tells of other players, realize if other players are reading their tells, and how good their hand is
    int aggressivenessStat;             //This stat will affect the CPU's willingness to bet very large sums of money, take risks, and put pressure on the rest of the table
    int bluffStat;                      //This stat will affect the CPU's ability to provide "fake" tells for opponents, and their willingness to make strong bets even with a poor hand (based on their ability to convince opponents of their bluff)
    PlayerPerception opinion;           //This variable will determine how the player feels about their own hand
    PlayerPerception otherPlayer[3];    //This variable will determine how the player feels about the rest of the players' hands

};

#endif // PLAYER_H
