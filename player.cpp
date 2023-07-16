//***********************************************
//      PLAYER CLASS - IMPLEMENTATION           *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a structure for    *
//  each individual player at the game table    *
//                                              *
//***********************************************


    #include <math.h>

    #include "player.h"

//******
    Player::Player(int handSize, Deck& dDeck,/*in&out*/ float fStartCash/*in*/) {
        fCash = fStartCash;                                                     //Starts each player off with a set amount of money
        hCards = new Hand(handSize, dDeck);                                     //Generates a hand of 5 cards for each player
        bFolded = false;                                                        //Sets the "folded" state to false - used to check if the player has folded this turn
        currWager = 0;
        opinions.resize(1);
    }

//******
    float Player::AnteUp(int anteSize, Deck& dInput) {
        if (fCash >= 5) {
            fCash -= anteSize;
            return anteSize;
        }
        else {
            Fold(dInput);
            return 0;
        }
    }

//******
    float Player::BetCash(Deck& dInput, float myPot, std::vector<std::vector<float>> potVect) {  //The player chooses how much money to bet on a hand
        if (fCash > 0) {
            float fTempCash = fCash + 1;                                                 //Sets temp cash just out of range to prime loop
            while (fTempCash > fCash || fTempCash < 0) {                                 //Loop repeats until fTempCash is a valid amount (greater than or equal to 0 and less than total cash)

                HeadsUpDisplay(myPot, potVect);
                std::cout << "\nEnter how much money you bet. Type 0 to fold.\n";
                std::cin >> fTempCash;
                if (fTempCash > fCash || fTempCash < 0 || !std::cin) {
                    std::cout << "\nInvalid input.  Only enter a value within your cash amount, or fold.\n";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    fTempCash = -1;
                } else if (fTempCash == 0) {                                                    //If the player enters 0, they fold their hand
                    Fold(dInput);
                    break;
                }
            }
            fTempCash = std::floor(fTempCash);
            fCash -= fTempCash;                                                     //Subtracts the total amount to be bet from the total cash the player has
            currWager += fTempCash;                                                  //Sets the current amount of money the player is betting to fTempCash
            return fTempCash;                                                       //Returns this value to the calling function    -   POT SHOULD BE CALLING OBJECT
        } else {
            std::cout << "\nOut of the money!\n\n";
            return 0;
        }
    }

//******
    float Player::CallBet(float callValue, Deck& dInput, float myPot, std::vector<std::vector<float>> potVect) {      //The player can choose to call or raise an existing bet
        int temp = 0;
        bool outOfMoney = false;
        if (fCash < callValue - currWager) {                                                //Checks if the player is out of the money - if so, they must go all in and not sidebet
            std::cout << "\nPlayer 1 is out of the money\n\n";
            outOfMoney = true;
        }
        if (FoldedHand()) {                                                                 //If the player has already folded, this paradigm persists
            std::cout << "\nPlayer 1 has folded\n";
            return 0;
        } else {
            int playerInput;
            bool betting = true;
            while (betting) {                                                               //This loop repeats as long as the player needs to call/raise/fold
                if (outOfMoney) {                                                               //If the player is out of the money, they must go all-in to stay in the game
                    std::cout << "\nYou must go all in to stay in the game.\n\n";
                }

                HeadsUpDisplay(myPot, potVect, callValue);                                      //Print the player's heads up display

                std::cout <<   "\n- Type the value you wish to raise the bet by (or go all-in if required)."
                          <<   "\n- Type 0 to call the bet (or to go all-in if required to stay in the game)."
                          <<   "\n- Type any value less than 0 to fold.\n\n";
                std::cin >> playerInput;                                                        //Player enters their choice. Negative values = fold. 0 = call. Positive values = raise
                if (playerInput == 0 && std::cin && !outOfMoney) {                              //Called value - returns the callValue back to the calling function
                    std::cout << "\nPlayer 1 calls the bet.\n\n";
                    return callValue;
                } else if (playerInput < 0) {                                                   //Fold return - returns 0 to calling function
                    std::cout << "\nPlayer 1 folds.\n\n";
                    Fold(dInput);
                    return 0;
                } else if (playerInput >= 0 && std::cin && outOfMoney) {                        //All in to stay in the game - returns entire cash value
                    std::cout << "\nPlayer 1 goes all in to stay in the game.\n\n";             //...this only works if outOfMoney is turned on
                    return fCash;
                } else {
                        if (fCash >= (callValue - currWager) + playerInput) {                   //If the player has enough cash for their input...
                            temp = callValue + playerInput;                                         //They raise the value by the input and returns the sum of it and callValue
                            temp = std::floor(temp);                                                //...this value is rounded down before returning
                            betting = false;
                        } else if (!std::cin) {                                                 //If they entred and invalid value, this is relayed to the player and they try again
                            std::cout << "\nInvalid input.  Please try again.\n\n";
                            std::cin.clear();
                            std::cin.ignore(1000, '\n');
                        } else {                                                                //If they don't have enough money, this is relayed and they try again

                            std::cout << "\nPlayer 1 does not have enough money for that raise.  Please try again.\n";
                        }
                    }
                }
            }
        return temp;                                                                    //Temp - the value the player is putting into the pot - is returned to the calling function
    }

//******
    void Player::GetCash(float fCashIn/*in*/) {                     //The player gets cash in return from the pot when they win
        fCash += fCashIn;
    }

//******
    float Player::TakeWager(float fCashIn/*in*/) {                  //The player's currWager variable is incremented by how much they are putting into the pot, and
        int temp = fCashIn - currWager;                             //...their cash is reduced by that much.  Then that value is returned to the calling function to increase
        currWager += temp;                                          //...the pot by how much the player is wagering
        fCash -= temp;
        return float(temp);
    }

//******
    float Player::AllIn() {                                         //Same as above, but returns the entire cash pool of the player to the caller
        int temp = fCash;
        currWager += fCash;
        fCash = 0;
        return float(temp);
    }

//******
    float Player::SoftWager(float fCashIn) {                        //This is used to modify the myPot variable - not the muiltipot.  Prevents double dipping into player money
        int temp = currWager;
        return fCashIn - temp;
    }

//******
    float Player::CheckCash() {                                     //Checks the amount of cash a player has
        return fCash;
    }

//******
    void Player::Fold(Deck& dInput/*out*/){                  //The player can forfeit their hand until the end of the floor and get a new hand
        DiscardHand(dInput);
        bFolded = true;
}

//******
    bool Player::FoldedHand() {                                 //Returns the status of bFolded - if the player has folded, returns true.  If not, returns false
        return bFolded;
    }

//******
    void Player::PlayHand() {                                   //Sorts the player's hand based on suit first, and then rank. Used to generate a point value from the hand
        hCards->SortHand();
        hCards->PrintHand();
        std::cout << std::fixed <<  std::setprecision(14) << "\n" << hCards->CountPoints(true) << " points\n\n";
    }

//******
        void Player::PlayHand(bool noPrint) {                   //Sorts the player's hand based on suit first, and then rank. Used to generate a point value from the hand
        hCards->SortHand();                                     //No printing using bool
        hCards->PrintHand();
        float dummy = hCards->CountPoints(true);
    }

//******
        void Player::DiscardCards(Deck& dInput) {               //Parses the playerHand into the discard function

            hCards->DiscardCards(dInput);
        }

//******
        void Player::DiscardHand(Deck& dInput) {                //Parses the player hand and discards the entire hand
            hCards->DiscardHand(dInput);
        }

//******
        void Player::NewHand(int handSize, Deck& dInput) {                    //Unfolds the player and draws them a new hand of cards
            bFolded = false;
            hCards->DiscardHand(dInput);                                        //Discards a full hand and draws that many cards
            hCards->DrawCard(handSize, dInput);
            currWager = 0;
        }

//******
        void Player::NewHand(int handSize, Deck& dInput, bool noDiscard) {  //Unfolds the player and draws however many cards the player needs to draw for a full hand
            bFolded = false;
            int tempInt = hCards->CardCount();                                  //tempInt is equal to the number of cards the player has
            hCards->DrawCard((handSize - tempInt), dInput);                     //draws however many cards the player needs - hand size constant minus card the player has
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
    void Player::SetOpinion(PlayerPerception input) {                           //Pushes a new opinion back on the player's perception vector.  What the player thinks of other players
        opinions.push_back(input);
    }

//******
    void Player::SetOpinion(PlayerPerception input, int selection) {            //Changes an existing opinion in the player's perception vector.
        opinions.at(selection) = input;
    }

//******
    void Player::SetCurrPot(int selection) {                                    //Sets the current pot the player is betting within
        currPot = selection;
    }

//******
    void Player::IncrementPot() {                                               //Increments the current pot the player is betting within
        currPot++;
    }

//******
    int Player::GetCurrPot() {                                                  //Returns the player's current betting pot to the calling function
        return currPot;
    }

//******
    void Player::ResetCurrPot() {                                               //Resets the current pot the player is betting on to 0.
        currPot = 0;
    }

//******
    void Player::HeadsUpDisplay(float myPot, std::vector<std::vector<float>> potVect) const {       //heads up display. Any time the player makes a decision, prints important info
        std::cout << "\n~~~~~~\n";
        std::cout << "Hand:\n\n";
        hCards->PrintHand();
        std::cout << "\n";
        std::cout << "\nCash:                        $" << fCash
                  << "\nMoney you have in the pot:   $" << currWager + ANTE << "\n";
        std::cout << "\n\nCumulative pot value:        $" << myPot << "\n";
        std::cout << "\n\n~~~~~~\n";
    }

//******
    //******
    void Player::HeadsUpDisplay(float myPot, std::vector<std::vector<float>> potVect, float callValue) const {  //The same as above but also prints the callValue
        std::cout << "\n~~~~~~\n";
        std::cout << "Hand:\n\n";
        hCards->PrintHand();
        std::cout << "\n";
        std::cout << "\nCash:                        $" << fCash
                  << "\nMoney you have in the pot:   $" << currWager + ANTE
                  << "\nCurrent call value:          $" << callValue << "\n";
        std::cout << "\nCumulative pot value:        $" << myPot << "\n";
        std::cout << "\n~~~~~~\n";
    }

//******
    Player::~Player() {
        delete hCards;
        hCards = nullptr;
    }                                             //Destructor - deallocates the player's hand and frees its memory



//******
    CPU::CPU(int handSize, Deck&/*in&out*/ myDeck, float/*in*/ myCash)      //Generates a player, gives them a hand of 5 cards and a sum of cash
            : Player(handSize, myDeck, myCash) {
        std::random_device statMaker;                          //Random device
        std::mt19937 seedMake(statMaker());                    //Random device seed
        std::uniform_int_distribution<int> randInt(1,10);      //The object that produces our random number - between 1 and 10
        perceptionStat = randInt(seedMake);
        aggressivenessStat = randInt(seedMake);
        bluffStat = randInt(seedMake);

    }

//******
    PlayerPerception CPU::JudgeHand() {                 //Gets a judgement for the hand and attempts to pass a bluffed judgement to the table

        float temp = hCards->CountPoints(false);
        std::random_device scoreMake;                           //Random device
        std::mt19937 seedMake(scoreMake());                     //Random device seed
        std::uniform_int_distribution<int> randInt(1, 100);     //The object that produces our random number
        int tempRoll = 0;                                       //Diceroll variable - used to streamline the dice rolling process

        if (temp <= MIN_STRENGTH) {                     //Weakest judgement
            opinions[0] = WEAK;
        }
        else if (temp <= TWOPAIR_MULT) {                //Second weakest
            opinions[0] = SUBPAR;
        }
        else if (temp <= TRIPS_MULT) {                  //Average
            opinions[0] = AVERAGE;
        }
        else if (temp <= STRAIGHT_MULT) {               //Second strongest
            opinions[0] = DECENT;
        }
        else {
            opinions[0] = STRONG;                       //Strongest
        }
        tempRoll = (randInt(seedMake));                 //Random number

        //Takes the player's bluff stat pluss 1/20th of their random roll.
        //If this number is greater than the easy roll constant OR they roll a natural twenty (93 and above)...
        if (bluffStat + tempRoll / DICE_MOD > EASY_ROLL || tempRoll > NAT_TWENTY) {

            tempRoll = (randInt(seedMake));       //Temproll is re-initialized

            //1/20th of the random roll is added to the aggressiveness stat
            //If the roll is a natural twenty OR they roll above a master roll value...

            if (aggressivenessStat + tempRoll / DICE_MOD > MASTER_ROLL || tempRoll > NAT_TWENTY) {

                GetTell(PlayerPerception(int(STRONG) - int(opinions[0])));      //Returns and stores the opposite of their hand's worth
                return PlayerPerception(int(STRONG) - int(opinions[0]));
            }
            else {                                  //If not...
                GetTell(CANNOT_READ);                   //Returns and outputs a poker face
                return CANNOT_READ;
            }
        }
        else {                              //If not greater than an easy roll or a nat twenty...
            GetTell();                          //Returns and outputs the player's actual judgement of their own hand
            return opinions[0];
        }
    }

//******
    PlayerPerception CPU::JudgeHand(bool noOuput) {                 //Gets a judgement for the hand and attempts to pass a bluffed judgement to the table
                                                                    //BOOL PASSED PREVENTS OUTPUT TO THE TERMINAL
                                                                    //The same as above without an output to the screen
        float temp = hCards->CountPoints(false);
        std::random_device scoreMake;                           //Random device
        std::mt19937 seedMake(scoreMake());                     //Random device seed
        std::uniform_int_distribution<int> randInt(1, 100);     //The object that produces our random number


        if (temp <= MIN_STRENGTH) {
            opinions[0] = WEAK;
        }
        else if (temp <= TWOPAIR_MULT) {
            opinions[0] = SUBPAR;
        }
        else if (temp <= TRIPS_MULT) {
            opinions[0] = AVERAGE;
        }
        else if (temp <= STRAIGHT_MULT) {
            opinions[0] = DECENT;
        }
        else {
            opinions[0] = STRONG;
        }

        if (bluffStat + (randInt(seedMake)) / DICE_MOD > 10) {
            return PlayerPerception(int(STRONG) - int(opinions[0]));
        }
        else {
            return opinions[0];
        }
    }

//******
    void CPU::GetTell(PlayerPerception opinion) {           //Outputs the player's bluffed judgement to the screen
        if (FoldedHand()) {
            std::cout << "\nThey have folded.\n\n";
        }
        else {
            switch(opinion) {
                case WEAK:      std::cout << "\nThey don't look too happy with the cards they were dealt.\n\n"; break;
                case SUBPAR:    std::cout << "\nThey look as though their cards could be better.\n\n"; break;
                case AVERAGE:   std::cout << "\nThey look indifferent; their hand must be serviceable.\n\n"; break;
                case DECENT:    std::cout << "\nThey look happy; they must have good cards in their hand.\n\n"; break;
                case STRONG:    std::cout << "\nThey almost look estactic - their hand must be excellent.\n\n"; break;
                default:        std::cout << "\nYou can't really get a good read on their face.  They could have anything.\n\n"; break;
            }
        }
    }

//******
    void CPU::GetTell() {           //Outputs the player's true judgement to the screen
        if (FoldedHand()) {
            std::cout << "\nThey have folded.\n\n";
        }
        else {
            switch(opinions[0]) {
                case WEAK:      std::cout << "\nThey don't look too happy with the cards they were dealt.\n\n"; break;
                case SUBPAR:    std::cout << "\nThey look as though their cards could be better.\n\n"; break;
                case AVERAGE:   std::cout << "\nThey look indifferent; their hand must be serviceable.\n\n"; break;
                case DECENT:    std::cout << "\nThey look happy; they must have good cards in their hand.\n\n"; break;
                case STRONG:    std::cout << "\nThey almost look estactic - their hand must be excellent.\n\n"; break;
                default:        std::cout << "\nYou can't really get a good read on their face.  They could have anything.\n\n"; break;
            }
        }
    }

//******
    void CPU::DiscardCards(Deck& dInput) {          //Allows the CPU to select and discard certain cards for a better hand
        if (opinions[0] < STRONG) {                 //If their hand is not considered "strong"...
            std::vector<int> selections;                //Vector of ints
            hCards->SelectCards(selections);            //A function is called using the vector above to store the selected cards to discard

            hCards->CPUDiscard(dInput, selections);     //The selected cards are parsed, iterated, discarded, and new cards are drawn
        } else {
            std::cout << "\nThe player keeps all cards in their hand.\n\n";
        }
    }

//******
    float CPU::BetCash(int playerNum, Deck& dInput, std::vector<PlayerPerception> inPerception, //OPPONENT OPINION OF SELF
                       std::vector<double> inScores, std::vector<int> inBluff,
                       std::vector<PlayerPerception> inOpinion /*OPPENENT OPPINION OF PLAYER*/) {                               //The CPU chooses how much money to bet on a hand

        float actionScore = 0;                                    //This score determines if the player will bet or if they will fold
        double fTemp;                                           //Temporarily hold float values
        int iTemp;                                              //Temporarily hold int values

        std::vector<PlayerPerception> yourOpinion(PLAYER_COUNT- 1);              //The opinion this CPU has about each player's hand
        std::vector<PlayerPerception> theirOpinion(PLAYER_COUNT - 1);             //The opinions that this CPU believes other player's hold about this CPU

        std::random_device scoreMake;                           //Random device
        std::mt19937 seedMake(scoreMake());                     //Random device seed
        std::uniform_int_distribution<int> randInt(1, 100);     //The object that produces our random number
        int tempRoll = 0;                                       //Variable used to hold dicerolls
        int playerRoll = 0;                                     //Variable used to hold player dicerolls
        int opponentRoll = 0;                                   //Variable used to hold opponent dicerolls
        //std::cout << "\nCPU test 01\n";
        if (CheckCash()) {
            //std::cout << "\nCPU test 02\n";
            float fTempCash = CheckCash() + 1;                                      //Sets temp cash just out of range to prime loop
            while (fTempCash > fCash || fTempCash < 0) {                            //Loop repeats until fTempCash is a valid amount (greater than or equal to 0 and less than total cash)

                tempRoll = randInt(seedMake);
                fTemp = (hCards->CountPoints(false) / POINTS_DIV) + (tempRoll / DICE_MOD);          //Store 1 / 100th the point value of the hand into fTemp + a dice roll evaluated at the roll 1 through 100 divided by 20.
                actionScore = fTemp * perceptionStat;                                               //The hand's value is multiplied by the player's perception stat - this means that great hands will be
                                                                                                    //...generating exponentially more action points than weak hands

                for (int iter = 0; iter < PLAYER_COUNT - 1; iter++) {                       //Goes through each opponent and tries to gather tells from them based on their overt tells and their bluff stat vs your perception stat w/ dice rolling
                    tempRoll = randInt(seedMake);
                    fTemp = perceptionStat + (tempRoll / DICE_MOD);
                    playerRoll = tempRoll;

                    tempRoll = randInt(seedMake);
                    float newTemp = (inBluff[iter] + (tempRoll / DICE_MOD));
                    opponentRoll = tempRoll;

                    if (fTemp >= newTemp || opponentRoll < NAT_ZERO || playerRoll > NAT_TWENTY) {
                        PlayerPerception tempPercept = inPerception[iter];                  //Stores the current opponent's opinion/bluff of itself in a temporary variable

                        tempRoll = randInt(seedMake);
                        double opponentScore = inScores[iter]
                                - (inBluff[iter] + (tempRoll / DICE_MOD));       //Stores the opponents hand score slightly obfuscated by the opponents bluff stat and a dice roll

                        opponentScore -= hCards->CountPoints(false);                                                 //Subtracts your score from the opponents score.  The remainder is used to evaluate how much better you think the opponents hand is
                        if (opponentScore >= 5) {

                            tempRoll = randInt(seedMake);
                            if (opponentScore >= 1000 - (perceptionStat * (tempRoll * DICE_MOD))) {  //If it's greater than 1000 minus your perception stat multiplied by a dice roll, you think it's a very strong hand you likely won't beat
                                yourOpinion[iter] = STRONG;
                            }
                            else {                                                                              //If it's greater than 5, it's a strong hand relative to your hand, but you might be able to beat it
                                yourOpinion[iter] = DECENT;
                            }
                        }
                        else if (opponentScore <= -5) {
                            tempRoll = randInt(seedMake);
                            if (opponentScore <= -1000 + (perceptionStat * (tempRoll * DICE_MOD))) { //If it's less than negative 1000 plus your perception stat multiplied by a dice roll, you think it's a very weak hand you'll likely beat easily
                                yourOpinion[iter] = WEAK;
                            }
                            else {                                                                              //If it's less than negative 5, it's a weak hand relative to your hand, but might still be able to beat you
                                yourOpinion[iter] = SUBPAR;
                            }
                        }
                        else {
                            yourOpinion[iter] = AVERAGE;                                                        //If it's between -5 and 5, it's an average hand - it could go either way
                        }

                    }

                    else {
                        yourOpinion[iter] = inPerception[iter];                                                 //If you cannot see through their bluff, you think their hand is as good as the impression they give you
                    }

                    tempRoll = randInt(seedMake);
                    actionScore -= ((2 * (int(yourOpinion[iter]) - int(opinions[0])) * (perceptionStat + (tempRoll / DICE_MOD)))
                                        + (int(yourOpinion[iter]) * (randInt(seedMake) / DICE_MOD)));                                     //This will take away or add to the action score based on how confident you feel about your own hand
                                                                                                                                            //...vs how you feel about the player tells, with a slight modifier based on raw player tell data
                                                                                                                                            //...added to a dice roll
                }

                for (auto i : inOpinion) {
                    tempRoll = randInt(seedMake);
                    if ((bluffStat + (tempRoll / DICE_MOD)) >= MED_ROLL || tempRoll > NAT_TWENTY) {                           //Uses bluff stat and a dice roll to see if the player can use information to adjust player action score
                        if (i = CANNOT_READ) {
                           actionScore += int (opinions[0] - AVERAGE) * (randInt(seedMake) / DICE_MOD);                           //If the opponent cannot be read or is a player, instead the action is modified based on how good the player's hand is
                        }
                        else if (i > AVERAGE && opinions[0] > AVERAGE) {                                                            //If the opponent thinks the player's hand is great and it is, action score is increased
                            actionScore += int(opinions[0] - AVERAGE) * (bluffStat * (randInt(seedMake) / DICE_MOD));
                        }
                        else if (i < AVERAGE && opinions[0] < AVERAGE) {                                                            //If the opponent thinks the player's hand is poor and it is, action score is decreased
                            actionScore -= int(AVERAGE - opinions[0]) * (bluffStat * (randInt(seedMake) / DICE_MOD));
                        }
                        else if (i < AVERAGE && opinions[0] >= AVERAGE) {                                                           //If the opponent thinks the player's hand is poor and it ISN'T, action score is INCREASED
                            actionScore += ((int (opinions[0] - AVERAGE) + 1) * (bluffStat * (randInt(seedMake)) / DICE_MOD));
                        }
                    }
                    else {                                                                                                      //In all other situations, action score is modified based on how good the player hand is
                        actionScore += int (opinions[0] - AVERAGE) * (randInt(seedMake) / DICE_MOD);
                    }
                }

                //if their action score is at least greater than a medium action, the routine of setting it between 1 and 10 based on raw hand value is performed
                //...otherwise, it remains at 0.
                int actionMod = 0;
                if (actionScore > MEDIUM_ACTION) {
                    if (actionScore >= DECISIVE_ACTION * DICE_MOD) {
                        actionMod = DICE_MOD;
                    }
                    else {
                        int temp = (DICE_MOD * DECISIVE_ACTION) - actionScore;
                        if (temp < 1) {
                            temp = 1;
                        }
                        actionMod = DICE_MOD / (temp);
                    }
                }


                tempRoll = randInt(seedMake);
                int tempRoll2 = randInt(seedMake);
                int tempRoll3 = randInt(seedMake);

                    //If the player's action score times their modified dice roll times (their agressiveness stat times a modified dice roll) is GREATER...
                        //...than the easy action constant, they will lead the bet this turn
                if (actionScore * (tempRoll / DICE_MOD)
                    * (aggressivenessStat * (randInt(seedMake) / DICE_MOD)) > EASY_ACTION){

                    //The cash they will raise the score by becomes equal to their cash divided by 50.  Then, this number is multiplied by 5 different elements
                            //1)        their raw aggressiveness stat
                            //2)        their first dice roll over the dice modifier (1-100 / 10)
                            //3)        their aggressiveness stat over 10 times their second dice roll over the dice modifier
                            //4)        their action modifier - a scale of 1 through 10 based on their raw hand value
                            //5)        their perception stat over (two times dice mod [20]), and this value times their action mod plus (their 3rd roll over diceMod)

                    fTempCash = (fCash / (DICE_MOD * 5.0)) * (((aggressivenessStat) + ((tempRoll / DICE_MOD)) + ((aggressivenessStat / DICE_MOD) * ((tempRoll2 / DICE_MOD)))
                                                            + (actionMod) + ((perceptionStat / (DICE_MOD * 2)) * (actionMod + tempRoll3 / DICE_MOD))));


                    if (fTempCash < 1) {    //if their fTempCash (amount they would like to bet) is less than 1, it is set to 1 instead
                        fTempCash = 1;
                    }
                    fTempCash = std::floor(fTempCash);      //fTempCash is rounded down
                    currWager = fTempCash;                  //CurrWager is set to the current fTempCash - this works because it would be the first bet of the game in this particular instance
                    std::cout << "\nPlayer " << playerNum << " bets $" << currWager << "\n\n";
                }

                else {                                                              //If the player's action score isn't high enough to justify betting...
                                                                                    //...they fold their hand.
                    fTempCash = 0;
                    std::cout << "\nPlayer " << playerNum << " has folded.\n\n";
                    Fold(dInput);
                    break;
                }
            }
            fCash -= fTempCash;                                                     //Subtracts the total amount to be bet from the total cash the player has
            return fTempCash;                                                       //Returns this value to the calling function    -   POT SHOULD BE CALLING OBJECT
        }
        else {
            std::cout << "\nPlayer " << playerNum << " is out of the money!\n\n";
            return 0;
        }
    }

//******
    float CPU::CallBet(float callValue, int playerNum, Deck& dInput,
                       std::vector<PlayerPerception> inPerception,
                       std::vector<double> inScores, std::vector<int> inBluff,
                       std::vector<PlayerPerception> inOpinion) {                               //The CPU chooses how much money to bet on a hand

        float actionScore = 0 - (callValue / MEDIUM_ACTION);     //This score determines if the player will bet or if they will fold
        double fTemp;                                            //Temporarily hold float values
        int iTemp;                                               //Temporarily hold int values

        std::vector<PlayerPerception> yourOpinion(PLAYER_COUNT - 1);    //The opinion this CPU has about each player's hand
        std::vector<PlayerPerception> theirOpinion(PLAYER_COUNT - 1);   //The opinions that this CPU believes other player's hold about this CPU

        std::random_device scoreMake;                           //Random device
        std::mt19937 seedMake(scoreMake());                     //Random device seed
        std::uniform_int_distribution<int> randInt(1, 100);     //The object that produces our random number
        int tempRoll = 0;                                       //Variable to hold the general dice roll
        int playerRoll = 0;                                     //Variable to hold the value of the player's dice roll
        int opponentRoll = 0;                                   //Variable to hold the value of the opponent's dice roll
        bool outOfMoney = false;

        if (fCash < (callValue - currWager)) {                                          //OutOfMoney turns on if the player has not enough money to meet the call
            std::cout << "\nPlayer " << playerNum << " is out of the money.\n\n";
            outOfMoney = true;
        }

        if (FoldedHand()) {                                                             //Returns 0 if the player has folded
            std::cout << "\nPlayer " << playerNum << " has folded.\n\n";
            return 0;
        } else {
            float fTempCash = CheckCash() + 1;                                      //Sets temp cash just out of range to prime loop
            while (fTempCash > fCash || fTempCash < 0) {                            //Loop repeats until fTempCash is a valid amount (greater than or equal to 0 and less than total cash)

                tempRoll = randInt(seedMake);
                fTemp = (hCards->CountPoints(false) / POINTS_DIV) + (tempRoll / DICE_MOD);      //Store 1 / 100th the point value of the hand into fTemp + a dice roll evaluated at the roll 1 through 100 divided by 20.
                actionScore = fTemp * perceptionStat;                                               //The hand's value is multiplied by the player's perception stat - this means that great hands will be
                                                                                                    //...generating exponentially more action points than weak hands

                for (int iter = 0; iter < PLAYER_COUNT - 1; iter++) {                       //Goes through each opponent and tries to gather tells from them based on their overt tells and their bluff stat vs your perception stat w/ dice rolling
                    tempRoll = randInt(seedMake);
                    fTemp = perceptionStat + (tempRoll / DICE_MOD);
                    playerRoll = tempRoll;

                    tempRoll = randInt(seedMake);
                    float newTemp = (inBluff[iter] + (tempRoll / DICE_MOD));
                    opponentRoll = tempRoll;

                    if (fTemp >= newTemp || opponentRoll < NAT_ZERO || playerRoll > NAT_TWENTY) {
                        PlayerPerception tempPercept = inPerception[iter];                  //Stores the current opponent's opinion/bluff of itself in a temporary variable
                        tempRoll = randInt(seedMake);
                        double opponentScore = inScores[iter]
                                - (inBluff[iter] + (tempRoll / DICE_MOD));       //Stores the opponents hand score slightly obfuscated by the opponents bluff stat and a dice roll

                        opponentScore -= hCards->CountPoints(false);                                                 //Subtracts your score from the opponents score.  The remainder is used to evaluate how much better you think the opponents hand is
                        if (opponentScore >= 5) {
                            if (opponentScore >= 1000 - (perceptionStat * (randInt(seedMake) * DICE_MOD))) {  //If it's greater than 1000 minus your perception stat multiplied by a dice roll, you think it's a very strong hand you likely won't beat
                                yourOpinion[iter] = STRONG;
                            }
                            else {                                                                              //If it's greater than 5, it's a strong hand relative to your hand, but you might be able to beat it
                                yourOpinion[iter] = DECENT;
                            }
                        }
                        else if (opponentScore <= -5) {
                            if (opponentScore <= -1000 + (perceptionStat * (randInt(seedMake) * DICE_MOD))) { //If it's less than negative 1000 plus your perception stat multiplied by a dice roll, you think it's a very weak hand you'll likely beat easily
                                yourOpinion[iter] = WEAK;
                            }
                            else {                                                                              //If it's less than negative 5, it's a weak hand relative to your hand, but might still be able to beat you
                                yourOpinion[iter] = SUBPAR;
                            }
                        }
                        else {
                            yourOpinion[iter] = AVERAGE;                                                        //If it's between -5 and 5, it's an average hand - it could go either way
                        }

                    }

                    else {
                        yourOpinion[iter] = inPerception[iter];                                                 //If you cannot see through their bluff, you think their hand is as good as the impression they give you
                    }

                    tempRoll = randInt(seedMake);
                    //This will take away or add to the action score based on how confident you feel about your own hand
                    //...vs how you feel about the player tells, with a slight modifier based on raw player tell data
                    //...added to a dice roll
                    actionScore -= ((2 * (int(yourOpinion[iter]) - int(opinions[0])) * (perceptionStat + (tempRoll / DICE_MOD)))
                                        + (int(yourOpinion[iter]) * (randInt(seedMake) / DICE_MOD)));                                     //This will take away or add to the action score based on how confident you feel about your own hand
                                                                                                                                            //...vs how you feel about the player tells, with a slight modifier based on raw player tell data
                                                                                                                                            //...added to a dice roll.  The action score is how the CPU will make decisions
                }

                for (auto i : inOpinion) {
                    tempRoll = randInt(seedMake);
                    if ((bluffStat + (tempRoll / DICE_MOD)) >= MED_ROLL || tempRoll > NAT_TWENTY) {                            //Uses bluff stat and a dice roll to see if the player can use information to adjust player action score
                        if (i = CANNOT_READ) {
                           actionScore += int (opinions[0] - AVERAGE) * (randInt(seedMake) / DICE_MOD);                           //If the opponent cannot be read or is a player, instead the action is modified based on how good the player's hand is
                        }
                        else if (i > AVERAGE && opinions[0] > AVERAGE) {                                                            //If the opponent thinks the player's hand is great and it is, action score is increased
                            actionScore += int(opinions[0] - AVERAGE) * (bluffStat * (randInt(seedMake) / DICE_MOD));
                        }
                        else if (i < AVERAGE && opinions[0] < AVERAGE) {                                                            //If the opponent thinks the player's hand is poor and it is, action score is decreased
                            actionScore -= int(AVERAGE - opinions[0]) * (bluffStat * (randInt(seedMake) / DICE_MOD));
                        }
                        else if (i < AVERAGE && opinions[0] >= AVERAGE) {                                                           //If the opponent thinks the player's hand is poor and it ISN'T, action score is INCREASED
                            actionScore += ((int (opinions[0] - AVERAGE) + 1) * (bluffStat * (randInt(seedMake)) / DICE_MOD));
                        }
                    }
                    else {                                                                                                      //In all other situations, action score is modified based on how good the player hand is
                        actionScore += int (opinions[0] - AVERAGE) * (randInt(seedMake) / DICE_MOD);
                    }
                }
                float tempForIf = actionScore;                                  //This variable takes and manipulates the action score
                                                                                    //*****SEEMS REDUNDANT***** why not just use action score?
                int actionMod = 0;                                              //This variable plays a part in the money selection section if the player bets
                                                                                    //This directly represents how strong the player's hand is for bet amount
                if (actionScore > MEDIUM_ACTION) {                              //If the player's action score is at least above the medium constant, actionMod
                                                                                    //...is at least 1
                    if (actionScore >= DECISIVE_ACTION * DICE_MOD) {                //If actionScore is greater than the dice modifier times the decisive constant...
                        actionMod = DICE_MOD;                                           //actionMod becomes the dice modifier - the highest possible value it can hold
                    }
                    else {                                                          //otherwise...
                        int temp = (DICE_MOD * DECISIVE_ACTION) - actionScore;          //temp is created as dicemod x decisive minus action score
                        if (temp < 1) {                                                     //temp is rounded up to 1 if it is less than 1
                            temp = 1;
                        }
                        actionMod = DICE_MOD / (temp);                          //ActionMod beccomes the dice modifier divided by temp
                    }
                }
                if (outOfMoney && fCash == 0){                                              //If the player is already all-in, this is relayed and 0 is returned
                    std::cout << "\nPlayer " << playerNum << " is already all-in.\n\n";
                    fTempCash = 0;

                    //If the player is out of the money AND their tempForIf/actionScore is greater than the decisive constant over the specific mod...
                        //... the player's cash over (their aggressiveness state times (a random number over dice modifier))...
                    //...then they can go all in to stay in the game.
                }else if (outOfMoney && tempForIf > DECISIVE_ACTION + (fCash / ((aggressivenessStat * (randInt(seedMake) / DICE_MOD))))) {
                    std::cout << "\nPlayer " << playerNum << " goes all-in to stay in the game.\n\n";
                    fTempCash = fCash;
                    return fTempCash;

                    //If they are NOT out of the money and their score is greater than the medium constant but is less than/equal to the decisive constant...
                    //...then they will call the bet
                } else if (!outOfMoney && tempForIf > MEDIUM_ACTION && tempForIf <= DECISIVE_ACTION) {
                    fTempCash = callValue;
                    std::cout << "\nPlayer " << playerNum << " calls the bet.\n\n";

                    //If they are NOT out of the money and their score is greater than the decisive constant...
                    //...then they will RAISE the bet if they have the money

                } else if (!outOfMoney && tempForIf > DECISIVE_ACTION) {

                    if (fCash - MIN_CALL > (callValue - currWager)) {   //If their cash minus the minimum call (10) is greater than the call value minus their currWager...
                                                                            //...they will raise the bet using stats and random rolls
                        int testint = 0;
                        tempRoll = randInt(seedMake);
                        int tempRoll2 = randInt(seedMake);
                        int tempRoll3 = randInt(seedMake);
                        float checkCash = fCash - (callValue - currWager);

                        //The cash they will raise the score by becomes equal to their cash divided by 50.  Then, this number is multiplied by 5 different elements
                            //1)        their raw aggressiveness stat
                            //2)        their first dice roll over the dice modifier (1-100 / 10)
                            //3)        their aggressiveness stat over 10 times their second dice roll over the dice modifier
                            //4)        their action modifier - a scale of 1 through 10 based on their raw hand value
                            //5)        their perception stat over (two times dice mod [20]), and this value times their action mod plus (their 3rd roll over diceMod)
                        fTempCash = ((checkCash) / (DICE_MOD * 5)) * ((aggressivenessStat) + (tempRoll / DICE_MOD) + (aggressivenessStat / DICE_MOD * (tempRoll2 / DICE_MOD))
                                                            + (actionMod) + ((perceptionStat / (DICE_MOD * 2)) * (actionMod + (tempRoll3 / DICE_MOD))));

                        fTempCash = std::floor(fTempCash);      //the cash they will wager is rounded down.
                        if (fTempCash < MIN_CALL) {             //If it's less than 10, it is rounded up to 10 at the least.
                            fTempCash = MIN_CALL;
                        }
                        std::cout << "\nPlayer " << playerNum << " raises the bet by " << fTempCash << "\n\n";
                        fTempCash += callValue;

                        //If their cash minus the minimum call is NOT greater than the call value minus their currWager, they CALL the bet instead of raising it
                    } else {
                        fTempCash = callValue;
                        std::cout << "\nPlayer " << playerNum << " calls the bet.\n\n";
                    }
                } else {                                                              //If the player bets nothing, they fold their hand
                    fTempCash = 0;
                    Fold(dInput);
                    std::cout << "\nPlayer " << playerNum << " has folded.\n\n";
                    return 0;
                }
                fTempCash -= currWager;                                             //Reduces fTempCash by currWager to end the loop
            }
            fTempCash += currWager;                                                 //Adds currWager back for correct math after the loop
                                                                                        //*****SEEMS A LITTLE JANKY***** ^^better way of doing this??

            return fTempCash;                                                       //Returns this value to the calling function    -   POT SHOULD BE CALLING OBJECT
        }
    }

//******
    int Player::GetBluffStat() {                                                    //Returns bluff stat of the player to the calling function
        return bluffStat;
    }

//******
    PlayerPerception Player::GetPlayerTells(int input) {                            //Returns a specific player's opinion OF a specific player to the calling function
        return opinions[input];
    }

//******
