#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "engine.h"
#include "strategy.h"
#include "utils.h"

#define ERROR_CODE 42
#define DRAW_CODE 1
#define WIN_CODE 0
#define LOSE_CODE 2

int main(void) {
    board *mainBoard;
    int seenInit = 0;
    int whatToDoWithGame;
    int inputCorectness;
    int myPlayerNumber = 0;
    command *newCommand;

    mainBoard = startGame();

    while (true) {
        if (seenInit == 0) {
            inputCorectness = parseCommand(&newCommand);

            if (inputCorectness == CORRECT_INPUT) {
                if (strcmp(newCommand->name, "INIT") == 0) {
                    ++seenInit;

                    if (newCommand->data[2] != PLAYER_1 &&
                        newCommand->data[2] != PLAYER_2) {
                        whatToDoWithGame = ERROR;
                    }
                    else {
                        whatToDoWithGame = init(
                                newCommand->data[0],
                                newCommand->data[1],
                                newCommand->data[3],
                                newCommand->data[4],
                                newCommand->data[5],
                                newCommand->data[6],
                                mainBoard);
                        myPlayerNumber = (int) newCommand->data[2];
                    }
                }
                else {
                    whatToDoWithGame = ERROR;
                    free(newCommand);
                }
                free(newCommand);
            } else {
                whatToDoWithGame = ERROR;
            }

        }
        else if (myPlayerNumber != currentPlayer(mainBoard)) {
            inputCorectness = parseCommand(&newCommand);
            if (inputCorectness != CORRECT_INPUT) {
                whatToDoWithGame = ERROR;
            }
            else {
                if (strcmp(newCommand->name, "MOVE") == 0) {
                    whatToDoWithGame = move(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[2],
                            newCommand->data[3],
                            mainBoard);
                    if (whatToDoWithGame != ERROR) {
                    }
                }
                else if (strcmp(newCommand->name, "PRODUCE_KNIGHT") == 0) {
                    whatToDoWithGame = produceKnight(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[2],
                            newCommand->data[3],
                            mainBoard);
                    if (whatToDoWithGame != ERROR) {
                    }
                }
                else if (strcmp(newCommand->name, "PRODUCE_PEASANT") == 0) {
                    whatToDoWithGame = producePeasant(
                            newCommand->data[0],
                            newCommand->data[1],
                            newCommand->data[2],
                            newCommand->data[3],
                            mainBoard);
                    if (whatToDoWithGame != ERROR) {
                    }
                }
                else if (strcmp(newCommand->name, "END_TURN") == 0) {
                    whatToDoWithGame = endTurn(mainBoard);
                }
                else {
                    whatToDoWithGame = ERROR;
                }

                free(newCommand);
            }
        } else {
            whatToDoWithGame = playYourTurn(mainBoard);
            //printList();
        }

        switch (whatToDoWithGame) {
            case CONTINUE_GAME:
                break;
            case DRAW:
                endGame(mainBoard);
                exit(DRAW_CODE);
            case PLAYER_1_WON:
                endGame(mainBoard);
                if (myPlayerNumber == PLAYER_1) {
                    exit(WIN_CODE);
                } else {
                    exit(LOSE_CODE);
                }
            case PLAYER_2_WON:
                endGame(mainBoard);
                if (myPlayerNumber == PLAYER_2) {
                    exit(WIN_CODE);
                } else {
                    exit(LOSE_CODE);
                }
            default:
                /*Either function returned END_GAME or some undefined value.
                 * Anyway, it means that an error has occurred.*/
                fprintf(stderr, "input error\n");
                endGame(mainBoard);
                exit(ERROR_CODE);
        }

    }

    return 0;
}
