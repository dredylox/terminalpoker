




#include <iostream>
#include "game.h"


int main() {

    Game myGame;                        //Game object
    std::string compatibility = u8"♣♦♥♠";

    if (compatibility.length() == 12) {  //Plays the game until the player quits, the player wins, or loses - if the computer cannot read the compatibility string, it's ran in a different compatibliity mode
        myGame.FiveCardDraw(false);
    } else {
        myGame.FiveCardDraw(true);
    }
    std::cout << "\nGoodbye!!\n";       //Wishes the player farewell after a game ends
    return 0;                           //End of program; succeess termination
}

