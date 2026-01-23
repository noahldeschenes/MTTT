#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "board_utils.h"
#include "node_utils.h"

#define ROW_LENGTH 10 // 9 characters for 'X', 'O' or '.', one for newline
#define SIDE_LENGTH 3 // 3x3 square, so the side_length is 3


Player char_to_player(char c){
    if (c == 'X') return PLAYER1;
    if (c == 'O') return PLAYER2;
    if (c == '.') return NO_PLAYER;
}

char player_to_char(Player p){
    if (p == PLAYER1) return 'X';
    if (p == PLAYER2) return 'O';
    if (p == NO_PLAYER) return '.';
}

void update_specific_SB(char *input, int index, SmallBoard sb){

    int x = index%SIDE_LENGTH;
    int y = index/SIDE_LENGTH;

    int start_index = y*ROW_LENGTH*SIDE_LENGTH+x*SIDE_LENGTH; // top right corner of sb in input string
    
    for (int i=0; i<SIDE_LENGTH; i++){
        for (int j=0; j<SIDE_LENGTH; j++){
            sb[j+SIDE_LENGTH*i] = char_to_player(input[start_index+j+i*ROW_LENGTH]);
        }
    }
}

void put_specific_LB_line(LargeBoard *board, int lineno, char *line){
    //x and y are incicies for LB, i and j for SB
    int y = lineno/SIDE_LENGTH;
    int j = lineno%SIDE_LENGTH;
    for (int x=0; x<SIDE_LENGTH; x++){
        for (int i=0; i<SIDE_LENGTH; i++){
            char c = player_to_char((*board)[x+y*SIDE_LENGTH][i+j*SIDE_LENGTH]);
            line[i+SIDE_LENGTH*x] = c;
        }
    }
    line[ROW_LENGTH-1] = '\n';
}

void sanitize_string_to_LB_input(char *input){
    for (int i=0; i<SIDE_LENGTH*SIDE_LENGTH; i++){
        for (int j=0; j<ROW_LENGTH; j++){
            char cur = input[i*ROW_LENGTH+j];
            assert(cur != '\0');
            if (j == ROW_LENGTH-1) assert(cur == '\n');
            else assert(cur == '.' || cur == 'X' || cur == 'O');
        }
    }
    assert(input[SIDE_LENGTH*SIDE_LENGTH*ROW_LENGTH] == '\0');
}

LargeBoard *string_to_LB(char *input){

    sanitize_string_to_LB_input(input);

    LargeBoard *board = calloc(1, sizeof(LargeBoard));
    
    for (int i=0; i<GRIDSIZE; i++){
        update_specific_SB(input, i, (*board)[i]);
    }

    return board;
    
}

char *LB_to_string(LargeBoard *board){
    int len = ROW_LENGTH*(SIDE_LENGTH*SIDE_LENGTH)+1;
    char *return_string = malloc(sizeof(char)*len);
    
    for (int i=0; i<SIDE_LENGTH*SIDE_LENGTH; i++){
        put_specific_LB_line(board, i, &return_string[ROW_LENGTH*i]);
    }
    return_string[len-1] = '\0';
    return return_string;
}

/*
Player check_line(SmallBoard board, int i1, int i2, int i3) {
    // player only wins a line if they get all 3 indicies, NO_PLAYER otherwise
    if (board[i1] == NO_PLAYER || board[i1] == TIE) return NO_PLAYER;
    if (board[i1] == board[i2] && board[i2] == board[i3]) return board[i1];
    return NO_PLAYER;
}

Player determine_SB_winner(SmallBoard board){
    Player results[8];
    // vertical lines
    results[0] = check_line(board, 0, 3, 6);
    results[1] = check_line(board, 1, 4, 7);
    results[2] = check_line(board, 2, 5, 8);
    // horizontal lines
    results[3] = check_line(board, 0, 1, 2);
    results[4] = check_line(board, 3, 4, 5);
    results[5] = check_line(board, 6, 7, 8);
    // diagonals
    results[6] = check_line(board, 0, 4, 8);
    results[7] = check_line(board, 6, 4, 2);
    
    // check to see if any lines win
    for (int i=0; i<8; i++){
        if (results[i] != NO_PLAYER) return results[i];
    }

    // check if any NO_PLAYER squares are left
    for (int i=0; i<GRIDSIZE; i++){
        if (board[i] == NO_PLAYER) return NO_PLAYER;
    }
    
    return TIE;

}
*/

Player determine_SB_winner(SmallBoard board){
    if (board[4] != NO_PLAYER && board[4] != TIE){ 
        if (board[0] == board[4] && board[4] == board[8]) return board[4];
        if (board[1] == board[4] && board[4] == board[7]) return board[4];
        if (board[2] == board[4] && board[4] == board[6]) return board[4];
        if (board[3] == board[4] && board[4] == board[5]) return board[4];
    }
    if (board[0] != NO_PLAYER && board[0] != TIE){
        if (board[0] == board[1] && board[1] == board[2]) return board[0];
        if (board[0] == board[3] && board[3] == board[6]) return board[0];
    }
    if (board[8] != NO_PLAYER && board[8] != TIE){
        if (board[8] == board[7] && board[7] == board[6]) return board[8];
        if (board[8] == board[5] && board[5] == board[2]) return board[8];
    }
    for (int i=0; i<GRIDSIZE; i++){
        if (board[i] == NO_PLAYER) return NO_PLAYER;
    }
    return TIE;
}

Player determine_LB_winner(LargeBoard *board){

    SmallBoard result_board; //board made from results from the LB's SmallBoards

    for (int i=0; i<GRIDSIZE; i++){
        result_board[i] = determine_SB_winner((*board)[i]);
    }

    return determine_SB_winner(result_board);
}


void play_move(LargeBoard *board, int i, int j, Player p){
    //plays the given move, assuming it's valid
    (*board)[i][j] = p;
}

bool valid_move(LargeBoard *board, int i, int j){
    // returns true if (i, j) is a valid move, false otherwise 
    if (determine_SB_winner((*board)[i]) != NO_PLAYER) return 0;
    if ((*board)[i][j] != NO_PLAYER) return 0;
    return 1;
}

int valid_move_no(LargeBoard *board, int SB_index){
    // counts the number of valid moves for a given board and SB_index
    // counts valid moves for every SB if SB_index == -1

    int min, max;
    if (SB_index == -1) {
        min = 0; 
        max = GRIDSIZE;
    }
    else {
        min = SB_index; 
        max = SB_index + 1;
    }

    int count = 0;

    for (int i=min; i<max; i++){
        for (int j=0; j<GRIDSIZE; j++){
            if (valid_move(board, i, j)) count++;
        }
    }

    return count;
}

int make_random_move(LargeBoard *board, int SB_index, Player p){
    assert(valid_move_no(board, SB_index) != 0);
    
    int index = (rand() % valid_move_no(board, SB_index));
    
    int min, max;
    if (SB_index == -1) {
        min = 0; 
        max = GRIDSIZE;
    }
    else {
        min = SB_index; 
        max = SB_index + 1;
    }

    for (int i=min; i<max; i++){
        for (int j=0; j<GRIDSIZE; j++){
            if (valid_move(board, i, j)){
                if (index == 0) {
                    play_move(board, i, j, p); 
                    if (determine_SB_winner((*board)[j]) == NO_PLAYER) return j;
                    else return -1;
                }
                else index--;
            }
        }
    }
}



