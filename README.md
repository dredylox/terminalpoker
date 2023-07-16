# primitive-poker

Primitive Poker is a simple command-line-based poker game.  1 player can play with 3 different CPU's - each with a randomly-generated "personality" - and play Five-Card draw.  Will you be the last player standing?  Or will you walk home with your tail between your legs?

---

****Specifications****

Primitive Poker is a game that utilizes object-oriented programming in a pure C++ environment.  It uses inheritance and a small amount of polymorphism both for child class function overriding, and for overloading functions that operate similarly but with minor changes. (usually printing vs non-printing iterations of the same function)  It consists of 9 classes created in development and a plethora of standard C++ libaraies.  It also includes a developer-defined templated node class and circular linked list class that can be (with a small amount of modification) repurposed for use in other projects

========

***C++ libraries and technologies used***
- Iostream, Iomanip, and String
   - All are used for output/input and for direct communication between the player and the game

- Math.h
   - This library is used for the floor() and a pow() function
      - floor() is used for rounding down values so absurd parenthesis aren't unrealistically used by CPU players when betting cash
      - pow() is used in all of the scoring functions - when grading a hand's raw card value, each card is graded by 1 to the power (pow()) of -13 minus the card value.  This seems convoluted, but is done to "spread" values among the decimal instead of just whole numbers, as with hand scoring on top of raw card value scoring, using just whole numbers would quickly reach the limit of a float if not a double.  It's more efficient to use the inverse power to scale each card's value "backwards" before evaluating hand strength

- Vector
   - This templated data structure library is used in a lot of places.  It is used to build the playerbase at the table before converting it to a circular linked list, hold player scores at the table, player opinions, player hands in the hand class, store bluff-checks and opinions of other players for each player themselves, and a two-dimensional vector is used to serve as the table's pot - with the inner vectors serving as each player's contribution to a tier in the pot, and the outer vector serving as each "tier" in the pot (for side-betting, folding, etc.)

- Stack
   - This templated data structure library is used in the discard pile and the deck.  It is used as the actual "data" portion of the deck and discard class, and temp stacks are created to hold card positions when the deck is "shuffled."

- Random
   - This library is used for the MT19937 randomization algorithm, the random_device data type, and the uniform_int_distribution object class.  It is used to generate random numbers which are used for CPU decision making and shuffling the deck randomly.  This allows for variance in game and makes gameplay more realistic.

- Limits
  - This library was used to evaluate the maximum size of integers, floats, and doubles for determining problems with the hand-scoring functionality

========

***Developer-defined classes used***
- Card
   - The Card class serves as the primary "playing piece" for the game.  It is an object that contains variables of both the enumerated types "suit" (the cards suit) and "num" (the card's rank from 2 through Ace) as well as string versions of these enumerated types pre-converted. It also has a compatibility mode boolean that is used when the computer cannot use actual suit symbols and must instead resort to using the first letter for each suit (S-pade, C-lub, D-iamond, H-eart)
   - Cards can have their values evaluated as a single card by their num, or in tandem with other cards using suit and num.  They have copy constructors for ease of creating temp stacks for reshuffling decks of cards.  All in all, they are primarily data-oriented, only with functions that make it easier to read said card's data

- Hand
   - The Hand class serves as each player's set of these card playing pieces.  It is an object that is both function and variable oriented.  Its only variable is a vector of cards that serves as the actual physical hand of cards each player has
   - It holds functions to print the player's hand, a set of cards in that hand, a single card, sorting the hand vector, (allowing more accurate mathematical evaluations of the hand) discarding and drawing, and evaluating points earned from this hand
   - It also has a function titled CPUDiscard which serves as a faux redefinition of the discard function, only used in the context of a CPU player

- Discard
   - The Discard class serves as the game table's discard pile.  It is an object that is primarily used to hold cards that are neither in the deck nor in a player's hand, as well as a location that cards go when a player folds or where cards are stored until the deck is reshuffled after a game
   - This object behaves almost exactly as a stack - of which its main data structure for holding cards is.  All functions are an extension of the stack data structure, and simply refer to those function calls when invoked, allowing peeking, popping, pushing, and size returning.

- Deck
   - The Deck class serves as the game table's source of game pieces. (cards)  It has a stack data structure and a discard pile object (which is practically another stack) as variables.
   - The Deck has the ability to perform the same operations as any stack, but also has the ability to shuffle itself, handle all function calls and data movement from the discard pile,
   - Comes with several alternative constructors that are used for testing purposes

- Player
   - The Player class serves as the game's players; both the user and the CPU (although the CPU inherit these traits in their own class; CPU)
   - Players have a hand of cards, (hand object) a float to represent their cash, a bool to determine if they have folded or not, ints for their current wager and their current pot tier, (for side-betting) a perception, aggressiveness, and bluff stat, (these only really have an effect on the CPU - the user player stats are always set to 5 out of 10) and a vector that holds the player's opinions on the players (how "good" they think each player's hand is, including themselves.)
   - As the primary object used by the players of the game, these objects are highly functional; they have methods for betting, calling, folding, raising, checking player cash, returning the player hand, getting a new hand, discarding hands, assigning player opininos, manipulating the player's pot and how much money they put into the pot, and every other method that would be required for a game of poker to flow correctly

- CPU
   - The CPU class is a child class of the Player class.  The CPU inherits all of the traits of the Player, but has a few new methods and a few overloaded methods
      - Each stat is randomly generated on a scale of 1 to 10 using the child constructor.  This allows each player to behave differently; some CPU's being more aggressive, some bluffing more, some being more perceptive, some being great at everything, and etc.
      - GetTell has two separate definitions, allowing one to be a bluff and one to be a tell depending on how well the player bluffs that hand
      - DiscardCards uses a complex decision tree, allowing the CPU to make a well informed decision about which cards staistically are the best to discard for their current hand
      - BetCash and CallCash also use a complex decision tree, as well as several random numbers as "dice rolls" in combination with their stats and hand strength to decide if they will bet, call, fold, or raise and how much money of each
      - PlayerPerception uses a score-based system based on the hands score to return their opinion to the calling function - also has an iteration of this method that prints this as a "tell" to the table

- Node
   - The Node class is a templated, developer-defined linked list node object.  It consists of a variable of data of whatever the client specifies, a pointer to the next node, and a pointer to the previous node
   - As a linked list node, it has all the required functionality - methods include setting next, setting previous, getting next, getting previous, and getting data.

- CircularList
   - The CircularList class is a templated, developer-defined circular linked list.  It uses the previously-discussed node class and the type templated with it for its own template, and uses that to create a doubly linked list whose head points backwards to the end, and visa-versa
   - It has all of the necessary methods of a linked list, including making a new list, adding nodes, traversal, removing nodes, and data retreival of current.
   - Variables include pointers to the head, current, next, previous, and additionally a pointer to the last node in the list - which is the previous of the head. It also has an integer of its length and a method to determine that length via traversal and iteration

- Game
   - The Game class serves as the overall game table, and contains all other objects in the game
   - While it has no private variables itself, the game function uses a boolean to loop and keep the game going.  It then creates a deck, a vector of players, (also dealing them hands of cards each) player perceptions, and turns this vector of players into a circular list of players, which are then used for gameplay purposes
   - The game loop consists of resetting the pot, creating a new 2D vector for the multi-pot/tiered pot system, getting an ante from all players, (and using the antes to see if the user has won or lost the game) printing the whole pot, getting tells from each player, printing the user's hand for them to see, allowing the players to discard cards, setting the current bet "leader" to set the bet this hand, taking the bet, getting calls/raises/folds from the rest of the table, incrementing the bet leader for the next round, printing all player hands, evaluating who won (or tied) the hand, printing everyone's current cash after a winner has been determined and paid out, and offering the user to play again if they'd like to.
   - The gameplay loop (and thus the primary gameplay function) will repeat until the user is done playing, they lose by not having enough to ante, or they win by causing all CPU's to be unable to ante

---

****Lessons Learned****

- In creating this game, I learned that while it's easy to overlook the importance of documentation, commenting on your code and laying out how each piece works is critical, as it's easy for other developers to not understand what your code does, as well as suprisingly easy to lose track of what your own code does when features begin to balloon.

- Using a notebook or other software for writing and journalling progress and ideas is great.  Being able to draw out a problem and really express what that problem is in words, drawings, and diagrams helps a lot.  Trying to "just conceptualize it in your head" is very inefficient and makes it very hard to remember the flow of the solution; like not commenting on code

- In spite of functionality, working code that acts as expected and "realistic performance" that meets the expectations of the real-world concept are very different beasts.  This game is fully functional, but it needs improvement with the statistical likeliness of the CPU to perform and behave like a poker player in real life.  This will require iteration over a long period of time; it will require an observation phase, where data is observed from real poker games over X amount of games. Then it will require an implentation and testing phase - where stats and the decision tree that use them are changed to account for these real life statistical ratio.  This, in turn, will make the CPU performance and gameplay much more "realistic," in the sense that the CPU players will behave much more like a human being instead of an algorithm; at least it will APPEAR as such to the people playing the game

- Understanding mathematics is crucial for a proper scoring system in a game like this.  In order to make sure scoring works correctly, no weaker hand of the highest value cards should beat a better hand of the lowest value cards; a pair of aces should never beat a two pair of two 2's and two 3's, in spite of an Ace being significantly superior in ranking. This is accomplished with some research of values that will ALWAYS multiply the values by more than is necessary to close the game.  This is why the constants for "modifiers" are seemingly arbitrarily exponentially larger as they increase in value - to prevent any hand scoring overlap.
   - Likewise, you have to account for memory usage as well.  Using whole ints for scoring resulted in more memory than even a long int to hold the huge number.  This is because hand scoring also uses the same scoring system.  By instead using the mathematical principle that a number to the power of negative X will always be lower than the same number to the power of negative X plus 1, (by nearly a factor of 10) we can abuse this and set the variable to a double (floating point variable instead of whole num like int) and allow single-card scoring to range from a 0.0000000000004 for a Two, to 1.0 for an Ace.  This allows us to have no modifier for a pair (simply making each card worth its exact value from 1 to 13 in terms of ranking) and start our modifiers at two pair, exponentially increasing, but still staying within our double's value limit.

- Decision trees are incredibly complicated and require intense documentation, aided by an easy-to-follow, easy-to-map flowchart to understand what is happening, when, and why.  Using randomlly-rolled numbers and a "personality" for the CPU (in the form of stats in this context) allows for much more realistic, harder-to-predict behavior - not having these random numbers would make the game "solved" and make it boring and unrealistic, whereas not having stats to curve the randomness of the RNG would cause the game to be arbitrary, and while not solvable, it would not be as fun or enjoyable to play; akin to playing with a child who doesn't actually know what they're doing.

- Debugging skills are critical.  Using the debug function to identify the exact problem is important, but understanding what the debugger is "trying to tell you" when it cannot pinpoint the exact problem (only where it thinks the program went wrong, such as pointer pointing to nothing being the "problem" on the surface but figuring out WHY it's pointing to nothing being the actual problem) and analyzing the code leading up to that point is also important.  Using the console to create temporary "tests" to see when things go awry is an exercise in tedium, but can help a lot in figuring out which line is the problem child and follow it back to the source of the issue.

---

****Legacy Versions - History of development****

***Version 1.0.0***
   - Full release

   - FUTURE GOALS
      - Advanced bluff behavior for the CPU - detailed in version 0.9.0 documentation
      - Statistical analysis of realistic player betting/folding/raising likelihoods
      - Need to implement a function to allow the player to disable suit symbols
      - Remove unneeded files from the program's source table
---

**LEGACY VERSIONS**

The following is a detailed history of the development of Primitive Poker and the features that were implemented in each snapshot

***Version 0.9.0***
- *GOALS ACCOMPLISHED*
   - Commented on every function/variable
   - Made it so the player can see what cards are being discarded by the CPU's when they discard cards
   - Created a heads up display function that shows all important information to the player whenever they must make a decision in game
   - Created a ShowSelection function that allows a selection of cards to be shown (such as discarded cards)
   - Added some functionality where player number can be seen during the discard phase - allowing the user player to know which CPU discarded what
   - Improved formatting for readability; including break lines so important sections can be isolated from the rest of the text, improving player comprehension
   - Made printing aces for ace selection more sensical
   - Made instances where the player discards 4 cards make more sense
   - Optimized code for the scoring and discarding functions
   - Managed to establish pot-splitting in the event of a tie with the multi-pot system
   - Multi-pot implemented and works as intended
   - Refactoring functions in main.cpp into Game.h and Game.cpp
       - This will be elaborated on further; game objects can perform the fiveCardDraw, sevenCardStud, texasHoldEm, and etc. functions for different game modes
   - Several logical errors
   - Implemented the ability for an Ace to go high or low; A-2-3-4-5 is a valid straight now
   - Multi-Pot implemented, albeit buggy
      - This serves as a vector of vectors of floats.  This allows for each player to get their money back from a tie in the future

- *GOALS TO BE ACCOMPLISHED*
   - Advanced bluff behavior for the CPU - detailed in version 0.9.0 documentation
   - Statistical analysis of realistic player betting/folding/raising likelihoods

---

***Version 0.8.0***
- *ACCOMPLISHED GOALS*
   - Removed testing outputs; turned into comments
      - Need to remove these comments in due time
   - Made output logic and formatting more clear and accurate to the final image of the game
   - Removed text that talks about action score
   - Added some nuance to ante'ing and folding that allows the computer to express that a player has folded if they don't have enough money to ante
      - This has weird behavior with the user player and needs to be investigated.
    - Refactored code into functions

- *RESOLVED ISSUES*
   - New out_of_range and stack_overflow errors have been discovered upon refactoring.  It's believed to be caused by the modification of the code during the refactoring process that caused this and not existing errors.  Need investigation
      - Out_of_range error when tells are moved to the first CPU
         - This is believed to be solved by changing the way tells are handled (pushed back instead of modified directly)
         - THIS NEEDS TO BE CHANGED; pushing back in perpetuity will not allow these tells to be read past the first game
            -Two solutions for this exist; either empty the vector at the end of each game, or initialize the vector at a size of 4 and modify each element directly
      - Segmentation Fault error on player 3 checking for cash - This seems to be an issue with incrementing the CPU pointer perhaps. Further investigation will need to be made in how the game object handles player pointers at this point, CPU pointer conversion, and incrementationcode
         - This has been resolved by modifying the gameplay loop to not include the creation of players - additional players were being created which was causing mysterious errors


---

**Version 0.7.0**
- *RESOLVED ISSUES*
   - Need error/buffer checking for I/O.  Currently the game can be softlocked by using the wrong input values when entering inputs
      - Added std::cin error checking as well as buffer clears and input ignores to allow the input to "refresh"
   - CPU players will still bet 0 dollars and softlock the game in perpetual loop
      - This is believed to be resolved now thanks to forcing the CPU to call if they run out of money.  If the CPU bets all of their cash and has 0 dollars, they will call instead of continuing to try and fail to raise the bet.
   - Cash being bet in currWager does not accurately reflect the inputs given
      - There were some logical errors when it came to assigning values when betting, allowing the CPU to bet over their cash value
   - Sometimes when in a series of calls and raises, calls and raises will not properly reflect on the table and will cause strange issues, like an opponent folding but the player still in a call/raise/fold cycle, and the player folding resulting in all 4 players folding
      - While this does not surface, multiple CPU's can raise/call at 0 dollars in perpetuity.  I think that CPU's calling are not flipping their "allCalled" switches on like they should.  Possibly an easy fix
   - The entire primary logic of the program is solved outisde of bug fixing.  The game loop completes as expected unless the CPU's are caught in a call loop
   - CPU Players will sometimes bet 0 dollars and succeed
      - Betting 0 dollars is still an issue; it, however, is caught by the loop to prevent further undefined behavior
   - CPU players will bet money and lose, and this value will not payout from the pot
      - This is resolved aside from other errors dealing with betting

---

***Version 0.6.0***
- Segmentation fault errors have been rectified in CPU betting and calling.  Proper traversal of the circular linked list has fixed most of these issues, and error watching can be performed easily thanks to a new console output that prints which player is currently selected in the circular list
- Betting and calling by CPU are now automated and are performed autonomously
   - Bugs and errors need to be worked out, however.
   - There is an error where a segmentation fault occurs if a player other than the first player leads the bet; when player 4 tries to call/raise, this error occurs. BELIEVED TO BE THE RESULT OF THE FUNCTION SELECTING PAST THE PLAYER VECTOR
   - There is an intermittent error with betting, where it will constantly cycle to player 1 to lead the bet.  This may have been fixed already while ironing out functionality of 0.6.2, but one should still be aware of this issue in case it surfaces in further development
   - The function for player betting and calling is very strict, absurdly so - some action scores get as low as lower than negative 100,000.  I'm not sure if this is behaving as intended or not, and it should be investigated further.
      - Likewise, tweaking the "action" consts might be a good idea after testing.  It's a bit difficult to gauge if the CPU is being overly-cautious; they fold very often.
      - THE ABOVE NEEDS TO BE RE-ANALYZED AFTER ANALYZING ACTUAL POKER GAMES TO SEE HOW MUCH FOLDING IS DONE
- Implemented a rough version of the initial betting function for CPU's - this is rough and needs careful attention to how it manipulates iter numbers to make sure that it is selecting the correct player and correct values, but it works at a bare minimal in ideal conditions
- Need to do the same for raise/calling - the initial bet seems to work just fine, however the player's function for raising/calling is currently used for CPU's in response to the initial bet.
- Begun to refine, define, and implement functionality for CPU calling and betting
- Change stats to be universal - players have them as well.  They will, howevever, have default values
- Begun to implement vectors for passing values into the CPU betting and calling/raising functions
   - This will require further investigation as far as mapping goes - Not sure if these map entirely appropriately
- Declared functions for getting values from each player in order to use them for CPU betting.  Need to define and refine them.

---

***Version 0.5.0***
- Fixed the unorthodox behavior by simplifying the discard function for CPU's by which is selects the cards that it would like to discard.  This causes the known unorthodox behavior to cease.
- Fully defined the selection and discard functions for the CPU
   - There is unorthodox behavior where if the first two cards are to be discarded as well as the last card, sometimes unintended cards are selected for discard.  I don't know exactly why this is the case, but it doesn't cause that many problems with gameplay, and if need be is servicable although not optimal.  Will investigate further during the MVP phase of release.
- Have begun to test and refine the functionality of the CPU's card selection and discard functions
   - This has intermittent memory errors when discarding and drawing new cards, and mysterious selection errors when picking cards to drop
      - Sometimes the CPU will discard an obvious pair - there are instances when the CPU could have had a 3 of a kind, but failed because it discarded a card that didn't make sense
      - There are instances where the CPU will choose the wrong color card to discard for a flush; it should select the suit of the highest value card first and foremost in the instance of a tie
      - I can imagine the above error about flushes will also apply to straight selection as well
   - Despite bugs and errors revealing themselves, progress has been made and general, rough skeleton of the subroutine is revealing itself
- Begun the process of implementing and refining all CPU algorithms, functions, and methods.
   - JudgeHand() and GiveTell() seem to be working correctly.
   - In the process of implementing the DiscardCard() function.  There seems to be issues with determining the best cards to drop to shoot for a flush or a straight; an invalid pointer, sigabort, and etc. misc errors with memory are occuring.  Need to investigate further as more refining and testing is done.
      - This involved the arduous process of ironing out an error with a pointer to an iterator - the pointer always pointed to where the iterator was pointing, resulting in issues.  Range-based for-loops don't appear to be the play when it comes to functions that take a previous card and compare it to a current card - could possibly be a hint for further errors and bugs
   - Have not yet attempted to test and/or implement the CPU's betting and calling/raising function

---


***Version 0.4.0***
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

---

***Version 0.3.0***
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

---

***Version 0.2.0***

- Added the <math.h> library to allow for exponent arithmetic
- Changed the scoring system to use doubles and an accelerated multiplier; makes sure that the correct victor is always chosen when a hand is won
   - This needs some refining - currently direct values are used instead of a table of const's.  Some functions can be extrapolated from the lines of code that perform these operations as well
- Added "winner" selection - each hand will pass its score total to a vector.  This vector is traversed and the "maximum" value is selected, determining the winner by choosing the player with the highest score.
   - Have not implemented a solution for ties yet
   - Not sure if when implemented that tied games will work with doubles, due to floating errors.  May need to do some testing and add a "round to the nearest decimal" function for proper functionality
- Merged the deckAndHandTest branch with main
- Implemented the circularlList class and the Player class to allow for mulitple players to draw hands of cards from the same deck, printing all hands in order with their score values
- Fixed logic issues with this implementation in order to print all hands without error

---

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
