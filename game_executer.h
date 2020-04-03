/**
 * game_executer:
 *	Holds the two frame functions to run a complete game.
 *	The first being the one that executes one of the use commands.
 *	The second being the frame for running an entire game from start to finish- to be used in main.
 */

#ifndef GAME_EXECUTER_H_
#define GAME_EXECUTER_H_

#include "MainAux.h"
#include "game.h"
#include "backtracking.h"
#include "solver.h"
#include "cmd_parser.h"
#include "error_msg.h"

/**
 *	Takes the current game state and the user command.
 *	Validates the command with the board as specified in page 8.
 *	If command is valid, executes the function responsible for that command.
 *	@parameters:
 *		game: an already initialised game structure.
 *		line: an already parsed command line.
 *	@return
 *		0 if had no problem and needs the board to be printed when it's done.
 *		1 if case:
 *				1)The command executed doesn't require to have the board printed afterwards.
 *				2)The command executed encountered an error before finishing.
 */
int execute_user_command(GAME *game,CMD_LINE *line);

/**
 * Runs a sudoku game from start to finish:
 * 	Gets user input, parses it to a command, executes the command on the current board
 * 	and repeats until exit was requested
 * 	or memory allocation failed and resulted in the program terminating.
 * @return: 0 when finished.
 */
int run_game();

#endif /* GAME_EXECUTER_H_ */
