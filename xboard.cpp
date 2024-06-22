#include<iostream>
#include"def.hpp"
using namespace std;

// new(for new game)
// sd 8 (depth is 8)
// time (after every move how much time left)
// level  (moves to go or total time are set once by level command)
// usermove e2e4 (another use move)
// move e7e5 (my move)  and then check wheather it is a stalemate or a  mate or a draw a threefold

int three_fold_rep(const s_board * pos){
    int i=0,r=0;
    for(i=0;i<pos->hisply;i++){
        if(pos->history[i].poskey==pos->poskey){
            r++;
        }
    }
    return r;
}

int draw_material(const s_board * pos){
    if(pos->piece_num[wp] || pos->piece_num[bp])
        return false;
    if(pos->piece_num[wq] || pos->piece_num[bq] || pos->piece_num[wr] || pos->piece_num[br])
        return false;
    // is we have 2 bishoop on the board so they can deliver mate
    // so no draw return false;
    if(pos->piece_num[wb]>1 || pos->piece_num[bb]>1)
        return false;
    if(pos->piece_num[wn]>1 || pos->piece_num[bn]>1)
        return false;
    if(pos->piece_num[wn] && pos->piece_num[wb])
        return false;
    if(pos->piece_num[bn] && pos->piece_num[bb]>1)
        return false;

    return true;// all othe conditions can return a draw
}

int check_result(s_board * pos){// will call everytime a move is made or we make a mvoe
// to check if game is draw
    if(pos->fiftymove>100){
        cout<<"1/2-1/2 {fifty move rule (claimed by vice)}"<<endl;
        return true;
    }
    if(three_fold_rep(pos)>=2){
        cout<<"1/2-1/2 {3-fold repetition (claimed by vice)}"<<endl;
        return true;
    }
    if(draw_material(pos)==true){
        cout<<"1/2-1/2 {insufficient material (claimed by vice)}"<<endl;
        return true;
    }

    // now chcek for stalemate or checkmate
    s_movelist list[1];
    generate_all_moves(pos,list);
    int move_num=0,found=0;

    for(move_num=0;move_num<list->count;move_num++){
        if(!make_move(pos,list->moves[move_num].move)){
            continue;
        }
        found++;// means we found a legal move we cant be in check or stalemate
        take_move(pos);// take that move back
        break;// break
    }
    if(found!=0){
        return false;// means we still have moves so no draw
    }
    // determine wheather mate or stalemate
    int in_check=square_attacked(pos->king_sq[pos->side],pos->side^1,pos);

    if(in_check==true){
        if(pos->side==white){// white to move
            cout<<"0-1 {black mates (claimed by vice)}"<<endl;// black checkmated us
            return true;
        }
        else{
            cout<<"0-1 {white mates (claimed by vice)}"<<endl;
            return true;
        }
    }
    else{
        cout<<endl<<"1/2-1/2 {stalemate (claimed by vice)}"<<endl;
        return true;
    }
    return false;
}