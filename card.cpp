//***********************************************
//      CARD CLASS (IMPLEMENTATION)             *
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


    #include "card.h"

//***********************************************

    Card::Card() { }                    //Default constructor - Does nothing as of yet

//******
    Card::Card(const Card& cOther) {    //Copy constructor - Creates a new object by copying the cOther

        SetComp(cOther.GetComp());
        SetSuit(int(cOther.GetSuit()));  //Set's the suit of the old card to the new card
        SetNum(int(cOther.GetNum()));    //Set's the rank of the old card to the new card
        SetState(cOther.GetState());     //Set's the state of the old card to the new card
    }

    //******
    Card::Card(int iSuitIn, int iNumIn) {   //Non-default constructor - Creates the card, uses the input parameter
                                        //...to generate suit and rank value.
        compMode = false;
        SetSuit(iSuitIn);               //Set's the card's suit to the desired suit
        SetNum(iNumIn);                 //Set's the card's rank to the desired rank
        state = IN_DECK;                //Set's the card's state to the desired state
    }

//******
    Card::Card(int iSuitIn, int iNumIn, bool compatiblityMode) {   //Non-default constructor - Creates the card, uses the input parameter
        compMode = compatiblityMode;    //Compability mode - used to change symbols if the computer cannot read unicode poker suits in text format
                                        //...to generate suit and rank value.
        SetSuit(iSuitIn);               //Set's the card's suit to the desired suit
        SetNum(iNumIn);                 //Set's the card's rank to the desired rank
        state = IN_DECK;                //Set's the card's state to the desired state
    }

//******
    CardSuit Card::GetSuit() const {             //Returns the card's suit to the calling function
        return suit;
    }
//******
    CardNum Card::GetNum() const {               //Returns the card's num to the calling function
        return num;
    }
//******
    std::string Card::GetSuitStr() const {       //Returns a str version of suit to calling function
        return suitStr;
    }
//******
    std::string Card::GetNumStr() const {        //Returns a str version of rank to the calling function
        return numStr;
    }
//******
    void Card::SetSuit(int iSuitIn) {         //Sets the suit to the input parameter
        suit = CardSuit(iSuitIn);
        suitStr = SUIT_STR[iSuitIn];
        if (compMode) {
            suitSym = SUIT_SYM_COMP[iSuitIn];
        } else {
            suitSym = SUIT_SYM[iSuitIn];
        }

    }
//******
    void Card::SetNum(int iNumIn) {           //Sets the rank to the input parameter
        num = CardNum(iNumIn);
        numStr = NUM_STR[iNumIn];
    }

//******
    void Card::SetState(CardState inState) {    //Sets the current card state
        state = inState;                            //****UNUSED****
    }

//****
    void Card::SetComp(bool compatibility) {
        compMode = compatibility;
    }

//*****
    bool Card::GetComp() const {
        return compMode;
    }

//******
    CardState Card::GetState() const {                //Gets the current card state
        return state;
    }

//******
    std::string Card::GetSuitSym() const {            //Gets the string of the suit from the card
        return suitSym;
    }

//******
    bool Card::operator==(const Card& cOther) const {   //Overload's the == operator for equivilency
        CardNum cnTemp = cOther.GetNum();
        CardSuit csTemp = cOther.GetSuit();
        if (suit == csTemp) {                   //If the rank and suit are equal from the currCard and the comparedCard...
            if (num == cnTemp) {                    //...true is returned.
                return true;                    //If not....
            }                                       //...false is returned
        }
        return false;
    }

/*
    CardSuit suit;
    CardNum num;
    std::string suitStr;
    std::string cardStr;
    CardState state;
*/
