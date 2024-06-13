#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

#define move(f,t,ca,pro,flag) ((f)|((t)<<7)|(ca<<14)|(pro<<20)|(flag))
#define square_offboard(sq) (file_board[(sq)]==OFFBOARD)

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

static void add_quite_move(const s_board * pos,int move,s_movelist * list){
    list->moves[list->count].move=move;
    list->moves[list->count].score=0;
    list->count++;
}

static void add_capture_move(const s_board * pos,int move,s_movelist * list){
    list->moves[list->count].move=move;
    list->moves[list->count].score=0;
    list->count++;
}

static void add_enpass_move(const s_board * pos,int move,s_movelist * list){
    list->moves[list->count].move=move;
    list->moves[list->count].score=0;
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
    else   
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);          
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
                    add_capture_move(pos,move(sq,sq+9,emptyy,emptyy,move_flag_ep),list);
                }   

                if(sq+11==pos->enpass){
                    add_capture_move(pos,move(sq,sq+11,emptyy,emptyy,move_flag_ep),list);
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
                    add_capture_move(pos,move(sq,sq-9,emptyy,emptyy,move_flag_ep),list);
                }   

                if(sq-11==pos->enpass){
                    add_capture_move(pos,move(sq,sq-11,emptyy,emptyy,move_flag_ep),list);
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