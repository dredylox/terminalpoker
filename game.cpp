






#include "game.h"

void Game::FiveCardDraw(bool compatibiltyMode) {
    bool gameLoop = true;           //Bool that makes sure the game continues to run even after a floor has completed
    Deck myDeck(compatibiltyMode);  //Stack structure - is both the games stack of cards and the games discard pile
    myDeck.Shuffle();               //Shuffles the deck into a random order

    std::vector<Score> vPlayerScore(PLAYER_COUNT);  //A vector of playerScores - one for each player
    std::vector<Player*> playerPtr(PLAYER_COUNT);   //A vector of player pointers - used to generate circular linked list
    std::vector<PlayerPerception> playerTells(PLAYER_COUNT);
    int currBet = 0;                                //This will be used to rotate the active betting player clockwise each floor
    int betIteration = 0;                           //This will be used to move afloor the table during the rais and call phase of betting
    std::string playerChoice;                       //Used for player input
    bool gameOver = false;

    NewPlayers(FIVE_CARD_DRAW_HANDSIZE, playerPtr, myDeck);

    CircularList<Player*> turnOrder (playerPtr[0]);                                 //Uses the first player to generate the turn order
    GameStart(vPlayerScore, playerPtr, turnOrder);

    while (gameLoop) {
                                                               //The game will continue to run until the loop is broken
        std::vector<float> currPot(PLAYER_COUNT, 0);            //hold's each player's currPot
        std::vector<std::vector<float>> potVect(1, currPot);    //2D vector that serves as the tiered pot for each player
        float myPot = 0;                                        //single pot - used for output

        for (auto& iter : playerPtr) {                          //Resets each player's current pot at the beginning of the game
            iter->ResetCurrPot();
        }

        AnteUp(turnOrder, myDeck, myPot, potVect, gameOver);    //Takes ante from each player and checks for a win or loss

        if (gameOver) {                                         //If a win or loss, returned to main
            return;
        }

        PrintPot(myPot);                                        //Prints myPot

        GetTells(turnOrder, playerTells);                       //Gets tells from each player

        PrintUserHand(turnOrder);                               //Print's the user's hand

        DiscardCards(turnOrder, playerPtr, vPlayerScore,        //Each player get's a chance to discard their cards for the turn
                    myDeck, playerTells);

        SetBetLead(currBet, turnOrder);                         //The players get a chance to lead the bet.  Starts with Player 1 and goes around the table

        float callValue = 0;                                    //Used to determine the call value
        std::vector<bool> allCalled(PLAYER_COUNT, false);       //used to determine if everyone has called or folded
        bool callingBets = true;                                //turns off when everyone is done calling
        int tempBet = currBet;                                  //used to increment the player betting on the next round
                                                                //*****THIS MIGHT BE REDUNDANT***** need to analyze if this is necessary

        BettingRound(callValue, allCalled, tempBet,                 //Starting with the current "bet lead," that player bets a value or folds.
                    myPot, turnOrder, betIteration,                 //...if they fold, the next player leads the bet or folds.
                    callingBets, playerTells, myDeck, potVect);     //...if this happens three times, the remaining player wins the round by default

        CallingRound(myPot, callValue, tempBet, betIteration, turnOrder,    //If a bet is set, each player gets a chance to call, raise the bet, or fold
                     allCalled, playerTells, myDeck, callingBets, potVect); //...when everyone has called or folded, this function returns.
                                                                            //...if someone raises, it must iterate through each player again until eveyrone calls or folds

        IncrementCurrBet(currBet, turnOrder);                   //Increments the player that will lead the bet next round

        PrintAllHands(turnOrder, vPlayerScore);                 //Prints all player's hands

        CheckWinner(turnOrder, vPlayerScore, myPot, potVect);   //Distributes the pots into appropriately tiered pots for sidebetting, and declares winners

        PrintCash(turnOrder);                                   //Prints the cash for all players

        PlayAgain(turnOrder, playerPtr, myDeck,                 //Gives the player the option to play again or quit.
                  gameLoop, FIVE_CARD_DRAW_HANDSIZE);
    }
}

//******
void Game::PrintPot(float myPot) {      //Prints the whole pot value

    std::cout << "The pot = " << std::fixed << std::setprecision(2) << "$" << myPot << "\n\n";
}

//******
    //Gets the ante from each player and declares game over or victory if necessary
void Game::AnteUp(CircularList<Player*>& turnOrder, Deck& myDeck, float& myPot, std::vector<std::vector<float>>& potVect, bool& gameOver) {

    std::cout << "\nThere is a $" << ANTE << " ante for this hand.\n\n";

    turnOrder.TraverseStart();                                                  //Begins the turn order at the beginning
    for (int iter = 0; iter < PLAYER_COUNT; iter++) {                           //Loops until it receives an ante from every player
        float temp = float(turnOrder.GetPlayer()->AnteUp(ANTE, myDeck));                    //Gets the ante from each player
        if (temp < 1) {                 //If the player cannot ante, they fold

            std::cout << "\n\nPlayer " << iter + 1 << " does not have enough money to ante.  They fold.\n\n";
            turnOrder.GetPlayer()->Fold(myDeck);
        }
        myPot += temp;                                                          //myPot is incremented by each player's ante
        int maxPot = potVect.size() - 1;                                        //the maxPot is the last tier in the multi-pot
        potVect[maxPot].at(iter) = temp;                                        //the player's element of the multipot tier is incrememnted by temp
        turnOrder.TraverseNext();                                               //Moves to the next player
    }
    turnOrder.TraverseStart();                                                  //Starts at first player
    std::vector<bool> gameOverCheck;                                            //vector of bools - [0] = game over loss; [1 - 3] = game over win
    bool playerLoss = false;                                                    //turns on if player loses
    bool playerWin = true;                                                      //turns off if triggered event that stops player from winning
    for (int iter = 0; iter < PLAYER_COUNT; iter++) {
        if (turnOrder.GetPlayer()->FoldedHand()) {                                  //if a player folded, their element is turned on in the vector
            gameOverCheck.push_back(true);
        } else {
            gameOverCheck.push_back(false);                                         //if not, their element is turned off
        }
        turnOrder.TraverseNext();                                                   //on each iteration, increment the list
    }
    for (int iter = 0; iter < gameOverCheck.size(); iter++) {
        if (iter == 0 && gameOverCheck.at(iter)) {                              //if the player's element ([0]) is true, player loss turns on
            playerLoss = true;
        } else if (iter > 0 && !gameOverCheck.at(iter)) {                       //if all other player elements ([1],[2],[3]) are true, player win stays on
            playerWin = false;
        }
    }
    if (playerLoss) {           //if player wins or loses, they are alerted of this and the bool is passed back as true to the caller, ending the game
        std::cout << "\n\nYou don't have enough money to stay in the game!\n\nG A M E   O V E R . . . \n\n\n";
        gameOver = true;
    } else if (playerWin) {
        std::cout << "\n\nNo other players have enough money to keep playing!\n\n Y O U ' R E   A   C A R D   S H A R K ! ! ! \n\n\n";
        gameOver = true;
    }
}

//******        //creates a vector of players that will be turned into a circular list of players
void Game::NewPlayers(int handSize, std::vector<Player*>& playerPtr, Deck& myDeck) {
    for (int iter = 0; iter < PLAYER_COUNT; iter++) {           //This loop will generate the players at the table - one physical, and 3 CPU
        if (iter == 0) {
            playerPtr[iter] = new Player(handSize, myDeck, 500.00);
        }
        else {
            playerPtr[iter] = new CPU(handSize, myDeck, 500.00);
        }
    }
}

//******
void Game::GameStart(std::vector<Score>& vPlayerScore, std::vector<Player*>& playerPtr, CircularList<Player*>& turnOrder) {
    for (int iter = 0; iter < PLAYER_COUNT; iter++) {                           //The player's score is generated based on the value
        vPlayerScore[iter].playerNum = iter + 1;                                //...of the cards in their hand
        vPlayerScore[iter].points = playerPtr[iter]->HandPoints();
    }

    for (int iter = 1; iter < PLAYER_COUNT; iter++) {                           //Fills the turn order object with all players
        turnOrder.NewNode(playerPtr[iter]);
    }

}

//******
void Game::GetTells(CircularList<Player*>& turnOrder, std::vector<PlayerPerception> playerTells) {
    turnOrder.TraverseStart();

    //TELLS TO THE TABLE
        for (int iter = 0; iter < PLAYER_COUNT; iter++) {                           //player is alerted they receive their cards
            std::cout << "\nPlayer " << iter + 1 << " is dealt their cards...";         //user player returns an "average" opinion to the table every time
            if (iter == 0) {
                playerTells.push_back(AVERAGE);
                std::cout << "\nYou receive your cards.\n\n";
            } else {
                CPU* tempPtr = dynamic_cast<CPU*>(turnOrder.GetPlayer());           //the CPU players' judgement functions are ran to decide their opinions
                playerTells.push_back(tempPtr->JudgeHand());                        //this is passed to the table
                tempPtr = nullptr;
            }
            turnOrder.TraverseNext();                                           //list is traversed
        }

    //TELLS TO THE PLAYERS
        turnOrder.TraverseStart();                              //List is started from the beginning
        for (int iter = 0; iter < PLAYER_COUNT; iter++) {
            for (int next = 1; next < PLAYER_COUNT; next++) {       //this staggered double for loop assures that each player gets the "correct" position
                int temp = iter + next;                             //...for their "opinion" about the player's tells.
                if (temp >= PLAYER_COUNT) {
                    temp -= PLAYER_COUNT;
                }
                turnOrder.GetPlayer()->SetOpinion(playerTells.at(temp));    //player receives the tells from the table and stores them in their own element
            }
            turnOrder.TraverseNext();                               //List is traversed
        }
}

//******
void Game::PrintUserHand(CircularList<Player*>& turnOrder) {            //Print's the user's hand
    turnOrder.TraverseStart();
    std::cout << "~~~~~~\nPlayer " << 1 << ":\n\n";
    turnOrder.GetPlayer()->PlayHand(true);
}

//******
//Each player gets a chance to discard cards
void Game::DiscardCards(CircularList<Player*>& turnOrder, std::vector<Player*>& playerPtr,
                  std::vector<Score>& vPlayerScore, Deck& myDeck, std::vector<PlayerPerception>& playerTells) {
    turnOrder.TraverseStart();
    for (int iter = 1; iter < PLAYER_COUNT + 1; iter++) {
        std::cout << "\n\nIt is player " << iter << "'s turn to discard cards.\n";
        if (iter == 1) {
            turnOrder.GetPlayer()->DiscardCards(myDeck);                //If it's the user player's turn, they choose which card to discard with input
        } else {
            CPU* tempPtr = dynamic_cast<CPU*>(turnOrder.GetPlayer());   //if it's the CPU's turn, they run the decision tree fucntion
            tempPtr->DiscardCards(myDeck);                              //player discards the cards
            tempPtr = nullptr;                                          //cast pointer is set to null
        }
        turnOrder.TraverseNext();                                   //traverses the list
        std::cout << "\n~~~~~~\n";
    }
    EvaluateCards(vPlayerScore, playerPtr, turnOrder, playerTells); //Player's evaluate their hands and pass their tells back to the table
}

//******
void Game::EvaluateCards(std::vector<Score>& vPlayerScore, std::vector<Player*>& playerPtr,
                   CircularList<Player*>& turnOrder, std::vector<PlayerPerception>& playerTells) {

    turnOrder.TraverseStart();                                          //Starts traversal
        for (int iter = 0; iter < PLAYER_COUNT; iter++) {               //iterates through each player
            if (iter == 0) {
                playerTells.at(iter) = AVERAGE;                         //user passes average to the function
                std::cout << "\nYou look at your cards.\n\n";
                PrintUserHand(turnOrder);                               //prints user hand
            } else {
                std::cout << "\nPlayer " << iter + 1 << " looks at their cards...";
                CPU* tempPtr = dynamic_cast<CPU*>(turnOrder.GetPlayer());
                playerTells.at(iter) = tempPtr->JudgeHand();                            //CPU uses decision tree function to pass tell
                tempPtr = nullptr;                                      //pointer set to null
            }
            turnOrder.TraverseNext();                               //traverses to the next player
        }


        for (int iter = 0; iter < PLAYER_COUNT; iter++) {
            vPlayerScore[iter].points = playerPtr[iter]->HandPoints();        //Evaluates how much each player's hand is worth
        }
}

//******
void Game::SetBetLead(int currBet, CircularList<Player*>& turnOrder) {      //Sets the currBet to the player after current currBet
    turnOrder.TraverseStart();
    for (int iter = currBet; iter > 0; iter--) {
        turnOrder.TraverseNext();
    }
}

//******
//BettingRound lets the players lead the bet in a round of poker
void Game::BettingRound(float& callValue, std::vector<bool>& allCalled, int tempBet, float& myPot,
                  CircularList<Player*>& turnOrder, int& betIteration, bool& callingBets,
                  std::vector<PlayerPerception> playerTells, Deck& myDeck, std::vector<std::vector<float>>& potVect) {

    bool firstBet = true;       //This bool will remain true until someone bets or 3 players pold ****might be redundant****
    int currPot = 0;            //The tier of pot being bet on
                                //****MIGHT BE UNECESSARY****
    while (firstBet) {          //while firstBet is turned on...
            int checkFold = 0;                                      //int keeps track of how many players have folded
            turnOrder.TraverseNext();                               //moves to the next player
            for (int iter = 0; iter < PLAYER_COUNT - 1; iter++) {   //goes through each player to see if 3 players have folded
                if (turnOrder.GetPlayer()->FoldedHand()) {          //if a player folds, checkFold is incremented by 1
                    checkFold++;
                }
                if (checkFold == PLAYER_COUNT - 1) {                //if three players have folded, checkBets is turned off (param) and so is firstBet (local)
                    callingBets = false;
                    firstBet = false;
                }
                turnOrder.TraverseNext();                           //iterates through list - this will cause the list to end up on the correct player when it's time
            }
            std::cout << "\nPlayer " << tempBet + 1 << " is leading the bet this hand\n";   //tempBet is the player that is betting
            PrintPot(myPot);                                                                //prints the pot total
            if (firstBet) {
                if (tempBet > 0) {                                                          //if tempBet is greater than 0, they're a CPU player
                    CPU* tempPtr = dynamic_cast<CPU*>(turnOrder.GetPlayer());               //casts the list element to a CPU - a child of player class

                    std::vector<PlayerPerception> tempOpinion;              //A vector of opponents opinions of the player's hand/tells
                    std::vector<int> inBluff;                               //A vector of opponents bluff stat - position matched with tempOpinion
                    std::vector<PlayerPerception> tempPerception;           //A vector of opponents opinions of themselves - position matched with the above
                    std::vector<double> inScores;                           //A vector of opponents hand values; used for player decision making if the player
                                                                            //...rolls exceptionally well.  Position matched with the above temp variables

                    for (int iter = 0; iter < PLAYER_COUNT; iter++) {   //iterates through every player - used to get opinions from each player
                        if (iter > 0) {
                            int next = iter + tempBet;
                                if (next >= PLAYER_COUNT) {
                                    next -= PLAYER_COUNT;
                                }
                            tempPerception.push_back(playerTells.at(next));                                             //pushes back the raw player opinion
                            tempOpinion.push_back(turnOrder.GetPlayer()->GetPlayerTells(PLAYER_COUNT - iter));
                                                                                                                        //Gets opponent opinion of a specific player's tell

                            inBluff.push_back(turnOrder.GetPlayer()->GetBluffStat());
                                                                                                                        //Gets opponent's bluff stat

                            inScores.push_back(turnOrder.GetPlayer()->HandPoints());
                                                                                                                        //Gets the actual value of a particular opponents hand

                        }
                        turnOrder.TraverseNext();                                                                       //traverse to the next player
                    }
                    callValue = tempPtr->BetCash(tempBet + 1, myDeck, tempPerception,               //CPU betting function
                                                                inScores, inBluff, tempOpinion);
                    allCalled.at(tempBet) = true;                                                   //allCalled is set to true at the player's location
                    tempPtr = nullptr;                                                              //pointer free'd
                } else {
                    callValue = turnOrder.GetPlayer()->BetCash(myDeck, myPot, potVect);             //user player bets if tempBet is 0
                    allCalled.at(tempBet) = true;                                                   //allCalled is set to true.  used for ending the calling round
                }

            }
            if (turnOrder.GetPlayer()->FoldedHand()) {                  //if the player folds, the next player gets to try betting
                tempBet++;                                              //increments tempBet and resets it to 0 if it goes above all player positions
                if (tempBet >= PLAYER_COUNT) {
                    tempBet = 0;
                }
            }
            else {
                firstBet = false;                                       //if the player doesn't fold, first bet is turned off and the function finishes
            }
            currPot = turnOrder.GetPlayer()->GetCurrPot();              //sets currPot to the betting player's currPot
            turnOrder.TraverseNext();                                   //traverses to the next plyer
            std::cout << "\n~~~~~~\n";
        }

        myPot += callValue;                                 //increments myPot by the callvalue
        potVect[currPot].at(tempBet) += callValue;          //increments the player's pot in the tier by the callvalue
        betIteration = tempBet;                             //betIteration becomes tempBet - this is used outside of the function
}

//******
//function for players to call the bet, raise the bet, or fold
void Game::CallingRound(float& myPot, float& callValue, int& tempBet, int& betIteration,
                    CircularList<Player*>& turnOrder, std::vector<bool> allCalled,
                    std::vector<PlayerPerception> playerTells, Deck& myDeck,
                    bool& callingBets, std::vector<std::vector<float>>& potVect) {

    while (callingBets) {                               //main loop
        PrintPot(myPot);                                    //prints the overall pot
        callValue = std::floor(callValue);                  //rounds the call value down
        tempBet++;                                          //incrememnts temp bet so the next player can call/raise/fold
        if (tempBet >= PLAYER_COUNT) {
            tempBet -= PLAYER_COUNT;
        }
        float temp = 0;                                     //temp serves as temporary call value
        betIteration++;
        if (betIteration >= PLAYER_COUNT) {
            betIteration -= PLAYER_COUNT;
        }
        if (!(turnOrder.GetPlayer()->FoldedHand())) {       //if they haven't folded, they can call, raise, or fold

            std::cout << "\nPlayer " << betIteration + 1 << " can choose to call, raise, or fold.";

            if (betIteration == 0) {                                                        //if betIteration is 0, then it's the user player's turn
                temp = turnOrder.GetPlayer()->CallBet(callValue, myDeck, myPot, potVect);   //user can call/raise/fold
                if (turnOrder.GetPlayer()->FoldedHand()) {                                  //if they have folded, then their bet iteration is turned on
                    allCalled.at(betIteration) = true;
                } else if (temp > callValue) {                                              //if they raised, the call value is changed
                    callValue = temp;
                    for (auto i : allCalled) {                                                  //all other bet iterations are turned off
                        i = false;
                    }
                    allCalled.at(betIteration) = true;                                      //the player's bet iteration is turned on
                    int currPot = turnOrder.GetPlayer()->GetCurrPot();                      //the current pot being used is the caller's current pot
                    myPot += turnOrder.GetPlayer()->SoftWager(callValue);                   //myPot is incrememented with a soft wager

                    potVect[currPot].at(betIteration) += turnOrder.GetPlayer()->TakeWager(callValue);   //increments the multi-pot at the correct location

                } else if (temp == callValue) {                 //if the player calls, their iteration is turned on
                    allCalled.at(betIteration) = true;
                    int currPot = turnOrder.GetPlayer()->GetCurrPot();                                      //currPot is changed
                    myPot += turnOrder.GetPlayer()->SoftWager(callValue);                                   //myPot is incremented
                    potVect[currPot].at(betIteration) += turnOrder.GetPlayer()->TakeWager(callValue);       //multipot is incremented

                } else if (temp < callValue && turnOrder.GetPlayer()->CheckCash() == temp) {                //if the player is all-in, it's the same as a call
                    allCalled.at(betIteration) = true;
                    int currPot = turnOrder.GetPlayer()->GetCurrPot();
                    myPot += turnOrder.GetPlayer()->SoftWager(callValue);
                    potVect[currPot].at(betIteration) += turnOrder.GetPlayer()->AllIn();                    //uses all-in function instead of take-wager

                } else if (temp == 0 && turnOrder.GetPlayer()->CheckCash() == 0) {                          //if the player is already all-in, their bet iteration is
                    allCalled.at(betIteration) = true;                                                      //...the only thing manipulated - it's turned on
                }
                turnOrder.TraverseNext();                                                               //list is iterated
            } else {
                CPU* tempPtr = dynamic_cast<CPU*>(turnOrder.GetPlayer());
                std::vector<PlayerPerception> tempOpinion;              //A vector of opponents opinions of the player's hand/tells
                std::vector<int> inBluff;                               //A vector of opponents bluff stat - position matched with tempOpinion
                std::vector<PlayerPerception> tempPerception;           //A vector of opponents opinions of themselves - position matched with the above
                std::vector<double> inScores;                           //A vector of opponents hand values; used for player decision making if the player
                                                                        //...rolls exceptionally well.  Position matched with the above temp variables
                for (int iter = 0; iter < PLAYER_COUNT; iter++) {

                    int next = iter + tempBet;
                    if (next >= PLAYER_COUNT) {
                        next -= PLAYER_COUNT;
                    }
                    tempPerception.push_back(playerTells.at(next));
                    tempOpinion.push_back(turnOrder.GetPlayer()->GetPlayerTells(PLAYER_COUNT - iter));
                                                                                                                    //Gets opponent opinion of a specific player's tell

                    inBluff.push_back(turnOrder.GetPlayer()->GetBluffStat());
                                                                                                                        //Gets opponent's bluff stat

                    inScores.push_back(turnOrder.GetPlayer()->HandPoints());
                                                                                                      //Gets the actual value of a particular opponents hand
                    turnOrder.TraverseNext();
                }
                    temp = tempPtr->CallBet(callValue, betIteration + 1, myDeck, tempPerception,        //function allowing the CPU to make a call decision is called
                                                        inScores, inBluff, tempOpinion);

                if (turnOrder.GetPlayer()->FoldedHand()) {                                  //the same logic as with the user player is performed for CPU below
                    allCalled.at(betIteration) = true;                                      //fold block
                } else if (temp > callValue && turnOrder.GetPlayer()->CheckCash() > 0) {
                    callValue = temp;
                    for (auto i : allCalled) {
                        i = false;
                    }
                    allCalled.at(betIteration) = true;
                    int currPot = turnOrder.GetPlayer()->GetCurrPot();
                    myPot += turnOrder.GetPlayer()->SoftWager(callValue);
                    potVect[currPot].at(betIteration) += turnOrder.GetPlayer()->TakeWager(callValue);

                } else if (temp == callValue) {                                             //call block
                    allCalled.at(betIteration) = true;
                    int currPot = turnOrder.GetPlayer()->GetCurrPot();
                    myPot += turnOrder.GetPlayer()->SoftWager(callValue);
                    potVect[currPot].at(betIteration) += turnOrder.GetPlayer()->TakeWager(callValue);

                }else if (temp < callValue && turnOrder.GetPlayer()->CheckCash() == temp) { //all-in block
                    allCalled.at(betIteration) = true;
                    int currPot = turnOrder.GetPlayer()->GetCurrPot();
                    myPot += turnOrder.GetPlayer()->SoftWager(callValue);
                    potVect[currPot].at(betIteration) += turnOrder.GetPlayer()->AllIn();

                } else if (temp == 0 && turnOrder.GetPlayer()->CheckCash() == 0) {          //already all-in block
                    allCalled.at(betIteration) = true;
                }
                tempPtr = nullptr;
                turnOrder.TraverseNext();
            }

        } else {
            std::cout << "\nPlayer " << betIteration + 1 << " has folded.\n\n";     //if the player has folded, their iteration is turned on,
            allCalled.at(betIteration) = true;                                      //...and the list is traversed
            turnOrder.TraverseNext();
        }
        callingBets = false;            //callingBets is turned off
        for (auto i : allCalled) {      //if any player's position in allCalled is NOT turned on, callingBets turns back on and loop repeats
            if (i == false) {
                callingBets = true;
            }
        }
        std::cout << "\n~~~~~~\n";
    }

}

//******
void Game::IncrementCurrBet(int& currBet, CircularList<Player*>& turnOrder) {   //increments curr bet to the next player
    currBet++;                                                                  //****POSSIBLY REDUNDANT**** investigate in calling functions
    if (currBet >= (PLAYER_COUNT)) {
        currBet -= PLAYER_COUNT;
    }

    turnOrder.TraverseStart();
}

//******
void Game::PrintAllHands(CircularList<Player*>& turnOrder, std::vector<Score>& vPlayerScore) {  //Print all player hands
    for (int iter = 0; iter < PLAYER_COUNT; iter++) {
        std::cout << "~~~~~~\nPlayer " << iter + 1 << ":\n\n";                      //If player has not folded, prints
        if (turnOrder.GetPlayer()->FoldedHand()) {
            std::cout << "\nPlayer " << iter + 1 << " has folded\n\n";              //If they folded, alerts that they folded
            vPlayerScore[iter].points = 0;                                              //if they folded, no points
        }
        else {
            turnOrder.GetPlayer()->PlayHand(true);                                  //Hand print
        }
        turnOrder.TraverseNext();                                               //Traverses to the next player
    }
}

//******
void Game::CheckWinner(CircularList<Player*>& turnOrder, std::vector<Score> vPlayerScore,
                        float& myPot, std::vector<std::vector<float>>& potVect) {

    double tempMax = 0;                                                     //the highest score
    int tempPos = 0;                                                        //the winning player
    std::vector<int> tiedPos;                                               //if there is a tie, then the players that tied will be pushed into this vector
    bool tieGame = false;                                                   //this bool turns on if the game is left at a tie

    for (int iter = 0; iter < potVect.size(); iter++) {                     //This for loop loops for each "tier" in the pot - a player that cannot
                                                                            //...a player that hasn't enough money to match the call can stil have the
                                                                            //...the best hand and win lower-tiered pots
        turnOrder.TraverseStart();                                              //Begin traversal
        tiedPos.clear();                                                        //clears the tie check vector
        tempMax = 0;                                                            //sets tempMax to 0     ****both of these can be declared locally****
        tempPos = 0;                                                            //sets tempPos to 0   <-****
        for (int next = 0; next < PLAYER_COUNT; next++) {
            if (vPlayerScore[next].points == tempMax && turnOrder.GetPlayer()->GetCurrPot() >= iter) {          //if there is a clear tie, the tying player is pushed
                tempMax = vPlayerScore[next].points;                                                            //...back onto the vector
                tempPos = vPlayerScore[next].playerNum;
                tiedPos.push_back(tempPos);
            } else if (vPlayerScore[next].points > tempMax && turnOrder.GetPlayer()->GetCurrPot() >= iter) {    //if there is a clear winner, the tie vector is emptied
                tiedPos.clear();                                                                                //...the sole winner is pushed back onto the vector however
                tempMax = vPlayerScore[next].points;
                tempPos = vPlayerScore[next].playerNum;
                tiedPos.push_back(tempPos);
            }
            turnOrder.TraverseNext();                                           //Move to the next player
        }
        int potValue = 0;                                                   //total value of a particular tier of pot
        for (auto getPot : potVect[iter]) {                                 //adds each player's pot in the tier into potValue
            potValue += getPot;
        }

        if (tiedPos.size() == 1) {                  //If there are no ties, the winner receives their money
            std::cout << "The winner of Pot #" << iter + 1 << ", with a total of $" << potValue << "... is Player " << tiedPos.at(0) << "!\n";
            turnOrder.TraverseStart();                                  //Start
            for (int next = 1; next < tiedPos.at(0); next++) {          //moves to the winning player
                turnOrder.TraverseNext();
            }
            for (int next = 0; next < PLAYER_COUNT; next++) {           //gets that winning player money from each pot in the tier
                turnOrder.GetPlayer()->GetCash(potVect[iter].at(next));
            }
        } else if (tiedPos.size() > 1) {            //If there is a tie, all tied player's receive their portion of the total winnings
            std::cout << "The following players have tied on pot " << iter + 1 << " and split the pot of $" << potValue << ":\n\n";
            for (auto i : tiedPos) {
                std::cout << "Player " << i << "\n";                //Each player's number is output
            }
            std::cout << "\n";
            float tempCash = 0;
            for (auto i : potVect[iter]) {                          //tempCash is the same as pot value
                tempCash += i;                                          //****this might be redundant****
            }
            for (auto i : tiedPos) {                        //iterates through each tied player
                turnOrder.TraverseStart();
                for (int next = 1; next < i; next++) {          //if a player in the list is a tied player, it will traverse to them in the list
                    turnOrder.TraverseNext();
                }                                                   //the tied player receives the money divided by the number of tied players
                turnOrder.GetPlayer()->GetCash(tempCash / tiedPos.size());
            }
        }
    }
    std::cout << "\n\n";
}

//******
void Game::PrintCash(CircularList<Player*>& turnOrder) {        //Print's each player's cash
    turnOrder.TraverseStart();
    for (int iter = 0; iter < PLAYER_COUNT; iter++) {
        std::cout << "Player " << iter + 1 << " has $" << turnOrder.GetPlayer()->CheckCash() << "\n";
        turnOrder.TraverseNext();
    }
    std::cout << "\n";
}

//******
//Basic input/output function - if the player types "yes," the game resets for a new game.  If they type "no," then the game ends
void Game::PlayAgain(CircularList<Player*>& turnOrder, std::vector<Player*>& playerPtr, Deck& myDeck, bool& gameLoop, int handSize) {
    turnOrder.TraverseStart();
    std::string playerChoice = "";
    bool choiceLoop = true;
    while (choiceLoop) {
        std::cout << "\n\nWould you like to play again?\n"
                  << "Type <yes> to play again, or <no> to quit\n\n";
        std::cin >> playerChoice;
        char choiceAuto = 'p';
        if (playerChoice.size() > 0) {
            choiceAuto = tolower(playerChoice.at(0));
        }
        switch(choiceAuto) {
            case 'y' : {
                for (auto i : playerPtr) {
                    if (!(i->FoldedHand())) {
                        i->DiscardHand(myDeck);
                    }
                }
                myDeck.Reshuffle();
                for (auto i : playerPtr) {
                    i->NewHand(handSize, myDeck, false);
                }
                choiceLoop = false;
                break;
            }
            case 'n' : {
                choiceLoop = false;
                gameLoop = false;
                for (auto i : playerPtr) {
                    if (!(i->FoldedHand())) {
                        i->DiscardHand(myDeck);
                    }
                    delete i;
                }
                myDeck.Reshuffle();
                playerPtr.clear();
                break;
            }
            default : {
                std::cout << "\nInvalid entry, try again\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                break;
            }
        }
    }
    playerChoice = "";
}

void Game::DistributePot(std::vector<std::vector<float>>& myPot, CircularList<Player*>& turnOrder) {
    turnOrder.TraverseStart();
    float largestValue = 0;
    float smallestValue = 0;
    for (int iter = 0; iter < myPot.size(); iter++) {
        for (int next = 0; next < PLAYER_COUNT; next++) {
            float temp = myPot[iter].at(next);
            if (temp > largestValue) {
                largestValue = temp;
            }
            turnOrder.TraverseNext();
        }
        smallestValue = largestValue;
        for (int next = 0; next < PLAYER_COUNT; next++) {
            float temp = myPot[iter].at(next);
            if (temp < smallestValue && temp > 0) {
                smallestValue = temp;
            }
            turnOrder.TraverseNext();
        }
        turnOrder.TraverseStart();
        for (int next = 0; next < PLAYER_COUNT; next++) {
            if (myPot[iter].at(next) > smallestValue) {
                int temp = myPot[iter].at(next) - smallestValue;
                if (myPot.size() == iter + 1) {
                    std::vector<float> tempVect(PLAYER_COUNT, 0);
                    tempVect.at(next) = temp;
                    myPot.push_back(tempVect);
                } else {
                    myPot[iter + 1].at(next) = temp;
                }
                turnOrder.GetPlayer()->IncrementPot();
            }
            turnOrder.TraverseNext();
        }
        if (myPot.size() > iter + 1) {
            for (int next = 0; next < PLAYER_COUNT; next++) {
                myPot[iter].at(next) = smallestValue;
            }
        }
    }
    turnOrder.TraverseStart();
}

