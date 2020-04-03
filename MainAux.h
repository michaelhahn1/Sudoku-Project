/*
 * MainAux:
 *      Responsible for general functions that don't depend on anything.
 *      for multiple proposes throughout the code.
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "SPBufferset.h"

/*Note: rows, cols are the actual dimensions of a matrix.
 * brows, bcols are the dimensions of a block*/
/**
 * Create a 2D integer array and allocate rows*cols memory for it.
 * @params: rows and cols who will be the number of rows and columns of the created matrix.
 * @return: pointer to the created 2D array.
 */
int** create_int_matrix(int rows,int cols);

/**
 * Release the space taken  by a 2D matrix with 'rows' number of rows.
 * @params: m the matrix to be freed. rows the number of 1D arrays to free in it.
 */
void free_int_matrix(int **m,int rows);

/**
 * Fill a 2D array size rows*cols with zeros.
 * @params: m the matrix to be zerofied. rows&cols the dimensions of the matrix.
 */
void zerofy_int_matrix(int **m, int rows,int cols);

/**
 * Checks if the string is a valid integer.
 * @params: the string to check.
 * @return
 * 1 if yes, 0 if not
 */
int isInteger(const char* str);

/**
 * Checks if the string is a valid float.
 * @params: the string to check.
 * @return
 * 1 if yes, 0 if not
 */
int isFloat(const char* str);

/**
 * Checks if char is considered whitespace
 * @params: the char to check.
 * @return
 * 1 if yes, 0 if not
 */
int isWhiteSpace(char c);

/**
 * Prints a seperator line of '-' for board printing.
 * print '-' with length 4*brows*bcols+brows+1
 * @params: brows and bcols to determine length of line.
 */
void print_separator_row(int brows,int bcols);

/**
 * Check if a string has only whitespaces.
 * @params: the string to be checked.
 * @return 1 if it does, else 0.
 */
int line_is_empty(char *str);

/**
 * Using fgets read a user entered line into given string variable until gets one that is parsable.
 * Empty lines are ignored and lines longer then 256 characters will be notified as invalid.
 * Prompt to enter a command is given until a valid parsable line is entered.
 * @params:str string
 */
void readline(char *str);

/**
 * Create a deep copy of the matrix original into the matrix copy.
 * @params: two initialised matrices.
 */
void copy_board(int** original, int** copy, int N);
#endif /* MAINAUX_H_ */
