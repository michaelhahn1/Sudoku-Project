/*
 * module that contains functions related to solving the board,
 * and functions that use the gurobi_solver model
 */
#ifndef SOLVER_H_
#define SOLVER_H_

#include "gurobi_solver.h"
#include "game.h"
#include "MainAux.h"
#include "moves_list.h"
/*
 * sets cell (y,x) to z and updates the moves list
 * board - the game board
 * mode - the game mode
 * x - the column
 * y - the row
 * z - the new value
 * lst - the moves list
 * @return - 0 if the cell was set successfully, -1 otherwise
 */
int set(BOARD* board,int mode,int x,int y, int z,MOVES_LIST* lst);

/*
 * check wether the board is solved
 * game - the game
 */
void check_if_board_solved(GAME* game);

/*
 * checks wether the board is full (no empty cells)
 * board - the game board
 * N - the number of rows/columns in the board
 */
int is_board_full(BOARD* board, int N);

/*
 * autofill obvious cells in the board
 * board - the game board
 * mode - the game mode
 * lst - the moves list
 *  * @return - 0 if the command was successful, -1 otherwise
 */
int autofill(BOARD *board,int mode,MOVES_LIST *lst);

/*
 * validate that the board is solvable
 * board - the game board
 * @return - 0 if the board is solvable, 1 if not and -1 if there was an error
 */
int validate(BOARD* board);

/*
 * hints the user the value of cell (y,x)
 * board - the game board
 * x - the column
 * y - the row
 */
void hint(BOARD* board,int x, int y);

/*
 * generate a new board
 * x - number of cells to fill
 * y - number of cells that are not deleted from the solve board
 * lst - the moves list
 * @return - 0 if successful, -1 otherwise
 */
int generate(BOARD* board, int x, int y,MOVES_LIST* lst);

/*
 * copies the empty_cells_array in the generate function
 */
void copy_empty_cells_array(int** original,int** duplicate,int N);

/*
 * guesses a solution to the board
 * board - the game board
 * x - the threshold
 * lst - the moves list
 * @return - 0 if successful, -1 otherwise
 */
int guess(BOARD* board,double x,MOVES_LIST* lst);

/*
 * hints the user the guess of cell (y,x)
 * board - the game board
 * x - the column
 * y - the row
 */
void guess_hint(BOARD* board, int x, int y);

/*
 * finds all empty cells in the board and enters their locations to empty_cells_array
 * board - the game board
 * empty_cells_array - a 2D array. empty_cells_array[i][0] is the row of the i'th cell and
 *  empty_cells_array[i][1] is his column
 *  @return - the number of empty cells
 */
int find_empty_cells(BOARD* board,int** empty_cells_array);

/*
 * fills x empty cells as part of the generate function. in each iteration the function
 * draws a random number in the range of the number of remaining empty cells and inserts
 * a random legal value to the cell that this number represents. then the function deletes
 * the filled cell from empty_cells_array
 * board - the game board
 * empty_cells_array - a 2D array which contains all the empty cells in the board
 * number_of_empty_cells - number of empty cells in the board
 * x - number of cells we need to fill
 * N - number of rows\columns in the board
 */
int fill_x_empty_cells(BOARD* board,int** empty_cells_array,int number_of_empty_cells,int x,int N);

/*
 * draws a random value from the possible values of the current cell
 * possible values - an array which represents the possible values of a cell
 * N - the number of rows/columns in the board
 * @return - if successful, return the legal value, otherwise return 0
 */
int draw_random_possible_value(int possible_values[],int N);

/*
 * remove a filled cell from a 2D array which represents the empty cells in the board,
 * as part of fill_x_empty_cells function
 * current - the index of the cell thats need to be removed
 * empty_cells_array - a 2D array which contains all the empty cells in the board
 * N - the number of rows/columns in the board
 */
void remove_filled_cell_from_2d_array(int current,int** empty_cells_array,int N);

/*
 * chooses y cells randomly and clears all the remaining cells from the board,
 * as part of the generate function
 * board - the board
 * y - number of cells not to clear
 * N - the number of rows/columns in the board
 */
void empty_all_but_y_cells(int** board,int y,int N);

/*
 * remove a chosen cell from an array, as part of empty_all_but_y_cells function
 * index - the chosen cell's index
 * all_cells_array - the array
 * N - number of rows/columns in the board
 */
void remove_filled_cell_from_1d_array(int index,int* all_cells_array,int N);

/*
 * fills the board after we found a solution to the board using linear programming.
 * for each cell, we check how many values are both legal and received a score higher
 * or equal to x. if there is one such value, we set the cell to this value, and if there
 * are multiple values we randomly choose which value will be placed at the cell, such that
 * the higher the score of a value, the better chance he will be chosen
 * board - the game board
 * solution - soluiton[0] contains the values of all variable as returned from the LP function
 * N - the number of row/columns in the board
 * x - the threshold
 * mapping_arr - the mapping array for a value between 1 to N for each cell in the board
 * possible_values- an array which represents the legal values for
 *
 */
void fill_guessed_board(BOARD* board,double** solution,int N, double x,int*** mapping_arr);

/**
 * Write the contents of the board to the given file path.
 */
void save_board(GAME *game,char *path);

#endif /* SOLVER_H_ */
