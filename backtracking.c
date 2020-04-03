#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "backtracking.h"


void num_solutions(BOARD* board){
	stack_node* cell;
	stack* my_stack;
	int counter=0;
	int current_index[2] = {0,0};
	int N = board->brows*board->bcols;
	int* possible_values= calloc(N+1,sizeof(int));
	int current_cell_value;
	if(possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	find_next_index(board,current_index);
	if (current_index[0]== N){
		printf("number of solutions to the board is: %d\n",1);
		free(possible_values);
		return;
	}
	find_possible_values(possible_values,board,current_index[0],current_index[1]);
	current_cell_value = find_next_legal_value(0,possible_values,N);
	board->values[current_index[0]][current_index[1]] = current_cell_value;
	cell = (stack_node*)malloc(sizeof(stack_node));
	if(cell==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	init_stack_node(cell,current_index,possible_values,current_cell_value,N);
	my_stack = (stack*)malloc(sizeof(stack));
	if(my_stack==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	init_stack(my_stack);
	insert_to_stack(my_stack,cell);
	find_next_index(board,current_index);
	current_cell_value=0;
	while(my_stack->number_of_nodes>0){ /*stack is not empty*/
		if (current_index[0]==N){ /*we reached the end of the board*/
			counter+=1;
			delete_top_of_the_stack(my_stack);
			board->values[N-1][N-1] = 0;
			if(my_stack->number_of_nodes>0){
			current_index[0] = my_stack->first->cell_row;
			current_index[1] = my_stack->first->cell_col;
			}
		}
		else
		{
			if((my_stack->first->cell_row!=current_index[0])||
					(my_stack->first->cell_col!=current_index[1])){/*we reached a new cell*/
				find_possible_values(possible_values,board,current_index[0],current_index[1]);
				current_cell_value = find_next_legal_value(0,possible_values,N);
				board->values[current_index[0]][current_index[1]] = current_cell_value;
				cell = (stack_node*)malloc(sizeof(stack_node));
				if(cell==NULL){
					printf("ALLOCATION FAILURE. Exiting... . .  .\n");
					exit(1);
				}
				init_stack_node(cell,current_index,possible_values,current_cell_value,N);
				insert_to_stack(my_stack,cell);
				find_next_index(board,current_index);
				current_cell_value=0;

			}
			else{
				current_cell_value = find_next_legal_value(my_stack->first->current_cell_value,
						my_stack->first->possible_values,N);
				if (current_cell_value==0){
					delete_top_of_the_stack(my_stack);
					board->values[current_index[0]][current_index[1]] = 0;
					if(my_stack->number_of_nodes>0){
						current_index[0] = my_stack->first->cell_row;
						current_index[1] = my_stack->first->cell_col;
					}
					continue;
				}
				board->values[current_index[0]][current_index[1]] = current_cell_value;
				my_stack->first->current_cell_value = current_cell_value;
				find_next_index(board,current_index);
			}
		}
	}
	free(my_stack);
	free(possible_values);
	printf("number of solutions to the board is: %d\n",counter);
	return;
}

void find_next_index(BOARD* board,int *current_index){
	int N = board->brows*board->bcols;
	if (board->values[current_index[0]][current_index[1]] ==0){
		return;
	}
	while(1){
		if (current_index[1]+1==N){
			current_index[0]+=1;
			current_index[1]=0;
			if (current_index[0]==N){
				return;
			}
		}
		else{
			current_index[1]+=1;
		}
		if(board->values[current_index[0]][current_index[1]]==0){
			return;
		}
	}
}

int find_next_legal_value(int start,int* possible_values,int N){
	int i;
	for(i=start+1;i<N+1;i++){
		if (possible_values[i]==1){
			return i;
		}
	}
	return 0;
}
