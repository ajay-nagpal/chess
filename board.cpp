#include<iostream>
#include"def.hpp"
#include<vector>

#include <iomanip>// to convert %3d  in c++  by using  set width

using namespace std;

int parse_fen( const char* fen ,s_board * pos){

    ASSERT(fen!=NULL);
    ASSERT(pos!=NULL);

    int rank=r8;
    int file =fa;
    int piece=0;
    int count=0;
    int i=0;

    int sq64=0;
    int sq120=0;

    reset_board(pos);

    while((rank>=r1)  && *fen){
        count=1;
        switch(*fen){
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
            case ' ':
                    rank--;
                    file=fa;
                    fen++;
                    continue;

            default:
                    cout<<"fen error "<<endl;
                    return -1;
        
        }
        
        for(i=0;i<count;i++){
            sq64=(rank * 8)+ file;
            sq120=SQ120(sq64);

            if(piece!=emptyy){
                pos->pieces[sq120]=piece;
            }
            file++;
        }

        fen++;
    }

    ASSERT(*fen=='w' || *fen =='b');

    pos->side=((*fen=='w')?white:black);
    fen+=2;// to come on casteling KQkq in string

    for(i=0;i<4;i++){
        if(*fen==' ')
            break;

        switch(*fen){
            case 'K':(pos->castle_perm)|=wkca;break;
            case 'Q':pos->castle_perm|=wqca;break;
            case 'k':pos->castle_perm|=bkca;break;
            case 'q':pos->castle_perm|=bqca;break;

            default: break;
        }
        fen++;// if casteling poss nhi hoti to dash hota h 
    }
    fen++;

    ASSERT(pos->castle_perm>=0 && pos->castle_perm<=15 );

    if(*fen !='-'){// enpass hoga to ek letter aur ek number hoga to next step
        file=fen[0]-'a'; // to vo a to h ya 1 to  8 hoga to extract krke assert se check
        rank=fen[1]-'1';

        cout<<endl<<file<<" "<<rank<<endl;

        ASSERT(file>=fa && file<=fh);
        ASSERT(rank>=r1 && rank<=r8);
     
        pos->enpass=(fr_to_sq(file,rank));// to set enpass
    }

    pos->poskey=generate_pos_key(pos);

    update_list_material(pos);

    return 0;
}

void reset_board(s_board *pos){
    int index=0;

    for(index=0;index<board_sq_num;index++)
        pos->pieces[index]=OFFBOARD;

    for(index=0;index<64;index++)
        pos->pieces[SQ120(index)]=emptyy;

    for(index=0;index<2;index++){
        pos->big_pieces[index]=0;
        pos->major_pieces[index]=0;
        pos->minor_pieces[index]=0;
        pos->material[index]=0;
        
    }

    for(index=0;index<3;index++)
        pos->pawns[index]=0ULL;

    for(index=0;index<13;index++)
        pos->piece_num[index]=0;

    pos->king_sq[white]=pos->king_sq[black]=no_sq;
    pos->side=both;
    pos->enpass=no_sq;

    pos->fiftymove=0;

    pos->ply=0;
    pos->hisply=0;

    pos->castle_perm=0;

    pos->poskey=0ULL;
    
    //call init pvtbale

    init_pvtable(pos->pvtable);
}

void print_board(const s_board *pos){
    int sq=0,file=0,rank=0,piece=0;

    cout<<endl<<"game board:"<<endl<<endl;

    for(rank=r8;rank>=r1;rank--){
        cout<<rank+1<<" ";
        for(file=fa;file<=fh;file++){
            sq=fr_to_sq(file,rank);
            piece=pos->pieces[sq];
            cout<<setw(3)<<piece_char[piece];
        }

        cout<<endl;
    }
    cout<<endl<<"  ";

    for(file=fa;file<=fh;file++){
        cout<<setw(3)<<char('a'+file);
    }
    cout<<endl;

    cout<<"side:"<<side_char[pos->side]<<endl;
    cout<<dec;
    cout<<"enpass:"<<(pos->enpass)<<endl;
    
    cout<<"castle: "<<((pos->castle_perm & wkca) ?'K':'-')<<
                                (pos->castle_perm & wqca ?'Q':'-')<<
                                (pos->castle_perm & bkca ?'k':'-')<<
                                (pos->castle_perm & bqca ?'q':'-');
    cout<<endl;
  
    cout<<"poskey:"<<setw(11)<<hex<<uppercase<<pos->poskey<<endl;
    cout<<dec;
}

void update_list_material(s_board *pos){
    int piece=0,sq=0,index=0,color=0;

    for(index=0;index<board_sq_num;index++){
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

            pos->material[color]+=piece_value[piece];
            // mana piecelist[wp][0]=a1;  lkike this
            pos->piece_list[piece][pos->piece_num[piece]]=sq;
            pos->piece_num[piece]++;// 0 se 1 ho gya ab

            if(piece==wk)
                pos->king_sq[white]=sq;
            
            if(piece==bk)
                pos->king_sq[black]=sq;

            // to set bits for pawn

            if(piece==wp){
                setbit(pos->pawns[white],SQ64(sq));// set the bit on the bitbpard on the position for white pawns
                // setting a bit in a a64 index format  and sq is in 120 indexing format
                setbit(pos->pawns[both],SQ64(sq));
            }
            else if(piece==bp){
                setbit(pos->pawns[black],SQ64(sq));
                setbit(pos->pawns[both],SQ64(sq));
            }
        }
    }
}


int check_board(const s_board *pos){
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
            ASSERT(pos->pieces[sq120]==temp_piece);
        }
    }

    // check piece count and other counterts
    for(sq64=0;sq64<64;sq64++){
        sq120=SQ120(sq64);

        temp_piece=pos->pieces[sq120];
        temp_piece_number[temp_piece]++;
        
        color=piece_color[temp_piece];

        if(piece_big[temp_piece]==true)
            temp_big_piece[color]++;
        if(piece_minor[temp_piece]==true)
            temp_minor_piece[color]++;
        if(piece_major[temp_piece]==true)
            temp_major_piece[color]++;

        temp_material[color]+=piece_value[temp_piece];
    }

    for(temp_piece=wp;temp_piece<=bk;temp_piece++){
        ASSERT(temp_piece_number[temp_piece]==pos->piece_num[temp_piece]);
    }

    pcount=cnt(temp_pawns[white]);
    ASSERT(pcount==pos->piece_num[wp]);

    pcount=cnt(temp_pawns[black]);
    ASSERT(pcount==pos->piece_num[bp]);

    pcount=cnt(temp_pawns[both]);
    ASSERT(pcount==(pos->piece_num[bp]+pos->piece_num[wp]));

    // check bitboard squares
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
    ASSERT(generate_pos_key(pos)==pos->poskey);//vid 20 9:00 se

    ASSERT(pos->enpass==no_sq || (rank_board[pos->enpass]==r6 && pos->side==white)
            || (rank_board[pos->enpass]==r3 && pos->side==black));

    ASSERT(pos->pieces[pos->king_sq[white]]==wk);
    ASSERT(pos->pieces[pos->king_sq[black]]==bk);

    ASSERT(pos->castle_perm >= 0 && pos->castle_perm <= 15);

    return true;

}
