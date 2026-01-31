#include <stdlib.h>
#include <stdio.h>

#include "board_utils.h"
#include "node_utils.h"
#include "MCTS.h"

/* 

This is also going to be where I plan things out for now.

Currently, my two biggest issues are that the init_children function
is too long and that it might be a source of big slowdowns if I'm not
careful.

I think the best way to optimize (without initializing children lazily)
is to enforce that we only initialize child nodes after they've been visited
at least once. Then we won't get as much bloat at the leaves of the tree.

Also, before doing the full MCTS, I think it would be best to write some unit
tests here and make some kind of main function that just runs them all, and 
compile it w/ make like in class.

Also also, it's probably bad practice to do init_node the way I did.
Passing a node pointer would make it consistent with init_children.


.........\n
.O..O....\n
.........\n
...XXX...\n
.........\n
.........\n
.........\n
.........\n
.........\n"


*/

void output(char testname[], int result){
    printf("Test: %s | result: ", testname);
    if (result == 0) printf("FAILURE\n");
    else printf("SUCCESS\n");
}


int determine_SB_winner_tests(){
    SmallBoard sb0 = {NO_PLAYER, NO_PLAYER, NO_PLAYER,
        NO_PLAYER, NO_PLAYER, NO_PLAYER,
        NO_PLAYER, NO_PLAYER, NO_PLAYER};

    output("determine_SB_winner(0)", (determine_SB_winner(sb0) == NO_PLAYER));

    SmallBoard sb1 = {PLAYER1, NO_PLAYER, NO_PLAYER,
        NO_PLAYER, PLAYER1, NO_PLAYER,
        NO_PLAYER, NO_PLAYER, PLAYER1};
    output("determine_SB_winner(1)", (determine_SB_winner(sb1) == PLAYER1));
    
    SmallBoard sb2 = {PLAYER1, NO_PLAYER, NO_PLAYER,
        NO_PLAYER, PLAYER1, NO_PLAYER,
        NO_PLAYER, PLAYER2, TIE};
    output("determine_SB_winner(2)", (determine_SB_winner(sb2) == NO_PLAYER));
    
    SmallBoard sb3 = {PLAYER1, PLAYER1, PLAYER2,
        PLAYER2, PLAYER1, PLAYER1,
        PLAYER1, PLAYER2, PLAYER2};
    output("determine_SB_winner(3)", (determine_SB_winner(sb3) == TIE));
    
}

int valid_move_no_tests(){
    char str[] = 
    "XXXOOOXXX\n"
    "XXXOOOXXX\n"
    "XXXOOOXXX\n"
    ".........\n"
    ".........\n"
    ".........\n"
    "XXXOOOXXX\n"
    "XXXOOOXXX\n"
    "XXXOOOXXX\n";

    LargeBoard *lb = string_to_LB(str);
    
    output("valid_move_no(0)", (valid_move_no(lb, -1) == 9*3));
    output("valid_move_no(1)", (valid_move_no(lb, 0) == 0));
    output("valid_move_no(2)", (valid_move_no(lb, 3) == 9));

    free(lb);

}



int main(int argc, char **argv){


    //determine_SB_winner_tests();
    
    //printf("\n--------------------------------------\n");
    //valid_move_no_tests();
    char str[] = 
    ".........\n"
    ".........\n"
    ".........\n"
    ".........\n"
    "....X....\n"
    ".....O...\n"
    ".........\n"
    ".........\n"
    ".........\n";

    LargeBoard *lb = string_to_LB(str);

    best_move(lb, PLAYER1, 8);

}




