
#include"cstdlib"
#include"def.h"
#include<iostream>
#include<vector>

using namespace std;


// macro


// working of this macro
// to fill all 64 bits with random numbers
// let 64 bits

// 0000 000000000000000 000000000000000 000000000000000 111111111111111     isse bda nhi hoga 15 bit ka random number
// 0000 000000000000000 000000000000000 111111111111111 000000000000000
// 0000 000000000000000 111111111111111 000000000000000  000000000000000
// 0000 000000000000000 111111111111111 000000000000000  000000000000000
// in 2nd line it is again generai=ting number but shifting 15 times so agle zero 1 bnege usse

// rand will generate a 15 bit random number

//then at last add all these number tpogether  and getting a 64 bit number filled with random 0 or 1

//'<<' has lower precedence than '+'; '+' will be evaluated first
#define rand64 (    ((u64) rand() )    |\
                    ((u64) rand() << 15) |\
                    ((u64) rand()<<30) |\
                    ((u64) rand()<<45) |\
                    (((u64) rand() & 0xf)<<60)    )// random number generate   the 0xf(1111) se and and then <60





vector<int>sq120to64(board_sq_num);//120 based ka 64 equi return krega

vector<int>sq64to120(64);//64 ka 120 return

vector<u64>setmask(64);
vector<u64>clearmask(64);

vector<vector<u64> >piece_keys(13,vector<u64>(120,0));// pieces on thier own cant be unique so we ned it by square thats why 2d array
// if any piece move to diff square then key should also changge cz pos has changed
//thats why pieces index(13) and square (120)
u64 side_key;// for hashing a random number
int a=10;

// 0 0 0 0  4 bits to represent white king ,queen   black king ,queen   1   2    4     8
vector<u64>castle_keys(16);

// array that will give us the ans of on which fule/rank our square is on currently
// also put it in def.h
vector<int>file_board(board_sq_num);
vector<int>rank_board(board_sq_num);

// now fill it with help of func

void init_file_rank_board(){
    int index=0,file=f1,rank=r1;
    int sq=a1,sq64=0;

    for(index=0;index<board_sq_num;index++){
        file_board[index]=OFFBOARD;
        rank_board[index]=OFFBOARD;
    }

    for(rank=r1;rank<=r8;rank++){
        for(file=fa;file<=fh;file++){

            sq=fr_to_sq(file,rank);// getting square using filerank to square macro
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









// now we will fill these 2 arrays  with random 64 numbers

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

    // in this fun we will initilize the set and clear mask 
    // so that we can use them to clear and set a bit


    for(index=0;index<64;index++){
        setmask[index]=0ULL;
        clearmask[index]=0ULL;
    }

    // set and clear msk
    for(index=0;index<64;index++){
        setmask[index] |= (1ULL<<index);
        
        clearmask[index]= ~setmask[index];
    }

    // create macros in def.h taki baar baar type na krna pade itna bda
}


// initialization 120 to 64 code

void initsq120to64(){
    int index=0;
    int file=fa;
    int rank=r1;

    int sq=a1;
    int sq64=0;// it will repesent 64 based array of that 120 array


    for(index=0;index<board_sq_num;index++){
        sq120to64[index]=65;// it cant return 65 0 to 63
    }

    for(index=0;index<64;index++){
        sq64to120[index]=120;// take value that cannot be pos like 120 ,0 to 119 hota h 120 aray
    }


    for(rank=r1;rank<=r8;rank++){
        for(file=fa;file<=fh;file++){

            sq=fr_to_sq(file,rank);// get the sq by passing file adn rank

            sq64to120[sq64]=sq;//64 to 120 at index of 64 which will give us sq

            sq120to64[sq]=sq64;// sq 120 to 64 at our curr sq set to sq64

            sq64++;
        }
    }
}


void AllInit(){// def,h me bhi dalenge so that main will be able to call it

    // all that initilzation fun

    initsq120to64();

    init_bitmask();
    
    inithashkey();

    init_file_rank_board();

}
    