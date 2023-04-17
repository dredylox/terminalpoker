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

//***********************************************

#include "hand.h"

//***********************************************

    Hand::Hand(Deck& dInput) {
        GetHand(dInput);
    }

//******
    void Hand::GetHand(Deck& dInput) {
        DrawCard(5, dInput);
    }

//******
    void Hand::SortHand() {
        std::vector<Card> vTempSort;                                //
        int iSize = vCards.size();
        Card* tempCard = nullptr;
        for (int iCount = 0; iCount < iSize; iCount++) {
            int vCardSize = vCards.size();
            for (int iter = 0; iter < vCardSize; iter++) {
                CardNum cnFinal;
                CardNum cnTemp;
                cnTemp = vCards.at(iter).GetNum();
                Card cardTemp(vCards.at(iter));
                if (cnTemp > cnFinal || iter == 0) {
                    if (tempCard != nullptr) {
                        delete tempCard;
                        tempCard = nullptr;
                    }
                    cnFinal = cnTemp;
                    tempCard =  new Card(cardTemp);
                }
            }
            vTempSort.push_back(*(tempCard));
            for (int iter = 0; iter < int(vCards.size()); iter++) {
                if (vCards.at(iter) == *(tempCard)) {
                    vCards.erase(vCards.begin() + iter);
                }
            }
            delete tempCard;
            tempCard = nullptr;
        }
        vCards = vTempSort;
    }

//******
    void Hand::DrawCard(int iCount, Deck& dInput) {
        for (int iter = 0; iter < iCount; iter++) {
            vCards.push_back(dInput.DealCard(HAND_STATE));
        }
    }
/*
//******
    void Hand::Discard(std::vector<int> vDiscard, Deck& dInput) {               //This function is terribad - don't use it yet
        int iSize = 0;
        std::vector<int>::iterator iter;
        for (iter = vDiscard.begin(); iter < vDiscard.end(); iter++) {
            vCards.erase(vCards.begin() + iter);
            iSize++;
        }
        DrawCard(iSize, dInput);                        //Draws cards for the user
    }
*/
//******
    void Hand::PrintHand() const {
        std::cout << "Player hand:\n\n";
        for (auto& iter : vCards) {
            std::cout << " ,,,,,,, ";
        }
        std::cout << "\n";


        for (auto& iter : vCards) {
            std::cout << " |" << iter.GetSuitSym() << "  " << iter.GetNumStr() << "| ";
        }
        std::cout << "\n";

        for (auto& iter : vCards) {
            std::cout << " |     | ";
        }
        std::cout << "\n";

        for (auto& iter : vCards) {
            std::cout << " |" << iter.GetSuitStr() << "| ";
        }
        std::cout << "\n";

        for (auto& iter : vCards) {
            std::cout << " |     | ";
        }
        std::cout << "\n";

        for (auto& iter : vCards) {
            std::cout << " |" << iter.GetNumStr() << "  " << iter.GetSuitSym() << "| ";
        }
        std::cout << "\n";

         for (auto& iter : vCards) {
            std::cout << " ``````` ";
        }
        std::cout << "\n";
    }

//******
    //******
    void Hand::PrintCard(Card cardin) const {
        std::cout << " ,,,,,,, ";
        std::cout << "\n";


        std::cout << " |" << cardin.GetSuitSym() << "  " << cardin.GetNumStr() << "| ";
        std::cout << "\n";
        std::cout << " |     | ";
        std::cout << "\n";

        std::cout << " |" << cardin.GetSuitStr() << "| ";
        std::cout << "\n";

        std::cout << " |     | ";
        std::cout << "\n";

        std::cout << " |" << cardin.GetNumStr() << "  " << cardin.GetSuitSym() << "| ";
        std::cout << "\n";

        std::cout << " ``````` ";
        std::cout << "\n";
    }


//******
    double Hand::CountPoints() {
        SortHand();
        long double totalScore = 0;
        int outputScore = 0;
        int iteration = 0;
        CardSuit csTemp;
        CardNum cnTemp;


        for (auto& iter : vCards) {
            //            13 ^ -x, where x is 13 - the value of the card + 1; This means ACE will return 1.0, whereas 2 will return 4E-14, etc
            //              MAKE A TABLE FOR THIS
            totalScore += double(1 / std::pow(13,(13 - (int(iter.GetNum()) + 1))));
        }

        int streak = 0;
        bool hasPair = false;
        bool hasTrips = false;
        bool hasQuads = false;
        bool twoPair = false;
        bool fullHouse = false;
        bool tripsFirst = false;
        std::vector<int> StreakedCards;
        for (auto& iter : vCards) {
            if (iteration > 0) {
                if (cnTemp == iter.GetNum()) {
                    if (hasPair && streak == 1) {
                        hasTrips = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else if (hasTrips && streak == 2) {
                        hasQuads = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else if (hasTrips && streak == 0) {
                        fullHouse = true;
                        tripsFirst = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        hasPair = true;
                        streak++;
                    }
                    else if (hasPair && streak == 0) {
                        twoPair = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else if (twoPair) {
                        hasTrips = true;
                        hasPair = true;
                        fullHouse = true;
                        twoPair = false;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else {
                        hasPair = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                }
                else {
                    streak = 0;
                }
            }
            cnTemp = iter.GetNum();
            iteration++;
        }
        unsigned long int temp = 0;
        if (hasPair) {
            if (hasTrips) {
                if (hasQuads) {
                    for (auto iter : StreakedCards) {
                        int holdCard = iter;
                        temp += (holdCard * 4000000);
                    }
                    outputScore = 800;
                }
                else if (fullHouse) {
                    if (tripsFirst) {
                        for (int iter = 0; iter < 3; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 300000);
                        }
                        for (int iter = 3; iter < 5; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 1000);
                        }
                    }
                    else {
                        for (int iter = 0; iter < 2; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 1000);
                        }
                        for (int iter = 2; iter < 5; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 300000);
                        }
                    }
                    outputScore += 600;
                }
                else {
                    for (auto iter : StreakedCards) {
                        int holdCard = iter;
                        temp += (holdCard * 720);
                    }
                    outputScore += 300;
                }
            }
            else if (twoPair) {
                for (int iter = 0; iter < 2; iter++) {
                    int holdCard = StreakedCards.at(iter);
                    temp += (holdCard * 30);
                }
                for (int iter = 2; iter < 4; iter++) {
                    int holdCard = StreakedCards.at(iter);
                }
                outputScore += 200;
            }
            else {
                for (auto iter : StreakedCards) {
                    int holdCard = iter;
                    temp += (holdCard);
                }
                outputScore += 100;
            }
            totalScore += double(temp);
            ShowScore(outputScore, totalScore);
            return totalScore;
        }

        bool isStraight = true;
        bool isFlush = true;
        iteration = 0;
        for (auto iter : vCards) {
            if (iteration > 0) {
                if (iter.GetSuit() != csTemp) {
                    isFlush = false;
                }
                if (iter.GetNum() != (cnTemp - CardNum(1))) {
                    isStraight = false;
                }
            }
            StreakedCards.push_back(int(iter.GetNum()) + 1);
            csTemp = iter.GetSuit();
            cnTemp = iter.GetNum();
            iteration++;
        }

        if (isStraight) {
            if (isFlush) {
                for (auto iter : StreakedCards) {
                    int holdCards = iter;
                    temp += (holdCards * 53000000);
                }
                outputScore += 400;
                if (temp > 2914000000) {
                    totalScore = 9999999999.0;
                    outputScore = 900;
                    ShowScore(outputScore, totalScore);
                    return totalScore;
                }
                outputScore += 500;

            }
            else {
                for (auto iter : StreakedCards) {
                    int holdCards = iter;
                    temp += (holdCards * 2400);
                }
                outputScore += 400;
            }
            totalScore += double(temp);
        }
        else if (isFlush) {
            for (auto iter : StreakedCards) {
                int holdCards = iter;
                temp += (holdCards * 7200);
                totalScore += double(temp);
            }
        }
        ShowScore(outputScore, totalScore);
        return totalScore;
    }

//******
    double Hand::CountPoints(bool noPrint) {
        SortHand();
        long double totalScore = 0;
        int outputScore = 0;
        int iteration = 0;
        CardSuit csTemp;
        CardNum cnTemp;


        for (auto& iter : vCards) {
            //            13 ^ -x, where x is 13 - the value of the card + 1; This means ACE will return 1.0, whereas 2 will return 4E-14, etc
            //              MAKE A TABLE FOR THIS
            totalScore += double(1 / std::pow(13,(13 - (int(iter.GetNum()) + 1))));
        }

        int streak = 0;
        bool hasPair = false;
        bool hasTrips = false;
        bool hasQuads = false;
        bool twoPair = false;
        bool fullHouse = false;
        bool tripsFirst = false;
        std::vector<int> StreakedCards;
        for (auto& iter : vCards) {
            if (iteration > 0) {
                if (cnTemp == iter.GetNum()) {
                    if (hasPair && streak == 1) {
                        hasTrips = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else if (hasTrips && streak == 2) {
                        hasQuads = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else if (hasTrips && streak == 0) {
                        fullHouse = true;
                        tripsFirst = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        hasPair = true;
                        streak++;
                    }
                    else if (hasPair && streak == 0) {
                        twoPair = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else if (twoPair) {
                        hasTrips = true;
                        hasPair = true;
                        fullHouse = true;
                        twoPair = false;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                    else {
                        hasPair = true;
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        StreakedCards.push_back(int(iter.GetNum()) + 1);
                        streak++;
                    }
                }
                else {
                    streak = 0;
                }
            }
            cnTemp = iter.GetNum();
            iteration++;
        }
        unsigned long int temp = 0;
        if (hasPair) {
            if (hasTrips) {
                if (hasQuads) {
                    for (auto iter : StreakedCards) {
                        int holdCard = iter;
                        temp += (holdCard * 4000000);
                    }
                    outputScore = 800;
                }
                else if (fullHouse) {
                    if (tripsFirst) {
                        for (int iter = 0; iter < 3; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 300000);
                        }
                        for (int iter = 3; iter < 5; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 1000);
                        }
                    }
                    else {
                        for (int iter = 0; iter < 2; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 1000);
                        }
                        for (int iter = 2; iter < 5; iter++) {
                            int holdCard = StreakedCards.at(iter);
                            temp += (holdCard * 300000);
                        }
                    }
                    outputScore += 600;
                }
                else {
                    for (auto iter : StreakedCards) {
                        int holdCard = iter;
                        temp += (holdCard * 720);
                    }
                    outputScore += 300;
                }
            }
            else if (twoPair) {
                for (int iter = 0; iter < 2; iter++) {
                    int holdCard = StreakedCards.at(iter);
                    temp += (holdCard * 30);
                }
                for (int iter = 2; iter < 4; iter++) {
                    int holdCard = StreakedCards.at(iter);
                }
                outputScore += 200;
            }
            else {
                for (auto iter : StreakedCards) {
                    int holdCard = iter;
                    temp += (holdCard);
                }
                outputScore += 100;
            }
            totalScore += double(temp);
//          ShowScore(outputScore, totalScore);
            return totalScore;
        }

        bool isStraight = true;
        bool isFlush = true;
        iteration = 0;
        for (auto iter : vCards) {
            if (iteration > 0) {
                if (iter.GetSuit() != csTemp) {
                    isFlush = false;
                }
                if (iter.GetNum() != (cnTemp - CardNum(1))) {
                    isStraight = false;
                }
            }
            StreakedCards.push_back(int(iter.GetNum()) + 1);
            csTemp = iter.GetSuit();
            cnTemp = iter.GetNum();
            iteration++;
        }

        if (isStraight) {
            if (isFlush) {
                for (auto iter : StreakedCards) {
                    int holdCards = iter;
                    temp += (holdCards * 53000000);
                }
                outputScore += 400;
                if (temp > 2914000000) {
                    totalScore = 9999999999.0;
                    outputScore = 900;
                    ShowScore(outputScore, totalScore);
                    return totalScore;
                }
                outputScore += 500;

            }
            else {
                for (auto iter : StreakedCards) {
                    int holdCards = iter;
                    temp += (holdCards * 2400);
                }
                outputScore += 400;
            }
            totalScore += double(temp);
        }
        else if (isFlush) {
            for (auto iter : StreakedCards) {
                int holdCards = iter;
                temp += (holdCards * 7200);
                totalScore += double(temp);
            }
        }
//      ShowScore(outputScore, totalScore);
        return totalScore;
    }

//******
    int Hand::CardCount() {
        return vCards.size();
    }

//******
    void Hand::DiscardCards(Deck& dInput) {
        int discardCount = 0;
        int discardChoice = 0;
        int keepChoice = 0;
        bool hasAce = false;
        std::vector<Card> tempVect;
        Card* tempCard = nullptr;
        while (discardCount < 1 || discardCount > 5) {
            std::cout << "How many cards would you like to discard?\n\n";
            std::cin >> discardCount;
            if (discardCount == 0) {
                return;
            }
            else if (discardCount == 4) {
                for (auto i : vCards) {
                    if (i.GetNum() == ACE) {
                        hasAce = true;
                        tempVect.push_back(i);
                    }
                }
                if (!hasAce) {
                    discardCount = 0;
                    std::cout << "\nCannot discard 4 - no aces\n";
                }
            }
            else if (discardCount > 4 || discardCount < 1) {
                discardCount = 0;
                std::cout << "\nInvalid entry - enter a number between 1 and 3, or up to 4 if you have an ace\n";
            }
        }
        if (hasAce) {
            if (tempVect.size() > 1) {
                for (auto i : tempVect) {
                    PrintCard(i);
                }
                while (keepChoice < 1 || keepChoice >= tempVect.size()) {
                    std::cout << "\nChoose which ace keep\n\n";
                }
            }
            else {
                tempCard = &(tempVect.at(0));
            }

        }
        for (int iter = 0; iter < discardCount; iter++) {
            discardChoice = -1;
            bool cancelEarly = false;
            int handSize = vCards.size();
            while (discardChoice < 1 || discardChoice > handSize) {
                PrintHand();
                std::cout << "\nChoose card #" << iter + 1 << " to discard.\n"
                          << "Enter 0 to stop discarding cards\n\n";
                std::cin >> discardChoice;
                if (discardChoice == 0) {
                    discardCount = discardCount - (iter + 1);
                    cancelEarly = true;
                    break;
                }
                else if (discardChoice < 1 || discardChoice > handSize) {
                    discardChoice = -1;
                    std::cout << "\nInvalid entry - enter a number between 1 and " << vCards.size() << "\n";
                }
                else if (hasAce) {
                    if (*(tempCard) == vCards.at(discardChoice - 1)) {
                        std::cout << "\nCan't discard your shown ace when discarding 4 cards\n\n";
                        discardChoice = -1;
                    }
                }
            }
            if (cancelEarly) {
                break;
            }
            dInput.ToDiscard(vCards.at(discardChoice - 1));
            vCards.erase(vCards.begin() + (discardChoice - 1));
//          vCards.resize(discardChoice - 1);
            std::cout << "\n\n";
            discardChoice = -1;
        }
        DrawCard(discardCount, dInput);
        SortHand();
//        PrintHand();
    }

//******
    void Hand::DiscardHand(Deck& dInput) {
        for (auto i : vCards) {
            dInput.ToDiscard(i);
        }
        for (int iter = vCards.size(); iter > 0; iter--) {
            vCards.erase(vCards.begin() + (iter - 1));
//          vCards.resize(iter - 1);
        }
        vCards.clear();
    }

//******        //THIS FUNCTION HAS UNDEFINED BEHAVIOR!!!!!!!!!!!!!


    void Hand::SelectCards(std::vector<int>& selections) {              //Will pass back a vector of the cards the CPU would want to discard
        Card* prevCard = nullptr;                                       //The last card analyzed by the player - Free
        std::vector<Card*> goodCards;                                   //Cards that the player will keep - Free
        std::vector<Card*> badCards;                                    //Cards that the player will throw away - Free
        std::vector<Card*> straightCheck;                               //A copy of bad cards - this will be used for the player to determine if they're nearing a straight - Free
        std::vector<Card*> flushCheck;                                  //A copy of bad cards - this will be used for the player to determine if they're nearing a flush - Free
        bool streak = false;                                            //State check to see if the player is currently on a streak of like cards
        bool hasAce = false;                                            //State check to see if the player has an ace
        int nearStraight = 0;                                           //Checks to see how close the player is to having a straight
        int nearFlush = 0;                                              //Checks to see how close the player is to having a flush
        int discardCount = 0;                                           //The number of cards the player would discard in this instance


        std::vector<std::vector<Card*>> straightCards;                  //Free
        std::vector<std::vector<Card*>> flushCards;                     //Free

        for (int iter = 0; iter < vCards.size(); iter++) {
            //std::cout << "\n\nTest 01\n\n";
            Card* i = &vCards.at(iter);
            if (i->GetNum() == ACE) {
                hasAce = true;
            }
            if (prevCard == nullptr) {
                prevCard = i;
              //  std::cout << "\n\nTest 02\n\n";
            }
            else {
                //std::cout << "\ni    = " << i->GetNum()
                  //            << "\nprev = " << prevCard->GetNum()
                    //          << "\ni suit   = " << i->GetSuit()
                      //        << "\nprevSuit = " << prevCard->GetSuit()
                        //      << "\n\n\n";
                if (i->GetNum() == prevCard->GetNum()) {

                    if (streak) {
                        goodCards.push_back(i);
                    }
                    else {
                        streak = true;
                        goodCards.push_back(prevCard);
                        goodCards.push_back(i);
                    }
                //std::cout << "\n\nTest 03\n\n";
                }
                else {
                    if (!(streak)) {
                        badCards.push_back(prevCard);
                        straightCheck.push_back(prevCard);
                        flushCheck.push_back(prevCard);
                    }
                    else {
                        streak = false;
                    }
                    if (i->GetSuit() == vCards.back().GetSuit() && i->GetNum() == vCards.back().GetNum()) {
                        badCards.push_back(i);
                        straightCheck.push_back(i);
                        flushCheck.push_back(i);
                    }
                  //  std::cout << "\n\nTest 04\n\n";
                }
            }
            prevCard = i;
            i = nullptr;                                                                                    //i free'd appropriately
        }
        std::cout << "\nGood cards - " << goodCards.size()
                  << "\nBad cards  - " << badCards.size()
                  << "\n\n\n";
        //**

        //std::cout << "\n\nTest 05\n\n";
                                                                                                            //NEED TO REANLYZE ALL OF THIS - THE WRONG VALUES ARE BEING SELECTED WHEN SORTING THROUGH THE VECTORS
                                                                                                            //!!!!!!!!!!!!!!!!!!!!
                                                                                                            //!!!!!!!!!!!!!!!!!!!!
                                                                                                            //!!!!!!!!!!!!!!!!!!!!
        if (goodCards.size() == 0) {
          //  std::cout << "\n\nTest 06\n\n";
            while (straightCheck.size() > 0) {
            //    std::cout << "\n\nStraightCheck size = " << straightCheck.size() << "\n\n";
              //  std::cout << "\n\nTest 07\n\n";
                std::vector<int> tempNums;
                CardNum tempNum;
                for (int iter = 0; iter < straightCheck.size(); iter++) {
                //    std::cout << "\n\nTest 08\n\n";
                    if (iter == 0) {
                  //      std::cout << "\n\nTest 41, iter " << iter << "\n\n";
                        tempNum = straightCheck.at(iter)->GetNum();
                    //    std::cout << "\n\nTest 42, iter " << iter << "\n\n";
                        tempNums.push_back(iter);
                      //  std::cout << "\n\nTest 43, iter " << iter << "\n\n";
                    }
                    else {
                        //std::cout << "\n\nTest 09\n\n";
                        if (straightCheck.at(iter)->GetNum() < tempNum && straightCheck.at(iter)->GetNum() > tempNum - 5) {
                          //  std::cout << "\n\nTest 44, iter " << iter << "\n\n";
                            tempNums.push_back(iter);
                        }
                    }
                }
                std::vector<Card*> tempVect;
                for (auto i : tempNums) {
                    //std::cout << "\n\nTest 10\n\n";
                    tempVect.push_back(straightCheck.at(i));
                }
                int rangeCount = 0;
                for (auto i : tempNums) {
                    i -= rangeCount;
                   // std::cout << "\n\nTest 11\n\n";
                  //  std::cout << "i = " << i << "\n\n";                                     //NEED TO REANALYZE
                    //std::cout << "straightCheck = " << straightCheck.size() << "\n\n";
                    straightCheck.at(i) = nullptr;
                    straightCheck.erase(straightCheck.begin() - i);
                    rangeCount++;
                }
                //std::cout << "\n\nTest 40\n\n";
                straightCards.push_back(tempVect);
                tempVect.clear();
                //std::cout << "\n\nTest 12\n\n";
            }
            //std::cout << "\n\nTest 13\n\n";
            while (flushCheck.size() > 0) {
              //  std::cout << "\n\nTest 14\n\n";
                std::vector<int> tempNums;
                CardSuit tempSuit;
                for (int iter = 0; iter < flushCheck.size(); iter++) {
                //    std::cout << "\n\nTest 15\n\n";
                    if (iter == 0) {
                  //      std::cout << "\n\nTest 16\n\n";
                        tempSuit = flushCheck.at(iter)->GetSuit();
                        tempNums.push_back(iter);
                    }
                    else {
                    //    std::cout << "\n\nTest 17\n\n";
                        if (tempSuit == flushCheck.at(iter)->GetSuit()) {
                            tempNums.push_back(iter);
                        }
                    }
                }
                //std::cout << "\n\nTest 18\n\n";
                std::vector<Card*> tempVect;
                for (auto i : tempNums) {
                  //  std::cout << "\n\nTest 19\n\n";
                    tempVect.push_back(flushCheck.at(i));
                }
                int rangeCheck = 0;
                for (auto i : tempNums) {
                    i -= rangeCheck;
                    //std::cout << "\n\nTest 20\n\n";
                    flushCheck.at(i) = nullptr;
                    flushCheck.erase(flushCheck.begin() + i);
                    rangeCheck++;
                }
                flushCards.push_back(tempVect);
                //std::cout << "\n\nTest 21\n\n";
                for (auto i : tempVect) {
                    i = nullptr;
                }
                 //std::cout << "\n\nTest 22\n\n";
                tempVect.clear();                                                   //Tempvect free'd appropriately
                //std::cout << "\n\nTest 23\n\n";
            }
            int flushNum = 0;
            int straightNum = 0;
            for (auto i : flushCards) {
                if (i.size() > flushNum) {
                    flushNum = i.size();
                }
            }
             //std::cout << "\n\nTest 24\n\n";

            for (auto i : straightCards) {
                if (i.size() > straightNum) {
                    straightNum = i.size();
                }
            }
             //std::cout << "\n\nTest 25\n\n";

            int biggestNum = 0;
            int biggestVect = 0;
             //std::cout << "\n\nTest 26\n\n";
            badCards.clear();
             //std::cout << "\n\nTest 27\n\n";
            if (flushNum >= straightNum) {
                //std::cout << "\n\nTest 28\n\n";
                for (int iter = 0; iter < flushCards.size(); iter++) {
                    if (flushCards.at(iter).size() > biggestNum) {
                        biggestNum = flushCards.at(iter).size();
                        biggestVect = iter;
                    }
                }
                //std::cout << "\n\nTest 29\n\n";

                for (int iter = 0; iter < flushCards.size(); iter++) {
                    if (iter == biggestVect) {
                        for (auto i : flushCards.at(iter)) {
                            goodCards.push_back(i);
                        }
                    }
                    else {
                        for (auto i : flushCards.at(iter)) {
                            badCards.push_back(i);
                        }
                    }
                }
                //std::cout << "\n\nTest 30\n\n";
            }
            else {
                //std::cout << "\n\nTest 31\n\n";
                for (int iter = 0; iter < straightCards.size(); iter++) {
                    if (straightCards.at(iter).size() > biggestNum) {
                        biggestNum = straightCards.at(iter).size();
                        biggestVect = iter;
                    }
                }
                //std::cout << "\n\nTest 32\n\n";

                for (int iter = 0; iter < straightCards.size(); iter++) {
                    if (iter == biggestVect) {
                        for (auto i : straightCards.at(iter)) {
                            goodCards.push_back(i);
                        }
                    }
                    else {
                        for (auto i : straightCards.at(iter)) {
                            badCards.push_back(i);
                        }
                    }
                }
                //std::cout << "\n\nTest 33\n\n";
            }
        }

        //**
        //std::cout << "\n\nTest 34\n\n";
        if (badCards.size() >= 3) {
            if (hasAce) {
                discardCount = 4;
            }
            else {
                discardCount = 3;
            }
          //  std::cout << "\n\nTest 35\n\n";
        }
        else {
            discardCount = badCards.size();
            //std::cout << "\n\nTest 36\n\n";

        }

        for (int iter = 0; iter < vCards.size(); iter++) {
            for (auto k : badCards) {
                if ((k->GetNum() == vCards.at(iter).GetNum()) && (k->GetSuit() == vCards.at(iter).GetSuit())) {
                    selections.push_back(iter);
                }
            }
        }
        //std::cout << "\n\nTest 37\n\n";

        prevCard = nullptr;                         //PrevCard free'd appropriately
        for (auto i : badCards) {                   //BadCards free'd appropriately
            i = nullptr;
        }
        badCards.clear();
        for (auto i : goodCards) {                  //GoodCards free'd appropriately
            i = nullptr;
        }
        goodCards.clear();
        for (auto i : straightCheck) {              //StraightCheck free'd appropriately
            i = nullptr;
        }
        straightCheck.clear();
        for (auto i : flushCheck) {                 //FlushCheck free'd appropriately
            i = nullptr;
        }
        flushCheck.clear();
        for (auto i : flushCards) {                 //FlushCards free'd appropriately
            for (auto k : i) {
                k = nullptr;
            }
            i.clear();
        }
        flushCards.clear();
        for (auto i : straightCards) {              //StraightCards free'd appropriately
            for (auto k : i) {
                k = nullptr;
            }
            i.clear();
        }
    //std::cout << "\n\nTest 38\n\n";
    }

//******
    void Hand::CPUDiscard(Deck& dInput, std::vector<int>& selections) {               //NEED TO CHANGE THIS FUNCTIONS LOGIC TO ALLOW FOR PLAYER STATS TO COME INTO PLAY!!!!!!!
        std::vector<int> cardNums;
        std::cout << "\n\nselections = " << selections.size() << "\n\n";
        for (int iter = 0; iter < selections.size(); iter++) {          //MIGHT NOT BE NECESSARY!!
            std::cout << "\n\nIter = " << iter << "\n\n";               //!!!!!!!!!!!!!!
            if (cardNums.size() > 0) {                                  //!!!!!!!!!!!!!!
                /*for (auto i : cardNums) {                               //!!!!!!!!!!!!!!
                     for (auto &k : selections) {
                        std::cout << "\n\nPrev k = " << k << "\n\n";
                        if (k > i) {
                            k--;
                            std::cout << "\n\nPost k = " << k << "\n\n";
                        }
                    }
                }
                */

                selections.at(iter) -= iter;
            }
            std::cout << selections.at(iter);
            if (selections.at(iter) < vCards.size()) {
                dInput.ToDiscard(vCards.at(selections.at(iter)));
                vCards.erase(vCards.begin() + (selections.at(iter)));
                cardNums.push_back(selections.at(iter));
            } else {
                std::cout << "\n\nError - selection is " << selections.at(iter) << " whereas hand size is " << vCards.size() << "\n\n";
            }
        }

        DrawCard(selections.size(), dInput);
        SortHand();
    }
//******
    void Hand::ShowScore(int iOutput, double iTotal) const {
        switch (iOutput) {
            case 100 : std::cout << "\nOne pair!\n"; break;
            case 200 : std::cout << "\nTwo pair!\n"; break;
            case 300 : std::cout << "\nThree of a kind!\n"; break;
            case 400 : std::cout << "\nStraight!\n"; break;
            case 500 : std::cout << "\nFlush!\n"; break;
            case 600 : std::cout << "\nFull House!!\n"; break;
            case 800 : std::cout << "\nFour of a Kind!!\n"; break;
            case 900 : {
                if (iTotal == 9999999999.0)
                    std::cout << "\nHoly moly... ROYAL FLUSH!!!!!\n";
                else
                    std::cout << "\nStraight Flush!!!\n";
                break;
            }
            default : std::cout << "\nNo hand.\n";
        }
    }


//******
    Hand::~Hand() {

    }
