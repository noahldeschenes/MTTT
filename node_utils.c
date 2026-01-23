#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <assert.h>
#include <float.h>
#include <string.h>

#include "node_utils.h"

#define C 1.414

double upper_confidence_bound(struct Node *node){

    // calculate the UCB of a given node, prioritizing nodes with no simulations

    if (node->total_simulations == 0) return DBL_MAX;

    double exploitation = node->total_wins / (double) node->total_simulations;
    double exploration = log(node->parent->total_simulations) / (double) node->total_simulations;

    return exploitation + (C * sqrt(exploration));

}

struct Node *get_highest_UCB_child(struct Node *node){
    // returns the given node's child with the max UCB value

    double max_UCB_val = -1.0;
    int max_UCB_index = 0;

    for (int i=0; i<node->child_arr_size; i++){
        double ucb_val = upper_confidence_bound(node->child_arr[i]);
        if (ucb_val > max_UCB_val){
            max_UCB_index = i;
            max_UCB_val = ucb_val;
        }
    }

    return node->child_arr[max_UCB_index];
}

struct Node *create_node(LargeBoard *board, Player cp, int SB_index, struct Node *par){
    struct Node *node = malloc(sizeof(struct Node));
    node->board = board;
    node->current_player = cp;
    node->SB_index = SB_index;
    node->parent = par;
    node->child_arr = NULL;
    node->child_arr_size = 0;
    node->total_simulations = 0;
    node->total_wins = 0;
    node->winner = determine_LB_winner(board);

    return node;

}



void init_children(struct Node *node){
    // could use refactoring

    // switch current player 
    Player cp;
    if (node->current_player == PLAYER1) cp = PLAYER2;
    else cp = PLAYER1;

    // allocating child_arr 
    
    node->child_arr_size = valid_move_no(node->board, node->SB_index);
    node->child_arr = malloc(sizeof(struct Node *)*node->child_arr_size);

    // change bounds s.t. we iterate over all SBs if SB_index is -1, 
    // just SB_index otherwise 

    int min, max;
    if (node->SB_index == -1) {
        min = 0; 
        max = GRIDSIZE;
    }
    else {
        min = node->SB_index; 
        max = node->SB_index + 1;
    }

    int count = 0;
    
    for (int i=min; i<max; i++){
        for (int j=0; j<GRIDSIZE; j++){

            if (!valid_move(node->board, i, j)) continue;

            // updating the board for the child
            LargeBoard *new_board = malloc(sizeof(LargeBoard));
            memcpy(new_board, node->board, sizeof(LargeBoard));
            play_move(new_board, i, j, cp);
            
            
            // if the SB that the player sends the opponent to is
            // not free, then we make it -1
            int new_SB_index = j;
            if (determine_SB_winner((*new_board)[j]) != NO_PLAYER){
                new_SB_index = -1;
            }

            //finally, we can initialize the child
            node->child_arr[count] = create_node(new_board, cp, new_SB_index, node);

            count++;

        }
    }
    
}

void free_tree(struct Node *root){
    for (int i=0; i<root->child_arr_size; i++){
        free_tree(root->child_arr[i]);
    }
    free(root->board);
    if (root->child_arr_size != 0){
        free(root->child_arr);
    }
    free(root);
}


