//***********************************************
//      NODE CLASS (Header/Implementation)      *
//                                              *
//***********************************************
//                                              *
//  This C++ class defines templated objects    *
//  that serve as an implementation of a data   *
//  structure node (specifically a circular     *
//  linked list)                                *
//                                              *
//***********************************************




#ifndef NODE_H
#define NODE_H

//************************************************

template <class Type>
class Node {
public:
    Node(Type tInput) {             //Constructor - Sets data to parameter
        data = tInput;                  //the actual data the node carry's
        next = nullptr;                 //next - the next item in the list
        prev = nullptr;                 //prev - the previous item in the list
    }

//******
    void SetNext(Node<Type>* nInput) {   //Sets the node's next pointer to the next node
        next = nInput;
    }

//******
    void SetPrev(Node<Type>* nInput) {   //Sets the node's prev pointer to the prev node
        prev = nInput;
    }

//******
    Node<Type>* GetPrev() {             //Gets the node's prev pointer
        return prev;
    }

//******
    Node<Type>* GetNext() {             //Gets the node's next pointer
        return next;
    }

//******
    Type GetData() {                    //Gets the actual data stored by the node
        return data;
    }

//******
    ~Node() {   }                       //Destructor - currently does nothing


private:
    Type data;                         //Holds data for the node (in this context, holds the player whose turn it is)
    Node<Type>* next;                   //Holds the next node in the list (in this context, the next player)
    Node<Type>* prev;                   //Holds the prev node in the list (in this context, the previous player)

};




#endif // NODE_H




