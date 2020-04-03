/*
 * a module that contains the stack which we use in the backtracking model
 */
#ifndef STACK_H_
#define STACK_H_

/**
 * a structure that represents a node in the struct stack. each node keeps
 * the information of a cell in a board
 * cell_row: the row of the cell in a board which this node represents
 * cell_col: the column of the cell in a board which this node represents
 * possible_values: the possible values of the cell in a board which this node represents
 * next: pointer to the next node in the stack
 *
 */
typedef struct stack_node {
	int cell_row;
	int cell_col;
	int* possible_values;
	int current_cell_value;
	struct stack_node* next;
}stack_node;
/**
 * a structure that represents the stack which the num_solutions functions uses.
 * first: pointer to the first node in the stack
 * number_of_nodes: number of nodes in the stack
 *
 */


typedef struct stack {
	struct stack_node* first;
	int number_of_nodes;
}stack;


/*
 * initializes a stack node to represent a cell in the board
 * cell - a pointer to the node the function initializes
 * current_index - an array of size 2 which represents the location of the cell
 * in the board
 * possible_values - an array which represents the possible values for the cell
 * current - current cell value
 * N - number of rows in the board
 */
void init_stack_node(stack_node *cell,int* current_index,int* possible_values,int current,int N);

/*
 * initializes a stack
 * my_stack - a pointer to the stack th function initializes
 */
void init_stack(stack* my_stack);

/*
 * inserts a node to the top of the stack
 * my_stack - pointer to the stack
 * node - pointer to the node
 */
void insert_to_stack(stack* my_stack,stack_node* node);

/*
 * deletes the top of the stack
 * my_stack - a pointer to the stack
 */
void delete_top_of_the_stack(stack* my_stack);

#endif /* STACK_H_ */
