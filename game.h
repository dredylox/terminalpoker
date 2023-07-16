







#ifndef GAME_H
#define GAME_H

#include <iostream>                 //For output to the terminal - cout, cin, getline(), setw, etc.
#include <limits>                   //For making sure points are allocated correctly - used for int and float lims
#include "card.h"                   //Card class - These objects are the main building block of the entire game set
#include "deck.h"                   //Deck class - The main point in which cards are drawn and distributed to each player
#include "hand.h"                   //Hand class - The main body by which the player stores their cards
#include "player.h"                 //Player class - the class that serves to represent each plyaer in the game; CPU or physical
#include "circularlList.h"          //circular linked list implementation - used to get and maintain the correct turn order for all
                                    //...players in a single game of poker

struct Score {                      //Struct that holds the player's name and their hand score
    int playerNum;                      //The player num - their particular assigned number
    double points;                      //The points - that given player's hand value
};

//**************************************************

const int FIVE_CARD_DRAW_HANDSIZE = 5;          //Hand size for 5 card draw
const int SEVEN_CARD_STUD_HANDSIZE = 7;         //Hand size for 7 card stud
const int TEXAS_HOLDEM_HANDSIZE = 2;            //Hand size for Texas Hold 'Em

//**************************************************

class Game {                    //Game class - used to actually play the game
private:

public:
    void FiveCardDraw(bool);        //Plays a game of 5 card draw

//******
    void PrintPot(float);       //Prints the pot (the money pool for the game)

//******
    //Gets an ante from each player to start the game.  If they cannot ante, they are out of the game
    void AnteUp(CircularList<Player*>&, Deck&, float&, std::vector<std::vector<float>>&, bool&);

//******
    //Creates a set of 4 players - 1 user and 3 CPU's
    void NewPlayers(int, std::vector<Player*>&, Deck&);

//******
    //Assigns players and their scores using the circular linked list
    void GameStart(std::vector<Score>&, std::vector<Player*>&, CircularList<Player*>&);

//******
    //Gets opinions from each player of how they feel about their hand
    void GetTells(CircularList<Player*>&, std::vector<PlayerPerception>);

//******
    //Prints the user player's hand
    void PrintUserHand(CircularList<Player*>&);

//******
    //The player discards cards - has diffferent routines depending on CPU or user
    void DiscardCards(CircularList<Player*>&, std::vector<Player*>&,
                  std::vector<Score>&, Deck&, std::vector<PlayerPerception>&);

//******
    //Each player looks at their cards
    //...prints hand for user
    //...the CPU gets to pass their tell to the table
    void EvaluateCards(std::vector<Score>&, std::vector<Player*>&,
                   CircularList<Player*>&, std::vector<PlayerPerception>&);

//******
    //Starting with the user player, the players lead the bet.
    //...on the next round, the next player will lead the bet
    //...if a player decides they want to fold, the next player will temporarily lead the bet
    //...this will repeat until 3 players fold or someone leads the bet
    void SetBetLead(int currBet, CircularList<Player*>& turnOrder);

//******
    //Refer to the comments above
    void BettingRound(float&, std::vector<bool>&, int, float&,
                  CircularList<Player*>&, int&, bool&,
                  std::vector<PlayerPerception>, Deck&,
                  std::vector<std::vector<float>>&);

//******
    //the player will call the bet, raise the bet, or fold.
    //...user players will input their values directly
    //...the CPU will use a decision tree algorithm using their stats to decide what to do
    void CallingRound(float&, float&, int&, int&, CircularList<Player*>&,
                  std::vector<bool>, std::vector<PlayerPerception>, Deck&,
                  bool&, std::vector<std::vector<float>>&);

//******
    //This function will increment the player that is leading the bet at the beginning of a hand
    void IncrementCurrBet(int&, CircularList<Player*>&);

//******
    //Print's every player's hand
    void PrintAllHands(CircularList<Player*>&, std::vector<Score>&);

//******
    //Checks each hand in the current round and determines a winner or ties based on card scores
    void CheckWinner(CircularList<Player*>&, std::vector<Score>, float&, std::vector<std::vector<float>>&);

//******
    //Print each player's cash
    void PrintCash(CircularList<Player*>&);

//******
    //Gives the user player the option to play again
    void PlayAgain(CircularList<Player*>&, std::vector<Player*>&, Deck&, bool&, int);

//******
    //This function goes through the multi-pot and the players and sets each player's bet
    //...into the appropriate pot - players betting more money are on higher tiers than lower betters
    void DistributePot(std::vector<std::vector<float>>&, CircularList<Player*>&);

};

#endif // GAME_H
