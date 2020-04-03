
#include <stdlib.h>
#include <stdio.h>
#include "ilp.h"


int LP(BOARD* board,double** solution,int*** mapping_arr){
	int error=0;
	error = run_linear_programming_using_gurobi(board,0,solution,mapping_arr);
	return error;
}

int ILP(BOARD* board,int** board_copy){
	double **solution = calloc(1,sizeof(double*));
	int i,j;
	int error=0;
	int ***mapping_arr;
	int N = board->bcols*board->brows;
	mapping_arr = calloc(N,sizeof(int**));
	if(solution==NULL||mapping_arr==NULL){
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
	error = run_linear_programming_using_gurobi(board,1,solution,mapping_arr);
	if(error==0){
		solved_board_for_ilp(mapping_arr,solution,board_copy,N);
	}
	free(solution[0]);
	free(solution);
	free_mapping_arr(mapping_arr,N);
	return error;
}

void fill_mapping_array(BOARD* board, int*** mapping_arr, int* possible_values,int N,int* result){
	int i,j,k,current_variable_number=0;
	result[0] = 0;
	result[1] = 0;
	for (i=0;i<N;i++){
		for(j=0;j<N;j++){
			if (board->values[i][j]!=0){
				for (k=0;k<N;k++){
					mapping_arr[i][j][k]= -1*board->values[i][j];
				}
			}
			else{
				find_possible_values(possible_values,board,i,j);
				if (possible_values[0]==0){
					result[0] = 1;
				}
				else{
					for(k=0;k<N;k++){
						if (possible_values[k+1]==1){
							mapping_arr[i][j][k] = current_variable_number;
							current_variable_number+=1;
						}
						else{
							mapping_arr[i][j][k] = -1000;
						}
					}
				}
			}
		}
		result[1] = current_variable_number;
	}
}

int add_constraints(GRBenv *env,GRBmodel* model,BOARD* board,int*** mapping_arr, int N){
	int *temp_ind_arr = calloc(N,sizeof(int));
	double *temp_val_arr = calloc(N,sizeof(double));
	int current_constrait_length=0,error=1;
	int row,col,value,i,j;
	if(temp_ind_arr==NULL||temp_val_arr==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<N;i++){
		temp_val_arr[i] = 1;
	}
	for(row=0;row<N;row++){/* one value in each cell*/
		for(col=0;col<N;col++){
			for (value=0;value<N;value++){
				if (mapping_arr[row][col][value]<0 && mapping_arr[row][col][value]>-1000){
				}
				else if (mapping_arr[row][col][value]>=0){
					temp_ind_arr[current_constrait_length]= mapping_arr[row][col][value];
					current_constrait_length+=1;
				}
			}
			if(current_constrait_length>0){
				error = GRBaddconstr(model, current_constrait_length, temp_ind_arr, temp_val_arr
						, GRB_EQUAL, 1,NULL);
				if (error) {
					free(temp_ind_arr);
					free(temp_val_arr);
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 1;
				}
			}
			current_constrait_length=0;
		}
	}
	for(row=0;row<N;row++){ 	/* each row has one occurrence of each value*/
		for (value=0;value<N;value++){
			for (col=0;col<N;col++){
				if (mapping_arr[row][col][value]<0 && mapping_arr[row][col][value]>-1000){
				}
				else if (mapping_arr[row][col][value]>=0){
					temp_ind_arr[current_constrait_length]= mapping_arr[row][col][value];
					current_constrait_length+=1;
				}
			}
			if(current_constrait_length>0){
				error = GRBaddconstr(model, current_constrait_length, temp_ind_arr,
						temp_val_arr, GRB_EQUAL, 1,NULL);
				if (error) {
					free(temp_ind_arr);
					free(temp_val_arr);
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 1;
				}
			}
			current_constrait_length=0;
		}
	}
	for (col=0;col<N;col++){ 	/*2. each column has one occurrence of each value*/
		for (value=0;value<N;value++){
			for (row=0;row<N;row++){
				if (mapping_arr[row][col][value]<0 && mapping_arr[row][col][value]>-1000){
				}
				else if (mapping_arr[row][col][value]>=0){
					temp_ind_arr[current_constrait_length]= mapping_arr[row][col][value];
					current_constrait_length+=1;
				}
			}
			if(current_constrait_length>0){
				error = GRBaddconstr(model, current_constrait_length, temp_ind_arr,
						temp_val_arr, GRB_EQUAL, 1,NULL);
				if (error) {
					free(temp_ind_arr);
					free(temp_val_arr);
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
					return 1;
				}
			}
			current_constrait_length=0;
		}
	}
	for (i=0;i<N;i+=board->brows){ 	/*2. each block has one occurrence of each value*/
		for (j=0;j<N;j+=board->bcols){
			for (value=0;value<N;value++){
				for (row=i;row<i+board->brows;row++){
					for (col = j;col<j+board->bcols;col++){

						if (mapping_arr[row][col][value]<0 && mapping_arr[row][col][value]>-1000){
						}
						else if (mapping_arr[row][col][value]>=0){
							temp_ind_arr[current_constrait_length]= mapping_arr[row][col][value];
							current_constrait_length+=1;
						}
					}
				}
				if(current_constrait_length>0){
					error = GRBaddconstr(model, current_constrait_length, temp_ind_arr,
							temp_val_arr, GRB_EQUAL, 1,NULL);
					if (error) {
						free(temp_ind_arr);
						free(temp_val_arr);
						printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
						return 1;
					}
				}
				current_constrait_length = 0;
			}
		}
	}
	free(temp_ind_arr);
	free(temp_val_arr);
	return 0;
}

int objective_function(GRBenv *env, GRBmodel *model,int* ind,char* vtype,int number_of_variables,
		int is_binary,int N){
	int i,error=0;
	double *obj = calloc(number_of_variables,sizeof(double));
	if(obj==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	for(i=0;i<number_of_variables;i++){
		ind[i]=i;
	}
	if(is_binary){
		for(i=0;i<number_of_variables;i++){
			vtype[i] = GRB_BINARY;
		}
		error = GRBaddvars(model, number_of_variables, 0, NULL, NULL, NULL, NULL, NULL,
				NULL, vtype, NULL);
	}
	else{ /*continuous*/
		for(i=0;i<number_of_variables;i++){
			vtype[i] = GRB_CONTINUOUS;
		}
		for (i=0;i<number_of_variables;i++){
			obj[i] = draw_random_number(N);
		}
		error = GRBaddvars(model, number_of_variables, 0, NULL, NULL, NULL, obj, NULL, NULL,
				vtype, NULL);
	}
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		free(obj);
		return -1;
	}
	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		free(obj);
		return -1;
	}
	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		free(obj);
		return -1;
	}
	free(obj);
	return 0;
}
double draw_random_number(int N){
	return (double)((rand() % N)+1)/N;
}

int run_linear_programming_using_gurobi(BOARD* board,int is_binary,double** sol,int*** mapping_arr)
{
	int optimstatus;
	int *fill_mapping_array_result = calloc(2,sizeof(int));
	int N = board->brows*board->bcols;
	int number_of_variables=0,error=0;
	int *possible_values = calloc(N+1,sizeof(int));
	int *ind;
	char *vtype;
	double objval=0;
	GRBenv   *env   = NULL;
	GRBmodel *model = NULL;
	if(fill_mapping_array_result==NULL||possible_values==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	is_board_erroneous(board);
	if(board->board_status==2){
		free(possible_values);
		free(fill_mapping_array_result);
		return -1;
	}
	error = GRBloadenv(&env, "mip1.log");
	if (error) {
		free(possible_values);
		free(fill_mapping_array_result);
		if (model!=NULL)
			GRBfreemodel(model);
		if (env!=NULL)
			GRBfreeenv(env);
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		free(possible_values);
		free(fill_mapping_array_result);
		if (model!=NULL)
			GRBfreemodel(model);
		if (env!=NULL)
			GRBfreeenv(env);
		return -1;
	}
	error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		if (model!=NULL)
			GRBfreemodel(model);
		if (env!=NULL)
			GRBfreeenv(env);
		free(possible_values);
		free(fill_mapping_array_result);
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}
	fill_mapping_array(board, mapping_arr,possible_values, N,fill_mapping_array_result);
	if (fill_mapping_array_result[0] == 1){
		printf("Model is infeasible or unbounded\n");
		GRBfreemodel(model);
		GRBfreeenv(env);
		free(possible_values);
		free(fill_mapping_array_result);
		return -1;
	}
	number_of_variables = fill_mapping_array_result[1];
	ind = calloc(number_of_variables,sizeof(int));
	vtype = calloc(number_of_variables,sizeof(char));
	sol[0] = calloc(number_of_variables,sizeof(double));
	if(ind==NULL||vtype==NULL||sol[0]==NULL){
		printf("ALLOCATION FAILURE. Exiting... . .  .\n");
		exit(1);
	}
	error = objective_function(env,model,ind,vtype,number_of_variables,is_binary,N);
	if (error){
		free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
		return -1;
	}
	error = add_constraints(env,model,board,mapping_arr,N);
	if(error) {
		free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
		return -1;
	}

	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
		return -1;
	}

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
		return -1;
	}

		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) {
			printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
			free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
			return -1;
		}


		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, number_of_variables, *sol);
		if (error) {
			printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
			free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
			return -1;
		}
	/* print results */
	printf("\nOptimization complete\n");
	/* solution found */
	if (optimstatus == GRB_OPTIMAL) {
	}
	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD) {
		printf("Model is infeasible or unbounded\n");
		error = 1;
	}
	/* error or calculation stopped */
	else {
		printf("check11\n");
		printf("Optimization was stopped early\n");
	}

	free_func_variables(env,model,possible_values,ind,vtype,fill_mapping_array_result);
	return 0;
}

void solved_board_for_ilp(int*** mapping_arr,double** sol,int** board_copy,int N){
	int i,j,k;
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			if (board_copy[i][j]==0){
				for (k=0;k<N;k++){
					if (mapping_arr[i][j][k]>=0 && sol[0][mapping_arr[i][j][k]]==1){
						board_copy[i][j] = k+1;
					}
				}
			}
		}
	}
}

void free_func_variables(GRBenv* env, GRBmodel* model,int* possible_values,int* ind,char* vtype,
		int* mapping_array_result){
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(possible_values);
	free(ind);
	free(vtype);
	free(mapping_array_result);
}
void free_mapping_arr(int*** mapping_arr,int N){
	int i,j;
	for (i=0;i<N;i++){
		for (j=0;j<N;j++){
			free(mapping_arr[i][j]);
		}
		free(mapping_arr[i]);
	}
	free(mapping_arr);
}
