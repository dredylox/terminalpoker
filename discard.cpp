//***********************************************
//      DISCARD CLASS  - IMPLEMENTATION         *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a stack structure  *
//  of card objects.  This is the place where   *
//  cards will go after a discard from drawing  *
//  or after a round of hands                   *
//                                              *
//***********************************************
//  Version 0.0.4.1 pushed by David Coleman III *
//  on  01 - 24 - 23                            *
//***********************************************

#include "discard.h"

//***********************************************

    Discard::Discard() {   }        //Default constructor - Generates a new empty stack to receive cards

//******
    Card Discard::GetTop() {         //Peeks a card from the top of the pile and distributes it to the deck
        return stPile.top();
    }
//******
    void Discard::Push(Card cInput) {   //Pushes a new card to the top of the stack
        stPile.push(cInput);
    }
//******
    void Discard::Pop() {            //Removes the card from the top of the stack
        stPile.pop();
    }
//******
    int Discard::GetSize() {         //Returns the current size of the discard pile to the calling function
        return stPile.size();
    }
//******
//    Discard::~Discard() { }          //Destructor - currently does nothing

/*
private:
    std::stack<Card> stPile;
};
*/
