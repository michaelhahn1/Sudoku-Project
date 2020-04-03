#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmd_parser.h"

/*STRUCT FUNCTIONS===START*/
CMD_LINE *createEmptyLine(){
	CMD_LINE *res=(CMD_LINE*)malloc(sizeof(CMD_LINE));
	if(!res){/*Allocation failed*/
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	res->ct=WAITING;/*Waiting for a command*/
	/*Explicitly initialising to verify what should change when*/
	res->argNumStat=0;
	res->modAvlStat=0;
	res->validStat=0;
	return res;
}

void print_cmd_name(CMD_TYPE ct){
	switch(ct){
		case SOLVE:
			printf("solve ");
			break;
		case EDIT:
			printf("edit ");
			break;
		case MARK_ERRORS:
			printf("mark_errors ");
			break;
		case PRINT_BOARD:
			printf("print_board ");
			break;
		case SET:
			printf("set ");
			break;
		case VALIDATE:
			printf("validate ");
			break;
		case GUESS:
			printf("guess ");
			break;
		case GENERATE:
			printf("generate ");
			break;
		case UNDO:
			printf("undo ");
			break;
		case REDO:
			printf("redo ");
			break;
		case SAVE:
			printf("save ");
			break;
		case HINT:
			printf("hint ");
			break;
		case GUESS_HINT:
			printf("guess_hint ");
			break;
		case NUM_SOLUTIONS:
			printf("num_solutions ");
			break;
		case AUTOFILL:
			printf("autofill ");
			break;
		case RESET:
			printf("reset ");
			break;
		case EXIT:
			printf("exit ");
			break;
		default:
			break;
	}
}

void delete_parse_line(CMD_LINE *line){
	int i;
	if(!line){
		return;
	}
	if(line->arguments!=NULL){
		for(i=0;i<3;i++){
			if(line->arguments[i]!=NULL){
				free(line->arguments[i]);
			}
		}
	}
	free(line);
}

void erase_line_content(CMD_LINE *line){
	int i;
	if(!line){
		return;
	}
	if(line->arguments!=NULL){
		for(i=0;i<3;i++){
			if(line->arguments[i]!=NULL){
				free(line->arguments[i]);
				line->arguments[i]=NULL;
			}
		}
	}
	line->ct=WAITING;/*Waiting for a command*/
	/*Explicitly initialising to verify what should change when*/
	line->argNumStat=0;
	line->modAvlStat=0;
	line->validStat=0;
}
/*STRUCT FUNCTIONS===END*/

/*COMMAND VALIDATION FUNCTIONS===START*/
int isValid_one_binary(char **args){
	if(isInteger(args[0]) && atoi(args[0])<=1){
		return 1;
	}
	return 0;
}

int isValid_one_float(char **args){
	if(isFloat(args[0])){
		return 1;
	}
	return 0;
}

int isInvalid_three_int(char **args){
	/*return 0 if everything is valid. 1 if first arg is wrong. 2 if second arg is wrong. 3 if third arg is wrong.*/
	if(!isInteger(args[0])){
		return 1;
	}
	if(!isInteger(args[1])){
		return 2;
	}
	if(!isInteger(args[2])){
		return 3;
	}
	return 0;
}

int isInvalid_two_int(char **args){
	/*return 0 if everything is valid. 1 if first arg is wrong. 2 if second arg is wrong.*/
	if(!isInteger(args[0])){
		return 1;
	}
	if(!isInteger(args[1])){
		return 2;
	}
	return 0;
}
/*COMMAND VALIDATION FUNCTIONS===END*/

/*COMMAND PARSING===START*/
void parseCommandTypeIntoStruct(const char* str,CMD_LINE *line){
	/*Note the strcmp is case-sensitive :)*/
	if(strcmp(str,"solve")==0){/*1*/
		line->ct= SOLVE;
		line->argNumStat=1;
		return;}
	if(strcmp(str,"edit")==0){/*2*/
		line->ct= EDIT;
		line->argNumStat=4;
		return;}
	if(strcmp(str,"mark_errors")==0){/*3*/
		line->ct= MARK_ERRORS;
		line->argNumStat=1;
		line->modAvlStat=2;
		return;}
	if(strcmp(str,"print_board")==0){/*4*/
		line->ct= PRINT_BOARD;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"set")==0){/*5*/
		line->ct= SET;
		line->argNumStat=3;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"validate")==0){/*6*/
		line->ct= VALIDATE;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"guess")==0){/*7*/
		line->ct= GUESS;
		line->argNumStat=1;
		line->modAvlStat=2;
		return;}
	if(strcmp(str,"generate")==0){/*8*/
		line->ct= GENERATE;
		line->argNumStat=2;
		line->modAvlStat=3;
		return;}
	if(strcmp(str,"undo")==0){/*9*/
		line->ct= UNDO;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"redo")==0){/*10*/
		line->ct= REDO;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"save")==0){/*11*/
		line->ct= SAVE;
		line->argNumStat=1;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"hint")==0){/*12*/
		line->ct= HINT;
		line->argNumStat=2;
		line->modAvlStat=2;
		return;}
	if(strcmp(str,"guess_hint")==0){/*13*/
		line->ct= GUESS_HINT;
		line->argNumStat=2;
		line->modAvlStat=2;
		return;}
	if(strcmp(str,"num_solutions")==0){/*14*/
		line->ct= NUM_SOLUTIONS;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"autofill")==0){/*15*/
		line->ct= AUTOFILL;
		line->modAvlStat=2;
		return;}
	if(strcmp(str,"reset")==0){/*16*/
		line->ct= RESET;
		line->modAvlStat=1;
		return;}
	if(strcmp(str,"exit")==0){/*17*/
		line->ct= EXIT;
		return;}
	line->ct= NOT_A_COMMAND;
	line->validStat=1;
}

void parse_line(char *str, CMD_LINE *line){
	const char delimeter[7]=" \t\r\n";
	char *token;
	int argumentCounter=0;
	SP_BUFF_SET();
	token=strtok(str,delimeter);
	if(token==NULL){
		/*Should't reach here but it is here just in case.*/
		return;
	}
	parseCommandTypeIntoStruct(token,line);
	if(line->validStat==1){/*Command name was invalid*/
		return;
	}
	token=strtok(NULL,delimeter);/*Try reading the next word*/
	while(token!=NULL){
		if(argumentCounter>2){
			line->validStat=3;
			return;
		}
		line->arguments[argumentCounter]=calloc(strlen(token+1),sizeof(char));

		if(line->arguments[argumentCounter]==NULL){
			printf("ALLOCATION FAILURE. Exiting... . .  .\n");
			exit(1);
		}
		strcpy(line->arguments[argumentCounter], token);
		if(line->arguments[argumentCounter]==NULL){
			printf("ALLOCATION FAILURE. Exiting... . .  .\n");
			exit(1);
		}
		argumentCounter++;
		token=strtok(NULL,delimeter);/*Get next token*/
	}
	/*Token is NULL = finished reading arguments before getting too many of those*/
	if(line->argNumStat!=argumentCounter && line->argNumStat!=4){
		/*Determine if too few or too many arguments were given for the specific command:*/
		if(line->argNumStat>argumentCounter){
			line->validStat=2;/*Too few arguments*/
			return;
		}
		else{
			line->validStat=3;/*Too many arguments*/
			return;
		}
	}
	if(line->argNumStat==4 && argumentCounter>1){/*EDIT checked separately here*/
		/*Too many arguments for edit command*/
		line->validStat=3;
		return;
	}
	/*Check if the arguments given are valid for the functions that require arguments*/
	if(line->argNumStat>0){
		if(line->ct==MARK_ERRORS && !isValid_one_binary(line->arguments)){
			line->validStat=4;
			return;
		}
		if(line->ct==SET && (isInvalid_three_int(line->arguments)!=0)){
			switch(isInvalid_three_int(line->arguments)){
				case 1:
					line->validStat=4;
					break;
				case 2:
					line->validStat=5;
					break;
				case 3:
					line->validStat=6;
					break;
				default:
					break;
			}
			return;
		}
		if(line->ct==GUESS && !isValid_one_float(line->arguments)){
			line->validStat=4;
			return;
		}
		if(line->argNumStat==2 && (isInvalid_two_int(line->arguments)!=0)){
			switch(isInvalid_two_int(line->arguments)){
				case 1:
					line->validStat=4;
					break;
				case 2:
					line->validStat=5;
					break;
				default:
					break;
			}
			return;
		}
	}
	/*Done parsing the command into the structure. With the basic validation status.
	 * Further validation of command parameters is done outside of parsing.*/
}
/*COMMAND PARSING===END*/
