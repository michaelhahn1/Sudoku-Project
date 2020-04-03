/*
 * error_msg:
 * 	module with functions that print different types of errors that can occur in game to sdtin.
 * 	for some functions the values that resulted in the error are given to be printed for the user.
 */

#ifndef ERROR_MSG_H_
#define ERROR_MSG_H_

#include "cmd_parser.h"

/**
 * There was a mismatch between the requested command and the current game mode. Print a fitting message.
 */
void incorrect_mode_err(int GameMode, int actual_available_game_modes);
/**
 * There were too many/ too few arguments given for the command, print a fitting message.
 */
void incorrect_arg_num_err(int ManyORFew, CMD_LINE *command);
/**
 * One of the arguments that were given to the command are not in the correct range. Print a fitting message.
 */
void arg_invalid_err(int numOfArg, CMD_LINE *command);
/**
 * Print a fitting message for a failure of handling a file.
 * 0 opening
 * 1 closing
 * 2 reading
 * 3 writing
 */
void file_handling_failure(int openORcloseORwriteORread);
/**
 * Print a message that a certain cell is fixed
 */
void fixed_cell_msg(int i, int j);
/**
 * Print a message that a certain cell is full
 */
void full_cell_msg(int i,int j);
/**
 * Print a message of an allocation failure.
 */
void allocation_failure();
/**
 * Print a message that the command can't execute be it requires the board to have no errors.
 */
void board_has_errors_msg(CMD_LINE *command);

#endif /* ERROR_MSG_H_ */
