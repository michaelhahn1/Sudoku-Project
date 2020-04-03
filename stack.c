

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"


void init_stack_node(stack_node* cell,int* current_index,int* possible_values,int current,int N){
	int* node_possible_values = calloc(N+1,sizeof(int));
	int i;
	if(node_possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<N+1;i++){
		node_possible_values[i] = possible_values[i];
	}
	cell->cell_row= current_index[0];
	cell->cell_col = current_index[1];
	cell->possible_values = node_possible_values;
	cell->current_cell_value = current;
	cell-> next = NULL;
}

void init_stack(stack* my_stack){
	my_stack->first = NULL;
	my_stack->number_of_nodes=0;
}

void insert_to_stack(stack* my_stack,stack_node* node){
	node->next = my_stack->first;
	my_stack->first = node;
	my_stack->number_of_nodes+=1;
}

void delete_top_of_the_stack(stack* my_stack){
	stack_node* temp = my_stack->first;
	if(my_stack->number_of_nodes==1){
		free(temp->possible_values);
		free(temp);
		my_stack->first = NULL;
		my_stack->number_of_nodes = 0;
	}
	else{
		my_stack->first = my_stack->first->next;
		free(temp->possible_values);
		free(temp);
		my_stack->number_of_nodes-=1;
	}

}

