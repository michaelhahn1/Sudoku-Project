#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "MainAux.h"

int** create_int_matrix(int rows,int cols){
	int i;
	int **m=malloc(rows*sizeof(int*));
	if(m==NULL){/*MALLOC FAILED*/
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<rows;i++){
		m[i]=malloc(cols*sizeof(int));
		if(m[i]==NULL){/*MALLOC FAILED*/
			printf("ALLOCATION FAILURE. Exiting... . .  .\n");
			exit(1);
		}
	}
	return m;
}

void free_int_matrix(int **m,int rows){
	int i;
	for(i=0;i<rows;i++){
		free(m[i]);
	}
	free(m);
}

void zerofy_int_matrix(int **m, int rows,int cols){
	int i,j;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			m[i][j]=0;
		}
	}
}

int isInteger(const char* str){
	unsigned int i=0;
	if(str==NULL){
		return 0;
	}
	for(;i<strlen(str);i++){
		if(isdigit(str[i])==0){/*string has a char in it that is not a number => return 0*/
			return 0;
		}
	}
	return 1;/*All digits were numbers => return 1*/
}

int isFloat(const char* str){
	unsigned int i=0;
	int numofdots=0;
	if(str==NULL){
		return 0;
	}
	for(;i<strlen(str);i++){
		if(isdigit(str[i])==0){
			if(str[i]!='.'){
				return 0;
			}
			else{
				numofdots++;
				if(numofdots>1){
					return 0;
				}
			}
		}
	}
	return 1;/*No error encountered, string can be converted to float*/
}

int isWhiteSpace(char c){
	if(c=='\n' || c=='\t' || c=='\f' || c=='\v' || c=='\r' || c==' '){
		return 1;
	}
	return 0;
}

void print_separator_row(int brows,int bcols){
	int i,num;
	num=4*brows*bcols+brows+1;/*Number of dashes= N+m+1 (Page 5)*/
	for(i=0;i<num;i++){
		printf("-");
	}
	printf("\n");
}

int line_is_empty(char *str){
	unsigned int i=0;
	while(i<strlen(str)){
		if(!isWhiteSpace(str[i])){
			return 0;
		}
		i++;
	}
	return 1;
}

void readline(char *str){
	int len=258;
	int askAgain=1;
	char *fgetsRes;
	while(askAgain){
		printf("Please enter your move:\n");
		if(feof(stdin)){
			printf("End of file was reached, exiting...\n");
			exit(0);
		}
		fgetsRes=fgets(str,len,stdin);
		if(ferror(stdin)|| fgetsRes==NULL){
			if(feof(stdin)){
				printf("End of file was reached, exiting...\n");
				exit(0);
			}
			printf("Reading from standard input failed.\n");
		}
		if(!line_is_empty(str) && str[strlen(str)-1]=='\n'){
			askAgain=0;
		}
		else{
			if(!line_is_empty(str)){
				printf("Lines longer then 256 characters are invalid.\n");
			}
			while(str[strlen(str)-1]!='\n' && !feof(stdin)){
				fgetsRes=fgets(str,len,stdin);
				if(ferror(stdin) || fgetsRes==NULL){
					printf("Reading from standard input failed.\n");
				}
			}
		}
	}
}

void copy_board(int** original, int** copy, int N){
	int i,j;
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			copy[i][j] = original[i][j];
		}
	}
}
