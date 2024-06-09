#include"def.hpp"
#include<iostream>
using namespace std;

char * print_sq(const int sq){
    static char sqstr[3];
    int file=file_board[sq];// 0 to 7 ke file rank dega
    int rank=rank_board[sq];

    // sprintf to char * chiye to char array hi use
    snprintf(sqstr,sizeof(sqstr), "%c%c" , ('a'+file),('1'+rank));
    return sqstr;
}

char * print_move(const int move){
    static char mvstr[6];

    // file from,rank from, file to,rank to
    int ff=file_board[from_sq(move)];
    int rf=rank_board[from_sq(move)];
    int ft=file_board[to_sq(move)];
    int rt=rank_board[to_sq(move)];

    //cout<<endl<<" ff "<<ff<<" rf "<<rf<<" ft "<<ft<<" rt "<<rt<<endl;

    int promoted=promoted(move);

    if(promoted){
        char pchar='q';// let assume it queen

        if(is_kn(promoted))
            pchar='n';
        else if(is_rq(promoted) && !is_bq(promoted))// if it is rook or queen  and not bishop or queen                                       
            pchar='r';// then obviously it is a rook
        else if(!is_rq(promoted) &&  is_bq(promoted))
            pchar='b';

        snprintf(mvstr,sizeof(mvstr), "%c%c%c%c%c",('a'+ff),('1'+rf),('a'+ft),('1'+rt),pchar);
    }
    else{
        snprintf(mvstr, sizeof(mvstr),"%c%c%c%c",('a'+ff),('1'+rf),('a'+ft),('1'+rt));
    }

    return mvstr;
}

void print_move_list(const s_movelist* list){
    int index=0,score=0,move=0;

    cout<<"movelist:"<<endl;

    for(index=0;index<list->count;index++){// for each move in a move list
        move=list->moves[index].move;
        score=list->moves[index].score;

        cout<<"move "<<index+1<<"  "<< print_move(move)<<"  score " <<score<<endl;
    }

    cout<<"movelist total " <<list->count<<" moves"<<endl;
}