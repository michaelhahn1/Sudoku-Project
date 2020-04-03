/*
 * backtracking:
 * module that is used to calculate num_solutions function
 */
#ifndef BACKTRACKING_H_
#define BACKTRACKING_H_

#include "game.h"
#include "solver.h"
#include "stack.h"

/*BACKTRACKING FUNCTIONS*/

/*
 * calculate the number of the possible solution to a given board using
 * exhaustive backtracking
 * board - the game board
 *`@return -  number of possible solutions
*/
void num_solutions(BOARD* board);

/*
 * finds the index of the next empty cell in the board
 * board - the game board
 * current_index - an int array of size 2, represents the row and column
 * of an index in the board
 * @return - updates @param current_index to the next empty index location.
 * if no such index exists, updates the value of current_index[0] to N.
 */
void find_next_index(BOARD* board,int *current_index);

/*
 * finds the next legal value in increasing order for a given cell.
 * start - an index
 * possible_values- an array which represents the legal values for
 * a specific cell
 * N = number of rows\columns in the board
 * @return - index of the next legal value
 */
int find_next_legal_value(int start,int* possible_values,int N);


#endif /* BACKTRACKING_H_ */
