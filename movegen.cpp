// imp steps to remember

/*
// making a  move 


1 make(move)  // function make which take integer move  it will also take a pointer to our position

2 get the from,to,cap from the move
3 store the current position in the pos->istory array  like we store key for a position,
    we store move being made, castele perm , fifty move etc  

4 move the current piece from pieces araray ,from sq (to)->to sq
5 if a capture was made, remove captured from the piece list// piece list array me se jo s_board me h
    and also reduce the piece number formiece num array

6 update fifty move rule, see if pawn was moved
7 promotions// if promoted thn remove the pawn from the piece list then add a knight/ rook / queen ./ bisjhop
    of the correct color into the piece list array

8 enpass captures( ise hmne capture section me nhi liya  obviously we need to remove the pawn
    that was captured and remove it from piece list as well)

9 set enpas square is pawn start.  if a pawn is moved adn we fined that pawn is moved two squares
    then we need to set our enpass square

10 (reiterate here )for all pieces added, moved removed , update
    all position counters and piece list  adjust the piece number,  keep track of major mini=or big pieces
    material count

11 maintain poskey  (cz every time a piece changfestate on board   either move or added or taken away
    we need to xor piece out completely or the piece out on the square it was on  and in on the square
    it has gone to)

12 dont forget castle permission  (every move that is made we are basically perfirming a bitwise AND 
    operation with an array where only the squares e8, h8,8 and e1,h1,a1  wheather the rooks and the
    kings sit   have values different than 15   , so if one of the moves it moves   it adjust automatically
    the castle permission)

13 change side, increment ply and hisply  (we have to change the side when the move is made, we have to
    increment the ply  so the number of half moves  made in a current search  and also hisply  ie in 
    overall game history's half move made from the star tof the game)

*/

#include<iostream>
#include"def.h"
#include<vector>

using namespace std;

// yha t bracket me isliye kuki sq+9 bheja jese to + and >> takraav to () best
#define move(f,t,ca,pro,flag) ((f)|((t)<<7)|(ca<<14)|(pro<<20)|(flag))// creating a move integer
#define square_offboard(sq) (file_board[(sq)]==OFFBOARD)// is the sq offboard or not
// apn ne validate me fun bhi create kiya h but ye easy


//for sliding pieces
// pawns me kafi move h  cap promote etc to black white donon ko separate kiya 
// pr jese ab sliding pieces ka krna ho  bisop rook queen
// to dono ko sath krenge 
// to ek array em lemnge white black bishop queeen rook   aur ese lemnge ki jb white ke 3 kr le to kuch
// stope aaye fir balack ka aayearray me taki pta lagas ke


const vector<int> loop_slide_piece{wb,wr,wq,0,bb,br,bq,0};

// butb  but but pta kese chlega ki side to move is black or white 
// to ek aur arrrya use krnge


 const vector<int> loop_slide_index{0,4};// white to 0 index se start nhi to 4 index se start

// jese mana 
//loop_slide_index[black]=4;
//then 
//loop_slide_piece[loop_slide_index[side]]==bb  side black thi
//  yha s estart hoga ye array



// for non slidng pieces we also do the same
const vector<int> loop_non_slide_piece{wn,wk,0,bn,bk,0};
const vector<int> loop_non_slide_index{0,3};


 // ab move generation  for non sliders
// to jese knight h to 8 diff directionm me ja skta h vo to
// to tele program that we need to loop through 8 different direction
// piece dir array bnaenge

// aur ye bhi btaenge ek arra se ki hr piece ke pas kitni direction me move h
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
// to matlb mana rook 4 dir h to 4 baar upr rook kk arr me loop krna h
// pawns set nhi kiye h kuki unka case alag se handle kiya h movegen me

/*
    onr onr 

    movegen (board,movelist){
        loop all piees
         then for each pieces
          slider move each direection  add move
            to aadd move   ist->moves[list count]=move;list->count++;
    }

*/




// ye function movegen.c me kai baar usse honge
// aur khi nhi honge to
// ise static bnao


static void add_quite_move(const s_board * pos,int move,s_movelist * list){// in this list we will be adding move
    list->moves[list->count].move=move;// sboard ke move me move dalaa kuki movelist me smove ka var h
    //score bhi set kro
    list->moves[list->count].score=0;
    list->count++;// increment the number of moves
}

static void add_capture_move(const s_board * pos,int move,s_movelist * list){// in this list we will be adding move
    list->moves[list->count].move=move;// sboard ke move me move dalaa kuki movelist me smove ka var h
    //score bhi set kro
    list->moves[list->count].score=0;
    list->count++;// increment the number of moves
}


static void add_enpass_move(const s_board * pos,int move,s_movelist * list){// in this list we will be adding move
    list->moves[list->count].move=move;// sboard ke move me move dalaa kuki movelist me smove ka var h
    //score bhi set kro
    list->moves[list->count].score=0;
    list->count++;// increment the number of moves
}


static void add_white_pawn_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){


    // assert to check that argument of cap piece is valid or empty
    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));


    if(rank_board[from]==r7){// 7 se 8 rank pr ja rha hhmne mana h to vha jake
    // ek ko promote krega
    // we will byild move using captured piece   and promoted wq, wn  etc kra h
        add_capture_move(pos,move(from,to,cap,wq,0),list);
        add_capture_move(pos,move(from,to,cap,wr,0),list);
        add_capture_move(pos,move(from,to,cap,wb,0),list);
        add_capture_move(pos,move(from,to,cap,wn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,cap,emptyy,0),list);          

}


static void add_white_pawn_move(const s_board * pos , const int from, const int to,s_movelist * list){

    // assert to check that piece is valid or empty
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));


    if(rank_board[from]==r7){// 7 se 8 rank pr ja rha hhmne mana h to vha jake
    // ek ko promote krega
    // we will byild move using captured piece   and promoted wq, wn  etc kra h
        add_capture_move(pos,move(from,to,emptyy,wq,0),list);// cpatured piece nhi h isliye empty
        add_capture_move(pos,move(from,to,emptyy,wr,0),list);
        add_capture_move(pos,move(from,to,emptyy,wb,0),list);
        add_capture_move(pos,move(from,to,emptyy,wn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);          

}


// do tthis for black also


static void add_black_pawn_cap_move(const s_board * pos , const int from, const int to,
                const int cap, s_movelist * list){

    
    // assert to check that argument of cap piece is valid or empty
    ASSERT(piece_valid_empty(cap));
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));


    if(rank_board[from]==r2){// ab yha r7 ki jgh r2 kra h
    // kuki black promote hi tb hpoga jb 2 se 1st rank pr jaega
    // white vale ka ulta kumki opposite side h
    // ek ko promote krega
    // we will byild move using captured piece  
        add_capture_move(pos,move(from,to,cap,bq,0),list);
        add_capture_move(pos,move(from,to,cap,br,0),list);
        add_capture_move(pos,move(from,to,cap,bb,0),list);
        add_capture_move(pos,move(from,to,cap,bn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,cap,emptyy,0),list);          

}

static void add_black_pawn_move(const s_board * pos , const int from, const int to,s_movelist * list){


    // assert to check that argument of cap piece is valid or empty

    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));


    if(rank_board[from]==r2){
    // ek ko promote krega
    // we will byild move using captured piece  
        add_capture_move(pos,move(from,to,emptyy,bq,0),list);// cpatured piece nhi h isliye empty
        add_capture_move(pos,move(from,to,emptyy,br,0),list);
        add_capture_move(pos,move(from,to,emptyy,bb,0),list);
        add_capture_move(pos,move(from,to,emptyy,bn,0),list);
    }    
    else   
        add_capture_move(pos,move(from,to,emptyy,emptyy,0),list);          

}




// now after all this we can generate our movegen fun

// isme jo print stetment h vo checking ke liye h
// last me kuchko replace krenge  by functions 
// like add quite move, ca add captured move  etc etc
//to store a move in  a movelist

void generate_all_moves(const s_board * pos,s_movelist * list){
    ASSERT(check_board(pos));
    list->count=0;// cz no move  generated

    int piece=emptyy;
    int side=pos->side;
    int sq=0,target_sq=0,pce_num=0;

    int dir=0,index=0,piece_index=0;

    //printf("\nside:%d\n",side);

    if(side==white){ // if the side is white  to geneerate the white pawn move 
        for(pce_num=0;pce_num<pos->piece_num[wp];pce_num++){// loop through all white pawns on the board

            sq=pos->piece_list[wp][pce_num];// kis square pr h vo pawn ye dekho  piece list se which is index by piece type
            // piece list tkake wp   and at which piece number we want ot look at
            ASSERT(sq_on_board(sq));
            

            // generate non captured moves  where pawn move simply forward and 
            // aur forward move ke liye uske vo sq empty hona chhiye jha move krna h
            // pawn phli baar me 1 ya  2 forwsrd move kr sta h   usk ebbad 1 forward
            //+10 hi jate h forward jese 34 pr tha paawn mana to 44 pr jaegge 
            if(pos->pieces[sq+10]==emptyy){// if empty
                // then add a white pawn move
                add_white_pawn_move(pos,sq,sq+10,list);

                //ya 20 pr phli baar me bs 
                // kuki 2 bsq aage move
                if(rank_board[sq]==r2 && pos->pieces[sq+20]==emptyy){  // if empty
                    // then add a quite move cz there ois no promotion adn we starting from second rank
                    // empty h promotion
                    add_quite_move(pos,move(sq,sq+20,emptyy,emptyy,move_flag_ps),list);
                }
            }

            // nextr 2 if's for generatign the captures
            // +9  ya +11 direction me hoga capture, pawn ko krna h agar koi to
            // jahir si baat h +9 ya +11 vale lka color different hoga apne sq se
            // +9 a +11 vala off board na ho + other condn
            // tehn we can add a white pawn captured move

            if(!square_offboard(sq+9)  && piece_color[pos->pieces[sq+9]]==black){// white bllack ko acptured krega
                add_white_pawn_cap_move(pos,sq,sq+9,pos->pieces[sq+9],list);
            }

            if(!square_offboard(sq+11)  && piece_color[pos->pieces[sq+11]]==black){
                add_white_pawn_cap_move(pos,sq,sq+11,pos->pieces[sq+11],list);
            }

            // other 2 if's for generat ethe enpass captures

            // imp
            // yha add capture move hi lagega kuki 6th rank pr promotion lagaegga nhi
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

        if(pos->castle_perm & wkca ){// means we got permission to castle king side
            if(pos->pieces[f1]==emptyy && pos->pieces[g1]==emptyy){
                if(!square_attacked(e1,black,pos) && !square_attacked(f1,black,pos)){
                    //is e1 and f1 attacked by black
                    //printf("\n wkca movegen\n");

                    add_quite_move(pos,move(e1,g1,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }

        if(pos->castle_perm & wqca ){//queen side castlin gpermission
            if(pos->pieces[d1]==emptyy && pos->pieces[c1]==emptyy && pos->pieces[b1]==emptyy){
                if(!square_attacked(e1,black,pos) && !square_attacked(d1,black,pos)){
                    //printf("\n wqca movegen\n");

                    add_quite_move(pos,move(e1,c1,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }
    }
    else{
        for(pce_num=0;pce_num<pos->piece_num[bp];pce_num++){// loop through all black pawns on the board

            sq=pos->piece_list[bp][pce_num];// kis square pr h vo pawn ye dekho  piece list se which is index by piece type
            // piece list tkake bp   and at which piece number we want ot look at
            ASSERT(sq_on_board(sq));
            
            // yha -10 direction hoga obvious h kuki black go from top to bottom
            if(pos->pieces[sq-10]==emptyy){// if empty
                // then add a black pawn move
                add_black_pawn_move(pos,sq,sq-10,list);

                //ya -20 pr phli baar me bs 
                // kuki 2 bsq aage move
                if(rank_board[sq]==r7 && pos->pieces[sq-20]==emptyy){  // if empty
                    // then add a quite move cz there ois no promotion adn we starting from second rank
                    // empty h promotion
                    add_quite_move(pos,move(sq,sq-20,emptyy,emptyy,move_flag_ps),list);
                }
            }

            // nextr 2 if's for generatign the captures
            // -9  ya -11 direction me hoga capture, pawn ko krna h agar koi to
            // jahir si baat h -9 ya -11 vale lka color different hoga apne sq se
            // -9 a -11 vala off board na ho + other condn
            // tehn we can add a black pawn captured move

            if(!square_offboard(sq-9)  && piece_color[pos->pieces[sq-9]]==white){// yha white kuki 
                // black, white ko captured krega
                add_black_pawn_cap_move(pos,sq,sq+9,pos->pieces[sq-9],list);
            }

            if(!square_offboard(sq-11)  && piece_color[pos->pieces[sq-11]]==white){
                add_black_pawn_cap_move(pos,sq,sq-11,pos->pieces[sq-11],list);
            }

            // other 2 if's for generat ethe enpass captures

            // imp
            // yha add capture move hi lagega kuki 6th rank pr promotion lagaegga nhi
            if(pos->enpass!=no_sq){
                if(sq-9==pos->enpass){
                    add_capture_move(pos,move(sq,sq-9,emptyy,emptyy,move_flag_ep),list);
                }   

                if(sq-11==pos->enpass){
                    add_capture_move(pos,move(sq,sq-11,emptyy,emptyy,move_flag_ep),list);
                }
            }
        }

        // for castling king side and queen side

        if(pos->castle_perm & bkca ){// means we got permission to castle king side
            if(pos->pieces[f8]==emptyy && pos->pieces[g8]==emptyy){
                if(!square_attacked(e8,white,pos) && !square_attacked(f8,white,pos)){
                    //printf("\n bkca movegen\n");

                    //e8 to g8
                    add_quite_move(pos,move(e8,g8,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }

        if(pos->castle_perm & bqca ){//queen side castlin gpermission
            if(pos->pieces[d8]==emptyy && pos->pieces[c8]==emptyy && pos->pieces[b8]==emptyy){
                if(!square_attacked(e8,white,pos) && !square_attacked(d8,white,pos)){
                   //printf("\n bqca movegen\n");

                   add_quite_move(pos,move(e8,c8,emptyy,emptyy,move_flag_cs),list);
                }
            }
        }

    }

    // loop for sliders// sliding pieces
    // top pr jo aarrray bnae uskae logic se

    piece_index=loop_slide_index[side];// black to 4 dega nhi to 0
    piece=loop_slide_piece[piece_index++];// us index se piece liayat hen increment the index

    while(piece!=0){
        ASSERT(piece_valid(piece));

        //printf("\n sliders : piece_index: %d piece:%d\n",piece_index,piece);

        // non sliders ka dekho phle loop andar ka
        // sliders ka same hi h 

        // once we got the piece type we do exactly same loop as we did in pawn moves
        // loop through all the pieces in pieces list

        for(pce_num=0;pce_num<pos->piece_num[piece];pce_num++){
            sq=pos->piece_list[piece][pce_num];
            ASSERT(sq_on_board(sq));
            //printf("\n piece: %c on %s \n",piece_char[piece],print_sq(sq));



            // ab move generation  for  sliders
            // non sliders jesa h same

            for(index=0;index<num_dir[piece];index++){// num dire ne total direction di utna loop chlega
                dir=piece_dir[piece][index];// then we simply taking ech of the direction at index 0 to index
                target_sq=sq+dir;// sq the piece is on +direction

                // yha we iterate futther in direction 
                while(!square_offboard(target_sq)){// lop isliye
                // kuki jese pura diagonal chl skta h ek piece to tb tk chlte jao jb tk offboard na a jaye
                    // dealing with capture
                    //black white 0 and 1 h 
                    // black ^1==white     white ^1== black  

                    if(pos->pieces[target_sq]!=emptyy){// then it can be a capture
                        if(piece_color[pos->pieces[target_sq]]==(side^1)){// color opposite ho
                            //printf("\t\t captured on %s\n",print_sq(target_sq));// cap sq

                            // captured piece would be the piece at out target square in our position
                            add_capture_move(pos,move(sq,target_sq,pos->pieces[target_sq],emptyy,0),list);
                        }
                        break;// kuki capture ho gya to ab continue thodi hoga 
                        // loop break krenge kuki jese kisi ko pura diagonal jana h
                        // pr usne capture kr liya to break ab
                    }
                    // otherwise
                    //printf("\t\t normal on %s\n",print_sq(target_sq));
                    add_quite_move(pos,move(sq,target_sq,emptyy,emptyy,0),list);

                    target_sq+=dir;
                }
            }
        }
        
        piece=loop_slide_piece[piece_index++];
    }

    // for non sliders

    piece_index=loop_non_slide_index[side];// black to 4 dega nhi to 0
    piece=loop_non_slide_piece[piece_index++];// us index se piece liayat hen increment the index

    while(piece!=0){
        ASSERT(piece_valid(piece));

        //printf("\n non sliders : piece_index: %d piece:%d\n",piece_index,piece);

        // once we got the piece type we do exactly same loop as we did in pawn moves
        // loop through all the pieces in pieces list

        for(pce_num=0;pce_num<pos->piece_num[piece];pce_num++){
            sq=pos->piece_list[piece][pce_num];
            ASSERT(sq_on_board(sq));
            //printf("\n piece: %c on %s \n",piece_char[piece],print_sq(sq));

            // ab move generation  for non sliders
            // to jese knight h to 8 diff directionm me ja skta h vo to
            // to tele program that we need to loop through 8 different direction
            // piece dir array bnaenge

            // aur ye bhi btaenge ek arra se ki hr piece ke pas kitni direction me move h
            // upr define kre h 2 arr
            // to matlb mana rook 4 dir h to 4 baar upr rook kk arr me loop krna h
            // pawns set nhi kiye h kuki unka case alag se handle kiya h movegen me

            for(index=0;index<num_dir[piece];index++){// num dire ne total direction di utna loop chlega
                dir=piece_dir[piece][index];// then we simply taking ech of the direction at index 0 to index
                target_sq=sq+dir;// sq the piece is on +direction

                if(square_offboard(target_sq))
                    continue;

                // dealing with capture
                //black white 0 and 1 h 
                // black ^1==white     white ^1== black  

                if(pos->pieces[target_sq]!=emptyy){// then it can be a capture
                    if(piece_color[pos->pieces[target_sq]]==(side^1)){// color opposite ho
                        //printf("\t\t captured on %s\n",print_sq(target_sq));// cap sq

                        // captured piece would be the piece at out target square in our position
                        add_capture_move(pos,move(sq,target_sq,pos->pieces[target_sq],emptyy,0),list);
                    }
                    continue;
                }
                // otherwise
                //printf("\t\t normal on %s\n",print_sq(target_sq));

                add_quite_move(pos,move(sq,target_sq,emptyy,emptyy,0),list);
            }
        }
        piece=loop_non_slide_piece[piece_index++];
    }
}

