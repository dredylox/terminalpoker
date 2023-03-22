//***********************************************
//      DISCARD CLASS                           *
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

#include "card.h"
#include <stack>

//***********************************************

#ifndef DISCARD_H
#define DISCARD_H

//***********************************************

class Discard {
public:
    Discard();                      //Default constructor - Generates a new empty stack to receive cards

//******
    Card GetTop();                  //Peeks a card from the top of the pile and distributes it to the deck

//******
    void Push(Card);                //Pushes a new card to the top of the stack

//******
    void Pop();                     //Removes the card from the top of the stack

//******
    int GetSize();                  //Returns the current size of the discard pile to the calling function

//******
  //  ~Discard();                        //Destructor - currently does nothing


private:
    std::stack<Card> stPile;
};

#endif // DISCARD_H




