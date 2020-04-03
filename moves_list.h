/*
 * error_msg:
 * module that contains the struct and the functions of the redo/undo list
 */

#include <stdlib.h>
#include <stdio.h>
#include "error_msg.h"
#ifndef MOVES_LIST_H_
#define MOVES_LIST_H_


/*
 * a node in a singly linked list that represents all parts of a specific move (for example - all
 * the cells which were changed in an autofill move)
 * i - the cell's row
 * j - the cell's column
 * previous_value - the cell's value before the move was made
 * updated_value - the cell's value after the move was made
 * next - pointer to the next node in the list
 */
typedef struct PART_OF_MOVE{
	int i;
	int j;
	int previous_value;
	int updated_value;
	struct PART_OF_MOVE* next;
}PART_OF_MOVE;

/*
 * represents a move that was made in the game
 * first_part_of_move - a pointer to the head of the singly linked list of all the parts
 * of the current move
 * next - the next move in the moves list
 * prev - the previous move int the moves list
 * is_starting_state - 0 for a regular move, 1 for the move which ew create when we initialize
 * the moves list
 */
typedef struct MOVE{
	PART_OF_MOVE* first_part_of_move;
	struct MOVE* next;
	struct MOVE* prev;
	int is_starting_state;
}MOVE;

/*
 * a structure that represents a doubly linked list, each node in the list
 * represents a move in the game
 * head - pointer to the head of the list
 * pointer_to_current_node - pointer to the node that represents the state the game board
 * is currently in
 */
typedef struct MOVES_LIST{
	MOVE* head;
	MOVE* pointer_to_current_node;
}MOVES_LIST;

/*
 * Initialises a move
 */
void initialize_move(MOVE* move);

/*
 * adds a move to the end of the moves list. deletes all the moves beyond the
 * the move that pointer_to_current_node is pointing on
 */
void add_move_to_moves_list(MOVES_LIST* lst, MOVE* move);

/*
 * deletes all the moves beyond the the move that lst->pointer_to_current_node
 *  is pointing on
 */
void clear_redo_part(MOVES_LIST* lst);

/*
 * deletes a move
 */
void delete_move(MOVE* move);

/*
 * deletes all parts of the move
 */
void delete_parts_of_move(PART_OF_MOVE* head);

/*
 * adds a new part to the current move, which represents a change in the board
 */
void add_part_of_move_to_current_move(MOVE* current_move, int i, int j,
		int previous_value, int updated_value);
/*
 * initializes a part of move
 */
void initialize_part_of_move(PART_OF_MOVE* new_part_of_move, int i, int j,
		int previous_value, int updated_value);

/*
 * deletes the moves list
 */
void delete_moves_list(MOVES_LIST* lst);

/*
 * creates a new moves list and initializes it. returns a pointer to the list
 */
MOVES_LIST *create_empty_moves_list();
/*
 * deletes all moves in the moves list besides the starting move
 */
void clear_moves_list(MOVES_LIST *lst);
#endif /* MOVES_LIST_H_ */
