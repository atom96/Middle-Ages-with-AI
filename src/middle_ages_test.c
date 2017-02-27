#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmocka.h"
#include "parse.h"
#include "engine.h"


#undef free

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

static char temporary_buffer[256];
static char *temporary_input;
static int input_size;
static int currentInputLocation;

extern int parseCommand(command **commandToWrite);

extern unitNode *findUnit(long int x, long int y, board *mainBoard);

extern void addUnit(char unitType, long int x, long int y, int playerWhoOwns,
                    board *mainBoard);

extern void removeUnit(long int x, long int y, int playerWhoOwns,
                       board *mainBoard);


int mock_fprintf(FILE *const file, const char *format, ...)
        CMOCKA_PRINTF_ATTRIBUTE(2, 3);

int mock_printf(const char *format, ...) CMOCKA_PRINTF_ATTRIBUTE(1, 2);

int mock_getchar();

extern int middle_main();

/* A mock fprintf function that checks the value of strings printed to the
 * standard error stream. */
int mock_fprintf(FILE *const file, const char *format, ...) {
    int return_value;
    va_list args;
    assert_true(file == stderr);
    va_start(args, format);
    return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer),
                             format, args);
    check_expected_ptr(temporary_buffer);
    va_end(args);
    return return_value;
}

/* A mock printf function that checks the value of strings printed to the
 * standard output stream. */
int mock_printf(const char *format, ...) {
    int return_value;
    va_list args;
    va_start(args, format);
    return_value = vsnprintf(temporary_buffer, sizeof(temporary_buffer),
                             format, args);
    check_expected_ptr(temporary_buffer);
    va_end(args);
    return return_value;
    return 1;
}

int mock_getchar() {
    if (currentInputLocation < input_size) {
        return temporary_input[currentInputLocation++];
    }
    return EOF;
}

int mock_ungetc(int c, FILE *stream) {
    temporary_buffer[--currentInputLocation] = c;

    return 0;
}


/**
 * Tests if proper MOVE command is resolved properly by parser
 */
static void test_parse_standard_command(void **state) {
    command *command;
    char input[] = "MOVE 17 20 11 40\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), CORRECT_INPUT);
    assert_string_equal(command->name, "MOVE");
    assert_int_equal(command->data[0], 17);
    assert_int_equal(command->data[1], 20);
    assert_int_equal(command->data[2], 11);
    assert_int_equal(command->data[3], 40);
    free(command);
}

/**
 * What happens when we send NULL to parseCommand
 */
static void test_parse_null_pointer(void **state) {
    char input[] = "MOVE 17 20 11 40\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(NULL), INCORRECT_INPUT);
}

/**
 * Tries to send MOVE with negative number
 */
static void test_parse_negative_number(void **state) {
    command *command;
    char input[] = "MOVE 1 1 -5 2\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), INCORRECT_INPUT);
}

/**
 * Tries to send MOVE with zero
 */
static void test_parse_zero(void **state) {
    command *command;
    char input[] = "MOVE 1 1 7 0\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), INCORRECT_INPUT);
}

/**
 * Tries to send MOVE with really big number
 */
static void test_parse_long_number(void **state) {
    command *command;
    char input[] = "MOVE 1 1 256644654654654654654654654654 2\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), INCORRECT_INPUT);
}

/**
 * Tries to send MOVE with too big number
 */
static void test_parse_too_big_number(void **state) {
    command *command;
    char input[] = "MOVE 1 2147483648 1 2\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), INCORRECT_INPUT);
}

/**
 * Tries to send MOVE with the biggest allowed number
 */
static void test_parse_corner_number(void **state) {
    command *command;
    char input[] = "MOVE 1 2147483647 1 2\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), CORRECT_INPUT);
    free(command);
}

/**
 * Gives parser not enought arguments with MOVE
 */
static void test_parse_not_enought_arguments(void **state) {
    command *command;
    char input[] = "MOVE 1 1 1\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), INCORRECT_INPUT);
}

/**
 * Gives parser too much arguments with move
 */
static void test_parse_too_much_arguments(void **state) {
    command *command;
    char input[] = "MOVE 1 1 1 2 54563\n";
    temporary_input = input;
    input_size = sizeof(input) / sizeof(*input) - 1;
    currentInputLocation = 0;

    assert_int_equal(parseCommand(&command), INCORRECT_INPUT);
}

/**
 * Test move() with standard arguments
 */
static void test_move_standard(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    assert_int_equal(move(1, 1, 1, 2, mainBoard), CONTINUE_GAME);
    assert_non_null(findUnit(1, 2, mainBoard));
    endGame(mainBoard);
}

/**
 * Tries to move unit in opponent's round
 */
static void test_move_wrong_player(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);

    assert_int_equal(move(1, 9, 1, 8, mainBoard), ERROR);
    assert_null(findUnit(1, 8, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to move unit far, far away. Definetely too far.
 */
static void test_move_too_distant(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);

    assert_int_equal(move(1, 1, 1, 6, mainBoard), ERROR);
    assert_null(findUnit(1, 6, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to move unit on zero coordinate.
 */
static void test_move_to_zero(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);

    assert_int_equal(move(1, 1, 0, 1, mainBoard), ERROR);
    assert_null(findUnit(1, 8, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to move unit out of board
 */
static void test_move_out_of_board(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KNIGHT, 10, 10, 1, mainBoard);

    assert_int_equal(move(10, 10, 10, 11, mainBoard), ERROR);
    assert_null(findUnit(10, 11, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to move unit into player's other unit
 */
static void test_move_to_your_unit(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);

    assert_int_equal(move(1, 1, 2, 1, mainBoard), ERROR);
    assert_non_null(findUnit(1, 1, mainBoard));
    assert_non_null(findUnit(2, 1, mainBoard));

    endGame(mainBoard);
}

/**
 * Tests battle between knight and knight
 */
static void test_move_battle_knight_knight(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KNIGHT, 5, 5, 1, mainBoard);
    addUnit(KNIGHT, 5, 6, 2, mainBoard);
    assert_int_equal(move(5, 5, 5, 6, mainBoard), CONTINUE_GAME);
    assert_null(findUnit(5, 5, mainBoard));
    assert_null(findUnit(5, 6, mainBoard));

    endGame(mainBoard);
}

/**
 * Tests battle between knight and peasant
 */
static void test_move_battle_knight_peasant(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KNIGHT, 5, 5, 1, mainBoard);
    addUnit(PEASANT, 5, 6, 2, mainBoard);
    assert_int_equal(move(5, 5, 5, 6, mainBoard), CONTINUE_GAME);
    assert_null(findUnit(5, 5, mainBoard));
    assert_non_null(findUnit(5, 6, mainBoard));
    assert_int_equal(findUnit(5, 6, mainBoard)->type, KNIGHT);
    endGame(mainBoard);
}

/**
 * Tests battle between knight and king
 */
static void test_move_battle_knight_king(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KNIGHT, 5, 5, 1, mainBoard);
    addUnit(KING, 5, 6, 2, mainBoard);
    assert_int_equal(move(5, 5, 5, 6, mainBoard), PLAYER_1_WON);
    assert_null(findUnit(5, 5, mainBoard));
    assert_non_null(findUnit(5, 6, mainBoard));
    assert_int_equal(findUnit(5, 6, mainBoard)->type, KNIGHT);

    endGame(mainBoard);
}

/**
 * Tests battle between peasant and peasant
 */
static void test_move_battle_peasant_peasant(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(PEASANT, 5, 5, 1, mainBoard);
    addUnit(PEASANT, 5, 6, 2, mainBoard);
    assert_int_equal(move(5, 5, 5, 6, mainBoard), CONTINUE_GAME);
    assert_null(findUnit(5, 5, mainBoard));
    assert_null(findUnit(5, 6, mainBoard));

    endGame(mainBoard);
}

/**
 * Tests battle between peasant and king
 */
static void test_move_battle_peasant_king(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(PEASANT, 5, 5, 1, mainBoard);
    addUnit(KING, 5, 6, 2, mainBoard);
    assert_int_equal(move(5, 5, 5, 6, mainBoard), CONTINUE_GAME);
    assert_null(findUnit(5, 5, mainBoard));
    assert_non_null(findUnit(5, 6, mainBoard));
    assert_int_equal(findUnit(5, 6, mainBoard)->type, KING);

    endGame(mainBoard);
}

/**
 * Tests battle between peasant and knight
 */
static void test_move_battle_peasant_knight(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KNIGHT, 5, 6, 2, mainBoard);
    addUnit(PEASANT, 5, 5, 1, mainBoard);

    assert_int_equal(move(5, 5, 5, 6, mainBoard), CONTINUE_GAME);
    assert_null(findUnit(5, 5, mainBoard));
    assert_non_null(findUnit(5, 6, mainBoard));
    assert_int_equal(findUnit(5, 6, mainBoard)->type, KNIGHT);

    endGame(mainBoard);
}

/**
 * Tests battle between king and king
 */
static void test_move_battle_king_king(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KING, 5, 5, 1, mainBoard);
    addUnit(KING, 5, 6, 2, mainBoard);

    assert_int_equal(move(5, 5, 5, 6, mainBoard), DRAW);
    assert_null(findUnit(5, 5, mainBoard));
    assert_null(findUnit(5, 6, mainBoard));

    endGame(mainBoard);
}

/**
 * Tests battle between king and peasant
 */
static void test_move_battle_king_peasant(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KING, 5, 5, 1, mainBoard);
    addUnit(PEASANT, 5, 6, 2, mainBoard);


    assert_int_equal(move(5, 5, 5, 6, mainBoard), CONTINUE_GAME);
    assert_null(findUnit(5, 5, mainBoard));
    assert_non_null(findUnit(5, 6, mainBoard));
    assert_int_equal(findUnit(5, 6, mainBoard)->type, KING);

    endGame(mainBoard);
}

/**
 * Tests battle between king and knight
 */
static void test_move_battle_king_knight(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(KING, 5, 5, 1, mainBoard);
    addUnit(KNIGHT, 5, 6, 2, mainBoard);

    assert_int_equal(move(5, 5, 5, 6, mainBoard), PLAYER_2_WON);
    assert_null(findUnit(5, 5, mainBoard));
    assert_non_null(findUnit(5, 6, mainBoard));
    assert_int_equal(findUnit(5, 6, mainBoard)->type, KNIGHT);

    endGame(mainBoard);
}

/**
 * Tests producing knight in standard way.
 */
static void test_produce_knight_standard(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    mainBoard->currentRound = 3;

    assert_int_equal(produceKnight(2, 1, 2, 2, mainBoard), CONTINUE_GAME);
    assert_non_null(findUnit(2, 2, mainBoard));
    assert_int_equal(findUnit(2, 2, mainBoard)->type, KNIGHT);

    endGame(mainBoard);
}

/**
 * Tries to produce knight in not proper round
 */
static void test_produce_knight_too_early(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);

    assert_int_equal(produceKnight(2, 1, 2, 2, mainBoard), ERROR);
    assert_null(findUnit(2, 2, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to produce knight with one Peasant, once in standard way and then too
 * early
 */
static void test_produce_knight_first_good_then_too_early(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    mainBoard->currentRound = 3;

    assert_int_equal(produceKnight(2, 1, 2, 2, mainBoard), CONTINUE_GAME);
    assert_non_null(findUnit(2, 2, mainBoard));
    assert_int_equal(findUnit(2, 2, mainBoard)->type, KNIGHT);

    removeUnit(2, 2, 1, mainBoard);
    mainBoard->currentRound = 2;

    assert_int_equal(produceKnight(2, 1, 2, 2, mainBoard), ERROR);
    assert_null(findUnit(2, 2, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to produce knight on taken coordinates by player's unit
 */
static void test_produce_knight_taken_coordinates(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    mainBoard->currentRound = 3;

    assert_int_equal(produceKnight(2, 1, 1, 1, mainBoard), ERROR);
    assert_non_null(findUnit(1, 1, mainBoard));
    assert_int_not_equal(findUnit(1, 1, mainBoard)->type, KNIGHT);

    endGame(mainBoard);
}

/**
 * Tries to produce knight on taken coordinates by opponetn's unit
 */
static void test_produce_knight_taken_coordinates_by_opponent(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    mainBoard->currentRound = 3;
    addUnit(PEASANT, 2, 2, PLAYER_2, mainBoard);

    assert_int_equal(produceKnight(2, 1, 2, 2, mainBoard), ERROR);
    assert_non_null(findUnit(2, 2, mainBoard));
    assert_int_not_equal(findUnit(2, 2, mainBoard)->type, KNIGHT);

    endGame(mainBoard);
}

/**
 * Tries to produce knigh too far away
 */
static void test_produce_knight_too_distant(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    mainBoard->currentRound = 3;

    assert_int_equal(produceKnight(2, 1, 2, 3, mainBoard), ERROR);

    assert_null(findUnit(2, 2, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to produce knigt on zero coordinate
 */
static void test_produce_knight_on_zero(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    mainBoard->currentRound = 3;

    assert_int_equal(produceKnight(2, 1, 2, 0, mainBoard), ERROR);

    assert_null(findUnit(2, 2, mainBoard));

    endGame(mainBoard);
}

/**
 * Tries to produce knight out of board
 */
static void test_produce_knight_outside_board(void **state) {
    board *mainBoard = startGame();
    init(10, 10, 1, 1, 1, 9, mainBoard);
    addUnit(PEASANT, 10, 10, 1, mainBoard);
    mainBoard->currentRound = 3;

    assert_int_equal(produceKnight(10, 10, 10, 11, mainBoard), ERROR);

    assert_null(findUnit(10, 11, mainBoard));

    endGame(mainBoard);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_parse_standard_command),
            cmocka_unit_test(test_parse_null_pointer),
            cmocka_unit_test(test_parse_negative_number),
            cmocka_unit_test(test_parse_zero),
            cmocka_unit_test(test_parse_long_number),
            cmocka_unit_test(test_parse_too_big_number),
            cmocka_unit_test(test_parse_corner_number),
            cmocka_unit_test(test_parse_not_enought_arguments),
            cmocka_unit_test(test_parse_too_much_arguments),

            cmocka_unit_test(test_move_standard),
            cmocka_unit_test(test_move_wrong_player),
            cmocka_unit_test(test_move_too_distant),
            cmocka_unit_test(test_move_to_zero),
            cmocka_unit_test(test_move_out_of_board),
            cmocka_unit_test(test_move_to_your_unit),

            cmocka_unit_test(test_move_battle_knight_knight),
            cmocka_unit_test(test_move_battle_knight_peasant),
            cmocka_unit_test(test_move_battle_knight_king),
            cmocka_unit_test(test_move_battle_peasant_peasant),
            cmocka_unit_test(test_move_battle_peasant_king),
            cmocka_unit_test(test_move_battle_peasant_knight),
            cmocka_unit_test(test_move_battle_king_king),
            cmocka_unit_test(test_move_battle_king_peasant),
            cmocka_unit_test(test_move_battle_king_knight),

            cmocka_unit_test(test_produce_knight_standard),
            cmocka_unit_test(test_produce_knight_too_early),
            cmocka_unit_test(test_produce_knight_first_good_then_too_early),
            cmocka_unit_test(test_produce_knight_taken_coordinates),
            cmocka_unit_test(test_produce_knight_taken_coordinates_by_opponent),
            cmocka_unit_test(test_produce_knight_too_distant),
            cmocka_unit_test(test_produce_knight_on_zero),
            cmocka_unit_test(test_produce_knight_outside_board),


    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
