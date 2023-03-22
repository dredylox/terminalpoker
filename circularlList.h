//***********************************************
//      CIRCULAR LIST CLASS (Header/Implement.  *
//                                              *
//***********************************************
//                                              *
//  This C++ class defines templated objects    *
//  that serve as a circular implementation of  *
//  a linked list data structure.               *
//                                              *
//***********************************************
//  Version 0.0.4.1 pushed by David Coleman III *
//  on  01 - 24 - 23                            *
//***********************************************



#ifndef CIRCULARLIST_H
#define CIRCULARLIST_H

#include "node.h"

//************************************************

const int PLAYER_COUNT = 4;

//************************************************

template <class Type>
class CircularList {
public:
    CircularList(Type& tInput) {                             //Constructor - Generates a circular list with one node
        head = new Node<Type>(tInput);
        head->SetNext(head);
        head->SetPrev(head);
        curr = head;
        next = head;
        prev = head;
        last = head;
        iLength = 1;
    }

//******
    void NewNode(Type tInput) {                             //Adds a new node to the list and sets it as last
        if(last->GetNext() == head) {                       //...links it to the previous last node and head
            curr = last;
            next = new Node<Type>(tInput);
            curr->SetNext(next);
            next->SetPrev(curr);
            next->SetNext(head);
            head->SetPrev(next);
            last = next;
            curr = last;
            iLength++;
        }
        else {                                              //If last is not set properly, the list is traversed and
            curr = head;                                    //...last is then set properly
            while (curr->GetNext() != head)
                curr = curr->GetNext();
            last = curr;
            next = new Node<Type>(tInput);
            curr->SetNext(next);
            next->SetPrev(curr);
            next->SetNext(head);
            head->SetPrev(next);
            last = next;
            curr = last;
            iLength++;
        }
    }
//******
    void TraverseStart() {                              //Primes for traversal - sets curr to head to begin traversing the list
        curr = head;                                    //...also sets prev to last and next to head + 1 in the list
        prev = last;
        next = curr->GetNext();
    }

//******
    void TraverseNext() {                               //Traverses the list by 1 node. Also traverses prev and next by 1
        curr = curr->GetNext();
        prev = curr->GetPrev();
        next = curr->GetNext();
    }

//******
    Type GetPlayer() {                                  //Returns the current player to the calling function;
        return curr->GetData();
    }

//******
    void RemovePlayer(int playerNum) {                  //Removes a player from the circular list
        TraverseStart();
        int tempNum = 1;
        while (tempNum < playerNum) {
            TraverseNext();
            tempNum++;
        }
        prev->SetNext(next);
        next->SetPrev(prev);
        delete curr;
        curr = next;
        next = curr->GetNext();
    }

//******
    Node<Type>* GetHead() {
        return head;
    }

//******
    Node<Type>* GetLast() {
        return last;
    }

//******
    Node<Type>* GetCurr() {
        return curr;
    }

//******
    bool TraverseEnd() {                                //Checks to see if the last player has taken their turn
        if (curr->GetNext() == head)                        //If yes, then returns true to the calling function
            return true;
        else                                                //If no, then returns false to the calling function
            return false;
    }

//******
    ~CircularList() {
        next = last;
        head->SetPrev(nullptr);
        while (curr != nullptr) {
            curr = last->GetPrev();
            delete next;
            next = curr;
            last = next;
        }
    }

private:
    Node<Type>* head;               //Head pointer - holds the list's structure
    Node<Type>* curr;               //Curr pointer - serves as the iterator for the list
    Node<Type>* next;               //Next pointer - serves as a means for curr to iterate upon the list
    Node<Type>* prev;               //Prev pointer - currently does nothing, but useful as a reverse-iterator for curr
    Node<Type>* last;               //Last pointer - holds the final value of the list. Useful for allowing the list to function as a
                                    //...linear structure while retaining the capabilities of a circular structure
    int iLength;                    //Legnth - holds the size of the list.  Currently no functionality

};



#endif // CIRCULARLIST_H


