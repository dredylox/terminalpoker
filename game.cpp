//***********************************************
//      GAME CLASS                              *
//                                              *
//***********************************************
//                                              *
//  This C++ class is the main source of the    *
//  poker game.  It serves as the "playspace"   *
//  for the game.                               *
//                                              *
//***********************************************
//  Version 0.0.4.1 pushed by David Coleman III *
//  on  01 - 24 - 23                            *
//***********************************************
/*

    Game() {                       //Default constructor - creates the decks, players, and rivers
        Player* pTemp;
        dDeck = new Deck;
        lPlayers = new CircularList<Player>;
        for (int iter = 0; iter < PLAYER_COUNT; iter++) {
            pTemp = new Player;
        }
    }

//******
    void PlayGame(){               //Starts the gameplay loop

    }

//******
    ~Game() {                      //Deallocates the deck, players, and river pointers

    }
/*
private:
    Deck* dDeck;                    //The deck being used for the current game
    River* dRiver;                  //The river being used for the current game
    CircularList<Player>* lPlayers;         //The list of players, sorted by turn order

};
*/
