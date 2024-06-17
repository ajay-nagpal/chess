#include<iostream>
#include"def.hpp"

#include"cstdlib"
#include<vector>

using namespace std;

#define perftfen "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"
#define wac1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main(){
    AllInit();
    
    s_board board[1];

    //call init pvtbale
    init_pvtable(board->pvtable);
   
    s_movelist list[1];
    s_search_info info[1];
    parse_fen(wac1,board);

    //char input[6];//5 for move 1 for enter
    //parse move me chage krna agar strign htao to char * ptr_char
    string input;
    int move=nomove;

    int max=0;// related to pv num

    while(1){
        print_board(board);
        cout<<endl<<"enter a move"<<endl;
        cin>>input;
        //fgets(input,6,stdin);

        if(input[0]=='q')
            break;
        else if(input[0]=='t'){
            take_move(board);
        }
        else if(input[0]=='s'){
           info->depth=6;
           search_pos(board,info);
        }
        else{
            // we have a move parse this move
            move=parse_move(input,board);
            if(move!=nomove){
                store_pvmove(board,move);// means we can stroe move  
                // ab 'p' pr retrival ho esa kuch kro 
                make_move(board,move);
                // if(is_repetition(board)){
                //     cout<<endl<<"repetition"<<endl;
                // }
            }
            else{
                cout<<endl<<"move not parsed "<<input<<endl;
            }
        }
        //fflush(stdin);
    }
    delete(board->pvtable->ptable);
    return 0;
}



