//***********************************************
//      TEST CLASS                              *
//                                              *
//***********************************************
//                                              *
//  This C++ class serves as a primitive mockup *
//  of a class that specifices objects designed *
//  to be used when testing code.  It allows    *
//  for a number to be iterated to represent    *
//  which "test" the developer is on, allowing  *
//  them to place markers to represent where    *
//  their code was just before failure.         *
//                                              *
//  Uses valued inputs, allowing manual inputs, *
//  complex functional capability, (allowing the*
//  result of arithmetic operations be the test *
//  iteration for example) for loop inputs,     *
//  (allowing the user to see which iteration   *
//  of the loop fail  of the loop failed) and   *
//  more.                                       *
//                                              *
//***********************************************
//                                              *
//  Version 0.1 pushed by David Coleman III on  *
//      01 - 18 - 23                            *
//                                              *
//  0.1 added Features:                         *
//  - Input function added - allows user to     *
//     use an integer as an argument for their  *
//     current test iteration                   *
//                                              *
//  - Print function added - allows user to     *
//     print the integer member variable that   *
//     stands for the current test iteration    *
//                                              *
//  - InputPrint function added - performs the  *
//     task of both Input and Print to save the *
//     client time and screen real estate       *
//                                              *
//***********************************************
//                                              *
//***********************************************
//                                              *
//  Version 0.1 pushed by David Coleman III on  *
//      01 - 18 - 23                            *
//***********************************************

#ifndef TEST_H
#define TEST_H

#include <iostream>

//************************************************

class Test {
public:
    Test() {                        //Default constructor - Sets the member variable to 0

        iTestIteration = 0;
    }

//******
    Test(int iInput) {

        iTestIteration = iInput;    //Constructor - Sets the member variable to the input
                                    //...parameter
    }

//******
    Test(const Test& oInput) {                  //Copy constructor - Uses overloaded = operator
        iTestIteration = oInput.Output();       //...to assign the parameter member to new obj
                                                //...member
    }

//******
    void operator=(const Test& oInput) {    //= overloading - allows direct assignemnt of
        iTestIteration = oInput.Output();   //...member variables with just artihmatic on
    }                                       //...object

//******
    int operator+(const Test& oInput) {    //Same as above, but adds the two and returns sum
        iTestIteration += oInput.Output();
        return iTestIteration;
    }

//******
    int operator-(const Test& oInput) {    //Same as above, but subtracts the two and
        iTestIteration -= oInput.Output();  //... returns the difference
        return iTestIteration;
    }

//******
    void InputPrint(int iInput) {   //Takes a value, uses it for the Input function, and
        Input(iInput);              //...prints it with the Print function;
        Print();
    }

//******
    void Print() {                  //Prints the value in iTestIteration to the screen

        std::cout << "\n\nTest - " << iTestIteration << "\n\n";
    }

//******
    void Input(int iInput) {        //Takes an int as a parameter and stores it
        iTestIteration = iInput;     //...in member iTestIteration
    }

//*****
    int Output() const {
        return iTestIteration;
    }

//******
    ~Test() {                       //Destructor (does nothing atm)

    }
    
//************************************************

private:
    int iTestIteration;   //The iteration that the test is on

};

#endif //TEST_H