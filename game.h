/*
 * game:
 *	Module for holding the structures for one game and a sudoku board.
 *	Has all the function for these structure and the functions for the user commands
 *	that does not use backtracking or linear programming.
 */

#ifndef GAME_H_
#define GAME_H_


#include "cmd_parser.h"
#include "error_msg.h"
#include "moves_list.h"
#include "MainAux.h"
/**
 * BOARD: Structure that holds informations about the board that is currently being used in game.
 * Fields:
 * brows+bcols: The number of rows and columns of a block in the board.
 * values: Matrix of the current values in the matrix.
 * fixed: Matrix of 1s and 0s. Has 1s where the cell is fixed.
 * errors: Matrix of 1s and 0s. Has 1s where the cells have an error.
 * (Number of 1s should always be %2=0)
 * board status:
 * 				0=normal.
 * 				1=solved.
 * 				2=has mistakes in it.
 */
typedef struct board{
	int brows;
	int bcols;
	int **values;
	int **fixed;
	int **errors;
	int board_status;
}BOARD;
/**
 * GAME: Current game.
 * Fields:
 * mode: What mode the game is currently in.
 * 		0=init
 * 		1=edit
 * 		2=solve
 * board: The boards that is currently being used.
 * actions_dll: DLL that has the actions that were performed on the board.
 * mark_err:
 * 			0=Default, mark errors in solve mode.
 * 			1=Don't mark errors in solve mode.
 * restart_req:
 * 			   0=there wasn't a request to restart.
 * 			   1=there was a request to restart.
 */
typedef struct game{
	int mode;
	BOARD *board;
	MOVES_LIST *actions_dll;
	int mark_err;
	int restart_req;
}GAME;

/*BOARD STRUCT FUNCTION-START*/
/**
 * Allocates memory and initialises new board structure with
 * brows rows in one block, and bcols columns in block.
 * @parameters: two non negative integers
 * @return: pointer to new board structure.
 */
BOARD *create_new_board(int brows, int bcols);

/**
 * Deallocates the space given for the board and it's fields.
 * @params: the board to be freed.
 */
void delete_board(BOARD *board);
/*BOARD STRUCT FUNCTION-END*/

/*GAME STRUCT FUNCTION-START*/
/**
 * Allocates memory and initialises new game structure.
 * @return: pointer to new initialised game structure.
 */
GAME *create_new_game();
/**
 * Deallocates memory given to the game structure.
 * @params: the game to be freed.
 */
void terminate_game(GAME *game);
/*GAME STRUCT FUNCTION-END*/

/*VALIDATION-START*/
/**
 * Check if the command name is valid.
 * @params: the valid status of a line,
 * the indicator of the modes the line is available in,
 * the current game mode indicator.
 * @return: 0 if the command is not valid or isn't available in current mode.
 * 			1 otherwise.
 */
int valid_cmd_name_v2(int line_validstat, int line_modstat, int cur_gamemod);

/**
 * Check if the correct number of parameters was given for the command.
 * @params: the valid status of a line, the CMD_LINE structure of the line.
 * @return: 0 if incorrect.
 * 			1 if correct.
 */
int correct_num_of_params_v2(int line_validstat,CMD_LINE *command);

/**
 * Check if parameters are in the right range/type for the command.
 * @params: the CMD_LINE structure of the line, it's valid state, the board the line was used on
 * @return: 0 if not, 1 if yes.
 */
int correct_param_range(CMD_LINE *command,int validstat,BOARD *board);

/**
 * For commands that require the board to be in a specific state,
 * check if the board is in the needed state.
 * @params: the CMD_LINE structure of the line, the board the line was used on
 * @return: 0 if not, 1 if yes.
 */
int board_valid_for_command_v2(CMD_LINE *line, BOARD *board);

/*
 * checks if board is erroneous and change the board status accordingly
 * board - the game board
 */
void is_board_erroneous(BOARD* board);
/*VALIDATION-END*/

/*SEMI COMMANDS-START*/
/**
 * Write block-rows, block-columns and the values of m with '.'/' ' depending on fixed matrix into file.
 * @params: m the matrix of sudoku board values.
 * fixed the matrix that saves 1 whereever there is a fixed cell.
 * brows+bcols the dimensions of a block in the board.
 * path- the path to write to.
 * mode- what is the current game mode.
 * @return: 1 there was an error and the board didn't save, 0 otherwise.
 */
int write_board_to_file(int **m,int **fixed, int brows, int bcols, char *path,int mode);

/**
 * Load a text file from path into a BOARD structure.
 * @params: the path of the file and the GAME structure of the currently running game so we can change it's board.
 * @return:
 * 2 if the file couldn't be reached or the block rows/columns that are written there are invalid.
 * 1 if during the loading process there was encountered an invalid character
 * 0 if the file in path was loaded into the current game board successfully
 */
int load_file_into_board(char *path, GAME *game);

/*
 * undo a move previously done by the user.
 * lst - the moves list
 * board - the game board
 * part_of_reset - 1 if undo was called by reset function, 0 otherwise.
 * if called by reset, changes in the board will not be printed
 */
int undo(MOVES_LIST* lst,BOARD* board,int part_of_reset);

/*
 * undo a move previously done by the user.
 * lst - the moves list
 * board - the game board
 */
int redo(MOVES_LIST* lst,BOARD* board);

/*
 * finds for each value whether he is legal for the cell board[i][j] or not
 * at the end of the function, possible_values[0] is the total number of legal
 * values for this cell. for each i in the range of legal values for the board,
 * possible_values[i] = 1 if i is legal for this cell, and
 * possible_values[i] = 0 if not
 */
void find_possible_values(int* possible_values,BOARD* board,int i,int j);
/*SEMI-COMMANDS-END*/

/*GAME COMMANDS-START*/

/**
 * Move game into solve mode with a board that is loaded from path.
 * @params: initialised GAME structure, the path of the file to be loaded
 * @return:
 * 1 if wasn't successful
 * 0 if was successful
 */
int solve(GAME *game, char *path);

/**
 * Move game into edit mode with a board that is loaded from path.
 * (if path is null then the board given is an empty 9*9 board)
 * @params: initialised GAME structure, the path of the file to be loaded
 * @return:
 * 1 if wasn't successful
 * 0 if was successful
 */
int edit(GAME *game,char *path);

/**
 * Changes the mark_err field of the current game to X.
 * @params: the GAME structure of the current game, integer to change mark_err to.
 */
void mark_err(GAME *game,int X);

/**
 * print the currently used board to stdout.
 * @params: the board to print. The indicator of the current mode in the game. The indicator of mark_err in current game.
 */
void print_board(BOARD *board,int mode,int show_err);

/*
 * resets the board to its original loaded state
 * lst - the moves list
 * board - the game board
 */
int reset(MOVES_LIST* lst,BOARD* board);

/**
 * Print an "exiting" message to stdout and terminate the current game.
 * @params: the GAME structure that we want to exit from.
 */
void myexit(GAME *game);
/*GAME COMMANDS-END*/

#endif /* GAME_H_ */
