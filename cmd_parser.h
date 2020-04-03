/**
 * cmd_parser:
 *	module that holds the struct and functions that are used to fully parse a string entered by the user in standard input
 *	into a command that can be used in a sudoku game.
 */

#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include "MainAux.h"

/*Enum list of possible user commands + place holders for when a line is allocated or it invalid.*/
typedef enum{
	SOLVE,
	EDIT,
	MARK_ERRORS,
	PRINT_BOARD,
	SET,
	VALIDATE,
	GUESS,
	GENERATE,
	UNDO,
	REDO,
	SAVE,
	HINT,
	GUESS_HINT,
	NUM_SOLUTIONS,
	AUTOFILL,
	RESET,
	EXIT,
	NOT_A_COMMAND, /*The command that the user input doesn't exist*/
	WAITING /*Place holder until there is anything entered*/
}CMD_TYPE;

/**
 * CMD_LINE: Type to encapsulate a parsed line
 * Fields:
 * ct: The command type from enum list above.
 * argNumStat: How many arguments does this command require:
 * 			   0=no arguments.
 * 			   1=one argument.
 * 			   2=two arguments.
 * 			   3=three arguments.
 * 			   4=one or two arguments.
 * modAvlStat: What modes is this command available in:
 * 			   0=All.
 * 			   1=Edit and Solve only.
 * 			   2=Solve only.
 * 			   3=Edit only.
 * validStat: Status of the validity of the command line:
 * 			   0=Valid.
 * 			   1=Command name(type) invalid.
 * 			   2=Too few parameters given.
 * 			   3=Too many parameters given.
 * 			   4=Incorrect range for parameter 1.
 * 			   5=Incorrect range for parameter 2.
 * 			   6=Incorrect range for parameter 3.
 * 			   (The rest (page 8) can only be checked in game).
 * arguments: Array of strings that holds the parameters given for the command if any are given.
 *
 */
typedef struct command_line{
	CMD_TYPE ct;
	int argNumStat;
	int modAvlStat;
	int validStat;
	char *arguments[3];/*The most arguments that can be given is 3*/
}CMD_LINE;

/*STRUCT FUNCTIONS===START*/
/**
 * Allocates memory and initialises a CMD_LINE structure.
 * @return:
 * Pointer to initialised CMD_LINE is successful, NULL otherwise
 */
CMD_LINE *createEmptyLine();

/**
 * Prints the command name by the ENUM value with only a space following it.
 * @params: CMD_TYPE from the enum list.
 */
void print_cmd_name(CMD_TYPE ct);

/**
 *	Free the allocated space of give structure of type CMD_LINE.
 *	@params: A CMD_LINE structure.
 */
void delete_parse_line(CMD_LINE *line);

/**
 *	Without deleting the struct, bring it back to the state it was in when it was first created.
 *	@params: A CMD_LINE structure.
 */
void erase_line_content(CMD_LINE *line);
/*STRUCT FUNCTIONS===END*/

/*COMMAND VALIDATION FUNCTIONS===START*/
/**
 * (Used for validating arguments for command SET.)
 * Validate that the 3 strings in array 'arguments' can be parsed to integers.
 * @params: array of strings with at least 3 strings.
 * @return:
 * 0 if valid.
 * 1 if the first arg cannot be parsed to an integer.
 * 2 if the second arg cannot be parsed to an integer.
 * 3 if the third arg cannot be parsed to an integer.
 */
int isInvalid_three_int(char **args);

/**
 *	(Used for validating arguments for commands GENERATE/HINT/GUESS_HINT.)
 *	Validate that the first 2 strings in array 'arguments' can be parsed to integers.
 *	@params: array of strings with at least 2 strings.
 *	@return:
 *	0 if valid.
 *	1 if first arg cannot be parsed to an integer.
 *	2 if the second arg cannot be parsed to an integer.
 */
int isInvalid_two_int(char **args);

/**
 * (Used for validating arguments for command MARK_ERRORS.)
 * Validate that the first string in array 'arguments' can be parsed into integer of value 0 or 1.
 * @params: array of strings with at least 1 string.
 * @return:
 * 1 if valid.
 * 0 if not.
 */
int isValid_one_binary(char **args);

/**
 * (Used for validating arguments for command GUESS.)
 * Validate that the first string in array 'arguments' can be parsed into float.
 * @params: array of strings with at least 1 string.
 * @return:
 * 1 if valid.
 * 0 if not.
 */
int isValid_one_float(char **args);/*GUESS*/
/*COMMAND VALIDATION FUNCTIONS===END*/

/*COMMAND PARSING===START*/
/**
 * Given a string and a CMD_LINE structure,
 * 1)Recognise if the string describes a command in the CMD_TYPE list.
 * 2)If it is, update the struct's ct field to the proper command
 * and update the info about the number of arguments needed
 * and modes where it is available accordingly.
 *
 * If the command is not in the list it'll update to NOT_A_COMMAND
 * if the string given is empty the CMD_LINE stays empty(WAITING).
 * @params: string to parse and a CMD_LINE structure to update according to that string.
 */
void parseCommandTypeIntoStruct(const char* str,CMD_LINE *line);

/**
 * Given a string and a CMD_LINE structure,
 * Read the string and parse it into a full command with arguments+info about the command.
 * If along the way the parsed command is discovered to be invalid,
 * change the validity status of the CMD_LINE and stop parsing.
 * @params: string to be parsed, a CMD_LINE structure to update according to that string.
 */
void parse_line(char *str, CMD_LINE *line);
/*COMMAND PARSING===END*/

#endif /* CMD_PARSER_H_ */
