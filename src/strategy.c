#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "utils.h"

static int peasants = 1;

static int produceNewPeasant(unitNode *currentUnit, board *mainBoard) {
    pair *currentCoordinates = unitCoordinates(currentUnit);
    pair *destinationCoordinates = findFreeField(currentUnit, mainBoard);
    int whatToDoWithGame = CONTINUE_GAME;

    if (destinationCoordinates != NULL) {
        printf("PRODUCE_PEASANT %ld %ld %ld %ld\n",
               currentCoordinates->x,
               currentCoordinates->y,
               destinationCoordinates->x,
               destinationCoordinates->y);

        fflush(stdout);

        whatToDoWithGame = producePeasant(currentCoordinates->x,
                                          currentCoordinates->y,
                                          destinationCoordinates->x,
                                          destinationCoordinates->y,
                                          mainBoard);

        free(destinationCoordinates);
    }

    free(currentCoordinates);

    return whatToDoWithGame;
}

static int produceNewKnight(unitNode *currentUnit, board *mainBoard) {
    pair *currentCoordinates = unitCoordinates(currentUnit);
    pair *destinationCoordinates = findFreeField(currentUnit, mainBoard);
    int whatToDoWithGame = CONTINUE_GAME;

    if (destinationCoordinates != NULL) {
        printf("PRODUCE_KNIGHT %ld %ld %ld %ld\n",
               currentCoordinates->x,
               currentCoordinates->y,
               destinationCoordinates->x,
               destinationCoordinates->y);

        fflush(stdout);

        whatToDoWithGame = produceKnight(currentCoordinates->x,
                                         currentCoordinates->y,
                                         destinationCoordinates->x,
                                         destinationCoordinates->y,
                                         mainBoard);

        free(destinationCoordinates);
    }

    free(currentCoordinates);

    return whatToDoWithGame;
}

static int makeMoveWithPeasant(unitNode *currentUnit, board *mainBoard) {
    if (roundWhenMoved(currentUnit) + 3 <= currentRound(mainBoard)) {
        if (peasants == 1) {
            ++peasants;
            return produceNewPeasant(currentUnit, mainBoard);
        }
        else {
            return produceNewKnight(currentUnit, mainBoard);
        }
    }

    return CONTINUE_GAME;
}

static int makeMoveWithKnight(unitNode *currentUnit, board *mainBoard) {
    pair *currentCoordinates = unitCoordinates(currentUnit);
    pair *destinationCoordinates = findClosestFreeField(
            currentUnit,
            findClosestUnit(currentUnit, mainBoard), mainBoard);

    int whatToDoWithGame = CONTINUE_GAME;

    if (destinationCoordinates != NULL) {
        printf("MOVE %ld %ld %ld %ld\n",
               currentCoordinates->x,
               currentCoordinates->y,
               destinationCoordinates->x,
               destinationCoordinates->y);

        fflush(stdout);

        whatToDoWithGame = move(currentCoordinates->x,
                                currentCoordinates->y,
                                destinationCoordinates->x,
                                destinationCoordinates->y,
                                mainBoard);

        free(destinationCoordinates);
    }

    free(currentCoordinates);

    return whatToDoWithGame;
}

extern int playYourTurn(board *mainBoard) {
    unitNode *currentUnit = nextUnit(mainBoard);
    int whatToDoWithGame = CONTINUE_GAME;


    while (currentUnit != NULL) {
        if (unitType(currentUnit) == PEASANT) {
            whatToDoWithGame = makeMoveWithPeasant(currentUnit, mainBoard);
        }
        else if (unitType(currentUnit) == KNIGHT) {
            whatToDoWithGame = makeMoveWithKnight(currentUnit, mainBoard);
        }

        if (whatToDoWithGame != CONTINUE_GAME) {
            return whatToDoWithGame;
        }
        currentUnit = nextUnit(mainBoard);
    }

    printf("END_TURN\n");
    fflush(stdout);
    return endTurn(mainBoard);
}