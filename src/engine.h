/** @file
* Interface of game engine.
*/

#ifndef ENGINE_H
#define ENGINE_H

#define ERROR 4
#define DRAW 3
#define PLAYER_2_WON 2
#define PLAYER_1_WON 1
#define CONTINUE_GAME 0
#define PLAYER_1 1
#define PLAYER_2 2
#define KING 'K'
#define KNIGHT 'R'
#define PEASANT 'C'

/**
 * Pair of long integers. Used mostly because function may not return two values
 */
struct pair {
    long int x;
    long int y;
};
typedef struct pair pair;

/**
 * Represents board on which game may be played
 */
struct board;
typedef struct board board;

/**
 * Represents unit on board
 */
struct unitNode;

typedef struct unitNode unitNode;

/**
 * Gives type of an unit
 * @param[in] unit Unit whose type we want to know
 * @return Type of unit
 */
char unitType(unitNode* unit);

/**
 * Gives next unit of current player. Designed to iterate over players units
 */
unitNode* nextUnit(board *mainBoard);

/**
 * Gives coordinates of an unit.
 * @param[in] unit Unit whose coordinates we want to know
 * @return Pair which represent coordinates of unit.
 */
pair* unitCoordinates(unitNode* unit);

/**
 * Finds closest unit on board.
 * @param[in] unit Unit whose neighbour we want to find
 * @param[in] mainBoard board on which the game is played
 */
unitNode* findClosestUnit(unitNode *unit, board *mainBoard);

/**
 * Finds field which is 'free' what means either there is no other unit
 * or there is an unit of opponent
 * @param[in] unitToMove Unit which we want to move
 * @param[in] closestUnit Unit to which we want to be closer
 * @param[in] mainBoard board on which the game is played
 *
 * @return Pair which represents free field closest to other unit
 */
pair* findClosestFreeField(unitNode *unitToMove, unitNode *closestUnit,
                           board *mainBoard);

/**
 * Finds field which is 'free' what means that there in no other unit on this.
 * @param[in] unitToMove Unit which we want to move
 * @param[in] mainBoard board on which the game is played
 * @return Pair which represents free field
 */
pair* findFreeField(unitNode *unitToMove, board *mainBoard);

/**
 * Answers, whose round is being played now.
 * @param[in] mainBoard board on which the game is played
 * @return Player who should make a move.
 */
int currentPlayer(board *mainBoard);

/**
 * Gives number of current round
 * @param[in] mainBoard board on which the game is played
 * @return Number of current round
 */
long int currentRound(board *mainBoard);

/**
 * Gives number of round in which unit was moved
 * @param[in] unit Unit whose round we want to know
 * @return Number of round in which unit was moved
 */
long int roundWhenMoved(unitNode* unit);

/**
 * Initializes a game. Needed before first INIT.
 */
board *startGame();

/**
 * Frees memory. Needed after finishing game.
 * @return mainBoard board on which the game is played
 */
void endGame(board *mainBoard);

/**
 * Initializes a game with size of a board, number of rounds and positions of
 * kings.
 * @param[in] boardSize size of a board
 * @param[in] maxNumberOfRounds maximal number of rounds which can be played
 * @param[in] xKingPlayer1 Number of a column in which Player1's King will be
 * located
 * @param[in] xKingPlayer1 Number of a row in which Player1's King will be
 * located
 * @param[in] yKingPlayer1 Number of a column in which Player2's King will be
 * located
 * @param[in] xKingPlayer2 Number of a row in which Player2's King will be
 * located
 * @param[in] yKingPlayer2 Number of a column in which Player2's King will be
 * located
 * @param[in] mainBoard board on which the game is played
 * @return
 * CONTINUE_GAME if game has to be continued <br>
 * END_GAME if game ended in standard way (player won or it is a draw).
 * Proper communicate has been already printed to stderr <br>
 * ERROR if it is illegal to make this operation
 */
int init(long int boardSize, long int maxNumberOfRounds, long int xKingPlayer1,
		long int yKingPlayer1, long int xKingPlayer2, long int yKingPlayer2,
         board *mainBoard);

/**
 * Makes a move.
 * @param[in] xCurrent Column number before a move.
 * @param[in] yCurrent Row number before a move.
 * @param[in] xDestined Column number after a move.
 * @param[in] yDestined Row number before a move.
 * @param[in] mainBoard board on which the game is played
 * @return
 * CONTINUE_GAME if game has to be continued <br>
 * END_GAME if game ended in standard way (player won or it is a draw).
 * Proper communicate has been already printed to stderr <br>
 * ERROR if it is illegal to make this operation
 */
int move(long int xCurrent, long int yCurrent, long int xDestined, long int
	yDestined, board *mainBoard);

/**
 * Allows Peasant in column xCurrent and row yCurrent to produce Knight
 * in column xDestined and row yDestined
 * @param[in] xCurrent Column number of a Peasant.
 * @param[in] yCurrent Row number before of a Peasant.
 * @param[in] xDestined Column number of Knight which has to be produced.
 * @param[in] yDestined Row number of Knight which has to be produced.
 * @param[in] mainBoard board on which the game is played
 * @return
 * CONTINUE_GAME if game has to be continued <br>
 * PLAYER_1_WON if game has ended and PLAYER_1 has won <br>
 * PLAYER_2_WON if game has ended and PLAYER_2 has won <br>
 * DRAW if game has ended and there is a draw <br>
 * ERROR if it is illegal to make this operation
 */
int produceKnight(long int xCurrent, long int yCurrent, long int xDestined,
		long int yDestined, board *mainBoard);

/**
* Allows Peasant in column xCurrent and row yCurrent to produce Peasant
* in column xDestined and row yDestined.
* @param[in] xCurrent Column number of a Peasant.
* @param[in] yCurrent Row number before of a Peasant.
* @param[in] xDestined Column number of Peasant which has to be produced.
* @param[in] yDestined Row number of Peasant which has to be produced.
* @param[in] mainBoard board on which the game is played
* @return
 * CONTINUE_GAME if game has to be continued <br>
 * PLAYER_1_WON if game has ended and PLAYER_1 has won <br>
 * PLAYER_2_WON if game has ended and PLAYER_2 has won <br>
 * DRAW if game has ended and there is a draw <br>
 * ERROR if it is illegal to make this operation
*/
int producePeasant(long int xCurrent, long int yCurrent, long int xDestined,
		long int yDestined, board *mainBoard);

/**
* Ends turn for current player
* @param[in] mainBoard board on which the game is played
* @return
 * CONTINUE_GAME if game has to be continued <br>
 * DRAW if game has ended and there is a draw <br>
 * ERROR if it is illegal to make this operation
*/
int endTurn(board *mainBoard);

#endif /* ENGINE_H */
