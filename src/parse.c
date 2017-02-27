#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

#define MAX_NAME_LENGTH 16
#define MAX_NUMBER_OF_DATA 7
#define MAX_NUMBER_LENGTH 10
#define MAX_NUMBER 2147483647

#define INIT_NUMBER_OF_ARGUMENTS 7
#define END_TURN_NUMBER_OF_ARGUMENTS 0
#define STANDARD_NUMBER_OF_ARGUMENTS 4

#define CORRECT_INPUT 1
#define INCORRECT_INPUT 0

typedef struct command {
    char name[MAX_NAME_LENGTH];
    long int data[MAX_NUMBER_OF_DATA];
} command;

int readWordFromInput(char stringToWrite[]) {
    int i = 0;
    int currentCharacter;
    while ((currentCharacter = getchar()) != ' ' && currentCharacter != '\n'
           && currentCharacter != EOF) {

        if (
                i > MAX_NAME_LENGTH ||
                !(isupper(currentCharacter) || currentCharacter == '_')) {
            return INCORRECT_INPUT;
        }
        stringToWrite[i] = (char) currentCharacter;
        ++i;
    }
    stringToWrite[i] = '\0';
    ungetc(currentCharacter, stdin);
    return CORRECT_INPUT;
}

int readNumberFromInput(long int *intToWrite) {
    int length = 0;
    int currentCharacter;
    long long int resultInt = 0;

    while (
            (currentCharacter = getchar()) != ' ' &&
            currentCharacter != '\n' &&
            currentCharacter != EOF) {

        if (length > MAX_NUMBER_LENGTH || !isdigit(currentCharacter)) {
            return INCORRECT_INPUT;
        }
        resultInt *= 10;
        resultInt += currentCharacter - '0';
        ++length;
    }

    if (length == 0 || resultInt > MAX_NUMBER || resultInt <= 0) {
        return INCORRECT_INPUT;
    }

    *intToWrite = (long int) (resultInt);

    ungetc(currentCharacter, stdin);

    return CORRECT_INPUT;
}

int readArguments(command *resultCommand) {
    int numOfArgsToRead;
    int i;
    char expectedSign = ' ';

    if (strcmp(resultCommand->name, "INIT") == 0) {
        numOfArgsToRead = INIT_NUMBER_OF_ARGUMENTS;
    } else if (strcmp(resultCommand->name, "END_TURN") == 0) {
        numOfArgsToRead = END_TURN_NUMBER_OF_ARGUMENTS;
    } else {
        numOfArgsToRead = STANDARD_NUMBER_OF_ARGUMENTS;
    }

    for (i = 0; i < numOfArgsToRead; ++i) {
        if (
                getchar() != expectedSign ||
                readNumberFromInput(&resultCommand->data[i]) != CORRECT_INPUT) {
            return INCORRECT_INPUT;
        }
    }

    expectedSign = '\n';

    if (getchar() == expectedSign) {
        return CORRECT_INPUT;
    } else {
        return INCORRECT_INPUT;
    }
}

int parseCommand(command **commandToWrite) {
    command *resultCommand = malloc(sizeof *resultCommand);
    if (commandToWrite == NULL) {
        free(resultCommand);
        return INCORRECT_INPUT;
    }
    if (readWordFromInput(resultCommand->name) != CORRECT_INPUT) {
        free(resultCommand);
        return INCORRECT_INPUT;
    }

    if (readArguments(resultCommand) != CORRECT_INPUT) {
        free(resultCommand);
        return INCORRECT_INPUT;
    }

    *commandToWrite = resultCommand;

    return CORRECT_INPUT;
}
