# primitive-poker
A simple text-based poker game WIP.

**Version 0.5.2.1**
- Fixed the unorthodox behavior by simplifying the discard function for CPU's by which is selects the cards that it would like to discard.  This causes the known unorthodox behavior to cease.

**Version 0.5.2**
- Fully defined the selection and discard functions for the CPU
   - There is unorthodox behavior where if the first two cards are to be discarded as well as the last card, sometimes unintended cards are selected for discard.  I don't know exactly why this is the case, but it doesn't cause that many problems with gameplay, and if need be is servicable although not optimal.  Will investigate further during the MVP phase of release.

**Version 0.5.1**
- Have begun to test and refine the functionality of the CPU's card selection and discard functions
   - This has intermittent memory errors when discarding and drawing new cards, and mysterious selection errors when picking cards to drop
      - Sometimes the CPU will discard an obvious pair - there are instances when the CPU could have had a 3 of a kind, but failed because it discarded a card that didn't make sense
      - There are instances where the CPU will choose the wrong color card to discard for a flush; it should select the suit of the highest value card first and foremost in the instance of a tie
      - I can imagine the above error about flushes will also apply to straight selection as well
   - Despite bugs and errors revealing themselves, progress has been made and general, rough skeleton of the subroutine is revealing itself

**Version 0.5.0**
- Begun the process of implementing and refining all CPU algorithms, functions, and methods.
   - JudgeHand() and GiveTell() seem to be working correctly.
   - In the process of implementing the DiscardCard() function.  There seems to be issues with determining the best cards to drop to shoot for a flush or a straight; an invalid pointer, sigabort, and etc. misc errors with memory are occuring.  Need to investigate further as more refining and testing is done.
      - This involved the arduous process of ironing out an error with a pointer to an iterator - the pointer always pointed to where the iterator was pointing, resulting in issues.  Range-based for-loops don't appear to be the play when it comes to functions that take a previous card and compare it to a current card - could possibly be a hint for further errors and bugs
   - Have not yet attempted to test and/or implement the CPU's betting and calling/raising function

============


**Version 0.4.0**
- Defined an (untested) method for which the CPU player can determine which cards are "good" and which cards are "bad" and should be discarded.
   - The CPU will prioritize "low-hanging fruit."  If it finds any matching cards, it will try and go for pairs, three-of-a-kind, full houses, and etc.
   - If it cannot find any matching cards, it will go for flushes, as they are statistically more likely than straights - it will see how many cards of matching suits it has, and then see how many cards "within range" of a straight it has.  If among the candidate sets of cards (using a vector of card vectors) it finds, it will pick the largest, unless there is a tie between a flush and a straight, in which it will pick the flush - as it is statistically a better bet (and statistically, the odds of winning with a flush are much higher than the odds of them beating your flush with a straight and beyond
   - If a flush ties a flush or a straight ties a straight, the first one it finds will be selected.  This is because due to the nature of the hands already being pre-sorted, the first sets of values in the vector of vectors will always contain the more "valuable" pair - the first cards in the vector are the highest ranked (Aces will always be first, two's will always be last)
   - It will then go throught the actual card vector for the players hand and match the actual card position in their hand with the card suit and value in the "bad cards" vector.  When it hits, it knows that this particular position is where that card is in the hand - it stores that number in a vector of ints, which is passed back to the calling function to be used to discard the particular bad cards from the CPU player's hand.
- Added and defined a subroutine by which the CPU can discard undesirable cards.
   - As it stands, this is a straightforward task that assumes that all CPU's will pick the most obvious option.  I am not sure if this will change to give the CPU more decision-making capability, as "poor" cards are pretty straight forward, and I don't want to overcomplicate CPU decision making.  I think, instead, a stat-based dice check will determine if the CPU will decide to discard cards at all, and another stat-based dice roll will determine how "strict" the CPU is; if they will be willing to keep their cards on a more risky hand.
- Further defined the CPU's betting algorithm and begun to define means for it to communicate with the player and the player's ability to communicate with the CPU in terms of betting and bluffing.
- Added functionality for CPU to gather information from other players based on stats, and to use this information to increase and decrease the likeliness that they will make a certain decision - such as betting, folding, and how much to bet by.
   - Very much a WIP - Need to add functionality for generating an "opinion" based on player tells before this will be functional
   - Also need to generate a similar function for deciding on raises and calls - the math for determining how "worth it" a call is vs folding as well as based on how much money they've already put into the pot need to be formulated
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
