
#ifndef INTEGERLINEARPROGRAMMING_H_
#define INTEGERLINEARPROGRAMMING_H_

#include "gurobi_c.h"
#include "game.h"

/*
 * solves the board using linear programming. if a solution to the board was found,
 * updates the solution parameter to hold the solution
 * board - the game board
 * solution - a pointer to an array of type double. solution[0] will hold the values of the variables
 * if the optimization is successful
 * mapping_arr - a 3D array which will map values of cells to a variable of the
 * linear programming
 * @return - 0 if a solution was found, -1 if an error has occurred and the optimization
 * wasn't completed, 1 if the optimization was completed but no solution was found
 */
int LP(BOARD* board,double** solution,int*** mapping_arr);

/*
 * solves the board using integer linear programming. if a solution to the board was found,
 * updates board_copy to hold the solution.
 * board - the game board
 * board_copy - copy of board->values
 * @return - 0 if a solution was found, -1 if an error has occurred and the optimization
 * wasn't completed, 1 if the optimization was completed but no solution was found
 */
int ILP(BOARD* board,int** board_copy);

/*
 * This function calculates how many variables are needed for solving the board via
 * the gurobi optimizer, and maps a value at a specific cell to the variable that represents
 * it, if there is one. the mapping works the following way: if a cell at row: i, column:j in
 * the board already has a value v, than mapping_arr[i][j][v] = -1*v (which doesn't represent
 * a variable.
 * if v is not legal for this cell and this cell is empty, then mapping_arr[i][j][v] = -1000
 * if v is legal for this cell and this cell is empty, than mapping_arr[i][j][v] is the number
 * of the variable which stand for this value at this cell
 * board - the game board
 * mapping_arr - the mapping array for a value between 1 to N for each cell in the board
 * possible_values- an array which represents the legal values for
 * a specific cell
 * N number of rows/columns in the board
 * result - an array of size 2. if result[0] = 0 then the mapping was successful.
 * if return[0] = 1 then there was an empty cell and there wasn't a legal value for it, and
 *, therefore, there is no solution to the board. return[1] is the number of variables needed
 * for solving the board.
 */
void fill_mapping_array(BOARD* board, int*** mapping_arr, int* possible_values,int N,int* result);
/*
 * add constraints to the model. the constraints added are:
 * 1) each cell has one value
 * 2) each row/column/block has exactly one occurrence of each value
 * env - gurobi environment
 * model - gurobi model
 * board - the game board
 * mapping_arr - a 3D array which will map values of cells to a variable of the
 * linear programming
 * N number of rows/columns in the board
 *
 */
int add_constraints(GRBenv *env,GRBmodel* model,BOARD* board,int*** mapping_arr, int N);
/*
 * sets the objective function for the gurobi optimizer. is is_binary=1 (meaning we are in ILP),
 * the objective function is set to 0.
 * if is_binary=0 (meaning we are in LP), every variable
 * is given a random weight between 1/n and 1, and the maximisation of the variables is calculated.
 * env - gurobi environment
 * model - gurobi model
 * ind - indexes of the variables
 * vtype - type of the variables
 * number_of_variables - number of variables in the model
 * is_binary - 1 for binary variables, 0 for continuous variables
 */
int objective_function(GRBenv *env, GRBmodel *model,int* ind,char* vtype,int number_of_variables,int is_binary,int N);
/*
 * returns a random number from the following list: 1/N, 2/N, 3/N ........(N-1)/N, 1
 */
double draw_random_number(int N);
/*
 * runs linear programming and integer linear programming using gurobi optimizer
 * board - the game board
 * is binary - if equals 0, then the function will run LP. otherwise, the function will
 * run LP
 * solution - a pointer to an array of type double. solution[0] will hold the values of the variables
 * if the optimization is successful
 */
int run_linear_programming_using_gurobi(BOARD* board,int is_binary,double** sol,int*** mapping_arr);

/*
 * updates board_copy to contain the solution for the board, as found by the
 * gurobi optimizer
 * mapping_arr - a 3D array which will map values of cells to a variable of the
 * linear programming
 * solution - a pointer to an array of type double. solution[0] will hold the values of the variables
 * if the optimization is successful
 * board_copy - a copy of board->values, when board is the game board
 * N - number of rows/columns in the board
 */
void solved_board_for_ilp(int*** mapping_arr,double** sol,int** board_copy,int N);

/*
 * solves the board using linear programming. if a solution to the board was found,
 * updates the solution parameter to hold the solution
 * board - the game board
 * solution - a pointer to an array of type double. solution[0] will hold the values of the variables
 * if the optimization is successful
 * mapping_arr - a 3D array which will map values of cells to a variable of the
 * linear programming
 * @return - 0 if a solution was found, -1 if an error has occurred and the optimization
 * wasn't completed, 1 if the optimization was completed but no solution was found
 */
int LP(BOARD* board,double** solution,int*** mapping_arr);

/*
 * solves the board using integer linear programming. if a solution to the board was found,
 * updates board_copy to hold the solution.
 * board - the game board
 * board_copy - copy of board->values
 * @return - 0 if a solution was found, -1 if an error has occurred and the optimization
 * wasn't completed, 1 if the optimization was completed but no solution was found
 */
int ILP(BOARD* board,int** board_copy);

/*
 * frees the variables of run_linear_programming_using_gurobi
 */
void free_func_variables(GRBenv* env, GRBmodel* model,int* possible_values,int* ind,char* vtype, int* mapping_array_result);

/*
 * frees the mapping array
 */
void free_mapping_arr(int*** mapping_arr,int N);

#endif /* INTEGERLINEARPROGRAMMING_H_ */

