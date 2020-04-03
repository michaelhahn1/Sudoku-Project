#include "game_executer.h"

int execute_user_command(GAME *game,CMD_LINE *line){
	int isnt_succssesful=0;
	if(!valid_cmd_name_v2(line->validStat, line->modAvlStat, game->mode)){
		return 1;
	}
	if(!correct_num_of_params_v2(line->validStat,line)){
		return 1;
	}
	if(!correct_param_range(line, line->validStat,game->board)){
		return 1;
	}
	if(!board_valid_for_command_v2(line,game->board)){
		return 1;
	}
	/*Do the actual command now that we know it is valid for executing (unless it is invalid number for specific board)*/
	switch(line->ct){
		case SOLVE:
			isnt_succssesful=solve(game,line->arguments[0]);
			break;
		case EDIT:
			if(line->arguments[0]==NULL){
				isnt_succssesful=edit(game,NULL);
			}
			else{
				isnt_succssesful=edit(game,line->arguments[0]);
			}
			break;
		case MARK_ERRORS:
			mark_err(game,atoi(line->arguments[0]));
			break;
		case PRINT_BOARD:
			print_board(game->board,game->mode,game->mark_err);
			break;
		case SET:
			isnt_succssesful=set(game->board,game->mode,atoi(line->arguments[0]),atoi(line->arguments[1]),atoi(line->arguments[2]),game->actions_dll);
			break;
		case VALIDATE:
			validate(game->board);
			break;
		case GUESS:
			isnt_succssesful=guess(game->board,atof(line->arguments[0]),game->actions_dll);
			break;
		case GENERATE:
			isnt_succssesful=generate(game->board,atoi(line->arguments[0]),atoi(line->arguments[1]),game->actions_dll);
			break;
		case UNDO:
			isnt_succssesful=undo(game->actions_dll,game->board,0);
			break;
		case REDO:
			isnt_succssesful=redo(game->actions_dll,game->board);
			break;
		case SAVE:
			save_board(game,line->arguments[0]);
			break;
		case HINT:
			hint(game->board,atoi(line->arguments[0]),atoi(line->arguments[1]));
			break;
		case GUESS_HINT:
			guess_hint(game->board,atoi(line->arguments[0]),atoi(line->arguments[1]));
			break;
		case NUM_SOLUTIONS:
			num_solutions(game->board);
			break;
		case AUTOFILL:
			isnt_succssesful=autofill(game->board,game->mode,game->actions_dll);
			break;
		case RESET:
			isnt_succssesful=reset(game->actions_dll,game->board);
			break;
		case EXIT:
			myexit(game);
			break;
		default:
			break;
	}
	return isnt_succssesful;
}

int run_game(){
	GAME *game=NULL;
	CMD_LINE *nextcmd=NULL;
	char *userInput=NULL;
	int game_ended=0;
	SP_BUFF_SET();

	/*Start a new game:*/
	printf("======SUDOKU-GAME:======\n");
	game=create_new_game();
	nextcmd=createEmptyLine();
	while(!game_ended){
		if(userInput!=NULL){
			free(userInput);
		}
		userInput=calloc(256,1);
		if(userInput==NULL){
			allocation_failure();
			exit(1);
		}

		readline(userInput);

		erase_line_content(nextcmd);
		parse_line(userInput,nextcmd);

		if(execute_user_command(game,nextcmd)==0){
			/*Check if needs to print the board.*/
			if(nextcmd->ct==EDIT || nextcmd->ct==SOLVE || nextcmd->ct==SET || nextcmd->ct==AUTOFILL
					|| nextcmd->ct==UNDO || nextcmd->ct==REDO || nextcmd->ct==GENERATE || nextcmd->ct==GUESS
					|| nextcmd->ct==RESET){
				if(nextcmd->ct==UNDO || nextcmd->ct==REDO){
					is_board_erroneous(game->board);
				}
				print_board(game->board,game->mode,game->mark_err);
			}

			/*Check if game was solved*/
			if(game->mode==2 && ( nextcmd->ct==AUTOFILL || nextcmd->ct==SOLVE || nextcmd->ct==SET || nextcmd->ct==GUESS || nextcmd->ct==UNDO || nextcmd->ct==REDO ) ){
				check_if_board_solved(game);
			}
		}
		/*Determine if game ended*/
		if(nextcmd->ct==EXIT){
			game_ended=1;
		}
	}
	/*Game has ended exit cleanly*/
	if(userInput!=NULL){
		free(userInput);
	}
	if(nextcmd->ct!=EXIT){
		terminate_game(game);
	}
	delete_parse_line(nextcmd);
	return 0;
}
