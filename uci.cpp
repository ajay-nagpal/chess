#include<iostream>
#include"def.hpp"
#include<string.h>

using namespace std;

// parsing the go command
void parse_go(string line, s_search_info * info, s_board * pos){

}

//either
//position fen
//position startpos
//position startpos moves e2e4 e7e5 b7b8q
void parse_pos(string line_in, s_board * pos){
    if( line_in.find("moves")==string::npos && line_in.find("fen")==string::npos){
        parse_fen(start_fen,pos);
    }
    else if(line_in.find("fen")!=string::npos){
        string s=line_in.substr(13);
        const char * ptr_char=s.c_str();
        parse_fen(ptr_char,pos);
    }

    int move=0;
    if(line_in.find("moves")!=string::npos){
        
        string s=line_in.substr(24);
        cout<<s<<endl;

        for(int i=0;i<s.size();i+=5){

            string s1=s.substr(i,4);
            cout<<s1<<endl;
            move=parse_move(s1,pos);
            cout<<s1<<endl;
            if(move==nomove){
                break;
            }
            cout<<s1<<endl;
            make_move(pos,move);
             cout<<s1<<endl;
            pos->ply=0;
        }
    }
    print_board(pos);
}

void uci_loop(){

    cout<<"id name "<<NAME<<endl;
    cout<<"id author Ajay"<<endl;
    cout<<"uci ok"<<endl;

    s_board pos[1];
    s_search_info info[1];
    init_pvtable(pos->pvtable);

    while(1){
        string line;
        cout<<"enter line"<<endl;
        getline(cin,line);

        if(line=="isready"){
            cout<<"ready ok"<<endl;
            continue;
        }
        else if(line.find("position")!=string::npos){// khali positioin nhi chlega usne 8 char strcpy kiye h kuki 
            parse_pos(line,pos);
        }
        else if(line=="ucinewgame"){
            parse_pos("position startpos\n",pos);
        }
        else if(line=="go"){
            parse_go(line,info,pos);// agar is go me quit set hua ho to last me alg se ifcondn se break
        }
        else if(line=="quit"){
            info->quit=true;
            break;
        }
        else if(line=="uci"){
            cout<<"id name "<<NAME<<endl;
            cout<<"id author Ajay"<<endl;
            cout<<"uci ok"<<endl;
        }
        if(info->quit){
            break;
        }
    }
    delete(pos->pvtable->ptable);
}