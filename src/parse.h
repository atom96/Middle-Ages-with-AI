/** @file
* Interface of parser.
*/

#ifndef PARSE_H
#define PARSE_H

#define CORRECT_INPUT 1
#define INCORRECT_INPUT 0

#define MAX_NAME_LENGTH 16
#define MAX_NUMBER_OF_DATA 7

/**
* Structure in which command form input can be storaged
*/

typedef struct command {
    char name[MAX_NAME_LENGTH];
    long int data[MAX_NUMBER_OF_DATA];
} command;


/** Reads a command.
* @param[out] commandToWrite Command read from output is written to this
* variable.
* @return
* CORRECT_INPUT if input was correct <br>
* INCORRECT_INPUT if input was incorrect
*/

int parseCommand(command **commandToWrite);

#endif /* PARSE_H */
