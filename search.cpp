#include<iostream>
#include"def.hpp"
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
}

static int alpha_beta(int alpha, int beta, int depth,s_board * pos,s_search_info * info, int do_null){
    // do_numm is a permission wheather we can make null move or not
    return 0;
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
    // it will handle itertive deepening
}