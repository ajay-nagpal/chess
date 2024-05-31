#include"cstdlib"
#include"def.hpp"
#include<iostream>
#include<vector>

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

void init_file_rank_board(){
    int index=0,file=f1,rank=r1;
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

    // only for checking purpose that it is printing correctly
    // no need in actual code

    
    // printf("fileboard:\n\n");

    // for(index=0;index<board_sq_num;index++){
    //     if(index%10==0 && index!=0)
    //         printf("\n");
        
    //     printf("%4d",file_board[index]);
    // }
    // printf("\n\n");
    // printf("rankboard:\n\n");

    // for(index=0;index<board_sq_num;index++){
    //     if(index%10==0 && index!=0)
    //         printf("\n");
        
    //     printf("%4d",rank_board[index]);
    // }
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
}
    