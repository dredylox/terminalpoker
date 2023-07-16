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




#ifndef CIRCULARLIST_H
#define CIRCULARLIST_H

#include <iostream>

#include "node.h"

//************************************************

const int PLAYER_COUNT = 4;         //Total number of players

//************************************************

template <class Type>               //This class is a circular linked list data structure
class CircularList {
public:
    CircularList(Type& tInput) {        //Constructor - Generates a circular list with one node
        head = new Node<Type>(tInput);      //Generates the list with a new node
        head->SetNext(head);                //Sets next to head
        head->SetPrev(head);                //Sets prev to head
        curr = head;                        //Sets everything to head
        next = head;                        //      /
        prev = head;                        //     /
        last = head;                        //  <-/
        iLength = 1;                        //Initializes the list length at 1
    }

//******
    void NewNode(Type tInput) {                             //Adds a new node to the list and sets it as last
        if(last->GetNext() == head) {                       //...links it to the previous last node and head
            curr = last;                                        //curr is last
            next = new Node<Type>(tInput);                      //next is a new node
            curr->SetNext(next);                                //curr's next is next
            next->SetPrev(curr);                                //next's prev is curr
            next->SetNext(head);                                //next's next is head
            head->SetPrev(next);                                //head's prev is next
            last = next;                                        //last is next
            curr = last;                                        //curr is last
            iLength++;                                          //increment length by 1
        }
        else {                                              //If last is not set properly, the list is traversed and
            curr = head;                                    //...last is then set properly
            while (curr->GetNext() != head)                     //Keeps traversing until next is head
                curr = curr->GetNext();
            last = curr;                                        //last is curr
            next = new Node<Type>(tInput);                      //next is a new node
            curr->SetNext(next);                                //curr's next is next
            next->SetPrev(curr);                                //next's prev is curr
            next->SetNext(head);                                //next's next is head
            head->SetPrev(next);                                //head's prev is next
            last = next;                                        //last is next
            curr = last;                                        //curr is last
            iLength++;                                          //length is incremented by 1
        }
    }
//******
    void TraverseStart() {                              //Primes for traversal - sets curr to head to begin traversing the list
        curr = head;                                    //...also sets prev to last and next to head + 1 in the list
        prev = last;                                    //sets curr to head, prev to last, and next to head's next
        next = curr->GetNext();
    }

//******
    void TraverseNext() {                               //Traverses the list by 1 node. Also traverses prev and next by 1
        curr = curr->GetNext();                             //curr to next
        prev = curr->GetPrev();                             //prev to new curr's prev
        next = curr->GetNext();                             //next to new curr's next
    }

//******
    int CurrPos() {                                     //Returns curr's pos to the calling function
        int pos = 0;                                        //pos variable
        Node<Type>* temp = head;                            //temp ptr set to head
        while (temp != curr) {                              //while temp isn't curr...
            temp = temp->GetNext();                             //temp is set to next
            pos++;                                              //pos is incremented
        }
        return pos;                                         //pos is returned
    }

//******
    Type GetPlayer() {                                  //Returns the current player to the calling function;
        return curr->GetData();
    }

//******
    void RemovePlayer(int playerNum) {                  //Removes a player from the circular list
        TraverseStart();                                    //Begins the traversal
        int tempNum = 1;                                    //temporary pos
        while (tempNum < playerNum) {                       //while temp is less than param
            TraverseNext();                                     //traverse and increment temp
            tempNum++;
        }
        prev->SetNext(next);                                //sets prev's next to next
        next->SetPrev(prev);                                //sets next's prev to prev
        delete curr;                                        //deletes curr
        curr = next;                                        //sets curr to next
        next = curr->GetNext();                             //sets next to curr's next
    }

//******
    Node<Type>* GetHead() {                             //Returns head to calling function
        return head;
    }

//******
    Node<Type>* GetLast() {                             //Returns last to calling function
        return last;
    }

//******
    Node<Type>* GetCurr() {                             //returns curr to calling function
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
    ~CircularList() {                                   //Destructor free's and destroys the list
        next = last;                                        //next is last
        head->SetPrev(nullptr);                             //head's prev is null
        while (curr != nullptr) {                           //while curr isn't null...
            curr = last->GetPrev();                             //curr is last's prev
            delete next;                                        //deallocate next
            next = curr;                                        //next is curr
            last = next;                                        //last is next
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


