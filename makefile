CC=gcc
OBJS= main.o backtracking.o cmd_parser.o error_msg.o MainAux.o game_executer.o game.o gurobi_solver.o solver.o moves_list.o stack.o
EXEC= sudoku-console
COMP_FLAGS= -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
all: $(EXEC)
main.o: main.c game_executer.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
backtracking.o: backtracking.c backtracking.h game.h solver.h stack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
cmd_parser.o: cmd_parser.c cmd_parser.h MainAux.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
error_msg.o: error_msg.c error_msg.h cmd_parser.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
MainAux.o: MainAux.c MainAux.h SPBufferset.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
game_executer.o: game_executer.c game_executer.h game.h backtracking.h solver.h cmd_parser.h MainAux.h error_msg.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h cmd_parser.h error_msg.h moves_list.h 
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
gurobi_solver.o: gurobi_solver.c gurobi_solver.h game.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
solver.o: solver.c solver.h game.h MainAux.h gurobi_solver.h moves_list.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
stack.o: stack.c stack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
moves_list.o: moves_list.c moves_list.h error_msg.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)