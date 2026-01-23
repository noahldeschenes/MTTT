#include "board_utils.h"

struct Node {

    // board data
    LargeBoard *board; 
    Player winner;
    Player current_player;
    int SB_index; // index of the SB current_player is playing on, -1 if anywhere

    // tree data
    struct Node *parent;
    struct Node **child_arr;
    int child_arr_size;

    // winrate data
    double total_wins;
    int total_simulations;

};

double upper_confidence_bound(struct Node *node);
struct Node *get_highest_UCB_child(struct Node *node);
struct Node *create_node(LargeBoard *board, Player cp, int SB_index, struct Node *par);
void init_children(struct Node *node);
void free_tree(struct Node *root);
