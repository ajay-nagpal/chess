#include"def.hpp"
#include<iostream>
#include<vector>

using namespace std;

#define hash_piece(piece,sq) (pos->poskey ^= (piece_keys[(piece)][(sq)]))
#define hash_castle          (pos->poskey ^= (castle_keys[pos->castle_perm]))
#define hash_side            (pos->poskey ^= (side_key))
#define hash_enpass          (pos->poskey ^= (piece_keys[emptyy][pos->enpass]))

//every time we make a move we basically do
//ca_perm &= cas_perm[from]
//and
//ca_perm &= cas_perm[to]
//if all castle perm are there then it will be 1111  ==15 

// so let 3 vla move hua black king , us time cas perm 1111 thi to
// 1111 & 3  now cas perm become 0011  means black ki both castle perm lost
const vector<int> cas_perm{
   
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void clear_piece(const int sq, s_board * pos){

    ASSERT(sq_on_board(sq));
    int piece=pos->pieces[sq];
    ASSERT(piece_valid(piece));

    int color=piece_color[piece];
    int index=0,t_piece_num=-1;

    hash_piece(piece,sq);

    pos->pieces[sq]=emptyy;
    pos->material[color]-=piece_value[piece];
   
    if(piece_big[piece]){
        pos->big_pieces[color]--;
        if(piece_major[piece]){
            pos->major_pieces[color]--;
        }
        else{
            pos->minor_pieces[color]--;
        }
    }
    else{// if its a pawn then clear  its bit
        // the pawns of the color of this piece
        // and  for both colors
        clearbit(pos->pawns[color],SQ64(sq));
        clearbit(pos->pawns[both],SQ64(sq));
    }

    // now how to remove piece from piece list
    /*
    let 5 paawn h to piece_num[piece]==5 dega yha piece wp h  loop fro 0 to <5
    
    pos->piece_list[piece][0]=sq0// each of thse will give a value of a square
    pos->piece_list[piece][1]=sq1
    pos->piece_list[piece][2]=sq2
    pos->piece_list[piece][3]=sq3
    pos->piece_list[piece][4]sq4

    and one of these square should equal to our sq 
    let mana sq 3 h

    sq3==sq
    so  t_piece_num=index of sq3=3
    */

    for(index=0;index<pos->piece_num[piece];index++){
        if(pos->piece_list[piece][index]==sq){
            t_piece_num=index;
            break;
        }
    }
    ASSERT(t_piece_num!=-1);

    pos->piece_num[piece]--;
    //pos->piece_num[piece]==4 ho gya ab 
    pos->piece_list[piece][t_piece_num]=pos->piece_list[piece][(pos->piece_num[piece])];
    //pos->piece_list[piece][3]=pos->piece_list[piece][(4)]=sq4    
}
// add piece clear piece ka opp
static void add_piece(const int sq,s_board * pos, const int piece){
    ASSERT(piece_valid(piece));
    ASSERT(sq_on_board(sq));

    int color=piece_color[piece];//color of the piece that we are adding 
    hash_piece(piece,sq);//hash this piece into hashket on that square

    pos->pieces[sq]=piece;//add that piece in piece array on that square

    if(piece_big[piece]){
        pos->big_pieces[color]++;
        if(piece_major[piece]){
            pos->major_pieces[color]++;
        }
        else{
            pos->minor_pieces[color]++;
        }
    }
    else{
        // if its a pawn then set the relevenet bit
        setbit(pos->pawns[color],SQ64(sq));
        setbit(pos->pawns[both],SQ64(sq));
    }

    pos->material[color]+=piece_value[piece];//add material value of that piece us material m
    pos->piece_list[piece][pos->piece_num[piece]++]=sq;// tehn add this piece to a piece list and increment the number of pieces
}

// move piece function
static void move_piece(const int from, const int to, s_board * pos){
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    int index=0;
    int piece=pos->pieces[from];// from ke piece konsa h and color konsa h
    int color=piece_color[piece];

    #ifdef DEBUG
        int t_piece_num=false;// we must find a piece from out piece list with same value as from
        //if we dont then this will stay false
    #endif

    hash_piece(piece,from);
    pos->pieces[from]=emptyy;// from vale ko empty set kra

    hash_piece(piece,to);
    pos->pieces[to]=piece;

    if(!piece_big[piece]){// if its a pawn then 
        // we need to clear a bit at from square
        clearbit(pos->pawns[color],SQ64(from));
        clearbit(pos->pawns[both],SQ64(from));
        // set a pawn atsquare 64
        setbit(pos->pawns[color],SQ64(to));
        setbit(pos->pawns[both],SQ64(to));
    }

    // now  we loop through all the pieces of this piece type
    for(index=0;index<pos->piece_num[piece];index++){
        if(pos->piece_list[piece][index]==from){
            pos->piece_list[piece][index]=to;
    #ifdef DEBUG
                t_piece_num=true;// set it true
    #endif
                break;
        }
    }
    ASSERT(t_piece_num);// we must do assert cz we must found the from piece
    // if we dont then out piece list dont matches with out pieces array
}

// make move function
int make_move(s_board * pos,int move){
   
    ASSERT(check_board(pos));

    int from=from_sq(move);
    int to=to_sq(move);
    int side=pos->side;

    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    ASSERT(side_valid(side));
    ASSERT(piece_valid(pos->pieces[from]));
    
    pos->history[pos->hisply].poskey=pos->poskey;// store the hash key in history array

    if(move & move_flag_ep){// means if enpass move
    
        if(side==white){// let white side making enpasscapture of black pawn
         
            // let wjhite pawn d5  64 pr h
            // let black vala c7 se c5  63 pr gya
            // white vala enpass capture krega c6  73 pr jake
            clear_piece(to-10,pos);// then remove the black pawn
            // which is  at -10 pos from white pawn 
        }
        else{// means black side making enpass capture means white vala capture hoga
            // let white side vala pawn enpass se capture hua black pawn se
            // let a2 se a4 pr gya white   aur black ne b4 se a3 pr jajek enpass kiya
            clear_piece(to+10,pos);// then remove the white pawn
            
            // which is  at +10 pos from black 

            // hmne yha clear kra h  means hashed it out isliye upr phle  his array me save kr liya 
        }
    }
    else if(move & move_flag_cs){
     
        switch(to){// to sq must be c1 c8 g1 g8 as we saw earlier jispr king jata h
            case c1:
                move_piece(a1,d1,pos);// then we move rook from a1 to d1 like this it works
                break;
            case c8:
                move_piece(a8,d8,pos);
                break;
            case g1:
                move_piece(h1,f1,pos);
                break;
            case g8:
                move_piece(h8,f8,pos);
                break;
            default: 
                ASSERT(false);// if not then castling move has illegle to square so we senfd assert false
                break;
        }
    }

    if(pos->enpass!=no_sq){// if current enpass is set then we  hash out this enpasss square 
        // because depending on what move is about to be made
        // we need to rehash the new enpass sq back in or we dont do anything
        hash_enpass;
    }
    hash_castle;// we hash out he current state of castling
    
    // then we store information history array
    pos->history[pos->hisply].moves=move;// store the move that was made
    pos->history[pos->hisply].fiftymove=pos->fiftymove;//current state of the fifty move rule
    pos->history[pos->hisply].enpass=pos->enpass;//current enpass square and the castle permission
    pos->history[pos->hisply].castle_perm=pos->castle_perm;

    // tehn we use the castle permission array to update our castle permission
    // so that if rook or king is moved then we  will remove the respective castle permission by using & operator
    pos->castle_perm &= cas_perm[from];
    pos->castle_perm &= cas_perm[to];
    //state of castling permission   might adjusted  or not here

    // and we set enpass to no sq
    pos->enpass=no_sq;
    // so hash back in the new state of castling permission 
    hash_castle;

    int capture=captured(move);
    pos->fiftymove++;// increment fifty move rule

    if(capture!=emptyy){
        ASSERT(piece_valid(capture));
        clear_piece(to,pos);// clear this piece from to square
        pos->fiftymove=0;// because when the captures made it get reset to zero
    }
    pos->hisply++;// used for indexing our history array
    pos->ply++;//used for search

    //ASSERT(pos->hisply >= 0 && pos->hisply < maxmoves);
    //ASSERT(pos->ply >= 0 && pos->ply < maxdepth);

    // look wheather we need to set a new enpass square

    // if piece was a pawn  moving from  a from square
    if(piece_pawn[pos->pieces[from]]){
        pos->fiftymove=0;// then reset the 50 moves 
  
        if(move & move_flag_ps){// if the move was a pawn start
            if(side==white ){// aagr white vale ko aaage move kiya let a2 to a4  to 
                pos->enpass=from+10;// a3 vale ko enpass sq set kro  from +10 h
                ASSERT(rank_board[pos->enpass]==r3);
            }
            else{
                pos->enpass=from-10;
                ASSERT(rank_board[pos->enpass]==r6);
            }
            hash_enpass;// cz we set a enpass sq so now hash in this enpass sq
        }
    }
    // now we have cleared all our pieces that haas been captured off.
    // clear piece aur add piece function bhi shi h 
    // so now we can move our piece on the board

    move_piece(from,to,pos);
    // now check weather we promoted something
    int prom_piece=promoted(move);
    if(prom_piece!=emptyy){
       
        // assert that it is a piece and  also not a pawn cz we cant be promoting to a pawn
        ASSERT(piece_valid(prom_piece)  &&  !piece_pawn[prom_piece]);
        
        clear_piece(to,pos);// clear the piece first and on the to square
        add_piece(to,pos,prom_piece);// add promoted piece
    }

    // set he king square, so if it was a king that moved we update the king sq in the position 
    if(piece_king[pos->pieces[to]]){
        pos->king_sq[pos->side]=to;
    }

    pos->side^=1;// now change the side
    hash_side;// hash in the side

    // we are done making all our move and changes on baord so assert checkboard again
    ASSERT(check_board(pos));
   
    if(square_attacked(pos->king_sq[side],pos->side,pos)){
        // now if king is attacked by new side kuki ^1 kr diya  means side change
        //[] vali side is not the currrent side to move  vo to pos->side h
        take_move(pos);
        return false;
    }
    return true;
}


// take move fun
// make move ka opp
void take_move(s_board *pos){

    ASSERT(check_board(pos));
    // decrement hisply and ply cz they were incremented during make move fun
    pos->hisply--;
    pos->ply--;

    // ASSERT(pos->hisply >= 0 && pos->hisply < maxmoves);
	// ASSERT(pos->ply >= 0 && pos->ply < max_depth);

    /// from history array we can get the move that was made  cz we stored it during makemove
    int move=pos->history[pos->hisply].moves;
    //from or to bhi reversed ho gye honge
    // we are going from, to to  from cz we are taking the move back
    int from=from_sq(move);
    int to=to_sq(move);

    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(pos->enpass!=no_sq){// if ep set hash it out and also hash out castle 
        hash_enpass;
    }
    hash_castle;
  
    // set new castle enpass permisssion from history array
    // it will become what it was before the move was made
    pos->castle_perm=pos->history[pos->hisply].castle_perm;
    pos->fiftymove=pos->history[pos->hisply].fiftymove;
    pos->enpass=pos->history[pos->hisply].enpass;

    if(pos->enpass!=no_sq){
    // hash back in enpass and castle
        hash_enpass;
    }
    hash_castle;

    // flip the side and hash the side
    (pos->side)^=1;
    hash_side;

    if(move_flag_ep & move){// if it is an enpass capture
        if(pos->side==white){ // if white made enpass capture 
            add_piece(to-10,pos,bp);// we add back in black pawn
        }
        else{
            add_piece(to+10,pos,wp);// else add white pawn
        }
    }
    else if(move_flag_cs & move){// if it is a castling move
        switch(to){// then we do switch on to square
            // ulta ho jaega jese make move me a1 d1 tha to yha d1 a1
            // depending on the to square
            // send the rook to its original starting square
            case c1:
                move_piece(d1,a1,pos);// then we move rook from a1 to d1 like this it works
                break;
            case c8:
                move_piece(d8,a8,pos);
                break;
            case g1:
                move_piece(f1,h1,pos);
                break;
            case g8:
                move_piece(f8,h8,pos);
                break;
            default: 
                ASSERT(false);// to check that if two square came up with an illeg;e square
                break;
        }
    }

    move_piece(to,from,pos);// now move the piece back means from to sq to from sq pr
    // obviously capture se phle hi likha jaega
    // because we want to move our piece away before we add the captured piece on the to square

    if(piece_king[pos->pieces[from]]){// update the king array
        pos->king_sq[pos->side]=from;
    }

    int capture=captured(move);
    if(capture!=emptyy){
        ASSERT(piece_valid(capture));
        add_piece(to,pos,capture);//add the captued piece on the to square
    }

    if(promoted(move)!=emptyy){//if promoted
        ASSERT(piece_valid(promoted(move)) && !piece_pawn[promoted(move)]);

        clear_piece(from,pos);// clear piece from  from positon
        add_piece(from,pos,((piece_color[promoted(move)]==white) ?wp:bp));// add a pawn 
        // kuki make mpove me promoted piece add kiya tha to yha use clkear krek hm pawn add krnge
    }

    ASSERT(check_board(pos));
}
