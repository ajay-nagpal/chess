#include"def.hpp"
#include<iostream>

using namespace std;

u64 generate_pos_key(const s_board *pos){
    int sq=0;
    u64 finalkey=0;
    int piece=emptyy;


    for(sq=0;sq<board_sq_num;sq++){
        piece=pos->pieces[sq];

        if(piece!=no_sq && piece!=emptyy && piece!=off_board){
            ASSERT(piece>=wp && piece<=bk);
            finalkey ^= piece_keys[piece][sq];
        }
    }

    if(pos->side==white)
        finalkey^=side_key;

    if(pos->enpass!=no_sq){
        ASSERT(pos->enpass>=0 && pos->enpass<board_sq_num);

        //ASSERT(sq_on_board(pos->enpass));
		//ASSERT(rank_board[pos->enpass] == r3 || rank_board[pos->enpass] == r6);

        finalkey ^= piece_keys[emptyy][pos->enpass];
    }

    ASSERT(pos->castle_perm>=0 && pos->castle_perm<=15);
     
    finalkey ^= castle_keys[pos->castle_perm];
    
    return finalkey;

}