

#ifndef PLAYER_ENUM
#define PLAYER_ENUM

typedef enum {
    NO_PLAYER,
    PLAYER1,
    PLAYER2,
    TIE
} Player;

#endif

#define GRIDSIZE 9

// a large board is a 3x3 grid of small 3x3 boards
typedef Player SmallBoard[GRIDSIZE];
typedef SmallBoard LargeBoard[GRIDSIZE];
typedef int bool;



LargeBoard *string_to_LB(char *input);
char *LB_to_string(LargeBoard *board);

Player determine_SB_winner(SmallBoard board);
Player determine_LB_winner(LargeBoard *board);

void play_move(LargeBoard *board, int i, int j, Player p);
bool valid_move(LargeBoard *board, int i, int j);
int valid_move_no(LargeBoard *board, int SB_index);
int make_random_move(LargeBoard *board, int SB_index, Player p);


/*
"X X O O X X O O X \n
 O X . X . O O X O \ n
 X O O O X X O X . \ n
 X X X O O O . X O \ n 
 O X X X O X O X . \ n
 . . . . . . . . . \ n
 X O O O O . . X . \ n
 O X O X . . O X . \ n
 . . . . . . . X . \ n "

 "O X O O X X O O X \n
  O X . X . O O X O \ n 
  X O O O X X O X . \ n 
  X X X O O O . X O \ n 
  O X X X O X O X . \ n 
  . . . . . . . . . \ n 
  X O O O O . . X . \ n 
  O X O X . . O X . \ n 
  . . . . . . . X . \ n "
 */

 