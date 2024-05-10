#include<iostream>
#include"def.h"
#include<vector>

#include <iomanip>// to convert %3d  in c++  by using  set width

using namespace std;//vid 13


// fen position/

//it is a string representm of a board 
// 8 7 6 5 4 3  2 1   vali rank ka btata h

// ese

//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

//rook ,knight bishop,queen king ...//7yh me blac pawn h 8 pos pr
// fir 8,8,8,8 khali,  fir white pawn 8  fir white baki h 
// fir w KQkq - 0 1   ka mtlb hai

//w or b represent white or black to mpove   abhi w h menas white ka turn h


//KQkq  representing casteling permission
// K,Q if white can castle king side,queen side
// k,q if black can castle king side,queen side

// - to represent en[pass square if avsilable abhi nhi h available isliye dash h 
// agar hota to uski po aaati jese abhi next step em aaegi
//0 represent 50 move real counter abhi  nhi hua 1 puramove(black white dono ne nhi chli)

//chl lete to +1 hota
// last ka 1 full move counter h jo starting me 1 h   agle move me bhi 1  uske agle move me 2 hoga 

// mana koi move chla let  white pawn
// means fen after move 1.e4

// rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1
// yha e3 enpass sq bna h
//PPPP1PPP   is row ka 4 pawn 1 khali 3 pawn 



//8/8/4P3  means    black side ke 2 row khali   3rd me 4 khali fir pawn fir 3 khali..



//And then after 1...c5:

//rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2

//And then after 2.Nf3:

//rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2


// for setting up position in board from a fen strign 
int parse_fen(const char* fen ,s_board * pos){// pointer to string fen  and pointer to board

    ASSERT(fen!=NULL);// to check that we do pointing at something otherwise problems crashing 
    ASSERT(pos!=NULL);

    int rank=r8;// cz us string me left se start h 8th rank se 
    int file =fa;// first file se start hui h strign
    int piece=0;// to hold the piece if we find one
    int count=0;// to count the number of emty squares
    int i=0;// for for loop

    int sq64=0;
    int sq120=0;

    reset_board(pos);

    while((rank>=r1)  && *fen){//rank>=r1  annd we are pointing to something in our strign
        count=1;
        switch(*fen){// fen is curr character we are pointing in fen strign
            case 'p': piece=bp;break;
            case 'r': piece=br;break;
            case 'n': piece=bn;break;
 
            case 'b': piece=bb;break;
            case 'k': piece=bk;break;
            case 'q': piece=bq;break;

            case 'P': piece=wp;break;
            case 'R': piece=wr;break;
            case 'N': piece=wn;break;

            case 'B': piece=wb;break;
            case 'K': piece=wk;break;
            case 'Q': piece=wq;break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                    piece=emptyy;
                    count= (*fen)-'0';// count the number of empty squares
                    break;

            case '/':
            case ' ':// both means either we are at the end of the pieces or or got the new rank
                    rank--;// so drop 1 rank  cz we are now stating from new rank
                    file=fa;// reset the file
                    fen++;// move next char in fen and continue the while loop
                    continue;

            default:// apart form all these if we gen something else then its an error
                    printf("fen error \n");
        
        }
        

        // let we are on file a an d ther ank is 4   
        // kuki number h to 4 loop 0 se 3,file a ,b c d e,   tk chlegi

        // let  ab  4rkqp h to    r a ata use mil jata   aur jb for loop me aate to file e hoti
        // us time to hm if vale se piece put  krte board p
        for(i=0;i<count;i++){// loop through the number of empty files in the square;
            sq64=rank * 8+ file;
            sq120=SQ120(sq64);

            if(piece!=emptyy){
                pos->pieces[sq120]=piece;
            }
            file++;
        }

        fen++;
    }


    // ab vo w/b aaega fir vo castling vale

    //for w/b

    ASSERT(*fen=='w' || *fen =='b');

    pos->side=(*fen=='w')?white:black;
    fen+=2;// to come on casteling KQkq in string

    for(i=0;i<4;i++){
        if(*fen==' ')
            break;

        switch(*fen){
            case 'K':pos->castle_perm|=wkca;break;
            case 'Q':pos->castle_perm|=wqca;break;
            case 'k':pos->castle_perm|=bkca;break;
            case 'q':pos->castle_perm|=bqca;break;

            default: break;
        }
        fen++;// if casteling poss nhi hoti to dash hota h 
    }
    fen++;

    ASSERT(pos->castle_perm>=0 && pos->castle_perm<=15 );

    // now for enpass squaew

    if(*fen !='-'){// enpass hoga to ek letter aur ek number hoga to next step
        file=fen[0]-'a'; // to vo a to h ya 1 to  8 hoga to extract krke assert se check
        rank=fen[1]-'1';

        ASSERT(file>=fa && file<=fh);
        ASSERT(rank>=r1 && rank<=r8);

        pos->enpass=fr_to_sq(file,rank);// to set enpass
    }

    pos->poskey=generate_pos_key(pos);// gnerate a hash key

    update_list_material(pos);

    return 0;

}


// gui se related  first we will makr all zero 
// 120 based se 64 base index dene ka  macro bnao def.h me sq64 ka opposite  sq120
void reset_board(s_board *pos){// fun taking pointer to a board structure
    int index=0;

    // first set the pieces on the board
    //OFFBOARD is meant to be one of the surrounding squares, and NO_SQ is a extra square 
    //that isn't one of the 64 - in reality, it's not necessary to have the two, 
    //as I guess you've realized, it was more to help my tiny brain. 
    //Such as setting enpassant to NO_SQ

    //I couldn't find where OFFBOARD is defined.
    //If you are only following the video, it does 
    //not show the typing in of OFFBOARD, but at 4:32/6:52 
    //of the video you can see the following 
    //line in defs.h:  A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD

    for(index=0;index<board_sq_num;index++){
        pos->pieces[index]=OFFBOARD;// boundry squares of 120 based array which have no use 
        // other then telling that boundry exeed are offboard squares
        // other 64 remaining squarre are also set to offboard here
    }

    // offboard se empty kro un 120 me 64 offboard ho gye the jo because they arenot offboard
    for(index=0;index<64;index++){
        pos->pieces[SQ120(index)]=emptyy;// 64 based vale ko 120 base me convert krke empty bna rhe
    }

    for(index=0;index<2;index++){// 2 hi big piece aur baki ke h isliye<2
        // white and black isliye 2
        pos->big_pieces[index]=0;
        pos->major_pieces[index]=0;
        pos->minor_pieces[index]=0;

        pos->material[index]=0;
        
    }

    for(index=0;index<3;index++)// ye <3 kuki white black and both
        pos->pawns[index]=0ULL;


    for(index=0;index<13;index++){
        pos->piece_num[index]=0;// loop resetting the piece number by piece type to 0
    }

    pos->king_sq[white]=pos->king_sq[black]=no_sq;// setting the sq in which white and black king is to no_sq

    pos->side=both;// isde as both color assert check ke time kaam aaegi checkmate funcrtion me
    pos->enpass=no_sq;

    pos->fiftymove=0;

    pos->ply=0;// number of half moves play in the current search
    pos->hisply=0;// number of half moves play in the whole game

    pos->castle_perm=0;

    pos->poskey=0ULL;// curr pos key for the pos 0ull

}


// creating a function to print the board

void print_board(const s_board *pos){
    int sq,file,rank,piece;

    cout<<endl<<"game board:"<<endl<<endl;

    for(rank=r8;rank>=r1;rank--){// kuki not=rmal board me rank 1 last me hoti h 8 se start hota h
        // + 1 isliye kuki 1 se 8 h hmne 0 se 7 mana tha enum me to +1
        cout<<rank+1;
        for(file=fa;file<=fh;file++){
            sq=fr_to_sq(file,rank);
            piece=pos->pieces[sq];

            //printf("%3c",piece_char[piece]);
            cout<<setw(3)<<piece_char[piece];
        }

        cout<<endl;
    }
    cout<<endl<<" ";

    // to print file letters

    for(file=fa;file<=fh;file++){
        //printf("%3c",'a'+file);
        cout<<setw(3)<<'a'+file;
    }
    cout<<endl;

    cout<<"side:"<<side_char[pos->side]<<endl;
    
    cout<<"enpass:"<<pos->enpass<<endl;// %d hi le rhe h kuki esa koi fun nhibnaya abhi jo  iska exact likha de de
    //// 1st baar me 99 kuki no sq h aure uski value hmne 99 set ki h
    // 2nd me 45 kuki e3 s q 45 pr h hmne deci print krai h values


    cout<<"castle: "<<(pos->castle_perm & wkca) ?'K':'-'<<
                                pos->castle_perm & wqca ?'Q':'-'<<
                                pos->castle_perm & bkca ?'k':'-'<<
                                (pos->castle_perm & bqca ?'q':'-');
    cout<<endl;
  
    //printf("poskey: %11llx\n",pos->poskey);// unsigned long long ,,s o print in hexa deccimal form
    cout<<"poskey:"<<setw(11)<<hex<<pos->poskey<<endl;
}


void update_list_material(s_board *pos){
    int piece,sq,index,color;

    for(index=0;index<board_sq_num;index++){// loops through every squaere on the board
        sq=index;
        piece=pos->pieces[index];

        if(piece!=OFFBOARD && piece!=emptyy){
            color=piece_color[piece];

            if(piece_big[piece]==true)
                pos->big_pieces[color]++;
            
            if(piece_major[piece]==true)
                pos->major_pieces[color]++;
            
            if(piece_minor[piece]==true)
                pos->minor_pieces[color]++;


            //now set the material

            pos->material[color]+=piece_value[piece];

            // now set the piece list// piece list[13][10]// eg [white pawn ya piece][piece number]of our current piece type 
            // mana piecelist[wp][0]=a1;  lkike this
            //piecelist[wp][1]=a2
            pos->piece_list[piece][pos->piece_num[piece]]=sq;
            pos->piece_num[piece]++;// 0 se 1 ho gya ab 

            if(piece==wk)
                pos->king_sq[white]=sq;// white black collor ko indicate kr reh h
            
            if(piece==bk)
                pos->king_sq[black]=sq;

            // to set bits for pawn

            if(piece==wp){
                setbit(pos->pawns[white],SQ64(sq));// set the bit on the bitbpard on the position for white pawns
                // setting a bit in a a64 index format  and sq is in 120 indexing format 
                // so
                //so SQ64 is converting it 
                setbit(pos->pawns[both],SQ64(sq));
            }
            else if(piece==bp){
                setbit(pos->pawns[black],SQ64(sq));
                setbit(pos->pawns[both],SQ64(sq));
            }
            // put another fen ,fen4 in vice.c for testing
        }
    }
}



//  checkboard function
// to check the integrity of our board structure that everything is good in actual board using temperory variable 
// is fun ko assert ke andar rkhna h bs 
// baki error line ye bta dega 
//  ASSERT(check_board(board));  ese

int check_board(const s_board *pos){// these temp variables  we will use to 
//hold the information to mirror our position pos
// hmari filled kri value temp vali pos ke equsl honi chiye
// if not hen theere is nan error
    vector<int> temp_piece_number{0,0,0,0,0,0,0,0,0,0,0,0,0};
    vector<int> temp_big_piece{0,0};
    vector<int> temp_major_piece{0,0};
    vector<int> temp_minor_piece{0,0};
    vector<int> temp_material{0,0};

    int sq64,temp_piece,temp_piece_num,sq120,color,pcount;

    vector<u64> temp_pawns={0ULL,0ULL,0ULL};

    temp_pawns[white]=pos->pawns[white];
    temp_pawns[black]=pos->pawns[black];
    temp_pawns[both]=pos->pawns[both];

    // check piece list

    // loop on piece type from wp to black king bk
    for(temp_piece=wp;temp_piece<=bk;temp_piece++){
        for(temp_piece_num=0;temp_piece_num<pos->piece_num[temp_piece];temp_piece_num++){

            sq120=pos->piece_list[temp_piece][temp_piece_num];// from our piece list get the square for our piece
            ASSERT(pos->pieces[sq120]==temp_piece);// they should both be same if not then we 
            // knew taht piece list is not aligned
        }
    }

    // check piece count and other counterts

    for(sq64=0;sq64<64;sq64++){
        sq120=SQ120(sq64);

        temp_piece=pos->pieces[sq120];
        temp_piece_number[temp_piece]++;// incrementing the number of pieces in our temp piece
        color=piece_color[temp_piece];// getting color of that piece then incrementing big,minor,major pieces

        if(piece_big[temp_piece]==true)
            temp_big_piece[color]++;
        if(piece_minor[temp_piece]==true)
            temp_minor_piece[color]++;
        if(piece_major[temp_piece]==true)
            temp_major_piece[color]++;

        temp_material[color]+=piece_value[temp_piece];


        //question arise that why we are doing it when we do everything 
        // in update list maerial and set everuthing therer
        //cz when we make a move on a boardand then make another move
        // everything is updated incrementally

        // but if we make let say 10 move s and undo 10 moves thn we have to replace adn incremenet updatelist material function
        // values incrementally
        // that make messs
        //thats why we are writing this function
    }


    for(temp_piece=wp;temp_piece<=bk;temp_piece++){
        ASSERT(temp_piece_number[temp_piece]==pos->piece_num[temp_piece]);
        // to check that number of pieces we found on the board and store in temp piece array
        //equals each piece type what our position says for piece number
        // otherwise something is wrong with our piece number
    }

    // cehck bitboard count

    // taking pawns count of white / black / both bitboard 
    // and asserting that this count is equal to whatmposition say is the number of white/black both pawns we have

    pcount=cnt(temp_pawns[white]);
    ASSERT(pcount==pos->piece_num[wp]);

    pcount=cnt(temp_pawns[black]);
    ASSERT(pcount==pos->piece_num[bp]);

    pcount=cnt(temp_pawns[both]);
    ASSERT(pcount==(pos->piece_num[bp]+pos->piece_num[wp]));



    // check bitboard squares

    //

    while(temp_pawns[white]){
        sq64=pop(&temp_pawns[white]);//pop removes a bit from a bitboard and returns a
        // 64 based square index we are popping from temp board and 
        ASSERT(pos->pieces[SQ120(sq64)]==wp);// getting 120 based square from that 64 based square
        // and checking from aour pieces array that there is wp bp on that square or not
    }

    while(temp_pawns[black]){
        sq64=pop(&temp_pawns[black]);
        ASSERT(pos->pieces[SQ120(sq64)]==bp);
    }

    while(temp_pawns[both]){
        sq64=pop(&temp_pawns[both]);
        ASSERT((pos->pieces[SQ120(sq64)]==bp) || (pos->pieces[SQ120(sq64)]==wp));
    }

    // material,minor,major,big peieces should be same 
    ASSERT(temp_material[white]==pos->material[white]
            && temp_material[black]==pos->material[black]);

    ASSERT(temp_minor_piece[white]==pos->minor_pieces[white]
            && temp_minor_piece[black]==pos->minor_pieces[black]);

    ASSERT(temp_major_piece[white]==pos->major_pieces[white]
            && temp_major_piece[black]==pos->major_pieces[black]);

    ASSERT(temp_big_piece[white]==pos->big_pieces[white]
            && temp_big_piece[black]==pos->big_pieces[black]);
    //side should be white or black its obvious
    ASSERT(pos->side==white || pos->side==black);
    // position key should be same sas freshly generated key
    ASSERT(generate_pos_key(pos)==pos->poskey);

    // if enpass square is either no square or it f the side to move is white then it should be on rank 6 
    // and if side to move is black then enpass sq must be on sq somewhere in rank 3

    //cz enpass nossq nhi h to rank 6 ya rank 3
    ASSERT(pos->enpass==no_sq || (rank_board[pos->enpass]==r6 && pos->side==white)
            || (rank_board[pos->enpass]==r3 && pos->side==black));

    ASSERT(pos->pieces[pos->king_sq[white]]==wk);
    ASSERT(pos->pieces[pos->king_sq[black]]==bk);

    ASSERT(pos->castle_perm >= 0 && pos->castle_perm <= 15);

    return true;

}