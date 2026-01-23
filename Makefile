CC=gcc
CFLAGS= -g -pg

tests: unit_tests.c unit_tests.h board_utils.c board_utils.h node_utils.c node_utils.h MCTS.c MCTS.h 
	$(CC) $(CFLAGS) -o tests unit_tests.c board_utils.c node_utils.c MCTS.c -lm
 
clean: 
	rm tests