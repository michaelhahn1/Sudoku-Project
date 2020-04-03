

#include <stdlib.h>
#include <stdio.h>
#include "solver.h"
#include <math.h>

int set(BOARD* board,int mode,int x,int y, int z,MOVES_LIST* lst){
	MOVE* set_move = (MOVE*)malloc(sizeof(MOVE));
	if(set_move==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	is_board_erroneous(board);
	if(mode==2 && board->fixed[y-1][x-1]==1){
		fixed_cell_msg(y,x);
		free(set_move);
		return -1;
	}

	initialize_move(set_move);
	add_part_of_move_to_current_move(set_move,y-1/*x-1*/,x-1/*y-1*/,board->values[y-1][x-1],z);
	board->values[y-1][x-1] = z;
	is_board_erroneous(board);
	add_move_to_moves_list(lst,set_move);
	return 0;
}

void check_if_board_solved(GAME* game){
	int board_is_full = is_board_full(game->board,game->board->bcols*game->board->brows);
	if(board_is_full){
		if(game->board->board_status==2){
			printf("This solution is erroneous.\n");
		}
		else{
			printf("The board was solved successfully! Going back to INIT mode.\n");
			game->mode=0;
			game->board->board_status = 1;
		}
	}
}
int is_board_full(BOARD* board, int N){
	int i,j;
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			if (board->values[i][j]==0){
				return 0;
			}
		}
	}
	return 1;
}

int  autofill(BOARD* board,int mode,MOVES_LIST* lst){
	MOVE* autofill_move = (MOVE*)malloc(sizeof(MOVE));
	int i,j,k;
	int N = board->bcols*board->brows;
	int** temp_board = create_int_matrix(N,N);
	int *possible_values = calloc(N+1,sizeof(int));
	if(autofill_move==NULL||possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	if (mode!=2){
		printf("this command is only available in solve mode\n");
		free(autofill_move);
		free(possible_values);
		free_int_matrix(temp_board,N);
		return -1;
	}
	initialize_move(autofill_move);
	copy_board(board->values,temp_board,N);
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			find_possible_values(possible_values,board,i,j);
			if (possible_values[0]==1){
				for(k=1;k<N+1;k++){
					if (possible_values[k]==1&&board->values[i][j]!=k){
						temp_board[i][j] = k;
						add_part_of_move_to_current_move(autofill_move,i,j,board->values[i][j],k);
						printf("the cell %d,%d has been set to %d\n",i+1,j+1,k);
					}
				}
			}
		}
	}
	copy_board(temp_board,board->values,N);
	add_move_to_moves_list(lst,autofill_move);
	free_int_matrix(temp_board,N);
	free(possible_values);
	return 0;
}

int validate(BOARD* board){
	int error=0;
	int N = board->bcols*board->brows;
	int **temp_board = create_int_matrix(N,N);
	copy_board(board->values,temp_board,N);
	error=ILP(board,temp_board);
	if(error==1){
		printf("board is not solvable\n");	}
	else if(error==-1){
		printf("program was not able to complete the validate function\n");
	}
	else{
		printf("board is solvable\n");	}
	free_int_matrix(temp_board,N);
	return error;
}

void hint(BOARD* board,int x, int y){
	int error=0;
	int N = board->bcols*board->brows;
	int **temp_board = create_int_matrix(N,N);
	if (board->board_status==2){
		printf("Hint couldn't execute command because board is erroneous.\n");
		return;
	}
	if (board->fixed[y-1][x-1]==1){
		printf("Cell (%d,%d) is fixed. You can't change fixed cells.\n",y-1,x-1);
		return;
	}
	if (board->values[y-1][x-1]!=0){
		printf("This cell already contains a value\n");
		return;
	}
	copy_board(board->values,temp_board,N);
	error=ILP(board,temp_board);
	if(error){
		printf("board is unsolvable\n");
	}
	else{
		printf("The hint for cell %d %d is %d\n",x,y,temp_board[y-1/*x-1*/][x-1/*y-1*/]);
	}
	free_int_matrix(temp_board,N);
}
int generate(BOARD* board, int x, int y,MOVES_LIST* lst){
	MOVE* generate_move = (MOVE*)malloc(sizeof(MOVE));
	int i,j;
	int success,found_solution=0,error;
	int number_of_empty_cells;
	int N = board->bcols*board->brows;
	int **temp_board = create_int_matrix(N,N);
	int **original_board = create_int_matrix(N,N);
	int **empty_cells_array = calloc(N*N,sizeof(int*));
	int **empty_cells_array_copy = calloc(N*N,sizeof(int*));
	if(generate_move==NULL||empty_cells_array==NULL||empty_cells_array_copy==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for (i=0;i<N*N;i++){
		empty_cells_array[i] = calloc(2,sizeof(int));
		empty_cells_array_copy[i] = calloc(2,sizeof(int));
		if(empty_cells_array[i]==NULL||empty_cells_array_copy[i]==NULL){
			printf("ALLOCATION FAILURE. Exiting... . .  .\n");
			exit(1);
		}
	}
	copy_board(board->values,original_board,N);
	copy_board(board->values,temp_board,N);
	number_of_empty_cells = find_empty_cells(board,empty_cells_array);
	if (number_of_empty_cells<x){
		printf("cannot perform generate. board contains less than %d empty cells\n",x);
		for (i=0;i<N*N;i++){
			free(empty_cells_array[i]);
			free(empty_cells_array_copy[i]);
		}
		free(empty_cells_array);
		free(empty_cells_array_copy);
		free_int_matrix(temp_board,N);
		free_int_matrix(original_board,N);
		free(generate_move);
		return -1;
	}
	for (i=0;i<1000;i++){
		copy_empty_cells_array(empty_cells_array,empty_cells_array_copy,N);
		success = fill_x_empty_cells(board,empty_cells_array_copy,number_of_empty_cells,x,N);
		if(success){
			copy_board(board->values,temp_board,N);
			error = ILP(board,temp_board);
			if(!error){
				found_solution = 1;
				break;
			}
		}
		copy_board(original_board,board->values,N);
	}
	if (found_solution){
		initialize_move(generate_move);
		copy_board(temp_board,board->values,N);
		empty_all_but_y_cells(temp_board,y,N);
		copy_board(temp_board,board->values,N);
		for (i=0;i<N;i++){
			for (j=0;j<N;j++){
				if (temp_board[i][j]!=original_board[i][j]){
					add_part_of_move_to_current_move(generate_move,i,j,original_board[i][j],
							temp_board[i][j]);
				}
			}
		}
		copy_board(temp_board,board->values,N);
	}
	else{
		printf("error in puzzle generator\n");
		free(generate_move);
		return -1;
	}
	for (i=0;i<N*N;i++){
		free(empty_cells_array[i]);
		free(empty_cells_array_copy[i]);
	}
	free(empty_cells_array);
	free(empty_cells_array_copy);
	free_int_matrix(temp_board,N);
	free_int_matrix(original_board,N);
	add_move_to_moves_list(lst,generate_move);
	return 0;

}

void copy_empty_cells_array(int** original,int** duplicate,int N){
	int i;
	for (i=0;i<N*N;i++){
		duplicate[i][0] = original[i][0];
		duplicate[i][1] = original[i][1];
	}
}

int guess(BOARD* board,double x,MOVES_LIST* lst){
	MOVE* guess_move = (MOVE*)malloc(sizeof(MOVE));
	double **solution = calloc(1,sizeof(double*));
	int error,i,j;
	int N = board->bcols*board->brows;
	int** original_board = create_int_matrix(N,N);
	int ***mapping_arr = calloc(N,sizeof(int**));
	if(solution==NULL||mapping_arr==NULL||guess_move==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<N;i++){
		mapping_arr[i] = calloc(N,sizeof(int*));
		if(mapping_arr[i]==NULL){
			printf("ALLOCATION FAILURE. Exiting... . .  .\n");
			exit(1);
		}
		for (j=0;j<N;j++){
			mapping_arr[i][j] = calloc(N,sizeof(int));
			if(mapping_arr[i][j]==NULL){
				printf("ALLOCATION FAILURE. Exiting... . .  .\n");
				exit(1);
			}
		}
	}
	copy_board(board->values,original_board,N);
	error = LP(board,solution,mapping_arr);
	if (error){
		printf("the program was not able to guess a solution\n");
		free(*solution);
		free(solution);
		free_mapping_arr(mapping_arr,N);
		free_int_matrix(original_board,N);
		free(guess_move);
		return -1;
	}
	initialize_move(guess_move);
	copy_board(original_board,board->values,N);
	fill_guessed_board(board,solution,N,x,mapping_arr);
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			if (board->values[i][j]!=original_board[i][j]){
				add_part_of_move_to_current_move(guess_move,i,j,original_board[i][j],
						board->values[i][j]);
			}
		}
	}
	free(*solution);
	free(solution);
	free_mapping_arr(mapping_arr,N);
	free_int_matrix(original_board,N);
	add_move_to_moves_list(lst,guess_move);
	return 0;
}
void guess_hint(BOARD* board, int x, int y){
	int i,j,error=0;
	double **solution;
	int N = board->bcols*board->brows;
	int **temp_board;
	int *possible_values = calloc(N+1,sizeof(int));
	int current_cell;
	int ***mapping_arr = calloc(N,sizeof(int**));
	if(mapping_arr==NULL||possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<N;i++){
		mapping_arr[i] = calloc(N,sizeof(int*));
		if(mapping_arr[i]==NULL){
			printf("ALLOCATION FAILURE. Exiting... . .  .\n");
			exit(1);
		}
		for (j=0;j<N;j++){
			mapping_arr[i][j] = calloc(N,sizeof(int));
			if(mapping_arr[i][j]==NULL){
				printf("ALLOCATION FAILURE. Exiting... . .  .\n");
				exit(1);
			}
		}
	}
	temp_board = create_int_matrix(N,N);
	if (board->fixed[y-1][x-1]==1){
		printf("cell is fixed\n");
		free(possible_values);
		free_int_matrix(temp_board,N);
		free_mapping_arr(mapping_arr,N);
		return;
	}
	if (board->values[y-1][x-1]!=0){
		printf("cell already contains a value\n");
		free(possible_values);
		free_int_matrix(temp_board,N);
		free_mapping_arr(mapping_arr,N);
		return;
	}
	solution = calloc(1,sizeof(double*));
	if(solution==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	copy_board(board->values,temp_board,N);
	error = LP(board,solution,mapping_arr);
	if (error){
		printf("the program was not able to guess a hint\n");
		free(*solution);
		free(solution);
		free_mapping_arr(mapping_arr,N);
		free_int_matrix(temp_board,N);
		free(possible_values);
		return;
	}
	find_possible_values(possible_values,board,y-1/*x-1*/,x-1/*y-1*/);/*RONIT*/
	for (i=0;i<N;i++){
		current_cell = mapping_arr[y-1/*x-1*/][x-1/*y-1*/][i];
		if (solution[0][current_cell]>0 && possible_values[i+1]==1){
			printf("%d has a score of %f\n",i+1,solution[0][current_cell]);
		}
	}
	free(*solution);
	free(solution);
	free_mapping_arr(mapping_arr,N);
	free_int_matrix(temp_board,N);
	free(possible_values);

}
int find_empty_cells(BOARD* board,int** empty_cells_array){
	int i,j;
	int empty_cells_counter=0;
	int N = board->bcols*board->brows;
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			if (board->values[i][j]==0){
				empty_cells_array[empty_cells_counter][0] = i;
				empty_cells_array[empty_cells_counter][1] = j;
				empty_cells_counter+=1;
			}
		}
	}
	return empty_cells_counter;
}

int fill_x_empty_cells(BOARD* board,int** empty_cells_array,int number_of_empty_cells,int x,int N){
	int i,current;
	int a,b;
	int value;
	int *possible_values = calloc(N+1,sizeof(int));
	if(possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<x;i++){
		current = rand() % number_of_empty_cells;
		a = empty_cells_array[current][0];
		b = empty_cells_array[current][1];
		find_possible_values(possible_values,board,a,b);
		value = draw_random_possible_value(possible_values,N);
		if(value==0){
			free(possible_values);
			return 0;
		}
		board->values[a][b] = value;
		remove_filled_cell_from_2d_array(current,empty_cells_array,N);
		number_of_empty_cells-=1;
	}
	free(possible_values);
	return 1;
}

int draw_random_possible_value(int possible_values[],int N){
	int number_of_legal_values = 0;
	int i;
	int index;
	for (i=1;i<N+1;i++){
		if (possible_values[i] == 1){
			number_of_legal_values+=1;
		}
	}
	if (number_of_legal_values==0){
		return 0;
	}
	index = (rand() % number_of_legal_values)+1;
	for(i=1;i<N+1;i++){
		if (possible_values[i]==1){
			index-=1;
		}
		if (index==0){
			return i;
		}
	}
	return 0;
}
void remove_filled_cell_from_2d_array(int current,int** empty_cells_array,int N){
	int i;
	for (i=current+1;i<N*N;i++){
		empty_cells_array[i-1][0] = empty_cells_array[i][0];
		empty_cells_array[i-1][1] = empty_cells_array[i][1];
	}
}

void empty_all_but_y_cells(int** board,int y,int N){
	int i,a,b;
	int index;
	int remaining_cells = N*N;
	int *all_cells_array = calloc(N*N,sizeof(int));
	int **new_board = create_int_matrix(N,N);
	if(all_cells_array==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	zerofy_int_matrix(new_board,N,N);
	for (i=0;i<N*N;i++){
		all_cells_array[i] = i;
	}
	for (i=0;i<y;i++){
		index = rand() % (remaining_cells);
		a = floor(all_cells_array[index]/N);
		b = all_cells_array[index]%N;
		new_board[a][b] = board[a][b];
		remove_filled_cell_from_1d_array(index,all_cells_array,N);

		remaining_cells-=1;
	}
	copy_board(new_board,board,N);
	free_int_matrix(new_board,N);
	free(all_cells_array);
}

void remove_filled_cell_from_1d_array(int index,int* all_cells_array,int N){
	int i;
	for (i=index+1;i<N*N;i++){
		all_cells_array[i-1] = all_cells_array[i];
	}
}

void fill_guessed_board(BOARD* board,double** solution,int N, double x,int*** mapping_arr){
	int i,j,k;
	double *values_score_by_lp = calloc(N,sizeof(double));
	int *possible_values = calloc(N+1,sizeof(int));
	int current_variable_number;
	int number_of_valid_variables=0;
	double sum_of_scores=0;
	double random_number_in_range;
	if(values_score_by_lp==NULL||possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			find_possible_values(possible_values,board,i,j);
			if (board->values[i][j]==0){
				for (k=0;k<N;k++){
					current_variable_number = mapping_arr[i][j][k];
					if(current_variable_number>=0 && solution[0][current_variable_number]>=x && possible_values[k+1]==1){
						values_score_by_lp[k] = solution[0][current_variable_number];
						number_of_valid_variables+=1;
					}
					else{
						values_score_by_lp[k] = 0;
					}
				}
				if(number_of_valid_variables==1){
					for(k=0;k<N;k++){
						if (values_score_by_lp[k]>0){
							board->values[i][j] = k+1;
						}
					}
				}
				else if (number_of_valid_variables>1){
					for (k=0;k<N;k++){
						sum_of_scores += values_score_by_lp[k];
					}
					random_number_in_range = (double)((rand()%1000000)*sum_of_scores) / 1000000;
					sum_of_scores = 0;
					for (k=0;k<N;k++){
						sum_of_scores+=values_score_by_lp[k];
						if (sum_of_scores>=random_number_in_range){
							board->values[i][j] = k+1;
							break;
						}
					}
				}
				number_of_valid_variables=0;
				for (k=0;k<N;k++){
					values_score_by_lp[k]=0;
				}

			}
			sum_of_scores = 0;
		}
	}
	free(values_score_by_lp);
	free(possible_values);
}



/*#11*/
void save_board(GAME *game,char *path){/*ASK ME*/
	/*We already know we are in Edit/Solve*/
	/*is_board_erroneous(game->board);*/
	if(game->mode==1){
		if(game->board->board_status==2){
			printf("Cannot save an erroneous board in edit mode.\n");
			return;
		}
		/*If board has mistakes in it do not save it:*/
		if(validate(game->board)==1){
			printf("Cannot save the board because in it's current state it has no solution.\n");
			return;
		}
	}
	write_board_to_file(game->board->values,game->board->fixed, game->board->brows, game->board->bcols, path,game->mode);
}
