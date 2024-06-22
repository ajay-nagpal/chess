#include<iostream>
#include"def.hpp"
#include<iomanip>

using namespace std;

#define infinite 30000
#define mate 29000

static void check_up(s_search_info * info){
    // tocheck if time up or inturrupt from gui

    // if we have set time limit and we ran out of time
    if(info->time_set==true && get_time_ms()>info->stop_time){
        info->stopped=true;
    }
    //read_input(info);
}

static void pick_next_move(int move_num,s_movelist * list){
    s_move temp;
    int index=0,best_score=0;
    int best_num=move_num;

    for(index=move_num;index<list->count;index++){
        if(list->moves[index].score>best_score){
            best_score=list->moves[index].score;
            best_num=index;
        }

        temp=list->moves[move_num];
        list->moves[move_num]=list->moves[best_num];
        list->moves[best_num]=temp;
    }
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

    //info->start_time=get_time_ms();//  uci.cpp parse go me set krenfge baad me
    info->stopped=0;// if interrupt of stoop search then 1
    info->nodes=0;

    info->fail_high=0;
    info->fail_high_first=0;
}

//now quesense search fun
// in this we carry on with alpha beta fun  but we dont have depth and we dont generate captures move
//basically in this we search all captures until they have been resolved
// then we return evaluation of the pos  to eliminate the horizon effect
// white ne black knight capture kr liya +300 point 
// black ne queen caopture kr li 3 knight k brabr   to horizon  effect white loss me rha

static int quiscence(int alpha,int beta,s_board * pos,s_search_info * info){
    ASSERT(check_board(pos));

    // for every 2048 nodes chck if we ran out of time and if then stopped
    if((info->nodes & 2047)==0){
        check_up(info);
    }

    info->nodes++;

    if(is_repetition(pos) || pos->fiftymove>=100){
        return 0;
    }
    if(pos->ply>maxdepth-1){// if going too deep return the eval
        return eval_pos(pos);
    }
    int score=eval_pos(pos);// we didnt even make a move yet

    if(score>=beta){// agar score jyada h to return beta cz we know our pos going to be better than beta
        return beta;
    }
    if(score>alpha){
        alpha=score;
    }
    s_movelist list[1];
    generate_all_caps(pos,list);

    int move_num=0,legal=0;// if legal move we will increment this
    int old_alpha=alpha;// if we will found new alpha witch beat new alpha then we will sotre  best move found in pv cz we beat the old alpha we found new best move
    int best_move=nomove;
    score=-infinite;
    int pvmove=probe_pvtable(pos);

    for(move_num=0;move_num<list->count;move_num++){
        pick_next_move(move_num,list);
        
        if(!make_move(pos,list->moves[move_num].move)){
            continue;
        }
        legal++;
        score=-quiscence(-beta,-alpha,pos,info);//negamax
        take_move(pos);

        if(info->stopped==true){
            return 0;// and tree will simply back ot the root
        }

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

    if(alpha!=old_alpha){
        store_pvmove(pos,best_move);
    }
    return alpha;
}

static int alpha_beta(int alpha, int beta, int depth,s_board * pos,s_search_info * info, int do_null){
    // do_numm is a permission wheather we can make null move or not
    
    ASSERT(check_board(pos));
    if(depth==0){
        return quiscence(alpha,beta,pos,info);
        // info->nodes++;
        // return eval_pos(pos);
    }
    // for every 2048 nodes chck if we ran out of time and if then stopped
    if((info->nodes & 2047)==0){
        check_up(info);
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
    int pvmove=probe_pvtable(pos);

    if(pvmove!=nomove){
        for(move_num=0;move_num<list->count;move_num++){
            if(list->moves[move_num].move==pvmove){
                list->moves[move_num].score=2000000;// jyada score means we will search it above alll move
                break;
            }
        }
    }

    for(move_num=0;move_num<list->count;move_num++){
        pick_next_move(move_num,list);
        
        if(!make_move(pos,list->moves[move_num].move)){
            continue;
        }
        legal++;
        score=-alpha_beta(-beta,-alpha,depth-1,pos,info,true);//negamax
        take_move(pos);

        if(info->stopped==true){
            return 0;
        }

        if(score>alpha){//update alpha
            // but if
            if(score>=beta){// beta cut off
                if(legal==1){
                    info->fail_high_first++;
                }
                info->fail_high++;

                if(!(list->moves[move_num].move & move_flag_cap)){// if not a cap
                    // 0 me 1 vala 1 me 0 valaa ese krt eh ise set
                    pos->search_killer[1][pos->ply]=pos->search_killer[0][pos->ply];
                    pos->search_killer[0][pos->ply]=list->moves[move_num].move;
                }

                return beta;
            }
            alpha =score;
            best_move=list->moves[move_num].move;
            //alpha cutoff area me h to improve search history
            if(!(list->moves[move_num].move & move_flag_cap)){// if not a cap
                // 0 me 1 vala 1 me 0 valaa ese krt eh ise set
                pos->search_history[pos->pieces[from_sq(best_move)]][to_sq(best_move)]+=depth;
            }
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
        
        // quiscensce me jb tree back to root (stopped ke baaad) to yha aate h
        // so here  before getting best move or pv move
        if(info->stopped==true){
            break;
        }
        
        pv_moves=get_pvline(curr_depth,pos);
        best_move=pos->pvarray[0];// set ist move the best move
        // gui ko send krne ke hisab se print
        cout<<endl<<"info score cp  "<<best_score<<" depth: "<<curr_depth<<" nodes: "<<info->nodes<<" time: "<<get_time_ms()-info->start_time<<endl;

        pv_moves=get_pvline(curr_depth,pos);// vice me max jesa
        cout<<"pv: ";
        for(pv_num=0;pv_num<pv_moves;pv_num++){
            cout<<print_move(pos->pvarray[pv_num])<<" ";
        }
        cout<<endl;

        //cout<<"ordering: "<<fixed<<setprecision(2)<<(info->fail_high_first/info->fail_high)<<endl;
    }
    // prev moev ka best move etc send kro gui ko agar break krkre loop se bahr a ajaye kuki stopped aa gya
    // at the en d of search we will send best move
    // info score cp 13 depth 1 nodes 13 time 15 pv f1b5
    cout<<endl<<"best move "<<print_move(best_move)<<endl;

}