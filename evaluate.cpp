#include<iostream>
#include"def.hpp"

using namespace std;

const int pawn_isolated=-10;// penalty for isolated pawn
// bonus for passed pawn index this  by rank
const vector<int>pawn_passed={0,5,10,20,35,60,100,200};

//open pawn The pawn is the only pawn of its color on its file. and No Enemy Pawns Blocking It:
//In summary, controlling open and semi-open files is a key strategic element in chess, allowing for greater mobility of rooks and queens, 
const int rook_open_file=10;//open file means there are no pawn on that file means  No pawns of either color are present on the file.
const int rook_semi_open_file=5;// semi open means there are no opposing pawn on that file means Pawns of only one color are present on the file.

const int queen_open_file=5;
const int queen_semi_open_file=3;

const int bishop_pair=30;
// endgame condn
// if there is no queen on board and  if material is less than this
#define endgame ( piece_value[wr] + 2 * piece_value[wn] + 2 * piece_value[wp]+ piece_value[wk])
// then it is a endgame

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

const vector<int> king_e = {// king in the end game
// penalty for staying at the corners	
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	40	,	40	,	20	,	10	,	0	,
	0	,	10	,	20	,	20	,	20	,	20	,	10	,	0	,
	-10,	0	,	10	,	10	,	10	,	10	,	0	,	-10	,
	-50	,	-10	,	0	,	0	,	0	,	0	,	-10	,	-50	
};

const vector<int> king_O = {	// opening m  penalty on walking forward
	0	,	5	,	5	,	-10	,	-10	,	0	,	10	,	5	,
	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,	-30	,
	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,	-50	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,
	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70	,	-70		
};


//8/6R1/2k5/6P1/8/8/4nP2/6K1 w - - 1 41 
int material_draw(const s_board *pos) {

	ASSERT(check_board(pos));
	
    if (!pos->piece_num[wr] && !pos->piece_num[br] && !pos->piece_num[wq] 
        && !pos->piece_num[bq]){// we dont have any rook  queen and bishop adn have <3 knights then
        // its a material draw
	    if (!pos->piece_num[bb] && !pos->piece_num[wb]){
	        if (pos->piece_num[wn] < 3 && pos->piece_num[bn] < 3){
                return true; 
            }
	    } 
        else if (!pos->piece_num[wn] && !pos->piece_num[bn]){
	        if (abs(pos->piece_num[wb] - pos->piece_num[bb]) < 2){ 
                return true; 
            }
	    } 
        else if ((pos->piece_num[wn] < 3 && !pos->piece_num[wb]) || (pos->piece_num[wb] == 1 &&
                 !pos->piece_num[wn])){
	        if ((pos->piece_num[bn] < 3 && !pos->piece_num[bb]) || (pos->piece_num[bb] == 1 && 
                !pos->piece_num[bn])){ 
                return true;
            }
	    }
	} 
    else if (!pos->piece_num[wq] && !pos->piece_num[bq]) {
        if (pos->piece_num[wr] == 1 && pos->piece_num[br] == 1){
            if ((pos->piece_num[wn] + pos->piece_num[wb]) < 2 && 
                (pos->piece_num[bn] + pos->piece_num[bb]) < 2){ 
                return true; 
            }
        } 
        else if (pos->piece_num[wr] == 1 && !pos->piece_num[br]){
            if ((pos->piece_num[wn] + pos->piece_num[wb] == 0) &&
                 (((pos->piece_num[bn] + pos->piece_num[bb]) == 1) ||
                  ((pos->piece_num[bn] + pos->piece_num[bb]) == 2))){ 
                return true; 
            }
        } 
        else if (pos->piece_num[br] == 1 && !pos->piece_num[wr]){
            if ((pos->piece_num[bn] + pos->piece_num[bb] == 0) && 
                (((pos->piece_num[wn] + pos->piece_num[wb]) == 1) || 
                ((pos->piece_num[wn] + pos->piece_num[wb]) == 2))){ 
                return true; 
            }
        }
    }
  return false;
}


int eval_pos(const s_board * pos){

    int piece=0,piece_num=0,sq=0;
    int score=pos->material[white]-pos->material[black];// assumption white score +ve blkack -ve
    // at the end  if white to move return +score  if black to move we will return -ve score
    // movegen jesa code thoda boht

    // if ono pawns on the board
    if(!pos->piece_num[wp] && !pos->piece_num[bp] && material_draw(pos)==true){
        return 0;
    }
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
			//cout<<"wp iso: "<<print_sq(sq)<<endl;
			score += pawn_isolated;
		}
		
		if( (white_passed_mask[SQ64(sq)] & pos->pawns[black]) == 0) {
            //cout<<"wp passed: "<<print_sq(sq)<<endl;
			score += pawn_passed[rank_board[sq]];
		}
    }

    piece=bp;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=pawn_table[mirror64(SQ64(sq))];

        if( (isolated_mask[SQ64(sq)] & pos->pawns[black]) == 0) {// and with bitboard
			//cout<<"bp iso: "<<print_sq(sq)<<endl;
			score -= pawn_isolated;// cz black wants to be score more negative// --10 means +10 so it added that neg score
		}
		
		if( (black_passed_mask[SQ64(sq)] & pos->pawns[white]) == 0) {// mean shteer are no white pawn tha twill stop it from prom
            //cout<<"bp passed: "<<print_sq(sq)<<endl;
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

        if(!(pos->pawns[both] & file_bb_mask[file_board[sq]])){// curr bothh pawn bitboard & file mask of the cuirr file wea are on  is zero
            score+=rook_open_file;// then we are on open file cz there are no pawn on that file of our color opponent ke color ka ho skta h apna rook h
        }
        else if(!(pos->pawns[white] & file_bb_mask[file_board[sq]])){
            score+=rook_semi_open_file;
        }
    }

    piece=br;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=rook_table[mirror64(SQ64(sq))];

        if(!(pos->pawns[both] & file_bb_mask[file_board[sq]])){// curr bothh pawn bitboard & file mask of the cuirr file wea are on  is zero
            score-=rook_open_file;// then we are on open file cz there are no pawn on that file of our color opponent ke color ka ho skta h apna rook h
        }
        else if(!(pos->pawns[black] & file_bb_mask[file_board[sq]])){
            score-=rook_semi_open_file;
        }
    }

    piece=wq;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));

        if(!(pos->pawns[both] & file_bb_mask[file_board[sq]])){// curr bothh pawn bitboard & file mask of the cuirr file wea are on  is zero
            score+=queen_open_file;// then we are on open file cz there are no pawn on that file of our color opponent ke color ka ho skta h apna rook h
        }
        else if(!(pos->pawns[white] & file_bb_mask[file_board[sq]])){
            score+=queen_semi_open_file;
        }
    }

    piece=bq;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));

        if(!(pos->pawns[both] & file_bb_mask[file_board[sq]])){// curr bothh pawn bitboard & file mask of the cuirr file wea are on  is zero
            score-=queen_open_file;// then we are on open file cz there are no pawn on that file of our color opponent ke color ka ho skta h apna rook h
        }
        else if(!(pos->pawns[black] & file_bb_mask[file_board[sq]])){
            score-=queen_semi_open_file;
        }
    }

    // for king

    piece=wk;
    sq=pos->piece_list[piece][0];

    if(pos->material[black]<=endgame){
        score+=king_e[SQ64(sq)];
    }
    else{
        score+=king_O[SQ64(sq)];
    }
    
    piece=bk;
    sq=pos->piece_list[piece][0];

    if(pos->material[white]<=endgame){
        score-=king_e[mirror64(SQ64(sq))];
    }
    else{
        score-=king_O[mirror64(SQ64(sq))];
    }

    if(pos->piece_num[wb]>=2)
        score+=bishop_pair;
    if(pos->piece_num[bb]>=2)
        score-=bishop_pair;

    if(pos->side==white){
        return score;
    }
    return -score;   
}

/*
nul move pruning 
apna move skip krna oppp ko 1 extra move dena
// for null move king cant be i check
// let apna alpha beta -100 and 100

opp k score ki val jb vo search krega
val=-alphabeta(depth-1-r,-beta,-beta+1);

means iska alpha and beta is -100 and -99
let score -120
 and we know
 Alpha represents the best value that the maximizer (usually the engine itself) 
 can guarantee at that level or above.
Beta represents the best value that the minimizer (usually the opponent) 
can guarantee at that level or below.

so its alpha beta remain unchanged 
it will return -(-100)  100  which is >=or beta so pruning hogi 
 kuki 1 free move pr bhi vo hme effect nih kr paya no need to search in this depth further
*/