//principle variation table
#include"def.hpp"
#include<iostream>

using namespace std;

const int hash_size=0x100000 *16;//16 mb , hex 1 mb h

int get_pvline(const int depth,s_board* pos){// depth we want ot print pv to
    ASSERT(depth<maxdepth);

    int move=probe_pvmove(pos);
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
        move=probe_pvmove(pos);// get the next move
    }

    while(pos->ply>0){// take back all the moves to return to the original pos where we wanted to get the pv
        take_move(pos);// depth 5 tk search kiya ab mana baad me dep 6 tk krna h to start se hi krna hoga
    }// ply isliye kuki everytime we make a amov eply get incremented by 1

    return count;
}

void clear_hash_table(s_hash_table * table){
    s_hash_entry * table_entry;// pointer to entry
    // loop through every entry in the table
    for(table_entry=table->ptable; table_entry<table->ptable + table->num_entries ; table_entry++){
        table_entry->poskey=0ULL;
        table_entry->move=nomove;
        table_entry->depth=0;
        table_entry->score=0;
        table_entry->flag=0;
    }
}

void init_hash_table(s_hash_table * table,const int mb){
    int hash_size=0x100000 * mb;
    table->num_entries=hash_size/sizeof(s_hash_entry);// eg 4 divide in 2 , one get 2/4
    table->num_entries -=2;// for indexing purpose

    if(table->ptable!=NULL) {
		delete(table->ptable);// table pointer jise point kr rha use free
	}
   
    //table->ptable = (s_hash_entry *) malloc(table->num_entries * sizeof(s_hash_entry));
    
    table->ptable=new(nothrow) s_hash_entry[table->num_entries];// cgp
    
    if(table->ptable==NULL) {
		cout<<"hash allocation failed, trying " <<mb/2<<" mb"<<endl;
		init_hash_table(table,mb/2);
	}
    else{
        clear_hash_table(table);
        cout<<endl<<"hash table init complete with "<<table->num_entries<<" entries"<<endl;     
    }
}

// fun to store move inside table
void store_hash_entry( s_board* pos, const int move,int score,const int flag,const int depth){
    int index=pos->poskey % pos->hash_table->num_entries;// index will be bw 0 to num of enteries-1
    //64 bit num % num enteries
    //Since pos->poskey is a 64-bit unsigned long long, it can have values in the range from 0 to 2^64 - 1.
    //: Assuming num_entries is a positive integer, its range is from 1 to INT_MAX (typically 2^31 - 1 on many systems, depending on the integer size)
    //he value of index will be within the range of valid array indices based on num_entries:
    //Lower Bound: The minimum value of index is 0.
    //Upper Bound: The maximum value of index is pos->hash_table->num_entries - 1.
  
    ASSERT(index>=0 && index<=pos->hash_table->num_entries-1);

    ASSERT(depth>=1 && depth<maxdepth);
    ASSERT(flag>=hf_alpha && flag<=hf_exact);
    ASSERT(score>=-infinite&&score<=infinite);
    ASSERT(pos->ply>=0&&pos->ply<maxdepth);

	if( pos->hash_table->ptable[index].poskey == 0){
		pos->hash_table->new_write++;// help to count brand new entries
	} else {
		pos->hash_table->over_write++;// otherwise we are overwriting
	}
	
	if(score > is_mate) 
        score += pos->ply;
    else if(score < -is_mate) 
        score -= pos->ply;
	
	pos->hash_table->ptable[index].move = move;
    pos->hash_table->ptable[index].poskey = pos->poskey;
	pos->hash_table->ptable[index].flag = flag;
	pos->hash_table->ptable[index].score = score;
	pos->hash_table->ptable[index].depth = depth;
}

// fun to retreve/prob move from table if there is any
int probe_pvmove(const s_board* pos){
    int index=pos->poskey % pos->hash_table->num_entries;
    ASSERT(index>=0 && index<=pos->hash_table->num_entries-1);

    if(pos->hash_table->ptable[index].poskey==pos->poskey){// if the key same at index
        return pos->hash_table->ptable[index].move;// return movewhich is  at that index
    }
    return nomove;
}
// two diff key modulus enteries can give same index
// retrieve pv me resolve krenge ise
/// move legality se check
// movegen me h moveexist fun


int probe_hash_entry(s_board *pos, int &move, int &score, int alpha, int beta, int depth) {

	int index = pos->poskey % pos->hash_table->num_entries;
	
	ASSERT(index >= 0 && index <= pos->hash_table->num_entries - 1);
    ASSERT(depth>=1 && depth<maxdepth);
    ASSERT(alpha<beta);
    ASSERT(alpha>=-infinite && alpha<=infinite);
    ASSERT(beta>=-infinite && beta<=infinite);
    ASSERT(pos->ply>=0 && pos->ply<maxdepth);
	
	if( pos->hash_table->ptable[index].poskey == pos->poskey ) {// if key match 
		move = pos->hash_table->ptable[index].move;// then we have our pv move
		if(pos->hash_table->ptable[index].depth >= depth){ // depth >=depth we are searching now
			
            pos->hash_table->hit++;// then we have a hash hit
			
			ASSERT(pos->hash_table->ptable[index].depth>=1&&pos->hash_table->ptable[index].depth<maxdepth);
            ASSERT(pos->hash_table->ptable[index].flag>=hf_alpha&&pos->hash_table->ptable[index].flag<=hf_exact);
			
			score = pos->hash_table->ptable[index].score;
			if(score > is_mate) 
                score -= pos->ply;
            else if(score < -is_mate) 
                score += pos->ply;
            
            // exact score to theoritically equal score
            // score <=alpha means we didnt beat alpha so return alpha theoritically 
            // we are returning true fasle
            // score>=beta then return beta theoritivcally cz no improvement
			
			switch(pos->hash_table->ptable[index].flag) {
				
                ASSERT(score>=-infinite&& score<=infinite);
                case hf_alpha: 
                    if(score<=alpha) {
                        score=alpha;
                        return true;
                    }
                    break;
                case hf_beta: 
                    if(score>=beta) {
                        score=beta;
                        return true;
                    }
                    break;
                case hf_exact:
                    return true;
                    break;
                default: 
                    ASSERT(false);
                     break;
            }
		}
	}
	return false;
}