#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "utils.h"

#define KING 'K'
#define KNIGHT 'R'
#define PEASANT 'C'
//#define CORNER_SIZE 10

#define ERROR 4
#define DRAW 3
#define PLAYER_2_WON 2
#define PLAYER_1_WON 1
#define CONTINUE_GAME 0

#define MIN_BOARD_SIZE 8
#define MIN_NUM_OF_ROUNDS 0
#define MAX_NUM_OF_ROUNDS 2147483648
#define MIN_DISTANCE 8

#define PLAYER_1 1
#define PLAYER_2 2

struct unitNode;

typedef struct unitNode unitNode;

struct pair {
    long int x;
    long int y;
};

typedef struct pair pair;

struct unitNode {
    char type;
    long int x;
    long int y;
    int playerWhoOwns;
    long int roundWhenMoved;
    unitNode *nextUnit;
};

struct board {
    unitNode *unitsPlayer1;
    unitNode *unitsPlayer2;
    unitNode *currentUnitPlayer1;
    unitNode *currentUnitPlayer2;
    unitNode *lastUnitPlayer1;
    unitNode *lastUnitPlayer2;

    long int boardSize;
    long int maxNumberOfRounds;
    long int currentRound;
    int currentPlayer;
};

typedef struct board board;

static long int absoluteValue(long int x) {
    if (x >= 0) {
        return x;
    }
    else {
        return -x;
    }
}

static long int maxValue(long int number1, long int number2) {
    if (number1 >= number2) {
        return number1;
    }
    else {
        return number2;
    }
}

static long int maximumMetric(long int x1, long int y1, long int x2,
                              long int y2) {

    return maxValue(absoluteValue(x1 - x2), absoluteValue(y1 - y2));
}

static long int distanceBetween(unitNode *unit1, unitNode *unit2) {
    return maximumMetric(unit1->x, unit1->y, unit2->x, unit2->y);
}

static int endAsDraw() {
    return DRAW;
}

static int endAsWin(int playerWhoWon) {
    return playerWhoWon;
}

static unitNode *currentList(board *mainBoard) {
    if (mainBoard->currentPlayer == PLAYER_1) {
        return mainBoard->unitsPlayer1;
    }
    else {
        return mainBoard->unitsPlayer2;
    }
}

static void addUnit(char unitType, long int x, long int y, int playerWhoOwns,
                    board *mainBoard) {
    unitNode *newUnit = malloc(sizeof *newUnit);

    newUnit->type = unitType;
    newUnit->x = x;
    newUnit->y = y;
    newUnit->playerWhoOwns = playerWhoOwns;
    newUnit->roundWhenMoved = mainBoard->currentRound - 1;
    newUnit->nextUnit = NULL;

    if (playerWhoOwns == PLAYER_1) {
        if (mainBoard->unitsPlayer1 == NULL) {
            mainBoard->unitsPlayer1 = newUnit;
            mainBoard->lastUnitPlayer1 = newUnit;
        }
        else {
            mainBoard->lastUnitPlayer1->nextUnit = newUnit;
            mainBoard->lastUnitPlayer1 = newUnit;
        }
    }
    else {
        if (mainBoard->unitsPlayer2 == NULL) {
            mainBoard->unitsPlayer2 = newUnit;
            mainBoard->lastUnitPlayer2 = newUnit;
        }
        else {
            mainBoard->lastUnitPlayer2->nextUnit = newUnit;
            mainBoard->lastUnitPlayer2 = newUnit;
        }
    }

    /* if(playerWhoOwns == PLAYER_1) {
          if(mainBoard->unitsPlayer1 != NULL) {
              mainBoard->lastUnitPlayer1->nextUnit = newUnit;
              mainBoard->lastUnitPlayer1 = newUnit;
          } else {
              mainBoard->lastUnitPlayer1 = newUnit;
              mainBoard->unitsPlayer1 = newUnit;
          }
      }
      else {
          if(mainBoard->unitsPlayer2 != NULL) {
              mainBoard->lastUnitPlayer2->nextUnit = newUnit;
              mainBoard->lastUnitPlayer2 = newUnit;
          } else {
              mainBoard->lastUnitPlayer2 = newUnit;
              mainBoard->unitsPlayer2 = newUnit;
          }
      }*/

    /* if(playerWhoOwns == PLAYER_1) {
         newUnit->nextUnit = mainBoard->unitsPlayer1;
         mainBoard->unitsPlayer1 = newUnit;
     }
     else {
         newUnit->nextUnit = mainBoard->unitsPlayer2;
         mainBoard->unitsPlayer2 = newUnit;
     }*/
}

static unitNode *findUnit(long int x, long int y, board *mainBoard) {
    unitNode *currentNode;

    currentNode = mainBoard->unitsPlayer1;

    while (currentNode != NULL) {
        if (currentNode->x == x && currentNode->y == y) {
            return currentNode;
        }
        currentNode = currentNode->nextUnit;
    }

    currentNode = mainBoard->unitsPlayer2;

    while (currentNode != NULL) {
        if (currentNode->x == x && currentNode->y == y) {
            return currentNode;
        }
        currentNode = currentNode->nextUnit;
    }

    return NULL;
}

static void removeUnit(long int x, long int y, int playerWhoOwns,
                       board *mainBoard) {
    unitNode *listGuardian = malloc(sizeof *listGuardian);
    unitNode *previousNode = listGuardian;
    unitNode *currentNode;

    if (playerWhoOwns == PLAYER_1) {
        currentNode = mainBoard->unitsPlayer1;
    } else {
        currentNode = mainBoard->unitsPlayer2;
    }

    listGuardian->nextUnit = currentNode;

    while (currentNode != NULL) {
        if (currentNode->x == x && currentNode->y == y) {
            previousNode->nextUnit = currentNode->nextUnit;

            if (mainBoard->lastUnitPlayer1 == currentNode) {
                mainBoard->lastUnitPlayer1 = previousNode;
            }
            if (mainBoard->lastUnitPlayer2 == currentNode) {
                mainBoard->lastUnitPlayer2 = previousNode;
            }
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->nextUnit;
    }

    assert (currentNode != NULL);

    free(currentNode);


    if (playerWhoOwns == PLAYER_1) {
        mainBoard->unitsPlayer1 = listGuardian->nextUnit;
    }
    else {
        mainBoard->unitsPlayer2 = listGuardian->nextUnit;
    }

    free(listGuardian);
}

extern board *startGame() {
    board *mainBoard;
    mainBoard = malloc(sizeof *mainBoard);
    mainBoard->currentRound = 1;
    mainBoard->currentPlayer = PLAYER_1;
    mainBoard->unitsPlayer1 = NULL;
    mainBoard->unitsPlayer2 = NULL;
    mainBoard->currentUnitPlayer1 = NULL;
    mainBoard->currentUnitPlayer2 = NULL;
    mainBoard->lastUnitPlayer1 = NULL;
    mainBoard->lastUnitPlayer2 = NULL;

    return mainBoard;
}

extern void endGame(board *mainBoard) {
    unitNode *currentList = mainBoard->unitsPlayer1;
    unitNode *nodeToDelete;

    while (currentList != NULL) {
        nodeToDelete = currentList;
        currentList = currentList->nextUnit;
        free(nodeToDelete);
    }

    currentList = mainBoard->unitsPlayer2;

    while (currentList != NULL) {
        nodeToDelete = currentList;
        currentList = currentList->nextUnit;
        free(nodeToDelete);
    }

    free(mainBoard);
}

extern int init(long int boardSize, long int maxNumberOfRounds,
                long int xKingPlayer1, long int yKingPlayer1,
                long int xKingPlayer2,
                long int yKingPlayer2, board *mainBoard) {

    if (
            boardSize <= MIN_BOARD_SIZE ||
            maxNumberOfRounds <= MIN_NUM_OF_ROUNDS ||
            maxNumberOfRounds >= MAX_NUM_OF_ROUNDS ||
            xKingPlayer1 <= 0 ||
            xKingPlayer2 <= 0 ||
            yKingPlayer1 <= 0 ||
            yKingPlayer2 <= 0 ||
            xKingPlayer1 > boardSize - 3 ||
            xKingPlayer2 > boardSize - 3 ||
            yKingPlayer1 > boardSize ||
            yKingPlayer2 > boardSize ||
            maximumMetric(
                    xKingPlayer1,
                    yKingPlayer1,
                    xKingPlayer2,
                    yKingPlayer2
            ) < MIN_DISTANCE) {
        return ERROR;
    }

    mainBoard->boardSize = boardSize;
    mainBoard->maxNumberOfRounds = maxNumberOfRounds;
    //mainBoard->topLeftPrintSize = minValue(boardSize, CORNER_SIZE);

    addUnit(KING, xKingPlayer1, yKingPlayer1, PLAYER_1, mainBoard);
    addUnit(PEASANT, xKingPlayer1 + 1, yKingPlayer1, PLAYER_1, mainBoard);
    addUnit(KNIGHT, xKingPlayer1 + 2, yKingPlayer1, PLAYER_1, mainBoard);
    addUnit(KNIGHT, xKingPlayer1 + 3, yKingPlayer1, PLAYER_1, mainBoard);

    addUnit(KING, xKingPlayer2, yKingPlayer2, PLAYER_2, mainBoard);
    addUnit(PEASANT, xKingPlayer2 + 1, yKingPlayer2, PLAYER_2, mainBoard);
    addUnit(KNIGHT, xKingPlayer2 + 2, yKingPlayer2, PLAYER_2, mainBoard);
    addUnit(KNIGHT, xKingPlayer2 + 3, yKingPlayer2, PLAYER_2, mainBoard);

    mainBoard->currentUnitPlayer1 = mainBoard->unitsPlayer1;
    mainBoard->currentUnitPlayer2 = mainBoard->unitsPlayer2;

    return CONTINUE_GAME;
}

static unitNode *whoWinBattle(unitNode *unit1, unitNode *unit2) {
    if (unit1->type == unit2->type) {
        return NULL;
    }

    switch (unit1->type) {
        case KNIGHT:
            return unit1;
        case PEASANT:
            return unit2;
        case KING:
            if (unit2->type == KNIGHT) {
                return unit2;
            } else {
                return unit1;
            }
        default:
            assert(false);
    }

    return NULL;
}

static void moveTo(unitNode *unit, long int x, long int y, board *mainBoard) {
    /*if (unit->x <= CORNER_SIZE && unit->y <= CORNER_SIZE) {
        mainBoard->topLeft[unit->y - 1][unit->x - 1] = EMPTY_FIELD;
    }*/

    unit->x = x;
    unit->y = y;
    unit->roundWhenMoved = mainBoard->currentRound;

    /*if (x <= CORNER_SIZE && y <= CORNER_SIZE) {
        mainBoard->topLeft[y - 1][x - 1] = unitCharRepresentation(
                unit->type,
                unit->playerWhoOwns
        );
    }*/
}

extern int move(long int xCurrent, long int yCurrent, long int xDestined,
                long int yDestined, board *mainBoard) {

    unitNode *currentUnit = findUnit(xCurrent, yCurrent, mainBoard);
    unitNode *unitOnField = findUnit(xDestined, yDestined, mainBoard);

    if (
            xCurrent <= 0 ||
            xDestined <= 0 ||
            yCurrent <= 0 ||
            yDestined <= 0 ||
            xCurrent > mainBoard->boardSize ||
            xDestined > mainBoard->boardSize ||
            yCurrent > mainBoard->boardSize ||
            yDestined > mainBoard->boardSize ||
            maximumMetric(xCurrent, yCurrent, xDestined, yDestined) != 1 ||
            currentUnit == NULL ||
            currentUnit->roundWhenMoved == mainBoard->currentRound ||
            currentUnit->playerWhoOwns != mainBoard->currentPlayer) {
        return ERROR;
    }

    if (unitOnField != NULL && unitOnField->playerWhoOwns
                               == currentUnit->playerWhoOwns) {

        return ERROR;
    }

    if (unitOnField == NULL) {
        moveTo(currentUnit, xDestined, yDestined, mainBoard);
    } else {
        unitNode *unitWhichWon = whoWinBattle(currentUnit, unitOnField);

        if (unitWhichWon == NULL) {
            char unitType = currentUnit->type;

            removeUnit(currentUnit->x,
                       currentUnit->y,
                       currentUnit->playerWhoOwns,
                       mainBoard);
            removeUnit(unitOnField->x,
                       unitOnField->y,
                       unitOnField->playerWhoOwns,
                       mainBoard);

            if (unitType == KING) {
                return endAsDraw();
            }
        }
        else if (unitWhichWon == currentUnit) {
            char unitType = unitOnField->type;

            removeUnit(unitOnField->x,
                       unitOnField->y,
                       unitOnField->playerWhoOwns,
                       mainBoard);
            moveTo(currentUnit, xDestined, yDestined, mainBoard);
            if (unitType == KING) {
                return endAsWin(currentUnit->playerWhoOwns);
            }
        }
        else if (unitWhichWon == unitOnField) {
            char unitType = currentUnit->type;

            removeUnit(currentUnit->x,
                       currentUnit->y,
                       currentUnit->playerWhoOwns,
                       mainBoard);

            if (unitType == KING) {
                return endAsWin(unitOnField->playerWhoOwns);
            }
        }
    }
    return CONTINUE_GAME;
}

extern int produceKnight(long int xCurrent, long int yCurrent,
                         long int xDestined, long int yDestined,
                         board *mainBoard) {

    unitNode *currentPeasant = findUnit(xCurrent, yCurrent, mainBoard);

    if (
            currentPeasant == NULL ||
            currentPeasant->type != PEASANT ||
            currentPeasant->playerWhoOwns != mainBoard->currentPlayer ||
            currentPeasant->roundWhenMoved + 3 > mainBoard->currentRound ||
            maximumMetric(xCurrent, yCurrent, xDestined, yDestined) != 1 ||
            findUnit(xDestined, yDestined, mainBoard) != NULL ||
            xDestined > mainBoard->boardSize ||
            yDestined > mainBoard->boardSize ||
            xDestined <= 0 ||
            yDestined <= 0) {
        return ERROR;
    }

    addUnit(KNIGHT, xDestined, yDestined, currentPeasant->playerWhoOwns,
            mainBoard);
    currentPeasant->roundWhenMoved = mainBoard->currentRound;
    return CONTINUE_GAME;
}

extern int producePeasant(long int xCurrent, long int yCurrent,
                          long int xDestined, long int yDestined,
                          board *mainBoard) {
    unitNode *currentPeasant = findUnit(xCurrent, yCurrent, mainBoard);

    if (
            currentPeasant == NULL ||
            currentPeasant->type != PEASANT ||
            currentPeasant->playerWhoOwns != mainBoard->currentPlayer ||
            currentPeasant->roundWhenMoved + 3 > mainBoard->currentRound ||
            maximumMetric(xCurrent, yCurrent, xDestined, yDestined) != 1 ||
            findUnit(xDestined, yDestined, mainBoard) != NULL ||
            xDestined > mainBoard->boardSize ||
            yDestined > mainBoard->boardSize) {
        return ERROR;
    }

    addUnit(PEASANT, xDestined, yDestined, currentPeasant->playerWhoOwns,
            mainBoard);
    currentPeasant->roundWhenMoved = mainBoard->currentRound;
    return CONTINUE_GAME;
}

extern int endTurn(board *mainBoard) {
    if (mainBoard->currentPlayer == PLAYER_1) {
        mainBoard->currentPlayer = PLAYER_2;
        mainBoard->currentUnitPlayer2 = mainBoard->unitsPlayer2;
    }
    else {
        mainBoard->currentPlayer = PLAYER_1;
        mainBoard->currentRound += 1;
        mainBoard->currentUnitPlayer1 = mainBoard->unitsPlayer1;

        if (mainBoard->currentRound > mainBoard->maxNumberOfRounds) {
            return endAsDraw();
        }
    }
    return CONTINUE_GAME;
}

extern unitNode *nextUnit(board *mainBoard) {
    if (mainBoard->currentPlayer == PLAYER_1) {
        unitNode *unitToReturn = mainBoard->currentUnitPlayer1;
        if (unitToReturn != NULL) {
            mainBoard->currentUnitPlayer1 = unitToReturn->nextUnit;
        }
        return unitToReturn;
    }
    else {
        unitNode *unitToReturn = mainBoard->currentUnitPlayer2;
        if (unitToReturn != NULL) {
            mainBoard->currentUnitPlayer2 = unitToReturn->nextUnit;
        }
        return unitToReturn;
    }
}

extern char unitType(unitNode *unit) {
    return unit->type;
}

extern unitNode *findClosestUnit(unitNode *unit, board *mainBoard) {
    unitNode *currentNode;
    unitNode *unitToReturn;

    if (unit->playerWhoOwns == PLAYER_1) {
        currentNode = mainBoard->unitsPlayer2;
    }
    else {
        currentNode = mainBoard->unitsPlayer1;
    }

    unitToReturn = currentNode;

    while (currentNode != NULL) {
        if (distanceBetween(unit, currentNode)
            < distanceBetween(unit, unitToReturn)) {

            unitToReturn = currentNode;
        } else if (distanceBetween(unit, currentNode)
                   == distanceBetween(unit, unitToReturn)) {

            switch (currentNode->type) {
                case KING:
                    unitToReturn = currentNode;
                    break;
                case PEASANT:
                    if (unitToReturn->type == KNIGHT) {
                        unitToReturn = currentNode;
                    }
                    break;
                default :
                    break;
            }

        }
        currentNode = currentNode->nextUnit;
    }

    return unitToReturn;
}

static void excludeWrongFields(unitNode *unitToMove, bool fieldsToCheck[][3],
                               board *mainBoard) {
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (
                    (unitToMove->x + j - 1 > 0) &&
                    (unitToMove->y + i - 1 > 0) &&
                    (unitToMove->x + j - 1 <= mainBoard->boardSize) &&
                    (unitToMove->y + i - 1 <= mainBoard->boardSize)) {

                fieldsToCheck[i][j] = true;
            } else {
                fieldsToCheck[i][j] = false;
            }
        }
    }
}

static void excludeTakenFields(unitNode *unitToMove, bool fieldsToCheck[][3],
                               unitNode *currentUnit) {
    int i, j;
    while (currentUnit != NULL) {
        for (i = 0; i < 3; ++i) {
            for (j = 0; j < 3; ++j) {
                if ((currentUnit->x == unitToMove->x + j - 1) &&
                    (currentUnit->y == unitToMove->y + i - 1)) {

                    fieldsToCheck[i][j] = false;
                }
            }
        }
        currentUnit = currentUnit->nextUnit;
    }
}

extern pair *findFreeField(unitNode *unitToMove, board *mainBoard) {
    pair *pairToReturn = NULL;
    bool fieldsToCheck[3][3];
    int i, j;

    excludeWrongFields(unitToMove, fieldsToCheck, mainBoard);
    excludeTakenFields(unitToMove, fieldsToCheck, mainBoard->unitsPlayer1);
    excludeTakenFields(unitToMove, fieldsToCheck, mainBoard->unitsPlayer2);


    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (fieldsToCheck[i][j]) {
                pairToReturn = malloc(sizeof *pairToReturn);
                pairToReturn->x = unitToMove->x + j - 1;
                pairToReturn->y = unitToMove->y + i - 1;
                return pairToReturn;
            }
        }
    }

    return pairToReturn;
}

static void freeTable(pair **tab, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        free(tab[i]);
    }
}

extern pair *findClosestFreeField(unitNode *unitToMove, unitNode *closestUnit,
                                  board *mainBoard) {
    pair *pairToReturn = NULL;
    pair *tempPair;
    bool fieldsToCheck[3][3];
    pair *pairsToSelect[8];
    int n = 0;
    int i, j;
    srand((unsigned int) time(0));

    excludeWrongFields(unitToMove, fieldsToCheck, mainBoard);
    excludeTakenFields(unitToMove, fieldsToCheck, currentList(mainBoard));

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (fieldsToCheck[i][j]) {
                if (n == 0) {
                    pairsToSelect[0] = malloc(sizeof *pairsToSelect[0]);

                    pairsToSelect[0]->x = unitToMove->x + j - 1;
                    pairsToSelect[0]->y = unitToMove->y + i - 1;
                    ++n;
                }
                else {
                    if (maximumMetric(unitToMove->x + j - 1,
                                      unitToMove->y + i - 1,
                                      closestUnit->x,
                                      closestUnit->y)
                        < maximumMetric(pairsToSelect[0]->x,
                                        pairsToSelect[0]->y,
                                        closestUnit->x,
                                        closestUnit->y)) {

                        freeTable(pairsToSelect, n);
                        n = 0;
                        pairsToSelect[0] = malloc(sizeof *pairsToSelect[0]);

                        pairsToSelect[0]->x = unitToMove->x + j - 1;
                        pairsToSelect[0]->y = unitToMove->y + i - 1;
                        ++n;
                    }
                    else if (maximumMetric(unitToMove->x + j - 1,
                                           unitToMove->y + i - 1,
                                           closestUnit->x,
                                           closestUnit->y)
                             == maximumMetric(pairsToSelect[0]->x,
                                              pairsToSelect[0]->y,
                                              closestUnit->x,
                                              closestUnit->y)) {

                        pairsToSelect[n] = malloc(sizeof *pairsToSelect[i]);
                        pairsToSelect[n]->x = unitToMove->x + j - 1;
                        pairsToSelect[n]->y = unitToMove->y + i - 1;
                        ++n;
                    }
                }
            }
        }
    }

    if (n > 0) {
        tempPair = pairsToSelect[rand() % n];
        pairToReturn = malloc(sizeof *pairToReturn);
        pairToReturn->x = tempPair->x;
        pairToReturn->y = tempPair->y;
        freeTable(pairsToSelect, n);
    }
    return pairToReturn;
}

extern pair *unitCoordinates(unitNode *unit) {
    pair *pairToReturn = malloc(sizeof *pairToReturn);
    assert(unit != NULL);
    pairToReturn->x = unit->x;
    pairToReturn->y = unit->y;
    return pairToReturn;
}

extern long int roundWhenMoved(unitNode *unit) {
    return unit->roundWhenMoved;
}

extern int currentPlayer(board *mainBoard) {
    return mainBoard->currentPlayer;
}

extern long int currentRound(board *mainBoard) {
    return mainBoard->currentRound;
}
