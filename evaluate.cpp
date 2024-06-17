#include<iostream>
#include"def.hpp"

using namespace std;

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

//to get mirror square
// jese a2 pr h to uska mirror black ka a7 pr hoga to a7 ka square return 
// 64 ka ek array mirror

vector<int> mirror= {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};
	
#define mirror64(sq) (mirror[sq])

int eval_pos(const s_board * pos){

    int piece=0,piece_num=0,sq=0;
    int score=pos->material[white]-pos->material[black];// assumption white score +ve blkack -ve
    // at the end  if white to move return +score  if black to move we will return -ve score
    // movegen jesa code thoda boht
    piece=wp;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score+=pawn_table[SQ64(sq)];
    }

    piece=bp;
    for(piece_num=0;piece_num<pos->piece_num[piece];piece_num++){
        sq=pos->piece_list[piece][piece_num];
        ASSERT(sq_on_board(sq));
        score-=pawn_table[mirror64(SQ64(sq))];
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