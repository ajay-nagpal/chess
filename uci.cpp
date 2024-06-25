#include<iostream>
#include"def.hpp"
#include<string.h>

using namespace std;

// parsing the go command
// things we can have in go  itime in  ms 1000ms==1s
//go depth 6 wtime 180000 btime 100000 binc 1000 binc 1000 movetime 1000 movestogo 40
void parse_go(string line, s_search_info * info, s_board * pos){
    int depth=-1, moves_togo=30,move_time=-1;
    int time=-1,inc=0;
    info->time_set=false;// means there is no time set // means infinite mode// in case we receive infinite time command

    size_t index=0;

    if(line.find("infinite")!=string::npos){
        ;
    }

    index=line.find("winc");
    if(index!=string::npos && pos->side==white){
        inc=stoi(line.substr(index+5));
    }

    index=line.find("binc");
    if(index!=string::npos && pos->side==black){
        inc=stoi(line.substr(index+5));
    }

    index=line.find("wtime");
    if(index!=string::npos && pos->side==white){
        time=stoi(line.substr(index+6));
    }

    index=line.find("btime");
    if(index!=string::npos && pos->side==black){
        time=stoi(line.substr(index+6));
    }

    index=line.find("movestogo");
    if(index!=string::npos){
        moves_togo=stoi(line.substr(index+10));
    }

    index=line.find("movetime");
    if(index!=string::npos){
        move_time=stoi(line.substr(index+9));
    }

    index=line.find("depth");
    if(index!=string::npos){
        depth=stoi(line.substr(index+6));
    }

    // now look at all our variables

    if(move_time!=-1){// if movetime is specified
        time=move_time;// set time to remaining to move time
        moves_togo=1;
    }

    info->start_time=get_time_ms();
    info->depth=depth;

    if(time!=-1){
        info->time_set=true;// so time was set
        time/=moves_togo;
        time-=50;// for safety purpose that we dont overrun time
        info->stop_time=info->start_time+ time +inc;// inc increment h
    }

    if(depth==-1){
        info->depth=maxdepth;
    }
    cout<<"time: "<<time<<" start: "<<info->start_time<<" stop: "<<info->stop_time<<
          " depth: "<<info->depth<<" timeset "<<info->time_set<<endl;

    search_pos(pos,info);
}

//either
//position fen
//position startpos
//position startpos moves e2e4 e7e5 b7b8q
//position fen fen paste then moves e1e2  
void parse_pos(string line_in, s_board * pos){
    if( line_in.find("moves")==string::npos && line_in.find("fen")==string::npos){
        parse_fen(start_fen,pos);
    }
    else if(line_in.find("fen")!=string::npos){
        string s=line_in.substr(13);
        cout<<s<<endl;
        const char * ptr_char=s.c_str();
        parse_fen(ptr_char,pos);
    }
    int move=0;
    
    // move ya to akela hoga ya sen ke sath vali string me
    if(line_in.find("moves")!=string::npos){
        string s;

        if(line_in.find("fen")!=string::npos){
            size_t i=line_in.find("moves");
            s=line_in.substr(i+6);

        }else{
            s=line_in.substr(24);
        }
        cout<<s<<endl;

        for(int i=0;i<s.size();i+=5){
            string s1=s.substr(i,4);
            move=parse_move(s1,pos);
            if(move==nomove){
                break;
            }
            make_move(pos,move);
            pos->ply=0;
        }
    }
    print_board(pos);
}

void uci_loop(s_board * pos, s_search_info * info){

    cout<<"id name "<<NAME<<endl;
    cout<<"id author Ajay"<<endl;
    cout<<"option name Hash type spin default 64 min 4 max "<<max_hash<<endl;
	cout<<"option name Book type check default true "<<endl;
    cout<<"uci ok"<<endl;

    string line;
    int mb=64;

    while(1){
        
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
        else if(line.find("go")!=string::npos){
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
        else if(line=="debug"){
            //debug_analysis_test(pos,info);
            break;
        }
        else if(line=="setoption name hash value"){
            size_t index=line.find("value");
            mb=stoi(line.substr(index+6));
            if(mb<4)
                mb=4;
            if(mb>max_hash)
                mb=max_hash;
            cout<<"set hash to "<<mb<<" mb"<<endl;
            init_hash_table(pos->hash_table,mb);
        }
        if(info->quit){
            break;
        }
    }
    // isliye iske bhi jrrort nhi main me kr denge jha se pass hoga
    //delete(pos->hash_table->ptable);
}