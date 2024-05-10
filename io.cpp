#include"def.h"
#include<iostream>
#include<vector>

using namespace std;

// fun to print a move in algebrcc notation to the board


// algebric

// let square b2 h 32 pr
// then let i am making move b2 to b4   then in algebric notationn will be b2b4
// let promoting a pawn y with g7 and g8   alge notation will be g7g8
// if promoted to queen the add q , b, r n   for bishop knight rook  


// square strign  to print sq
char * print_sq(const int sq){
    static char sqstr[3];// cz we will be using the infrormation stored in array outisde of this function
    // static isliye taki return ke baaad bhi exist
    int file=file_board[sq];
    int rank=rank_board[sq];

    sprintf(sqstr, "%c%c" , ('a'+file),('1'+rank));// char +int to char int me promote hogaand  uska ascii+rank ans
    // but but %c  h to vo '1'+7=='8' hoga
    
    return sqstr;
}

// now print move fun

char * print_move(const int move){
    static char mvstr[6];

    // file from,rank from, file to,rank to
    int ff=file_board[from_sq(move)];
    int rf=rank_board[from_sq(move)];
    int ft=file_board[to_sq(move)];
    int rt=rank_board[to_sq(move)];

    cout<<endl<<"ff "<<ff<<" rf "<<rf<<" ft "<<ft<<" rt "<<rt<<endl;

    // what piece we promoted to if any
    int promoted=promoted(move);

    if(promoted){
        char pchar='q';// let assume it queen

        if(is_kn(promoted))
            pchar='n';
        else if(is_rq(promoted) && !is_bq(promoted))// if it is rook or queen  and not bishop or queen                                       
            pchar='r';// then obviously it is a rook
        else if(!is_rq(promoted) &&  is_bq(promoted))
            pchar='b';

        sprintf(mvstr, "%c%c%c%c%c",('a'+ff),('1'+rf),('a'+ft),('1'+rt),pchar);
    }
    else{
        sprintf(mvstr, "%c%c%c%c",('a'+ff),('1'+rf),('a'+ft),('1'+rt));
    }

    return mvstr;
}

void print_move_list(const s_movelist* list){
    int index=0,score=0,move=0;

    cout<<"movelist:"<<endl;

    for(index=0;index<list->count;index++){// for each move in a move list
        move=list->moves[index].move;
        score=list->moves[index].score;

        printf("move:%d > %s (score: %d)\n",index+1,print_move(move),score);
    }

    printf("movelist total %d moves:\n",list->count);
}