#include<iostream>
#include"def.hpp"

#include"cstdlib"
#include<vector>

using namespace std;

#define perftfen "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"


int main(){
    AllInit();
    
    s_board board[1];
    //vector<s_board>board(1);
    s_movelist list[1];

    parse_fen(start_fen,board);

    //char input[6];//5 for move 1 for enter
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
        else if(input[0]=='p'){
            //perft_test(4,board);// to print time elapsed

            max=get_pvline(4,board);// set max to the count when we get the v line 
            // count return hota h pv line se
            // 4 move make krne board pr hmne fix kr diya depth ko yha
            cout<<endl<<"pvline of "<<max<<" moves"<<endl;

            for(int pvnum=0;pvnum<max;pvnum++){
                move=board->pvarray[pvnum];// get the move form pv arrray and print it
                cout<<print_move(move)<<" ";
            }
            cout<<endl;
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

    return 0;
}



