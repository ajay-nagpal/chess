#include<iostream>
#include"def.hpp"

#include"cstdlib"
#include<vector>

#include <iomanip>// for %5d soln

using namespace std;

// wiki fen page other 3 fen as example
#define fen1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"

//#define fen1 "8/3q4/8/8/4Q3/8/8/8 w - - 0 2 "
#define fen2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define fen3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define fen4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

#define pawnmoves_w "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define pawnmoves_b "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"

#define knight_king_b "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define knight_king_w "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

#define rook "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"
#define queen "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1"
#define bishop "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"

#define castle1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define castle2 "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"

#define castle3 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

#define perftfen "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

void printbin(int move){
    int index=0;
    cout<<endl<<"as binary"<<endl;

    for(index=27;index>=0;index--){
        if((1<<index) & move)
            cout<<"1";
        else
            cout<<"0";

        if(index%4==0) 
            cout<<" ";
    }
    cout<<endl;
}


int main(){
    AllInit();
    //return 0;
    
   int move=0;
   int from=a2,to=h7,cap=wr,prom=bk;

   move=((from)|(to<<7)|(cap<<14)|(prom<<20));

    cout<<endl<<"from :"<< from_sq(move)  <<" to: "<< to_sq(move)<<
      " cap: " <<captured(move)<<" prom: "<<promoted(move)<<endl;

    // printf("algebric from:%s\n",print_sq(from));
    // printf("algebric to:%s\n",print_sq(to));

    cout<<"algebric from : "<<print_sq(from)<<endl;
    cout<<"algebric to : "<<print_sq(to)<<endl;

    printf("algebric move:%s\n",print_move(move));




   /*
   //  for testing generate white papawn move   and black pawn moves
    // s_board board[1];

    // parse_fen(pawnmoves_w,board);
    // cout<<endl<<"BOARD: "<<endl;
    // print_board(board);

    // s_movelist list[1];
    // generate_all_moves(board,list);
    // print_move_list(list);// pawnmoves jo hmne define kiya h uski board pr pos ye decide kr rhi h ki kya kya move gen kr skta h pawn vhi output hoga is ka
    

    // s_board board[1];

    // parse_fen(pawnmoves_b,board);
    // cout<<endl<<"BOARD: "<<endl;
    // print_board(board);

    // s_movelist list[1];
    // generate_all_moves(board,list);
    // print_move_list(list);

    */



   /*
   // for testing sliders and non sliders loops of movegen.c

   s_board board[1];

    parse_fen(pawnmoves_b,board);
    printf("\n BOARD:\n");
    cout<<endl<<"BOARD: "<<endl;

    s_movelist list[1];
    generate_all_moves(board,list);
    
    */


   /*
   // testing knight_king move

    s_board board[1];
    s_movelist list[1];

    parse_fen(knight_king_w,board);
    cout<<endl<<"BOARD: "<<endl;
    print_board(board);

    generate_all_moves(board,list);
    */



   /*

    // test for sliding piece move gen 
    s_board board[1];
    s_movelist list[1];

    //parse_fen(rook,board);
    //parse_fen(queen,board);
    parse_fen(bishop,board);

    cout<<endl<<"BOARD: "<<endl;
    print_board(board);

    generate_all_moves(board,list);

    */


   /*
   // castling movegen testing

    s_board board[1];
    s_movelist list[1];

    //parse_fen(castle1,board);
    parse_fen(castle2,board); 

    cout<<endl<<"BOARD: "<<endl;
    //print_board(board);

    generate_all_moves(board,list);

    */



   /*
   //now after replacing print by function in move gen finction   to create list 
   // we do testing

   // create castle 3 in vice.c

    s_board board[1];
    s_movelist list[1];

    parse_fen(castle3,board); 

    generate_all_moves(board,list);
    print_move_list(list);

    */

   


   // makemove testing

  //  s_board board[1];
  //  s_movelist list[1];

  //  parse_fen(start_fen,board);
  //  generate_all_moves(board,list);

  //  int move=0,move_num=0;

  //  print_board(board);
  //  getchar();

  //  for(move_num=0;move_num<list->count;move_num++){// loop through moves
  //       move=list->moves[move_num].move;
  //       cout<<endl<<" move "<<move<<endl;

  //       if(!make_move(board,move)){// return false if move is illegle
  //           continue;
  //       }

  //       cout<<endl<<" made: "<<print_move(move)<<endl;// print the move to the screen
  //       print_board(board);

  //       take_move(board);
  //       cout<<endl<<" taken: "<<print_move(move)<<endl;
  //       print_board(board);

  //       getchar();

  //  }

   


  // makemove testing by perft testing
    // s_board board[1];
    // s_movelist list[1];

    // parse_fen(start_fen,board);
    // perft_test(3,board);// depth 3 pr 8902 leaf nodes hote h data table se so lets see our output

}



