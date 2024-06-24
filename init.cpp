#include"cstdlib"
#include"def.hpp"
#include<iostream>
#include<vector>
#include <iomanip>

using namespace std;
#define rand64 (    ((u64) rand() )    |\
                    ((u64) rand() << 15) |\
                    ((u64) rand()<<30) |\
                    ((u64) rand()<<45) |\
                    (((u64) rand() & 0xf)<<60)    )

vector<int>sq120to64(board_sq_num);
vector<int>sq64to120(64);

vector<u64>setmask(64);
vector<u64>clearmask(64);

vector<vector<u64> >piece_keys(13,vector<u64>(120));
u64 side_key;
int a=10;

vector<u64>castle_keys(16);

vector<int>file_board(board_sq_num);
vector<int>rank_board(board_sq_num);


// look at isolated and past pawns  which make weak pos
// past pawn jo forward ja skta h ura opponent side tk bina uske pawn se captured or hindered hue a4 se a8 tk chla hgya let
// isolated papwn means no pawn of the ssame side on a neighbor files 
// so now we will use our pawn bitboard

/*
let bitboard ke pawn pos h ye
let jana na h ki f file open h kya means no pawn on that file
us ka mask se and krke jaan skte h pawn h ya nhi

so create these mask for file and  rank 
*/


/*
koi pawn passed h kaya explanation
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 x 0 0 0  le tpawn on  e3
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

to file d d4 se d8
file e ffrom e4 to e8 
and file f f4 to f8  pr koi black pawn nhi h then x is passed
to mask will look like this
0 0 0 1 1 1 0 0
0 0 0 1 1 1 0 0
0 0 0 1 1 1 0 0
0 0 0 1 1 1 0 0
0 0 0 1 1 1 0 0
0 0 0 0 x 0 0 0  
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

to if we and black bit bard with this mask then
aur result zero aa jaye to means there is no black pawn  stopping out pawn x means pawn x is parser
*/


vector<u64>file_bb_mask(8);// index by rank and file number
vector<u64>rank_bb_mask(8);

vector<u64>black_passed_mask(64);
vector<u64>white_passed_mask(64);
vector<u64>isolated_mask(64);

// mask initilize for each file and rank
void init_eval_masks(){
    int sq,tsq,f,r;

    for(sq=0;sq<8;sq++){
        file_bb_mask[sq]=0ULL;
        rank_bb_mask[sq]=0ULL;
    }
    for(r=r8;r>=r1;r--){
        for(f=fa;f<=fh;f++){
            sq=r*8+f;
            file_bb_mask[f]|=(1ULL<<sq);// set the relevent bit for file and rank mask
            rank_bb_mask[r]|=(1ULL<<sq);
        }
    }
    // now for other 3 masks

    for(sq=0;sq<64;sq++){
        isolated_mask[sq]=0ULL;
        white_passed_mask[sq]=0ULL;
        black_passed_mask[sq]=0ULL;
    }
    for(sq=0;sq<64;sq++){// for each square iterate in a specific direction
        tsq=sq+8;// +8 goes up 1 rank
        while(tsq<64){// for all square
            white_passed_mask[sq]|=(1ULL<<tsq);
            tsq+=8;
        }
        tsq=sq-8;// go down till 0 rank
        while(tsq>=0){
            black_passed_mask[sq]|=(1ULL<<tsq);
            tsq-=8;
        }

        if(file_board[SQ120(sq)]>fa){// visit all files right dire
            isolated_mask[sq]|=file_bb_mask[file_board[SQ120(sq)]-1];

            tsq=sq+7;// for all files  upward
            while(tsq < 64){
                white_passed_mask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq-9;// for all files downward
            while(tsq >= 0){
                black_passed_mask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }

        if(file_board[SQ120(sq)] < fh){// visit all files left dir
            isolated_mask[sq] |= file_bb_mask[file_board[SQ120(sq)] + 1];

            tsq = sq + 9;// for all files upward
            while(tsq < 64){
                white_passed_mask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 7;// for all files downward
            while(tsq >= 0){
                black_passed_mask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
    }
}

void init_file_rank_board(){
    int index=0,file=fa,rank=r1;
    int sq=a1,sq64=0;

    for(index=0;index<board_sq_num;index++){
        file_board[index]=OFFBOARD;
        rank_board[index]=OFFBOARD;
    }

    for(rank=r1;rank<=r8;rank++){
        for(file=fa;file<=fh;file++){

            sq=fr_to_sq(file,rank);
            file_board[sq]=file;
            rank_board[sq]=rank;
        }
    }
}

void inithashkey(){
    int index=0;
    int index2=0;

    for(index=0;index<13;index++){
        for(index2=0;index2<120;index2++){

            piece_keys[index][index2]=rand64;
        }
    }

    side_key=rand64;

    for(index=0;index<16;index++){
        castle_keys[index]=rand64;
    }

}

void init_bitmask(){
    int index=0;

    for(index=0;index<64;index++){
        setmask[index]=0ULL;
        clearmask[index]=0ULL;
    }
    for(index=0;index<64;index++){
        setmask[index] |= (1ULL<<index);
        
        clearmask[index]= ~setmask[index];
    }
}

void initsq120to64(){
    int index=0;
    int file=fa;
    //int rank= rank::r1;
    int rank=r1;

    int sq=a1;
    int sq64=0;

    for(index=0;index<board_sq_num;index++){
        sq120to64[index]=65;
    }

    for(index=0;index<64;index++){
        sq64to120[index]=120;
    }

    for(rank=r1;rank<=r8;rank++){
        for(file=fa;file<=fh;file++){

            sq=fr_to_sq(file,rank);
            sq64to120[sq64]=sq;
            sq120to64[sq]=sq64;
            sq64++;
        }
    }
}

void AllInit(){

    initsq120to64();

    init_bitmask();
    
    inithashkey();

    init_file_rank_board();
    // iske baad mask vala
    init_eval_masks();

    init_mvvlva();
}
    