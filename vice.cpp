#include<iostream>
#include"def.hpp"

#include"cstdlib"
#include<vector>

using namespace std;

#define perftfen "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"


int main(){
    AllInit();
    
    s_board board[1];
    s_movelist list[1];

    parse_fen(start_fen,board);

    //char input[6];//5 for move 1 for enter
    string input;
    int move=nomove;

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
        else if(input[0]=='p'){
            perft_test(4,board);// to print time elapsed
        }
        else{
            // we have a move parse this move
            move=parse_move(input,board);

            if(move!=nomove){
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

    return 0;
}



