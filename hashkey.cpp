#include"def.h"
#include<iostream>

using namespace std;

u64 generate_pos_key(const s_board *pos){// pointer to the pos we want to generae=te key for
//const cause it wont be ultered inside the function
    int sq=0;
    u64 finalkey=0;// final key that we are going to return
    int piece=emptyy;


    for(sq=0;sq<board_sq_num;sq++){
        // loop all the square of the board and set the piece whatever value at that time at that square of thar array

        piece=pos->pieces[sq];

        if(piece!=no_sq && piece!=emptyy && piece!=OFFBOARD){// if it is not no_sq then it must be from actual chessboard
            ASSERT(piece>=wp && piece<=bk);// this should be true if a piece is present and not empty
            finalkey ^= piece_keys[piece][sq];// then we hash whatever  number is present at the piecekeys array
        }
    }

    if(pos->side==white)
        finalkey^=side_key;// if its white then hash in the side key

    if(pos->enpass!=no_sq){
        ASSERT(pos->enpass>=0 && pos->enpass<board_sq_num);//>=0 <120

        ASSERT(sq_on_board(pos->enpass));
		ASSERT(rank_board[pos->enpass] == r3 || rank_board[pos->enpass] == r6);

        finalkey ^= piece_keys[emptyy][pos->enpass];// cz doing for enpass and we generate some 
        // for piece keys  ,   so now we need empty
    }

    ASSERT(pos->castle_perm>=0 && pos->castle_perm<=15);// 0 to 15 hoti h casteling
     
    finalkey ^= castle_keys[pos->castle_perm];
    
    return finalkey;

}