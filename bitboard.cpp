#include<iostream>
#include"def.hpp"
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

int popbit(u64 *bb){
    u64 b= *bb ^(*bb-1);
    unsigned int fold=(unsigned) ((b &  0xffffffff)  ^ (b>>32));
    *bb &=(*bb -1);

    return bit_table[(fold * 0x783a9b23) >>26];
}

int countbit(u64 b){
    int r;
    for(r=0;b; r++ , b&=b-1);
        
    return r;
}

void print_bit_board(u64 bb){
    u64 shiftme=1ULL;

    int rank=0;
    int file=0;
    int sq=0;
    int sq64=0;

    cout<<endl;

    for(rank=r8;rank>=r1;rank--){
        for(file=fa;file<=fh;file++){
            sq=fr_to_sq(file,rank);

            sq64=SQ64(sq);

            if((shiftme<<sq64)& bb)
                cout<<"X";
            else    
                cout<<"-";
        }
        cout<<endl;
    }
    cout<<endl<<endl;
}



