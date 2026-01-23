#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "node_utils.h"
#include "board_utils.h"

#define MAX_SIMULATIONS 1000000


struct Node *selection(struct Node *root){
    if (root->total_simulations == 0 || root->winner != NO_PLAYER) return root;
    if (root->total_simulations == 1) init_children(root);
    return selection(get_highest_UCB_child(root));
}

Player simulate(LargeBoard *board, Player cur, int SB_index){
    Player winner = determine_LB_winner(board);
    if (winner != NO_PLAYER) return winner;

    int new_SB_index = make_random_move(board, SB_index, cur);
    
    if (cur == PLAYER1) cur = PLAYER2; else cur = PLAYER1;

    return simulate(board, cur, new_SB_index);

}


void backpropagate(struct Node *leaf, Player winner){
    if (leaf == NULL) return;

    leaf->total_simulations++;

    if (winner == TIE) leaf->total_wins += 0.5;
    if (winner == leaf->current_player) leaf->total_wins += 1.0;

    backpropagate(leaf->parent, winner);
}

int iterate_through_root_children(struct Node *root){
    if (root->child_arr_size == 0) return -1;
    struct Node *best_wr_child = root->child_arr[0];
    double best_wr = best_wr_child->total_wins/best_wr_child->total_simulations;

    for (int i=0; i<root->child_arr_size; i++){
        double cur_wr = root->child_arr[i]->total_wins/root->child_arr[i]->total_simulations;
        printf("%d: %f\n", i, cur_wr);
        if (best_wr >= cur_wr) continue;
        best_wr = cur_wr;
        best_wr_child = root->child_arr[i];
    }

    double wr0 = root->child_arr[0]->total_wins/root->child_arr[0]->total_simulations;
    double wr2 = root->child_arr[2]->total_wins/root->child_arr[2]->total_simulations;
    double wr3 = root->child_arr[3]->total_wins/root->child_arr[3]->total_simulations;
    double wr5 = root->child_arr[5]->total_wins/root->child_arr[5]->total_simulations;
    double wr6 = root->child_arr[6]->total_wins/root->child_arr[6]->total_simulations;
    double wr8 = root->child_arr[8]->total_wins/root->child_arr[8]->total_simulations;

    printf("%f\n", wr0-wr2);
    printf("%f\n", wr3-wr5);
    printf("%f\n", wr6-wr8);
    return -1;
}



int best_move(LargeBoard *board, Player current_player, int SB_index){
    /*
    first digit is the index in the LB, second digit is the index in the SB
    
    logic is this: 
        1. create a root node using the board given
        2. in a loop, use select() to get a node, then simulate() on the board
           given, and finally backpropagate from said node the result from simulate()
        3. when a result is desired, take the child move with the highest
           W/L ratio
        4. can also print results from other nodes to see what the eval function looks like
    
    something else to consider is if several simulate/backpropagates would bias the
    eval function; could also do a tree exploration phase and then a fleshing out phase which
    doesn't create new nodes.

    also, need to make a free_MCTS_tree function and free other things

    more unit tests for utils though, first

    */

    struct Node *root = create_node(board, current_player, SB_index, NULL);
    if (root->winner != NO_PLAYER) return -1;

    struct Node *selected_node;
    LargeBoard *simulation_board = malloc(sizeof(LargeBoard));
    time_t start_time = time(NULL);

    while (root->total_simulations < MAX_SIMULATIONS){
        selected_node = selection(root);
        memcpy(simulation_board, selected_node->board, sizeof(LargeBoard));
        Player winner = simulate(simulation_board, current_player, selected_node->SB_index);
        backpropagate(selected_node, winner);
    }

    printf("seconds: %ld\n", time(NULL)-start_time);


    iterate_through_root_children(root);

    free_tree(root);
    free(simulation_board);
    
    return -1;

}

