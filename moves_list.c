#include <stdlib.h>
#include <stdio.h>
#include "moves_list.h"

void initialize_move(MOVE* move){
	move->first_part_of_move = NULL;
	move->next = NULL;
	move->prev = NULL;
	move->is_starting_state = 0;
}
void add_move_to_moves_list(MOVES_LIST* lst, MOVE* move) {
	clear_redo_part(lst);
	move->prev = lst->pointer_to_current_node;
	lst->pointer_to_current_node->next = move;
	lst->pointer_to_current_node = lst->pointer_to_current_node->next;
}

void clear_redo_part(MOVES_LIST* lst) {
	MOVE* current_move = lst->pointer_to_current_node->next;
	MOVE* next_move = NULL;
	if (current_move==NULL){
		return;
	}
	while (current_move->next != NULL ) {
		next_move = current_move->next;
		delete_move(current_move);
		current_move = next_move;
	}
	if (current_move != NULL ) {
		delete_move(current_move);
	}
	lst->pointer_to_current_node->next = NULL;
}

void delete_move(MOVE* move) {
	if(move->is_starting_state!=1){
		delete_parts_of_move(move->first_part_of_move);
	}
	free(move);
}

void delete_parts_of_move(PART_OF_MOVE* head) {
	PART_OF_MOVE* current_part = head;
	PART_OF_MOVE* next_part = NULL;
	if (current_part==NULL){
		return;
	}

	while (current_part->next != NULL ) {
		next_part = current_part->next;
		free(current_part);
		current_part = next_part;
	}
	if (current_part != NULL ) {
		free(current_part);
	}
}
void add_part_of_move_to_current_move(MOVE* current_move, int i, int j,int previous_value,
		int updated_value)
{
	PART_OF_MOVE* temp = current_move->first_part_of_move;
	PART_OF_MOVE* new_part_of_move = (PART_OF_MOVE*) malloc(sizeof(PART_OF_MOVE));
	if(new_part_of_move==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	initialize_part_of_move(new_part_of_move, i, j, previous_value,
			updated_value);
	current_move->first_part_of_move = new_part_of_move;
	new_part_of_move->next = temp;
}
void initialize_part_of_move(PART_OF_MOVE* new_part_of_move, int i, int j,
		int previous_value, int updated_value) {
	new_part_of_move->i = i;
	new_part_of_move->j = j;
	new_part_of_move->previous_value = previous_value;
	new_part_of_move->updated_value = updated_value;
}

void delete_moves_list(MOVES_LIST* lst){
	if(!lst){
		return;
	}
	if(lst->head==NULL){
		free(lst);
		return;
	}
	lst->pointer_to_current_node = lst->head;
	clear_redo_part(lst);
	delete_move(lst->head);
	free(lst);

}
MOVES_LIST *create_empty_moves_list(){
	MOVE* move = (MOVE*)calloc(1,sizeof(MOVE));
	MOVES_LIST *res=(MOVES_LIST*)malloc(sizeof(MOVES_LIST));
	if(move==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	move->first_part_of_move = NULL;
	move->next = NULL;
	move->prev = NULL;
	move->is_starting_state=1;
	if(res==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	res->head=move;
	res->pointer_to_current_node=move;
	return res;
}

void clear_moves_list(MOVES_LIST *lst){
	lst->pointer_to_current_node = lst->head;
	clear_redo_part(lst);

}
