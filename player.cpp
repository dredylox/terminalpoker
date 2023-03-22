//***********************************************
//      PLAYER CLASS - IMPLEMENTATION           *
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

    #include "player.h"

//******
    Player::Player(Deck& dDeck,/*in&out*/ float fStartCash/*in*/) {
        fCash = fStartCash;                                                     //Starts each player off with a set amount of money
        hCards = new Hand(dDeck);                                               //Generates a hand of 5 cards for each player
        bFolded = false;                                                       //Sets the "folded" state to false - used to check if the player has folded this turn
        currWager = 0;
    }

//******
    float Player::AnteUp(int anteSize) {
        fCash -= anteSize;
        return anteSize;
    }

//******
    float Player::BetCash(Deck& dInput) {                                           //The player chooses how much money to bet on a hand
        if (fCash > 0) {
            float fTempCash = fCash + 1;                                            //Sets temp cash just out of range to prime loop
            while (fTempCash > fCash || fTempCash < 0) {                            //Loop repeats until fTempCash is a valid amount (greater than or equal to 0 and less than total cash)
                std::cout << "\nEnter how much money you bet. Type 0 to fold.\n";
                std::cin >> fTempCash;
                if (fTempCash == 0) {                                                //If the player enters 0, they fold their hand
                    Fold(dInput);
                    break;
                }
                else if (fTempCash > fCash || fTempCash < 0) {
                    std::cout << "\nInvalid input.  Only enter a value within your cash amount, or fold.\n";
                }
            }
            fCash -= fTempCash;                                                     //Subtracts the total amount to be bet from the total cash the player has
            currWager = fTempCash;                                                  //Sets the current amount of money the player is betting to fTempCash
            return fTempCash;                                                       //Returns this value to the calling function    -   POT SHOULD BE CALLING OBJECT
        }
        else {
            std::cout << "\nOut of the money!\n\n";
            Fold(dInput);
            return -1;
        }
    }

//******
    float Player::CallBet(float callValue, Deck& dInput) {             //The CPU can choose to call or raise an existing bet
        int temp = 0;
        if (fCash < callValue - currWager) {
            std::cout << "\nPlayer 1 is out of the money\n\n";
            Fold(dInput);
            return 0;
        }
        else if (FoldedHand()) {
            std::cout << "\nPlayer 1 has folded\n";
            return 0;
        }
        else {
            int playerInput;
            bool betting = true;
            while (betting) {
                std::cout << "\nDo you raise, call, or fold? \n"
                          << "To call, type 0. To raise, type the amount you bet over the call\n"
                          << "To fold, type a negative value.\n\n"
                          << "Current call is:    " << callValue << "\n\n";
                std::cin >> playerInput;
                if (playerInput == 0) {
                    std::cout << "\nPlayer 1 calls the bet.\n\n";
                    return callValue;
                }
                else if (playerInput < 0) {
                    std::cout << "\nPlayer 1 folds.\n\n";
                    Fold(dInput);
                    return 0;
                }
                else {
                        if (fCash >= (callValue + playerInput)) {
                            temp = callValue + playerInput;
                            betting = false;
                        }
                        else {
                            std::cout << "\nPlayer 1 does not have enough money for that raise.  Please try again.\n";
                        }
                    }
                }
            }
        return temp;
    }

//******
    void Player::GetCash(float fCashIn/*in*/) {                     //The player gets cash in return from the pot when they win
        fCash += fCashIn;
    }

//******
    float Player::TakeWager(float fCashIn/*in*/) {
        int temp = fCashIn - currWager;
        currWager += temp;
        fCash -= temp;
        return temp;
    }

//******
    float Player::CheckCash() {                                     //Checks the amount of cash a player has
        return fCash;
    }

//******

//     void Player::Drop(Deck&/*inandout*/) {                          //The player can drop up to 3 cards - 4 if holding an ace - and draw that many
/*
        vector<int> vCardsToDrop;
        vector<Card>::iterator vIter = hCards.vCards.begin();
        int iMaxDrop = 3;
        int iDropCount = -1;
        for (vIter; vIter < hCards.vCards.end(); vIter++) {
            if (*vIter.GetNum() == ACE)
                iMaxDrop = 4;
        }
        while (iDropCount < 0 && iDropCount > iMaxDrop) {
            std::cout << "\nEnter how many cards you'd like to drop: \n\n";
            std::cin >> iDropCount;
        }
        for (int iIter = 0; iIter < iDropCount; iIter++) {
            int iCardChoice;
            std::cout << "\nWhich card do you wish to drop? (from left to right, enter 1 through 5)\n\n";
            std::cin >> iCardChoice;
            if (vCardsToDrop.size() == 0)
                vCardsToDrop.push_back(iCardChoice);
            else {
                int iTemp = vCardsToDrop.size() + 1;
                int iNeg = (vCardsToDrop.size() * (-1));
                int iHandSize = hCards.vCards.size() - iTemp + 1;
                iCardChoice = iCardChoice - iTemp;
                if (iCardChoice < iHandSize && iCardChoice > iNeg)
                    std::cout << "\nYou already chose that card.\n";
                else if (iCardChoice <= iNeg)
            }
        }
    }
*/

//******
    void Player::Fold(Deck& dInput/*out*/){                  //The player can forfeit their hand until the end of the round and get a new hand
        DiscardHand(dInput);
        bFolded = true;
}

//******
    bool Player::FoldedHand() {
        return bFolded;
    }

//******
    void Player::PlayHand() {                                   //Sorts the player's hand based on suit first, and then rank. Used to generate a point value from the hand
        hCards->SortHand();
        hCards->PrintHand();
//        ShowScore(outputScore, totalScore);
        std::cout << std::fixed <<  std::setprecision(14) << "\n" << hCards->CountPoints() << " points\n\n";
    }

//******
        void Player::PlayHand(bool noPrint) {                   //Sorts the player's hand based on suit first, and then rank. Used to generate a point value from the hand
        hCards->SortHand();                                     //No printing using bool
        hCards->PrintHand();
//        ShowScore(outputScore, totalScore);
//      std::cout << std::fixed <<  std::setprecision(14) << "\n" << hCards->CountPoints() << " points\n\n";
    }

//******
        void Player::DiscardCards(Deck& dInput) {
            hCards->DiscardCards(dInput);
        }

//******
        void Player::DiscardHand(Deck& dInput) {
            hCards->DiscardHand(dInput);
        }

//******
        void Player::NewHand(Deck& dInput) {
            bFolded = false;
            hCards->DiscardHand(dInput);
            hCards->DrawCard(5, dInput);
        }

//******
        void Player::NewHand(Deck& dInput, bool noDiscard) {
            bFolded = false;
            int tempInt = hCards->CardCount();
            hCards->DrawCard((5 - tempInt), dInput);
            currWager = 0;
        }

//******
        int Player::GetWager(){                             //Returns the player's current wager amount to the calling function
            return currWager;
        }

//******
    float Player::HandPoints() {                                  //Generates a "value" of the sorted hand
        hCards->SortHand();

        return hCards->CountPoints(false);                      //False prevents hand values from being printed to the screen
    }

//******
    //Hand Play() {                               //The player plays their sorted hand
    //    return hCards;
    //}

//******
    Player::~Player() {
        delete hCards;
        hCards = nullptr;
        std::cout << "\nDestructor test - Player\n\n";
    }                      //Destructor - currently does nothing

/*
private:
    Hand hCards(Deck& dInput);
    float fCash;

};

*/



//******
    CPU::CPU(Deck&/*in&out*/ myDeck, float/*in*/ myCash)      //Generates a player, gives them a hand of 5 cards and a sum of cash
            : Player(myDeck, myCash) {
        std::random_device statMaker;                       //Random device
        std::mt19937 seedMake(statMaker());                  //Random device seed
        std::uniform_int_distribution<int> randInt(0,5);       //The object that produces our random number
        intellectStat = randInt(seedMake);
        aggressivenessStat = randInt(seedMake);
        carefulnessStat = randInt(seedMake);
    }

//******
    float CPU::BetCash(int playerNum, Deck& dInput) {                               //The CPU chooses how much money to bet on a hand
        if (CheckCash() > 0) {
            float fTempCash = CheckCash() + 1;                                      //Sets temp cash just out of range to prime loop
            while (fTempCash > fCash || fTempCash < 0) {                            //Loop repeats until fTempCash is a valid amount (greater than or equal to 0 and less than total cash)
                std::cout << "\nEnter how much money you bet. Type 0 to fold.\n";
                std::cin >> fTempCash;
                if (fTempCash == 0) {                                                //If the player enters 0, they fold their hand
                    Fold(dInput);
                    break;
                }
                else if (fTempCash > CheckCash() || fTempCash < 0) {
                    std::cout << "\nInvalid input.  Only enter a value within your cash amount, or fold.\n";
                }
            }
            fCash -= fTempCash;                                                     //Subtracts the total amount to be bet from the total cash the player has
            return fTempCash;                                                       //Returns this value to the calling function    -   POT SHOULD BE CALLING OBJECT
        }
        else {
            std::cout << "\nOut of the money!\n\n";
            Fold(dInput);
            return -1;
        }
    }

//******
    float CPU::CallBet(float callValue, int playerNum, Deck& dInput) {             //The CPU can choose to call or raise an existing bet
        if (fCash < callValue) {
            std::cout << "\nPlayer 1 is out of the money\n\n";
            Fold(dInput);
            return callValue;
        }
        else if (FoldedHand()) {
            std::cout << "\nPlayer 1 has folded\n";
            return callValue;
        }
        else {
            int playerInput;
            bool betting = true;
            while (betting) {
                std::cout << "\nDo you raise, call, or fold? \n"
                          << "To call, type 0. To raise, type the amount you bet over the call\n"
                          << "To fold, type a negative value.\n\n"
                          << "Current call is:    " << callValue << "\n\n";
                std::cin >> playerInput;
                if (playerInput == 0) {
                    std::cout << "\nPlayer " << playerNum << " calls the bet.\n\n";
                    return callValue;
                }
                else if (playerInput < 0) {
                    std::cout << "\nPlayer " << playerNum << " folds.\n\n";
                    Fold(dInput);
                    return callValue;
                }
                else {
                        if (fCash >= (callValue + playerInput)) {
                            callValue += playerInput;
                            betting = false;
                        }
                        else {
                            std::cout << "\nPlayer " << playerNum << " does not have enough money for that raise.  Please try again.\n";
                        }
                    }
                }
            }
        return callValue;
    }

//******
    CPU::~CPU() {
        std::cout << "\nDestructor test - CPU\n\n";
    }
