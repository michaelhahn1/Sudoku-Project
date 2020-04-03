#include "error_msg.h"
#include <stdio.h>
#include <stdlib.h>

void incorrect_mode_err(int GameMode, int actual_available_game_modes){
	if(actual_available_game_modes==1){
		printf("Current mode is INIT. This command is available only in EDIT and SOLVE modes.\n");
	}
	else if(actual_available_game_modes==2){
		if(GameMode==0){
			printf("Current mode is INIT. This command is available only in SOLVE mode.\n");
		}
		if(GameMode==1){
			printf("Current mode is EDIT. This command is available only in SOLVE mode.\n");
		}
	}
	else{/*actual_available_game_modes==3*/
		if(GameMode==0){
			printf("Current mode is INIT. This command is available only in EDIT mode.\n");
		}
		if(GameMode==2){
			printf("Current mode is SOLVE. This command is available only in EDIT mode.\n");
		}
	}
}

void incorrect_arg_num_err(int ManyORFew, CMD_LINE *command){
	if(ManyORFew==0){/*Too few*/
		print_cmd_name(command->ct);
		printf("requires %d arguments. You entered too few.\n",command->argNumStat);
	}
	else{/*Too many*/
		print_cmd_name(command->ct);
		if(command->argNumStat==4){
			printf("requires 0 or 1 arguments. You entered too many.\n");
		}
		else{
			printf("requires %d arguments. You entered too many.\n",command->argNumStat);
		}
	}
}

void arg_invalid_err(int numOfArg, CMD_LINE *command){
		print_cmd_name(command->ct);
		printf("couldn't execute because argument %d (%s) is not of the correct range of values.\n",numOfArg+1,command->arguments[numOfArg]);
}

void file_handling_failure(int openORcloseORwriteORread){
	if(openORcloseORwriteORread==0){
		printf("Failed opening the file.\n");
	}
	if(openORcloseORwriteORread==1){
		printf("Failed closing the file.\n");
	}
	if(openORcloseORwriteORread==2){
		printf("Failed reading the file.\n");
	}
	if(openORcloseORwriteORread==3){
		printf("Failed writing to the file.\n");
	}
}

void fixed_cell_msg(int i,int j){
	printf("Cell (%d,%d) is fixed. You can't change fixed cells.\n",i,j);
}

void full_cell_msg(int i,int j){
	printf("Cell (%d,%d) is full. Choose a different one.\n",i,j);
}

void allocation_failure(){
	printf("ALLOCATION FAILURE. Exiting... . .  .\n");
}

void board_has_errors_msg(CMD_LINE *command){
	print_cmd_name(command->ct);
	printf("couldn't execute command because board is erroneous.\n");
}
