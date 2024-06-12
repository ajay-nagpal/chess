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

//it will return a move as san integer a1a4(r/n/b/q  if any promoted)  like this
int parse_move(string ptr_char,s_board* pos){//const nhi kuki move generaete krenge 
    ASSERT(check_board(pos));
    
    if(ptr_char[1]>'8' || ptr_char[1]<'1')
        return nomove;
    if(ptr_char[3]>'8' || ptr_char[3]<'1')
        return nomove;
    if(ptr_char[0]>'h' || ptr_char[0]<'a')
        return nomove;
    if(ptr_char[2]>'h' || ptr_char[2]<'a')
        return nomove;

    int from=fr_to_sq(ptr_char[0]-'a',ptr_char[1]-'1');
    int to=  fr_to_sq(ptr_char[2]-'a',ptr_char[3]-'1');

    ASSERT(sq_on_board(from) && sq_on_board(to));

    cout<<endl<<"ptr char: "<<ptr_char<<" from: "<<from<<" to: "<<to<<endl;
    
    s_movelist list[1];
    generate_all_moves(pos,list);

    int move=0,move_num=0,prom_pce=emptyy;

    for(move_num=0;move_num<list->count;move_num++){
        move=list->moves[move_num].move;// set move integer to the move in the  move list

        if(from_sq(move)==from && to_sq(move)==to){
            // from to samae hone pr vhi move h 
            // promoted ke case me nhi hota to use if else se handle
            prom_pce=promoted(move);
            if(prom_pce!=emptyy){
                if(is_rq(prom_pce) && !is_bq(prom_pce) && ptr_char[4]=='r')
                    return move;
                else if(!is_rq(prom_pce) && is_bq(prom_pce) && ptr_char[4]=='b')
                    return move;
                else if(is_rq(prom_pce) && is_bq(prom_pce) && ptr_char[4]=='q')
                    return move;
                else if(is_kn(prom_pce) && ptr_char[4]=='n')
                    return move;
                continue;
            }
            return move;
        }
    }
    return nomove;
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