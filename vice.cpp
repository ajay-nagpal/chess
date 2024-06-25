#include<iostream>
#include"def.hpp"

#include"cstdlib"
#include<vector>

using namespace std;

#define perftfen "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define wac1 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main(){
    all_init();
    
    s_board pos[1];
    s_search_info info[1];
    info->quit=false;
    pos->hash_table->ptable=NULL;

    init_hash_table(pos->hash_table,64);

    cout<<"welcome to Vice!. type 'vice' for console mode..."<<endl;
    string line;

    while(1){
        cout<<"enter line"<<endl;
        getline(cin,line);

        if(line=="uci"){
            uci_loop(pos,info);
            if(info->quit==true)
                break;
            continue;
        }
        else if(line=="xboard"){
            xboard_loop(pos,info);
            if(info->quit==true)
                break;
            continue;
        }
        else if(line=="vice"){
            console_loop(pos,info);
            if(info->quit==true)
                break;
            continue;
        }
        else if(line=="quit"){
            break;
        }
    }
    //uci_loop(); // to check till uci
    delete(pos->hash_table->ptable);
    return 0;
}



