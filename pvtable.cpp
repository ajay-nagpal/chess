//principle variation table
#include"def.hpp"
#include<iostream>
using namespace std;

const int pvsize=0x100000 *2;//2 mb  hex 1 mb h

int get_pvline(const int depth,s_board* pos){// depth we want ot print pv to
    ASSERT(depth<maxdepth);

    int move=probe_pvtable(pos);
    int count=0;// count of movews we managed to put in array pvarray 

    while(move!=nomove && count<depth){
        ASSERT(count<maxdepth);

        if(move_exist(pos,move)){//if exist
            make_move(pos,move);// make the move
            pos->pvarray[count++]=move;// store in array
        }
        else{
            break;
        }
        move=probe_pvtable(pos);// get the next move
    }

    while(pos->ply>0){// take back all the moves to return to the original pos where we wanted to get the pv
        take_move(pos);// depth 5 tk search kiya ab mana baad me dep 6 tk krna h to start se hi krna hoga
    }// ply isliye kuki everytime we make a amov eply get incremented by 1

    return count;
}

void clear_pvtable(s_pvtable * table){
    s_pventry * pventry;// pointer to entry
    // loop through every entry in the table
    for(pventry=table->ptable; pventry<table->ptable + table->num_entries ; pventry++){
        pventry->poskey=0ULL;
        pventry->move=nomove;
    }
}

void init_pvtable(s_pvtable * table){
    table->num_entries=pvsize/sizeof(s_pventry);// eg 4 divide in 2 , one get 2/4
    table->num_entries -=2;// for indexing purpose
    //free(table->ptable);
    delete(table->ptable);// table pointer jise point kr rha use free
    //table->ptable = (s_pventry *) malloc(table->num_entries * sizeof(s_pventry));
    table->ptable=new s_pventry[table->num_entries];// cgp

    clear_pvtable(table);
    cout<<endl<<"pvtable init complete with "<<table->num_entries<<" entries"<<endl;
}

// fun to store move inside table
void store_pvmove(const s_board* pos, const int move){
    int index=pos->poskey % pos->pvtable->num_entries;// index will be bw 0 to num of enteries-1
    //64 bit num % num enteries
    //Since pos->poskey is a 64-bit unsigned long long, it can have values in the range from 0 to 2^64 - 1.
    //: Assuming num_entries is a positive integer, its range is from 1 to INT_MAX (typically 2^31 - 1 on many systems, depending on the integer size)
    //he value of index will be within the range of valid array indices based on num_entries:
    //Lower Bound: The minimum value of index is 0.
    //Upper Bound: The maximum value of index is pos->pvtable->num_entries - 1.
  
    ASSERT(index>=0 && index<=pos->pvtable->num_entries-1);

    pos->pvtable->ptable[index].move=move;// inside the pv table at the position of index
    pos->pvtable->ptable[index].poskey=pos->poskey;
}

// fun to retreve/prob move from table if there is any
int probe_pvtable(const s_board* pos){
    int index=pos->poskey % pos->pvtable->num_entries;
    ASSERT(index>=0 && index<=pos->pvtable->num_entries-1);

    if(pos->pvtable->ptable[index].poskey==pos->poskey){// if the key same at index
        return pos->pvtable->ptable[index].move;// return movewhich is  at that index
    }
    return nomove;
}
// two diff key modulus enteries can give same index
// retrieve pv me resolve krenge ise
/// move legality se check
// movegen me h moveexist fun


