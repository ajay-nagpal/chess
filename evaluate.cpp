#include<iostream>
#include"def.hpp"

using namespace std;

const int pawn_isolated=-10;// penalty for isolated pawn
// bonus for passed pawn index this  by rank
const vector<int>pawn_passed={0,5,10,20,35,60,100,200};
const int rook_open_file=5;

// fun to return evaluate score of a pos  in hundreads of pawn
const vector<int> pawn_table= {// values are in 100s of a apawn means 10==.1
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const vector<int> knight_table={
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};

const vector<int> bishop_table= {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};

const vector<int> rook_table= {
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0		
};

const vector<int> king_e = {	
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	
};

const vector<int> king_O = {	
	0	,	5	,	5	,	-10	,	-10	,	0	,	10	,	5	,
	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,
	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70		
};

int eval_pos(const s_board * pos){

    int piece=0,piece_num=0,sq=0;
    int score=pos->material[white]-pos->material[black];// assumption white score +ve blkack -ve
    // at the end  if white to move return +score  if black to move we will return -ve score
    // movegen jesa code thoda boht
    piece=wp;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];// 120 based
        ASSERT(sq_on_board(sq));
        score+=pawn_table[SQ64(sq)];
        /*
            101
            101
            101 let middle 0 ki jgh pawn to if we and  this mask with white pawn and result is zero
            thjen isolated pawn
        */
        if( (isolated_mask[SQ64(sq)] & pos->pawns[white]) == 0) {// and with bitboard
			cout<<"wp iso: "<<print_sq(sq)<<endl;
			score += pawn_isolated;
		}
		
		if( (white_passed_mask[SQ64(sq)] & pos->pawns[black]) == 0) {
            cout<<"wp passed: "<<print_sq(sq)<<endl;
			score += pawn_passed[rank_board[sq]];
		}
    }

    piece=bp;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=pawn_table[mirror64(SQ64(sq))];

        if( (isolated_mask[SQ64(sq)] & pos->pawns[black]) == 0) {// and with bitboard
			cout<<"bp iso: "<<print_sq(sq)<<endl;
			score -= pawn_isolated;// cz black wants to be score more negative// --10 means +10 so it added that neg score
		}
		
		if( (black_passed_mask[SQ64(sq)] & pos->pawns[white]) == 0) {// mean shteer are no white pawn tha twill stop it from prom
            cout<<"bp passed: "<<print_sq(sq)<<endl;
			score -= pawn_passed[7-rank_board[sq]];// cz pawn passed index byr rank h aur black down the board jata h to 7-
		}
    }

    piece=wn;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score+=knight_table[SQ64(sq)];
    }

    piece=bn;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=knight_table[mirror64(SQ64(sq))];
    }

    piece=wb;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score+=bishop_table[SQ64(sq)];
    }

    piece=bb;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=bishop_table[mirror64(SQ64(sq))];
    }

    piece=wr;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score+=rook_table[SQ64(sq)];
    }

    piece=br;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=rook_table[mirror64(SQ64(sq))];
    }

    if(pos->side==white){
        return score;
    }
    return -score;   
}