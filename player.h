//***********************************************
//      PLAYER CLASS                            *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a structure for    *
//  each individual player at the game table    *
//                                              *
//***********************************************


#ifndef PLAYER_H
#define PLAYER_H

#include "circularlList.h"
#include "hand.h"
#include <random>

//***********************************************

//PlayerPerception is the type that determines HOW a player feels about their hand
//It is also used to determine how good a player THINKS another player's hand is based
    //...on their tells.

enum PlayerPerception {WEAK, SUBPAR, NO_ANALYSIS, AVERAGE, CANNOT_READ, DECENT, STRONG};                //The perception that a CPU player will have about another player.  These are held both by a player's output and other player's perceptions

//***********************************************

const int DICE_MOD = 10;                //Dice modifier.  A 1-100 roll over this will result in a D10 roll essentially

const int NAT_TWENTY = 95;              //Natural 20 - Rolling above a 94 results in exceptional dice roll
const int NAT_ZERO = 4;                 //Natural 0 - rolling less than 5 results in exceptionally awful roll

const int MASTER_ROLL = 18;             //Most difficult dice check
const int HARD_ROLL = 16;               //Difficult dice check
const int MED_ROLL = 14;                //Meidum dice check
const int EASY_ROLL = 13;               //Easy dice check
const int TRIVIAL_ROLL = 12;            //Easiest dice check

const int EASY_ACTION = 15;             //Easiest actionScore check
const int MEDIUM_ACTION = 30;           //Medium actionScore check
const int DECISIVE_ACTION = 75;         //hardest actionScore check
const int POINTS_DIV = 100;             //Used to divide raw action score for advanced checking in function

const int ANTE = 5;                 //Constant for ante - the amount of money each player has to put into the pot in order to participate
const int MIN_CALL = 10;

//***********************************************

class Player {
public:
    Player(int, Deck&/*in&out*/, float/*in*/);                      //Generates a player, gives them a hand of 5 cards and a sum of cash

//******
    float AnteUp(int, Deck&);                                       //The player wagers the ante for the hand

//******
    virtual float BetCash(Deck&, float,
                          std::vector<std::vector<float>>);         //The player chooses how much money to bet on a hand - Redefined if used by a CPU

//******
    virtual float CallBet(float, Deck&, float,
                          std::vector<std::vector<float>>);         //The player can choose to call or raise an existing bet - Redefined if used by a CPU

//******
    float TakeWager(float);                                         //The player's wagered money is given to the dealer and placed in the pot

//******
    float AllIn();                              //Used for the player to bet their entire cash pool

//******
    float SoftWager(float);                     //Used to false-wager money into the output pot without taking money from player

//******
    bool FoldedHand();                          //Returns whether or not the player has folded their hand or not

//******
    void GetCash(float/*in*/);                  //The player gets cash in return from the pot when they win

//******
    float CheckCash();                          //Checks the amount of cash a player has

//******
    void Fold(Deck&);                           //The player can forfeit their hand until the end of the floor and get a new hand

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
    void NewHand(int, Deck&/*in&out*/);              //Discards player's entire hand and draws a new one

//******
    void NewHand(int, Deck&,/*in&out*/ bool);        //Draws enough cards to give the player a new hand - bool = no discarding

//******
    void GetHand(Deck& /*in&out*/);             //Draws as many cards as necessary to get a full hand of cards

//******
    int GetWager();                             //Returns the player's current wager amount to the calling function

//******
    float HandPoints();                         //Returns the player's point values to the calling function for this hand

//******
    PlayerPerception GetPlayerTells(int);       //Returns the player's belief of how good a hand is based on their own card values and opponent tells

//******
    int GetBluffStat();                         //Returns the player's bluff stat to the calling function

//******
    int GetPerceptionStat();                    //Returns the player's perception stat to the calling function

//******
    int GetAggressivenessStat();                //Returns the player's aggressiveness stat to the calling function

//******
    void SetOpinion(PlayerPerception);          //Pushes an opinion on the back the opinion vector

//******
    void SetOpinion(PlayerPerception, int);     //Replaces an opinion into a particular location in the opinions vector

//******
    void SetCurrPot(int);                       //Sets the pot the player is currently betting in

//******
    void IncrementPot();                        //Move the player to the next pot

//******
    int GetCurrPot();                           //Get the number of the pot the player is currently betting in

//******
    void ResetCurrPot();                        //Sets the pot the player is betting in to zero

//******
    void HeadsUpDisplay(float, std::vector<std::vector<float>>) const;          //Prints all pertinent info for the user player minus callValue

//******
    void HeadsUpDisplay(float, std::vector<std::vector<float>>, float) const;   //Prints all pertinent info for user player including callValue

//******
    ~Player();                                  //Destructor - currently does nothing

protected:
    Hand* hCards;                               //Vector of Card Pointers - serves as the player's hand of cards
    float fCash;                                //The player's cash - increase when winning hands, decreases when losing hands
    bool bFolded;                               //Bool that turns on when the player folds
    int currWager;                              //The amount of money the player has in the pot currently
    int currPot;                                //The current pot the player is betting in - this is used for side betting
    int perceptionStat;                         //This stat will affect the player's ability to read tells of other players, realize if other players are reading their tells, and how good their hand is
    int aggressivenessStat;                     //This stat will affect the player's willingness to bet very large sums of money, take risks, and put pressure on the rest of the table
    int bluffStat;                              //This stat will affect the player's ability to provide "fake" tells for opponents, and their willingness to make strong bets even with a poor hand (based on their ability to convince opponents of their bluff)
    std::vector<PlayerPerception> opinions;    //This is the opinion the player has over the rest of the table
    bool holding;                               //This bool determines if the player will keep a hand no matter what.  This is for advanced behavior where the player's bluff AND aggressiveness are both high enough
                                                //...for them to decide to make a big bet on a weaker hand
};

class CPU : public Player {
public:

//******
    CPU(int, Deck&/*in&out*/ myDeck, float/*in*/ myCash);                        //Generates a player, gives them a hand of 5 cards and a sum of cash

//******
    float BetCash(int, Deck&, std::vector<PlayerPerception>);                          //The CPU chooses how much money to bet on a hand - bets are made based on how well the CPU can
                                                                            //...bluff, how well they can call opponents tells, and how aggressive the player is
//******
    float CallBet(float, int, Deck&, std::vector<PlayerPerception>);                   //The CPU can choose to call or raise an existing bet - calls and raises are made based on how well the CPU can
                                                                            //...bluff, how well they can call opponents tells, and how aggressive the player is
//******
    void GetTell(PlayerPerception);                                         //Gets a bluffed tell from the player

//******
    void GetTell();                                                         //Gets a true tell from the player

//******
    void DiscardCards(Deck&);                                               //The player discards cards into the discard pile

//******
    //The player bets cash - used for leading the bet
    float BetCash(int playerNum, Deck& dInput, std::vector<PlayerPerception> inPerception,
                       std::vector<double> inScores, std::vector<int> inBluff,
                       std::vector<PlayerPerception> inOpinion);

//******
    //The player calls, raises, or folds the existing bet
    float CallBet(float callValue, int playerNum, Deck& dInput,
                       std::vector<PlayerPerception> inPerception,
                       std::vector<double> inScores, std::vector<int> inBluff,
                       std::vector<PlayerPerception> inOpinion);

//******
    PlayerPerception JudgeHand();       //Returns the player's opinion of their own hand to the calling function - gives a visible tell to the terminal

//******
    PlayerPerception JudgeHand(bool);   //Returns the player's opinion of their own hand to the calling function - bool parameter prevents a visible tell from being printed

//******
private:

};

#endif // PLAYER_H
