#include"def.h"
#include<iostream>
#include<vector>

using namespace std;
/// ye lines kafi baar likhni padegi isliye macro bna lo iske
#define hash_piece(piece,sq) (pos->poskey ^= (piece_keys[(piece)][(sq)]))
#define hash_castle (pos->poskey ^= (castle_keys[pos->castle_perm]))
#define hash_side (pos->poskey ^= (side_key))
#define hash_enpass (pos->poskey ^= (piece_keys[emptyy][pos->enpass]))


const vector<int> cas_perm{// it is basically mirror of our board with 120 squares
    // all are set to 15 apart from a1,e1,h1,  in white side
    // for black isde a8 ,e8 and h8

    // because everry time we move a piece
    // we are doing castle_perm &= cas_perm[from] and   castle_perm &= cas_perm[to]
    // why we doing this
    // cz 
    // castle permission occupies 4 bits

    // agar all permission h to 1111 hoga jo 15 hota h

    // vo 6 ko chodkr
    // kuki jese e8 pr black queen h
    // let usne move kiya
    // to and krenge  array se
    // castle perm &= 3
    // 1111 & 11  -->   0011  so we will losss the permission to castle on both king and queen side

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


// clear piece function

static void clear_piece(const int sq, s_board * pos){// want to clear the piece on square sq

    ASSERT(sq_on_board(sq));
    int piece=pos->pieces[sq];
    ASSERT(piece_valid(piece));

    int color=piece_color[piece];
    int index=0,t_piece_num=0;

    hash_piece(piece,sq);// it will do xor our pieceon that square hashing it out ofthe position key

    pos->pieces[sq]=emptyy;// set that square to empty on pieces array
    pos->material[color]-=piece_value[piece];// substrat the piece being removed from our mateirakl score
    //for the cut perticular color

   
    // if it is a big piece not a pawn then adjust our  big piece major  or minor counter accordingly

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

    // now how to remove pieces

    /*
    let 5 paawn h to piece_num[piece]==5 dega yha piece wp h  loop fro 0 to <5
    pos->piece_list[piece][0]=sq0// each of thse will give a value of a square
    pos->piece_list[piece][1]=sq1
    pos->piece_list[piece][2]=sq2
    pos->piece_list[piece][3]=sq3
    pos->piece_list[piece][4]sq4

    and one of these square should equal tho our sq 

    let mana sq 3 h

    sq3==sq
    so  t_piece_num=3

    */


    for(index=0;index<pos->piece_num[piece];index++){
        if(pos->piece_list[piece][index]==sq){
            t_piece_num=index;
            break;
        }
    }

    ASSERT(t_piece_num!=-1);

    pos->piece_num[piece]--;// now decrement the piece number
    // to jo vo phle 5 tha piece_num[piece]  ye ab 4 ho jaega
    
    // now position of wp in piece list at t_piece num 
    //pos->piece_list[piece][t_piece_num]=pos->piece_list[piece][pos->piece_num[piece]]=sq4
    //pos->piece_list[wp][3]=pos->piece_list[wp][4]=sq4

    pos->piece_list[piece][t_piece_num]=pos->piece_list[piece][(pos->piece_num[piece])];

    // now our piece list liiks like this
    
     /*
    piece_num[piece]==4  0 to <4
    pos->piece_list[piece][0]=sq0
    pos->piece_list[piece][1]=sq1
    pos->piece_list[piece][2]=sq2
    pos->piece_list[piece][3]=sq4 // imp 
    // yha vo line hat gyi h ab sq3 vali  clear ho gya  now we going straight to sq4 from sq2
    // thats how we remove a piece from a piece list
    */

}


static void add_piece(const int sq,s_board * pos, const int piece){
    ASSERT(piece_valid(piece));
    ASSERT(sq_on_board(sq));

    int color=piece_color[piece];
    hash_piece(piece,sq);

    pos->pieces[sq]=piece;
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

    pos->material[color]+=piece_value[piece];// add a material value for this pieceto that perticulare side material
    pos->piece_list[piece][pos->piece_num[piece]++]=sq;// tehn add this piece to a piece list
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

    hash_piece(piece,from);
    pos->pieces[to]=piece;// put the piece in pieces array on to square index

    if(!piece_big[piece]){// if its a pawn then 
        // we need to clear a bit at from square
        clearbit(pos->pawns[color],SQ64(from));
        clearbit(pos->pawns[both],SQ64(from));
        // set a pawn atsquare 64
        setbit(pos->pawns[color],SQ64(to));
        setbit(pos->pawns[both],SQ64(to));
    }

    // now  we loop through all the pieces of this piece type
    // from vala mila 
    // to use to vala set kr denge


    for(index=0;index<pos->piece_num[piece];index++){
        if(pos->piece_list[piece][index]==from){
            pos->piece_list[piece][index]=to;
            #ifdef DEBUG
                t_piece_num=true;// set it true
            #endif
                break;// from vali mili tobreak out of the loop
        }
    }
    ASSERT(t_piece_num);// we must do assert cz we must found the from piece
    // if we dont then out piece list dont matches with out pieces array
}


// make move function

int make_move(s_board * pos,int move){// return false if after making the move the side that makes the move 
    // left itself in check 
    // cz side two move then be able to capture the king
    /// illegle move
    //else return 1  so check for checkmate 
    cout<<endl<<" in make move 1"<<endl;
    ASSERT(check_board(pos));// to check thjat if this pos is ok
    cout<<endl<<" in make move 2"<<endl;

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
    else if(move & move_flag_cs){// if not enpass then might be castling move
     
        switch(to){// to sq must be c1 c8 g1 g8 as we saw earlier
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
    pos->history[pos->hisply].moves=pos->poskey;// store the move that was made
    pos->history[pos->hisply].fiftymove=pos->poskey;//current state of the fifty move rule
    pos->history[pos->hisply].enpass=pos->poskey;//current enpass square and the castle permission
    pos->history[pos->hisply].castle_perm=pos->poskey;

    // tehn we use the castle permission array to update our castle permission
    // so that if rook or king is moved then we  will remove the respective castle permission by using & operator
    pos->castle_perm &= cas_perm[from];
    pos->castle_perm &= cas_perm[to];
    //state of castling permission   might adjusted  or not here

    // and we set enpass to no sq
    pos->enpass=no_sq;
   
    // so hash back in the new state of castling permission 
    hash_castle;


    int captured=captured(move);// if any piece get captured we get that piece
    pos->fiftymove++;// increment fifty move rule

    if(captured!=emptyy){// if captured 
        ASSERT(piece_valid(captured));// assert to check if is valid piece
        clear_piece(to,pos);// clear this piece from to square
        pos->fiftymove=0;// because when the captures made it get reset to zero
    }

    pos->hisply++;// used for indexing our history array
    pos->ply++;

    ASSERT(pos->hisply >= 0 && pos->hisply < maxmoves);
    //ASSERT(pos->ply >= 0 && pos->ply < maxdepth);

    // look wheather we need to set a new enpass square

    // if piece was a pawn  moving from  a from square
    if(piece_pawn[pos->pieces[from]]){// data.c me h piece pawn array 13 size ka extern kra h def.h me
        pos->fiftymove=0;// then reset the 50 moves  capture vale em bhi zero kiya tha pawn me bhi krrenge
  
        if(move & move_flag_ps){// if the move was a pawn start
            if(side==white ){// aagr white vale ko aaage move kiya let a2 to a4  to 
                pos->enpass=from+10;// a3 vale ko enpass sq set kro  from +10 h
                ASSERT(rank_board[pos->enpass]==r3);// assert to check that the rank of enpass sq should be 3
            }
            else{
                pos->enpass=from-10;// black vala c7 se c5 pr gya to c5 ko enpass set jo from -10 h
                ASSERT(rank_board[pos->enpass]==r6);// check enpass sq ki rank 6 hogi
            }
            hash_enpass;// cz we set a enpass sq so now hash in this enpass sq
        }
    }
    // now we have cleared all our pieces that haas been captured off.
    // clear piece aur add piece function bhi shi h 
    // so now we can move our piece on the board
    // maan lo move phle kr diya hota captured cklear krne se phle 
    // let queen chli hoti fir baad me pta chlta ki queen ne captured kiya h use clear kro to vo 
    // sb clear vgerea sbka order h shi se likha h

    move_piece(from,to,pos);
    // now check weather we promoted something
    int prom_piece=promoted(move);
    if(prom_piece!=emptyy){
       
        // assert that it is a piece and  also not a pawn cz we cant be promoting to a pawn
        ASSERT(piece_valid(prom_piece)  &&  !piece_pawn[prom_piece]);
        
        clear_piece(to,pos);// clear the piece first and on the to square
        add_piece(to,pos,prom_piece);// add promoted piece

        // ulta likha to we will be clearing the piece that we promoted to so order ka dhyan rkhna h
    }

    // set he king square, so if it was a king that moved we update the king sq in the position 
    if(piece_king[pos->pieces[to]]){
        pos->king_sq[pos->side]=to;
    }

    pos->side^=1;// now change the side
    hash_side;// hash in the side

    // we are done making all our move and change son baord so assert checkboard again
    ASSERT(check_board(pos));
   
    if(square_attacked(pos->king_sq[side],pos->side,pos)){
        // adn if the  side that did the move (pos->king_sq[side])  ye side jo h bracked me h
        // ing sq is attacked by the new side to move(pos->side) 
        // then obviously the person that left he king to checked pos
        // take back the move
        //return false
        
        take_move(pos);
        return false;
    }
    

    // nhi to retrurn true
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
    // we are going from to to to the from cz we are taking the move back
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


    if(pos->enpass!=no_sq){// if no sq
    // hash back in enpass and castle

        hash_enpass;

    }

    hash_castle;

    // flip the side and hash the side
    pos->side^=1;
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
    //otherwise we would be moveing the captured piece back to the from square

    if(piece_king[pos->pieces[from]]){// update the king array
        pos->king_sq[pos->side]=from;
    }

    int captured=captured(move);
    if(captured!=emptyy){// if we captured anything
        ASSERT(piece_valid(captured));
        add_piece(to,pos,captured);//add the captued piece on the to square
    }

    if(promoted(move)!=emptyy){//if promoted
        ASSERT(piece_valid(promoted(move)) && !piece_pawn[promoted(move)]);

        clear_piece(from,pos);// clear piece from  from positon
        add_piece(from,pos,(piece_color[promoted(move)]==white ?wp:bp));// add a pawn 
        // kuki make mpove me promoted piece add kiya tha to yha use clkear krek hm pawn add krnge
    }

    ASSERT(check_board(pos));
}
