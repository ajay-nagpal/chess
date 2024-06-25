#include"def.hpp"
#include<iostream>
#include<vector>

using namespace std;

const vector<int>knight_dir{-8,-19,-21,-12,8,19,21,12};
const vector<int>rook_dir{-1,-10,1,10};
const vector<int> bishop_dir{-9,-11,11,9};
const vector<int> king_dir{-1,-10,1,10,-9,-11,11,9};

int square_attacked(const int sq,const int side, const s_board *pos){

    ASSERT(sq_on_board(sq));
    ASSERT(side_valid(side));
    ASSERT(check_board(pos));

    int piece=0,index=0,temp_sq=0,dir=0;
    //pawns
    if(side==white){// if the attacking side is whitejese 55  to 44 ya 46 me se kisi pr white pawn to apna 55 white pawn se atatacked
        if(pos->pieces[sq-11]==wp || pos->pieces[sq-9]==wp){
            return true;
        }
    }
    else{
        if(pos->pieces[sq+11]==bp || pos->pieces[sq+9]==bp){
            return true;
        }
    }
    // knights
    for(index=0;index<8;index++){// looping through 8 knight direction
        piece=pos->pieces[sq+knight_dir[index]];
        if(piece!=off_board && is_kn(piece) && piece_color[piece]==side){
            return true;
        }
    }
    // for rooks  queeen
    for(index=0;index<4;index++){
        dir=rook_dir[index];
        temp_sq=sq+dir;
        piece=pos->pieces[temp_sq];

        while(piece!=off_board){
            if(piece!=emptyy){
                if(is_rq(piece)  && piece_color[piece]==side){
                    return true;
                }
                break;
            }
            temp_sq+=dir;// if it empty then carry on the next square
            piece=pos->pieces[temp_sq];
        }
    }
    // bishpo,, queens
    for(index=0;index<4;index++){
        dir=bishop_dir[index];
        temp_sq=sq+dir;
        piece=pos->pieces[temp_sq];

        while(piece!=off_board){
            if(piece!=emptyy){
                if(is_bq(piece)  && piece_color[piece]==side){
                    return true;
                }
                break;
            }
            temp_sq+=dir;
            piece=pos->pieces[temp_sq];
        }
    }
    // kings 

    for(index=0;index<8;index++){
        piece=pos->pieces[sq+king_dir[index]];
        if(piece!=off_board && is_ki(piece) && piece_color[piece]==side)
            return true;
    }

    return false;
}