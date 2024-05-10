#include<iostream>
#include"def.h"
#include<vector>

using namespace std;

const vector<int> bit_table{
    63,30,3,32,25,41,22,33,
    15,50,42,13,11,53,19,34
    ,61,29,2,51,21,43,45,10,
    18,47,1,54,9,57,0,35,
    62,31,40,4,49,5,52,26,
    60,6,23,44,46,27,56,16,
    7,39,48,24,59,14,12,55,
    38,28,58,20,37,17,36,8
};

int popbit(u64 *bb){// it takes first bit starting at least significant bit
    // in a bitboard and returns the index that this bit was set at
    // so that we can know which square that the bit was set on
    // and sets that bits then to zero

    u64 b= *bb ^(*bb-1);

    unsigned int fold=(unsigned) ((b &  0xffffffff)  ^ (b>>32));

    *bb &=(*bb -1);

    return bit_table[(fold * 0x783a9b23) >>26];
}

int countbit(u64 b){// counts and return the number of bits that are 1 inside a bitboard
    int r;

    for(r=0;b; r++ , b&=b-1);
        
    return r;
}

// add these function in def.h so that we can use them  + macros 


void print_bit_board(u64 bb){// add this func in def.h 
    u64 shiftme=1ULL;

    int rank=0;
    int file=0;
    int sq=0;
    int sq64=0;

    // print krnge board ok isme rank 1 ,2 3 4 bottom se h jese original me hoti h
    // thats why we will loop from 8 to 1

    cout<<endl;

    for(rank=r8;rank>=r1;rank--){
        for(file=fa;file<=fh;file++){
            sq=fr_to_sq(file,rank);// 120 based index

            // we need 64 based index so use macro 
            sq64=SQ64(sq);// using macro 

            // if a bit is set at our currr square
            // then print X otherwise print -dash


            // let e4 h,,55 index dega (120 based)  isse 64 base indx me 24 value
            //use set kr denge   

            // then we know if we shift iULL by 28 and then it return a non zero then we 
            // can print an X otherwise dash(-)

            if((shiftme<<sq64)& bb)// bb bitboard
                printf("X");
            else    
                printf("-");
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<endl;

}



