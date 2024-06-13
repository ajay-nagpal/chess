#include<iostream>
#include"def.hpp"
using namespace std;

int is_repetition(const s_board * pos){
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

void search_pos(s_board * pos){

}