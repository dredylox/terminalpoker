# primitive-poker
A simple text-based poker game WIP.

**Version 0.4.0**
- Begun to define the CPU's betting functionality using a convergent decision-tree algorithm that will allow the CPU to use its perception, bluff, and aggressiveness stats to evaluate other players' tells and bluffs, its own hand value, the likeliness it thinks it has to win a hand, how much it's willing to bet on a good or bad hand, and how likely it is to fold given the circumstances

============

**Version 0.3.0**
- Small update to function that prints a player's hand when they have folded - older version had incorrect player numbers printing.  Fixed now.
- Fixed the mysterious 4th player calling bug by fixing a for loop that assigns point values to a player based on their hand contents
- Fixed a bug where a player's points persisted even after folding
- Betting using the player functionality and subroutines for CPU players is fully functional now - CPU specific functionality needs to be implemented
- Now allows the initial bet to be a fold and to pass the leading bet to the next player if desired
   - This has a big bug where calling can pose a problem on the 4th player.  I don't know why this is currently and it needs to be investigated.
- Adjusted player and CPU betting so that a full round of betting can be completed FOR all players, BY the user player
   - The same needs laid out in 0.3.1.3 still need further implementation
- Reached a state where betting is fully integreated with the rest of the code - it is now possible for the player to bet mid-game
- Fixed arithmetic on player betting - need to adjust this for player and CPU however
   - Need to develop a "dumb" CPU player, so that the player isn't the one betting on behalf of the CPU's
   - Need a method so that the player can see who is betting at a particular moment
- Created quick and rough prototypes of the betting, calling, folding, and raising subroutines
   - The CPU versions of these functions all use the normal player versions for now.  This will be improved in time
- Implemented the betting of player cash in the main function.
   - This is currently rough and does not allow all players to make a bet as it should.
- Begun to define the differences between a user player and a CPU player
- Added stats to the CPU that will be a part of the CPU decision making engine
   - This will determine whether the CPU will be more likely to draw risky cards, take risky bets, raise, call, and/or fold
- Added primitive version of "game" routines in main
   - These need to be extracted into functions and optimized
   - Use undefined function prototypes for players that need to be declared and defined
- Started the process of adding betting to the game
   - This feature will continue to be a primitive iteration until an "AI" is added for the computer players
- Added a functional discard pile class and bound it to the deck class
- Made it so decks shuffle back from the discard pile if necessary (this will be a modular feature in the future)
- Added discard functionality
- Added game loops; allow the player to make decisions in games and choose to continue playing or to quit.

============

***Version 0.2***

- Added the <math.h> library to allow for exponent arithmetic
- Changed the scoring system to use doubles and an accelerated multiplier; makes sure that the correct victor is always chosen when a hand is won
   - This needs some refining - currently direct values are used instead of a table of const's.  Some functions can be extrapolated from the lines of code that perform these operations as well
- Added "winner" selection - each hand will pass its score total to a vector.  This vector is traversed and the "maximum" value is selected, determining the winner by choosing the player with the highest score.
   - Have not implemented a solution for ties yet
   - Not sure if when implemented that tied games will work with doubles, due to floating errors.  May need to do some testing and add a "round to the nearest decimal" function for proper functionality
- Merged the deckAndHandTest branch with main
- Implemented the circularlList class and the Player class to allow for mulitple players to draw hands of cards from the same deck, printing all hands in order with their score values
- Fixed logic issues with this implementation in order to print all hands without error

============

***Version 0.1.0***
- Created a graphical display of a player’s hand when showing hand
- Added sorting to the hand as well as point counting
- Created a separate point counter that allows the program to output any “winning” hands they have 
   - This includes pair, 2 pair, 3 of a kind, 4 of a kind, full house, flush, straight, straight flush, and royal flush
- Fully defined and implemented shuffle function and hand print function. Deck can be created, shuffled, and deal a full hand of cards to a player, whose hand can then be displayed on the screen correctly 
   - This was fixed by using heap allocation of the vector vTemp instead of stack allocation, which originally caused undefined behavior 
- Began to perform isolated tests on hand and deck. 
- While dealing cards to a hand and displaying the hand works, shuffling the deck currently has bugs 
- Defined and implemented a sorting algorithm for sorting hands 
- Added files for the discard and pot class 
- Began to implement functionality for sorting player hands to estimate the value of the player’s hand 
- Began to layout the structure of the Game class, which connects the bulk of classes together in one single “playspace” for the gameplay 
- Began to define and implement various functionality of the Hand class and its interactions with Deck and Card classes 
- Fully defined and implemented the Card class with all basic features, including a copy constructor 
- Began to define and add features to the Deck class, including shuffling and dealing 
- Tweaked errors and syntax issues with cod
