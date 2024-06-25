#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

#define move(f,t,ca,pro,flag) ((f)|((t)<<7)|(ca<<14)|(pro<<20)|(flag))
#define square_offboard(sq) (file_board[(sq)]==off_board)

const vector<int> loop_slide_piece{wb,wr,wq,0,bb,br,bq,0};
const vector<int> loop_slide_index{0,4};

const vector<int> loop_non_slide_piece{wn,wk,0,bn,bk,0};
const vector<int> loop_non_slide_index{0,3};

const vector<vector<int> > piece_dir{
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },// knight
	{ -9, -11, 11, 9, 0, 0, 0, 0 },// bishop
	{ -1, -10,	1, 10, 0, 0, 0, 0 },// rook
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },// black knight
	{ -9, -11, 11, 9, 0, 0, 0, 0 },// bishop
	{ -1, -10,	1, 10, 0, 0, 0, 0 },//rook
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
};

const vector<int>num_dir{0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};


// move ordering order for our code
// pv move
// cap move (2 type) mvv(most valuable vicitim)lva(least valuaable attacker)  
    //we will use SEE(static exchange evaluater) for this  which valuate if capture are losing on a square
    // evaluating recapture on the square
//killer moves// moves which have made beta cutoff
// historyscore

/*
most valuable victims order (q,r,b,n,p) then order of capture of these in least valuable attacker

p * q(pawn takes queen)
n * q
b * q
r * q
..

p * r(pawn takes rook )  and so on 
..
  so build an array index by[victim][attacker]  and score do inhe 
   ki highest score p * q ka ho  and so on  like lowest score q * p (queen takes pawn)

if victim queen to score 500   if (pawn capture then 505, knight capture queen 504  so on )
        rook 400  

*/
const vector<int> victim_score={ 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };
static vector<vector<int>> mvvlva_score(13,vector<int>(13));

void init_mvvlva(){
    int attacker, victim;

    // dono loop white ke h aur king bhi cover ho gya but kuki legal move apne code me esa nhi hota to 
    // to no need of correcting
    // legal move vale hi calc krke print krega
    for(attacker=wp; attacker<=bk;attacker++){
        for(victim=wp;victim<=bk;victim++){
            mvvlva_score[victim][attacker]=victim_score[victim]+6-(victim_score[attacker]/100);
            // let queen victim to 500 +6(for general)
            // attacker pawn to 100  means 100/100==1
            // overall 505 what we wanted
        }
    }
}
// ab score add kr paenge balki ke jo fun bnae unka 
// jese add enpass smove me score 105 cz pawn takes pawn 
// 100 +6-100/100  105
// capture move me victim attacker se score set

static void add_quite_move(const s_board * pos,int move,s_movelist * list){
    ASSERT(sq_on_board(from_sq(move)));
    ASSERT(sq_on_board(to_sq(move)));
    
    list->moves[list->count].move=move;

    // if killer at best slot ie 0 slot ie first slot==  move we are adding
    if(pos->search_killer[0][pos->ply]==move){
        list->moves[list->count].score=900000;
    }
    else if(pos->search_killer[1][pos->ply]==move){
        list->moves[list->count].score=800000;
    }
    else{
        list->moves[list->count].score=pos->search_history[pos->pieces[from_sq(move)]][to_sq(move)];
    }

    list->count++;
}

static void add_capture_move(const s_board * pos,int move,s_movelist * list){
    ASSERT(sq_on_board(from_sq(move)));
    ASSERT(sq_on_board(to_sq(move)));
    //ASSERT(piece_valid(captured(move)));
    ASSERT(piece_valid_empty(captured(move)));

    list->moves[list->count].move=move;
    // score mvvlva se set
    list->moves[list->count].score=mvvlva_score[captured(move)][pos->pieces[from_sq(move)]]+1000000;
    list->count++;
}

static void add_enpass_move(const s_board * pos,int move,s_movelist * list){
    ASSERT(sq_on_board(from_sq(move)));
    ASSERT(sq_on_board(to_sq(move)));

    list->moves[list->count].move=move;
    // jese add enpass smove me score 105 cz pawn takes pawn 
    // 100 +6-100/100  105
    list->moves[list->count].score=105+1000000;
    list->count++;
}

static void add_white_pawn_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r7){
        add_capture_move(pos,move(from,to,cap,wq,0),list);
        add_capture_move(pos,move(from,to,cap,wr,0),list);
        add_capture_move(pos,move(from,to,cap,wb,0),list);
        add_capture_move(pos,move(from,to,cap,wn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,cap,emptyy,0),list);          
}

static void add_white_pawn_move(const s_board * pos , const int from, const int to,s_movelist * list){
    
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r7){
        add_capture_move(pos,move(from,to,emptyy,wq,0),list);
        add_capture_move(pos,move(from,to,emptyy,wr,0),list);
        add_capture_move(pos,move(from,to,emptyy,wb,0),list);
        add_capture_move(pos,move(from,to,emptyy,wn,0),list);
    }    
    else{
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);
    }         
}

static void add_black_pawn_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r2){
        add_capture_move(pos,move(from,to,cap,bq,0),list);
        add_capture_move(pos,move(from,to,cap,br,0),list);
        add_capture_move(pos,move(from,to,cap,bb,0),list);
        add_capture_move(pos,move(from,to,cap,bn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,cap,emptyy,0),list);          
}

static void add_black_pawn_move(const s_board * pos , const int from, const int to,s_movelist * list){

    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(rank_board[from]==r2){
    
        add_capture_move(pos,move(from,to,emptyy,bq,0),list);
        add_capture_move(pos,move(from,to,emptyy,br,0),list);
        add_capture_move(pos,move(from,to,emptyy,bb,0),list);
        add_capture_move(pos,move(from,to,emptyy,bn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);          

}

int move_exist(s_board * pos,const int move){
    s_movelist list[1];
    generate_all_moves(pos,list);

    for(int move_num=0;move_num<list->count;move_num++){

        if(!make_move(pos,list->moves[move_num].move)){// move not legle continue
            continue;
        }
        take_move(pos);// if legle take it back and see if
        if(list->moves[move_num].move==move){// that move equals to the move that we sent in
            return true;
        }
    }
    return false;
}

void generate_all_moves(const s_board * pos,s_movelist * list){
    ASSERT(check_board(pos));
    list->count=0;

    int piece=emptyy;
    int side=pos->side;
    int sq=0,target_sq=0,pce_num=0;

    int dir=0,index=0,piece_index=0;

    if(side==white){ 
        for(pce_num=0;pce_num<pos->piece_num[wp];pce_num++){// loop through all white pawns on the board
            sq=pos->piece_list[wp][pce_num];
            ASSERT(sq_on_board(sq));
            if(pos->pieces[sq+10]==emptyy){
                add_white_pawn_move(pos,sq,sq+10,list);
                if(rank_board[sq]==r2 && pos->pieces[sq+20]==emptyy){ 
                    add_quite_move(pos,move(sq,sq+20,emptyy,emptyy,move_flag_ps),list);
                }
            }

            if(!square_offboard(sq+9)  && piece_color[pos->pieces[sq+9]]==black){// white bllack ko acptured krega
                add_white_pawn_cap_move(pos,sq,sq+9,pos->pieces[sq+9],list);
            }

            if(!square_offboard(sq+11)  && piece_color[pos->pieces[sq+11]]==black){
                add_white_pawn_cap_move(pos,sq,sq+11,pos->pieces[sq+11],list);
            }

            if(pos->enpass!=no_sq){
                if(sq+9==pos->enpass){
                    add_enpass_move(pos,move(sq,sq+9,emptyy,emptyy,move_flag_ep),list);
                }   
                if(sq+11==pos->enpass){
                    add_enpass_move(pos,move(sq,sq+11,emptyy,emptyy,move_flag_ep),list);
                }
            }
        }
        // for castling king side and queen side
        if(pos->castle_perm & wkca ){
            if(pos->pieces[f1]==emptyy && pos->pieces[g1]==emptyy){
                if(!square_attacked(e1,black,pos) && !square_attacked(f1,black,pos)){
                    add_quite_move(pos,move(e1,g1,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }
        if(pos->castle_perm & wqca ){
            if(pos->pieces[d1]==emptyy && pos->pieces[c1]==emptyy && pos->pieces[b1]==emptyy){
                if(!square_attacked(e1,black,pos) && !square_attacked(d1,black,pos)){
                    add_quite_move(pos,move(e1,c1,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }
    }
    else{
        for(pce_num=0;pce_num<pos->piece_num[bp];pce_num++){

            sq=pos->piece_list[bp][pce_num];
            ASSERT(sq_on_board(sq));
    
            if(pos->pieces[sq-10]==emptyy){
                add_black_pawn_move(pos,sq,(sq-10),list);

                if(rank_board[sq]==r7 && pos->pieces[sq-20]==emptyy){  
                    add_quite_move(pos,move(sq,(sq-20),emptyy,emptyy,move_flag_ps),list);
                }
            }

            if(!square_offboard(sq-9)  && piece_color[pos->pieces[sq-9]]==white){
                add_black_pawn_cap_move(pos,sq,sq-9,pos->pieces[sq-9],list);
            }

            if(!square_offboard(sq-11)  && piece_color[pos->pieces[sq-11]]==white){
                add_black_pawn_cap_move(pos,sq,sq-11,pos->pieces[sq-11],list);
            }

            if(pos->enpass!=no_sq){
                if(sq-9==pos->enpass){
                    add_enpass_move(pos,move(sq,sq-9,emptyy,emptyy,move_flag_ep),list);
                }   

                if(sq-11==pos->enpass){
                    add_enpass_move(pos,move(sq,sq-11,emptyy,emptyy,move_flag_ep),list);
                }
            }
        }

        if(pos->castle_perm & bkca ){
            if(pos->pieces[f8]==emptyy && pos->pieces[g8]==emptyy){
                if(!square_attacked(e8,white,pos) && !square_attacked(f8,white,pos)){
                    add_quite_move(pos,move(e8,g8,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }

        if(pos->castle_perm & bqca ){
            if(pos->pieces[d8]==emptyy && pos->pieces[c8]==emptyy && pos->pieces[b8]==emptyy){
                if(!square_attacked(e8,white,pos) && !square_attacked(d8,white,pos)){
                   add_quite_move(pos,move(e8,c8,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }
    }

    //sliders
    piece_index=loop_slide_index[side];
    piece=loop_slide_piece[piece_index++];

    while(piece!=0){
        ASSERT(piece_valid(piece));

        for(pce_num=0;pce_num<pos->piece_num[piece];pce_num++){
            sq=pos->piece_list[piece][pce_num];
            ASSERT(sq_on_board(sq));
            
            for(index=0;index<num_dir[piece];index++){
                dir=piece_dir[piece][index];
                target_sq=sq+dir;

                while(!square_offboard(target_sq)){

                    if(pos->pieces[target_sq]!=emptyy){
                        if(piece_color[pos->pieces[target_sq]]==(side^1)){
                            add_capture_move(pos,move(sq,target_sq,pos->pieces[target_sq],emptyy,0),list);
                        }
                        break;
                    }
                    add_quite_move(pos,move(sq,target_sq,emptyy,emptyy,0),list);
                    target_sq+=dir;
                }
            }
        }
        piece=loop_slide_piece[piece_index++];
    }

    piece_index=loop_non_slide_index[side];// black to 4 dega nhi to 0
    piece=loop_non_slide_piece[piece_index++];

    while(piece!=0){
        ASSERT(piece_valid(piece));

        for(pce_num=0;pce_num<pos->piece_num[piece];pce_num++){
            
            sq=pos->piece_list[piece][pce_num];
            ASSERT(sq_on_board(sq));
           
            for(index=0;index<num_dir[piece];index++){
                dir=piece_dir[piece][index];
                target_sq=sq+dir;

                if(square_offboard(target_sq))
                    continue;

                if(pos->pieces[target_sq]!=emptyy){
                    if(piece_color[pos->pieces[target_sq]]==(side^1)){
                        add_capture_move(pos,move(sq,target_sq,pos->pieces[target_sq],emptyy,0),list);
                    }
                    continue;
                }
                add_quite_move(pos,move(sq,target_sq,emptyy,emptyy,0),list);
            }
        }
        piece=loop_non_slide_piece[piece_index++];
    }
}

// fun for generate caps  , , generate all move ko hi copy paste rkrkr thpfda change krkre nama bdlna h
// and remove all the code that generate all non capture
// remove empty vala , castling coede  , add quite move line remove

void generate_all_caps(const s_board * pos,s_movelist * list){
    ASSERT(check_board(pos));
    list->count=0;

    int piece=emptyy;
    int side=pos->side;
    int sq=0,target_sq=0,pce_num=0;

    int dir=0,index=0,piece_index=0;

    if(side==white){ 
        for(pce_num=0;pce_num<pos->piece_num[wp];pce_num++){// loop through all white pawns on the board

            sq=pos->piece_list[wp][pce_num];
            ASSERT(sq_on_board(sq));
        
            if(!square_offboard(sq+9)  && piece_color[pos->pieces[sq+9]]==black){// white bllack ko acptured krega
                add_white_pawn_cap_move(pos,sq,sq+9,pos->pieces[sq+9],list);
            }

            if(!square_offboard(sq+11)  && piece_color[pos->pieces[sq+11]]==black){
                add_white_pawn_cap_move(pos,sq,sq+11,pos->pieces[sq+11],list);
            }

            if(pos->enpass!=no_sq){
                if(sq+9==pos->enpass){
                    add_enpass_move(pos,move(sq,sq+9,emptyy,emptyy,move_flag_ep),list);
                }   

                if(sq+11==pos->enpass){
                    add_enpass_move(pos,move(sq,sq+11,emptyy,emptyy,move_flag_ep),list);
                }
            }
        }
    }
    else{
        for(pce_num=0;pce_num<pos->piece_num[bp];pce_num++){

            sq=pos->piece_list[bp][pce_num];
            ASSERT(sq_on_board(sq));
    
            if(!square_offboard(sq-9)  && piece_color[pos->pieces[sq-9]]==white){
                add_black_pawn_cap_move(pos,sq,sq-9,pos->pieces[sq-9],list);
            }

            if(!square_offboard(sq-11)  && piece_color[pos->pieces[sq-11]]==white){
                add_black_pawn_cap_move(pos,sq,sq-11,pos->pieces[sq-11],list);
            }

            if(pos->enpass!=no_sq){
                if(sq-9==pos->enpass){
                    add_enpass_move(pos,move(sq,sq-9,emptyy,emptyy,move_flag_ep),list);
                }   

                if(sq-11==pos->enpass){
                    add_enpass_move(pos,move(sq,sq-11,emptyy,emptyy,move_flag_ep),list);
                }
            }
        }
    }

    //sliders
    piece_index=loop_slide_index[side];
    piece=loop_slide_piece[piece_index++];

    while(piece!=0){
        ASSERT(piece_valid(piece));

        for(pce_num=0;pce_num<pos->piece_num[piece];pce_num++){
            sq=pos->piece_list[piece][pce_num];
            ASSERT(sq_on_board(sq));
            
            for(index=0;index<num_dir[piece];index++){
                dir=piece_dir[piece][index];
                target_sq=sq+dir;

                while(!square_offboard(target_sq)){

                    if(pos->pieces[target_sq]!=emptyy){
                        if(piece_color[pos->pieces[target_sq]]==(side^1)){
                            add_capture_move(pos,move(sq,target_sq,pos->pieces[target_sq],emptyy,0),list);
                        }
                        break;
                    }
                    target_sq+=dir;
                }
            }
        }
        piece=loop_slide_piece[piece_index++];
    }

    piece_index=loop_non_slide_index[side];// black to 4 dega nhi to 0
    piece=loop_non_slide_piece[piece_index++];

    while(piece!=0){
        ASSERT(piece_valid(piece));

        for(pce_num=0;pce_num<pos->piece_num[piece];pce_num++){
            
            sq=pos->piece_list[piece][pce_num];
            ASSERT(sq_on_board(sq));
           
            for(index=0;index<num_dir[piece];index++){
                dir=piece_dir[piece][index];
                target_sq=sq+dir;

                if(square_offboard(target_sq))
                    continue;

                if(pos->pieces[target_sq]!=emptyy){
                    if(piece_color[pos->pieces[target_sq]]==(side^1)){
                        add_capture_move(pos,move(sq,target_sq,pos->pieces[target_sq],emptyy,0),list);
                    }
                    continue;
                }
            }
        }
        piece=loop_non_slide_piece[piece_index++];
    }
}