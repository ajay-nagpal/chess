#include"def.hpp"
#include<iostream>

using namespace std;

long leaf_nodes;

void perft(int depth, s_board *pos){
    ASSERT(check_board(pos));
    
    if(depth==0){// recursion ki return condn
        leaf_nodes++;
        return ;
    }

    s_movelist list[1];
    generate_all_moves(pos,list);

    int move_num=0;
    for(move_num=0;move_num<list->count;move_num++){
        if(!make_move(pos,list->moves[move_num].move)){
            continue;
        }

        perft(depth-1,pos);// recurse with depth-1
        take_move(pos);
    }
    return;
}

void perft_test(int depth, s_board *pos){// leaf nodes for each move
    ASSERT(check_board(pos));
    print_board(pos);

    cout<<endl<<"starting test to depth: "<<depth<<endl;

    leaf_nodes=0;

    s_movelist list[1]; 
    generate_all_moves(pos,list);

    int move=0,move_num=0;

    for(move_num=0;move_num<list->count;move_num++){
        move=list->moves[move_num].move;

        if(!make_move(pos,move)){
            continue;
        }

        long cum_nodes=leaf_nodes;// creates a cumulative nodes variable on each loop
        perft(depth-1,pos);

        take_move(pos);

        long old_nodes=leaf_nodes-cum_nodes;// and used it to count
        // the number of leaf nodes searched on the previous move and print those to screen

        cout<<endl<<"move: "<<move_num+1<<" : "<<print_move(move)<<" : "<<old_nodes<<endl;
    }
    // print complete test result to the screen
    cout<<endl<<"test complete: "<<leaf_nodes<<" "<< " nodes visited"<<endl;
    return;
}