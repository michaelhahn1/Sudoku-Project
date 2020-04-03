#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "game.h"

/*BOARD STRUCT FUNCTIONS-START*/
BOARD *create_new_board(int brows, int bcols){
	BOARD *res=(BOARD*)malloc(sizeof(BOARD));
	if(!res){
		allocation_failure();
		exit(1);
	}
	res->brows=brows;
	res->bcols=bcols;
	res->board_status=0;
	res->values=create_int_matrix(brows*bcols, brows*bcols);
	zerofy_int_matrix(res->values,brows*bcols,brows*bcols);
	res->errors=create_int_matrix(brows*bcols, brows*bcols);
	zerofy_int_matrix(res->errors,brows*bcols,brows*bcols);
	res->fixed=create_int_matrix(brows*bcols, brows*bcols);
	zerofy_int_matrix(res->fixed,brows*bcols,brows*bcols);
	return res;
}

void delete_board(BOARD *board){
	int rows;
	if(!board){
		return;
	}
	rows= board->bcols * board->brows;
	free_int_matrix(board->fixed,rows);
	free_int_matrix(board->errors,rows);
	free_int_matrix(board->values,rows);
	free(board);
}
/*BOARD STRUCT FUNCTIONS-END*/

/*GAME STRUCT FUNCITONS-START*/
GAME *create_new_game(){
	GAME *res = (GAME*)malloc(sizeof(GAME));
	if(res==NULL){
		allocation_failure();
		return NULL;
		/*XXXX EXIT.*/
	}
	res->mode=0;
	res->mark_err=1;
	res->restart_req=0;
	res->board=NULL;
	res->actions_dll=create_empty_moves_list();
	return res;
}

void terminate_game(GAME *game){
	if(!game){
		return;
	}
	delete_moves_list(game->actions_dll);

	delete_board(game->board);

	free(game);
}
/*GAME STRUCT FUNCITONS-END*/

/*VALIDATION-START*/
int valid_cmd_name_v2(int line_validstat, int line_modstat, int cur_gamemod){
	if(line_validstat==1){
		printf("The command name is invalid, this command doesn't exist.\n");
		return 0;
	}
	if(line_modstat!=0){
		if(line_modstat==2 && cur_gamemod!=2){
			incorrect_mode_err(cur_gamemod, line_modstat);
			return 0;
		}
		else if(line_modstat==3 && cur_gamemod!=1){
			incorrect_mode_err(cur_gamemod, line_modstat);
			return 0;
		}
		else{/*line_modstat==1*/
			if(cur_gamemod==0){
				incorrect_mode_err(cur_gamemod, line_modstat);
				return 0;
			}
		}
	}
	return 1;
}

int correct_num_of_params_v2(int line_validstat,CMD_LINE *command){
	if(line_validstat==2){
		incorrect_arg_num_err(0, command);
		return 0;
	}
	if(line_validstat==3){
		incorrect_arg_num_err(1, command);
		return 0;
	}
	return 1;
}

int correct_param_range(CMD_LINE *command,int validstat,BOARD *board){
	if(validstat>=4 && validstat<=6){
		arg_invalid_err(validstat-4, command);
		return 0;
	}
	if(board!=NULL){
		if(command->ct==HINT || command->ct==SET || command->ct==GUESS_HINT){
			if(atoi(command->arguments[0])<0 || atoi(command->arguments[0])>(board->bcols)*(board->brows)){/*RONIT*/
				arg_invalid_err(0, command);
				return 0;
			}
			if(atoi(command->arguments[1])<0 || atoi(command->arguments[1])>(board->bcols)*(board->brows)){/*RONIT*/
				arg_invalid_err(1, command);
				return 0;
			}
		}
		if(command->ct==SET){
			if(atoi(command->arguments[2])<0 || atoi(command->arguments[2])>(board->bcols)*(board->brows)){
				arg_invalid_err(2, command);
				return 0;
			}
		}
		if(command->ct==GENERATE){
			if(atoi(command->arguments[0])<0 || atoi(command->arguments[0])>(board->bcols)*(board->brows)*(board->bcols)*(board->brows)){
				arg_invalid_err(0, command);
				return 0;
			}
			if(atoi(command->arguments[1])<0 || atoi(command->arguments[1])>(board->bcols)*(board->brows)*(board->bcols)*(board->brows)){
				arg_invalid_err(1, command);
				return 0;
			}
		}
	}
	return 1;
}

int board_valid_for_command_v2(CMD_LINE *line, BOARD *board){
	if(line->ct==VALIDATE || line->ct==GUESS || line->ct==NUM_SOLUTIONS || line->ct==AUTOFILL){
		if(board->board_status==2){
			board_has_errors_msg(line);
			return 0;
		}
	}
	if(line->ct==HINT || line->ct==GUESS_HINT){
		if(board->board_status==2){
			board_has_errors_msg(line);
			return 0;
		}
		if(board->fixed[atoi(line->arguments[0])-1][atoi(line->arguments[1])-1]==1){
			fixed_cell_msg(atoi(line->arguments[0])-1,atoi(line->arguments[1])-1);
			return 0;
		}
		if(board->values[atoi(line->arguments[0])-1][atoi(line->arguments[1])-1]>0){
			full_cell_msg(atoi(line->arguments[0])-1,atoi(line->arguments[1])-1);
			return 0;
		}
	}
	return 1;
}

void is_board_erroneous(BOARD* board){
	int N = board->bcols*board->brows;
	int board_is_erroneous = 0;
	int *possible_values = calloc(N+1,sizeof(int));
	int i,j;
	int cell_value;
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			cell_value = board->values[i][j];
			if (cell_value!=0){
				find_possible_values(possible_values,board,i,j);
				if (possible_values[cell_value]==0){
					board_is_erroneous = 1;
					board->errors[i][j] = 1;
				}
				else{
					board->errors[i][j] = 0;
				}
			}
			else{
				board->errors[i][j] = 0;
			}
		}
	}
	if(board_is_erroneous){
		board->board_status = 2;
	}
	else if(board->board_status==2){
		board->board_status = 0;
	}
	free(possible_values);
}
/*VALIDATION-END*/

/*SEMI COMMANDS-START*/
int write_board_to_file(int **m,int **fixed, int brows, int bcols, char *path,int mode){
	FILE *fp=NULL;
	int i,j;
	int error;
	fp=fopen(path,"w");
	if(fp==NULL){
		printf("Could not open: %s\n",path);
		return 1;
	}
	/*Print dimensions of block to file*/
	fprintf(fp,"%d %d\n",brows,bcols);
	/*Print to file (brows*bcols)^2 cells*/
	for(i=0;i<(brows*bcols);i++){
		for(j=0;j<(brows*bcols);j++){
			fprintf(fp,"%d",m[i][j]);
			if(fixed[i][j]==1 || (mode==1 && m[i][j]!=0)){
				fprintf(fp,".");
			}
			if(j!=brows*bcols-1){/*if not the last cell in line*/
				fprintf(fp," ");
			}
		}
		fprintf(fp,"\n");
	}
	error=fclose(fp);
	if(error!=0){
		file_handling_failure(1);
		return 1;
	}
	return 0;
}

int load_file_into_board(char *path, GAME *game){
	/*After loading the board dryly, there should be a scan on it
	 * to keep track on the errors loaded.*/
	FILE *fp;
	BOARD *b;
	int brows,bcols,curNum,dimension,i,j;
	char sufix;
	fp=fopen(path,"r");
	if(fp==NULL){
		file_handling_failure(0);
		return 2;
	}
	if(fscanf(fp,"%d %d",&brows,&bcols)!=2){
		printf("File is not valid for loading a board because it has an invalid character.\n");
		return 2;
	}
	dimension=brows*bcols;
	b=create_new_board(brows,bcols);
	/*2)read the rest of the file an input the vals in the matrixes*/
	for(i=0;i<dimension;i++){
		for(j=0;j<dimension;j++){
			if(fscanf(fp,"%d",&curNum)!=1){
				fclose(fp);
				delete_board(b);
				return 1;
			}
			sufix=fgetc(fp);
			/*if 0. which is not legal*/
			if(curNum==0 && sufix=='.'){
				printf("File is not valid for loading a board because it has a fixed empty cell.\n");
				fclose(fp);
				delete_board(b);
				return 1;
			}
			b->values[i][j]=curNum;
			if(sufix=='.'){
				b->fixed[i][j]=1;
			}
			else if(isWhiteSpace(sufix)){
				b->fixed[i][j]=0;
			}
			else if(sufix==EOF && (i!=dimension-1 || j!=dimension-1)){
				fclose(fp);
				delete_board(b);
				return 1;
			}
			else if((i!=dimension-1 || j!=dimension-1)){
				printf("File is not valid for loading a board because it has an invalid character (%c).\n",sufix);
				fclose(fp);
				delete_board(b);
				return 1;
			}
		}
	}
	if(fclose(fp)!=0){
		file_handling_failure(1);
		delete_board(b);
		return 1;
	}
	if(game->board!=NULL){/*Free the old board since we don't need it anymore*/
		delete_board(game->board);
		game->board=NULL;
	}
	game->board=b;
	return 0;
}

int undo(MOVES_LIST* lst,BOARD* board,int part_of_reset){
	PART_OF_MOVE* current_part_of_move;
	if (lst->pointer_to_current_node->is_starting_state==1){/*the left need to happn first*/
		if(!part_of_reset){
			printf("There are no moves to undo.\n");
		}
		return -1;
	}
	current_part_of_move = lst->pointer_to_current_node->first_part_of_move;
	while(current_part_of_move!=NULL){
		board->values[current_part_of_move->i][current_part_of_move->j] = current_part_of_move->previous_value;
		if(!part_of_reset){
			printf("Cell row=%d col=%d was changed from %d to %d\n",current_part_of_move->i+1,
					current_part_of_move->j+1,current_part_of_move->updated_value,current_part_of_move->previous_value);
		}
		current_part_of_move = current_part_of_move->next;
	}
	lst->pointer_to_current_node = lst->pointer_to_current_node->prev;
	return 0;
}

int redo(MOVES_LIST* lst,BOARD* board){
	PART_OF_MOVE* current_part_of_move;
	if (lst->pointer_to_current_node==NULL||lst->pointer_to_current_node->next==NULL){
		printf("There are no moves to redo\n");
		return -1;
	}
	lst->pointer_to_current_node = lst->pointer_to_current_node->next;
	current_part_of_move = lst->pointer_to_current_node->first_part_of_move;
	while(current_part_of_move!=NULL){
		board->values[current_part_of_move->i][current_part_of_move->j] = current_part_of_move->updated_value;
		printf("Cell (%d,%d) was changed from %d to %d\n",current_part_of_move->i+1,
				current_part_of_move->j+1,current_part_of_move->previous_value,current_part_of_move->updated_value);
		current_part_of_move = current_part_of_move->next;
	}
	return 0;
}

void find_possible_values(int* possible_values,BOARD* board,int i,int j){
	int counter1 =0 ;
	int N = board->bcols*board->brows;
	int k=0,m=0;
	int block_starting_row = (i/board->brows)*board->brows;
	int block_starting_col = (j/board->bcols)*board->bcols;

	possible_values[0]=0;
	for (k=1;k<N+1;k++){
		possible_values[k]=1;
	}
	for (k=0;k<N;k++){ /*row*/
		if (k!=j){
			possible_values[board->values[i][k]] = 0;
		}
	}
	for (k=0;k<N;k++){ /*column*/
		if (k!=i){
			possible_values[board->values[k][j]] = 0;
		}
	}

	for (k=block_starting_row;k<block_starting_row+board->brows;k++){
		for (m=block_starting_col;m<block_starting_col+board->bcols;m++){
			counter1+=1;

			if (k!=i || m!=j){
				possible_values[board->values[k][m]]=0;

			}
		}

	}
	for (k=1;k<N+1;k++){
		if (possible_values[k]==1){
			possible_values[0]+=1;
		}
	}
}
/*SEMI-COMMANDS-END*/

/*THE COMMANDS-START*/
/*#1*/
int first_load_solve(GAME *game, char *path){
	int flag;
	flag=load_file_into_board(path,game);
	if(flag!=0){
		if(game->board!=NULL){
			delete_board(game->board);
			game->mode=0;
		}
		return 1;
	}
	is_board_erroneous(game->board);
	game->mode=2;
	return 0;
}

int normal_load_solve(GAME *game, char *path){
	int flag,dim;
	BOARD *temp=create_new_board(game->board->brows,game->board->bcols);
	dim=game->board->bcols*game->board->brows;

	temp->board_status=game->board->board_status;
	copy_board(game->board->values,temp->values,dim);
	copy_board(game->board->fixed,temp->fixed,dim);
	copy_board(game->board->errors,temp->errors,dim);

	flag=load_file_into_board(path,game);
	if(flag!=0){
		if(flag==1){
			if(game->board!=NULL){
				delete_board(game->board);
				game->board=create_new_board(temp->brows,temp->bcols);
				game->board->board_status=temp->board_status;
				copy_board(temp->values,game->board->values,dim);
				copy_board(temp->fixed,game->board->fixed,dim);
				copy_board(temp->errors,game->board->errors,dim);

				delete_board(temp);

				game->mode=2;
				printf("The board stayed as before.\n");
				return 1;
			}
		}
		if(flag==2){
			game->board->board_status=temp->board_status;
			copy_board(temp->values,game->board->values,dim);
			copy_board(temp->fixed,game->board->fixed,dim);
			copy_board(temp->errors,game->board->errors,dim);

			delete_board(temp);

			game->mode=2;
			printf("The board stayed as before.\n");
			return 1;
		}
	}
	delete_board(temp);
	clear_moves_list(game->actions_dll);
	is_board_erroneous(game->board);
	game->mode=2;
	return 0;
}

int solve(GAME *game,char *path){
	int ret;
	if(game->board==NULL){
		ret=first_load_solve(game,path);
	}
	else{
		ret=normal_load_solve(game,path);
	}
	return ret;
}

/*#2*/
int first_load_edit(GAME *game,char *path){
	int flag;
	if(path==NULL){
		game->board=create_new_board(3,3);
		game->mode=1;
		return 0;
	}
	flag=load_file_into_board(path,game);
	if(flag!=0){
		if(game->board!=NULL){
			delete_board(game->board);
			game->mode=0;
		}
		return 1;
	}
	is_board_erroneous(game->board);
	game->mode=1;
	return 0;
}

int normal_load_edit(GAME *game,char *path){
	int flag,dim;
	BOARD *temp;
	if(path==NULL){
		delete_board(game->board);
		clear_moves_list(game->actions_dll);
		game->board=create_new_board(3,3);
		game->mode=1;
		return 0;
	}
	temp=create_new_board(game->board->brows,game->board->bcols);
	dim=game->board->bcols*game->board->brows;

	temp->board_status=game->board->board_status;
	copy_board(game->board->values,temp->values,dim);
	copy_board(game->board->fixed,temp->fixed,dim);
	copy_board(game->board->errors,temp->errors,dim);

	flag=load_file_into_board(path,game);
	if(flag!=0){
		if(flag==1){
			if(game->board!=NULL){
				delete_board(game->board);

				game->board=create_new_board(temp->brows,temp->bcols);
				game->board->board_status=temp->board_status;
				copy_board(temp->values,game->board->values,dim);
				copy_board(temp->fixed,game->board->fixed,dim);
				copy_board(temp->errors,game->board->errors,dim);

				delete_board(temp);

				game->mode=1;
				printf("The board stayed as before.\n");
				return 1;
			}
		}
		if(flag==2){
			game->board->board_status=temp->board_status;
			copy_board(temp->values,game->board->values,dim);
			copy_board(temp->fixed,game->board->fixed,dim);
			copy_board(temp->errors,game->board->errors,dim);

			delete_board(temp);

			game->mode=1;
			printf("The board stayed as before.\n");
			return 1;
		}
	}
	delete_board(temp);
	clear_moves_list(game->actions_dll);
	is_board_erroneous(game->board);
	game->mode=1;
	return 0;
}

int edit(GAME *game,char *path){
	int ret;
	if(game->board==NULL){
		ret=first_load_edit(game,path);
	}
	else{
		ret=normal_load_edit(game,path);
	}
	return ret;
}
/*#3*/
void mark_err(GAME *game,int X){
	game->mark_err=X;
}
/*#4*/
void print_board(BOARD *board,int mode,int show_err){
	int i,j,k;
	int brows,bcols;
	int show_errors;
	int printed_cells_counter=0;
	brows=board->brows;
	bcols=board->bcols;
	if(mode==1){
		show_errors=1;
	}
	else{
		show_errors=show_err;
	}
	for(i=0;i<bcols;i++){/*for each line of blocks*/
		print_separator_row( brows, bcols);
		for(j=i*brows;j<i*brows+brows;j++){/*for each row in the line of blocks*/
			for(k=0;k<brows*bcols;k++){/*for each column in the line of blocks*/
				if(printed_cells_counter%bcols==0){
					printf("|");
				}
				if((board->values)[j][k]==0){
					printf("   ");
					printf(" ");
				}
				else{
					printf(" %2d",(board->values)[j][k]);
					/*Checks if fixed/erroneous cell and prints accordingly*/
					if((board->fixed)[j][k]==1){
						printf(".");
					}
					else if(show_errors==1 && (board->errors)[j][k]==1){
						printf("*");
					}
					else{
						printf(" ");
					}
				}
				printed_cells_counter+=1;
			}
			printf("|");
			printf("\n");
		}
	}
	print_separator_row( brows, bcols);
}

/*#16*/
int reset(MOVES_LIST* lst,BOARD* board){
	while(lst->pointer_to_current_node->is_starting_state!=1){
		undo(lst,board,1);
	}
	return 0;
}
/*#17*/
void myexit(GAME *game){
	printf("Exiting.. . .\n");/*RONIT*/
	terminate_game(game);
}
/*THE COMMANDS-END*/
