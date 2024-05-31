#include<iostream>
#include"def.hpp"
#include<vector>

#include <iomanip>// to convert %3d  in c++  by using  set width

using namespace std;

int parse_fen(const char* fen ,s_board * pos){

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


    // ab vo w/b aaega fir vo castling vale

    //for w/b

    ASSERT(*fen=='w' || *fen =='b');

    pos->side=(*fen=='w')?white:black;
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

    // now for enpass squaew

    if(*fen !='-'){// enpass hoga to ek letter aur ek number hoga to next step
        file=fen[0]-'a'; // to vo a to h ya 1 to  8 hoga to extract krke assert se check
        rank=fen[1]-'1';

        cout<<endl<<file<<" "<<rank<<endl;

        ASSERT(file>=fa && file<=fh);
        ASSERT(rank>=r1 && rank<=r8);
        pos->enpass=0;
        pos->enpass=(fr_to_sq(file,rank));// to set enpass
        printf("%d \n ",pos->enpass);
        cout<<(pos->enpass)<<endl;
    }

    pos->poskey=generate_pos_key(pos);// gnerate a hash key

    //update_list_material(pos);

    return 0;

}

void reset_board(s_board *pos){
    int index=0;

    for(index=0;index<board_sq_num;index++)
        pos->pieces[index]=OFFBOARD;

    for(index=0;index<64;index++)
        pos->pieces[SQ120(index)]=emptyy;

    for(index=0;index<3;index++){
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
}

void print_board(const s_board *pos){
    int sq,file,rank,piece;

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
    printf("enpass %d\n",pos->enpass);
    cout<<"enpass:"<<pos->enpass<<endl;

    cout<<"castle: "<<((pos->castle_perm & wkca) ?'K':'-')<<
                                (pos->castle_perm & wqca ?'Q':'-')<<
                                (pos->castle_perm & bkca ?'k':'-')<<
                                (pos->castle_perm & bqca ?'q':'-');
    cout<<endl;
  
    cout<<"poskey:"<<setw(11)<<hex<<uppercase<<pos->poskey<<endl;
}

