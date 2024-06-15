#include<iostream>
#include"def.hpp"
#include<iomanip>

using namespace std;


static void check_up(){
    // tocheck if time up or inturrupt from gui
}

static int is_repetition(const s_board * pos){
    //loop through all pos key in our history
    // capturesd kra ya pawn move hua to repetition nhi hota cz backward nhi ja skte vaps in 2 m
    // fifty move rule reset ho jata h in 2 condn me
    //so we can also start looking lfrom the lst time fifty move rule was reset for less loop
    int index=0;
    
    //history ki las entry se 1 km
    for(index=pos->hisply-pos->fiftymove;index<pos->hisply-1;index++){
        ASSERT(index>=0 && index<maxmoves);
        if(pos->poskey==pos->history[index].poskey){
            return true;
        }
    }
    return false;
}

static void clear_for_search(s_board * pos,s_search_info * info){
    // here we simply clear stats and histroy heristic array like pv etc  make it 0 
    // to ready for new search

    int index=0,index2=0;

    for(index=0;index<13;index++){
        for(index2=0;index2<board_sq_num;index2++){
            pos->search_history[index][index2]=0;
        }
    }

    for(index=0;index<2;index++){
        for(index2=0;index2<maxdepth;index2++){
            pos->search_killer[index][index2]=0;
        }
    }
    clear_pvtable(pos->pvtable);// for new search clear old pv

    pos->ply=0;// number of half moves played in the current search  so =0 before every search

    info->start_time=get_time_ms();// stop time fun that connect to the gui me set hoga
    info->stopped=0;// if interrupt of stoop search then 1
    info->nodes=0;

    info->fail_high=0,info->fail_high_first=0;

}

static int alpha_beta(int alpha, int beta, int depth,s_board * pos,s_search_info * info, int do_null){
    // do_numm is a permission wheather we can make null move or not
    
    ASSERT(check_board(pos));
    if(depth==0){
        info->nodes++;
        return eval_pos(pos);
    }
    info->nodes++;// depth is not 0 means we visited the node nincrement it

    if(is_repetition(pos) || pos->fiftymove>=100){// repear or pos draw
        return 0;
    }

    if(pos->ply>maxdepth-1){
        return eval_pos(pos);
    }

    //vector<s_movelist> list(1);
    s_movelist list[1];
    generate_all_moves(pos,list);

    int move_num=0,legal=0;// if legal move we will increment this
    int old_alpha=alpha;// if we will found new alpha witch beat new alpha then we will sotre  best move found in pv cz we beat the old alpha we found new best move
    int best_move=nomove;
    int score=-infinite;

    for(move_num=0;move_num<list->count;move_num++){
        if(!make_move(pos,list->moves[move_num].move)){
            continue;
        }
        legal++;
        score=-alpha_beta(-beta,-alpha,depth-1,pos,info,true);//negamax
        take_move(pos);

        if(score>alpha){//update alpha
            // but if
            if(score>=beta){// beta cut off
                if(legal==1){
                    info->fail_high_first++;
                }
                info->fail_high++;

                return beta;
            }
            alpha =score;
            best_move=list->moves[move_num].move;
        }
    }
    // check mate and stalemate
    if(legal==0){
        if(square_attacked(pos->king_sq[pos->side],pos->side^1,pos)){
            return -mate+pos->ply;//pos->ply gives distance to mate from the root
        }
        else{// not a mate then its stalemate and thats draw return 0
            return 0;
        }
    }
    if(alpha!=old_alpha){
        store_pvmove(pos,best_move);
    }
    return alpha;
}

//now quesense search fun
// in this we carry on with alpha beta fun  but we dont have depth and we dont generate captures move
//basically in this we search all captures until they have been resolved
// then we return evaluation of the pos  to eliminate the horizon effect
// white ne black knight capture kr liya +300 point 
// black ne queen caopture kr li 3 knight k brabr   to horizon  effect white loss me rha

static int quiscence(int alpha,int beta,s_board * pos,s_search_info * info){
    return 0;
}

void search_pos(s_board * pos,s_search_info * info){
    // it will handle itertive deepening  cz by tis we ge t best pv for each depth
    // which we wont get if we directly search to a specific depth 
    /// it works like this

    // for dpeth 1 to maxxdepth
        //search alphabeta
        //next depth
    int best_move=nomove;// agar nye loop me time khtm to previous loopka hi return kr dete h best time and score
    int best_score=-infinite;
    int curr_depth=0;
    int pv_num=0,pv_moves=0;// pvmove vhi max jesa kaam krr ha h count of pv line type

    clear_for_search(pos,info);

    for(curr_depth=1; curr_depth<=info->depth;curr_depth++){
                                //alpha //beta
        best_score=alpha_beta(-infinite,infinite,curr_depth,pos,info,true);

        // agar yha out of time aa gya to prev moev vgera return return 
        
        pv_moves=get_pvline(curr_depth,pos);
        best_move=pos->pvarray[0];// set ist move the best move

        cout<<endl<<"depth: "<<curr_depth<<" score: "<<best_score<<" move: "<<print_move(best_move)<<" nodes: "<<info->nodes<<endl;

        pv_moves=get_pvline(curr_depth,pos);// vice me max jesa
        cout<<"pv: ";
        for(pv_num=0;pv_num<pv_moves;pv_num++){
            cout<<print_move(pos->pvarray[pv_num])<<" ";
        }
        cout<<endl;

        //printf("ordering:%.2f\n",info->fail_high_first/info->fail_high);
        cout<<"ordering: "<<fixed<<setprecision(2)<<info->fail_high_first/info->fail_high<<endl;
    }
}