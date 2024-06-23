#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

// new(for new game)
// sd 8 (depth is 8)
// time (after every move how much time left)
// level  (moves to go or total time are set once by level command)
// usermove e2e4 (another use move)
// move e7e5 (my move)  and then check wheather it is a stalemate or a  mate or a draw a threefold

int three_fold_rep(const s_board * pos){
    ASSERT(check_board(pos));
    int i=0,r=0;
    for(i=0;i<pos->hisply;i++){
        if(pos->history[i].poskey==pos->poskey){
            r++;
        }
    }
    return r;
}

int draw_material(const s_board * pos){
    ASSERT(check_board(pos));
    if(pos->piece_num[wp] || pos->piece_num[bp])
        return false;
    if(pos->piece_num[wq] || pos->piece_num[bq] || pos->piece_num[wr] || pos->piece_num[br])
        return false;
    // is we have 2 bishoop on the board so they can deliver mate
    // so no draw return false;
    if(pos->piece_num[wb]>1 || pos->piece_num[bb]>1)
        return false;
    if(pos->piece_num[wn]>1 || pos->piece_num[bn]>1)
        return false;
    if(pos->piece_num[wn] && pos->piece_num[wb])
        return false;
    if(pos->piece_num[bn] && pos->piece_num[bb]>1)
        return false;

    return true;// all othe conditions can return a draw
}

int check_result(s_board * pos){// will call everytime a move is made or we make a mvoe
// to check if game is draw
    ASSERT(check_board(pos));
    
    if(pos->fiftymove>100){
        cout<<"1/2-1/2 {fifty move rule (claimed by vice)}"<<endl;
        return true;
    }
    if(three_fold_rep(pos)>=2){
        cout<<"1/2-1/2 {3-fold repetition (claimed by vice)}"<<endl;
        return true;
    }
    if(draw_material(pos)==true){
        cout<<"1/2-1/2 {insufficient material (claimed by vice)}"<<endl;
        return true;
    }

    // now chcek for stalemate or checkmate
    s_movelist list[1];
    generate_all_moves(pos,list);
    int move_num=0,found=0;

    for(move_num=0;move_num<list->count;move_num++){
        if(!make_move(pos,list->moves[move_num].move)){
            continue;
        }
        found++;// means we found a legal move we cant be in check or stalemate
        take_move(pos);// take that move back
        break;// break
    }
    if(found!=0){
        return false;// means we still have moves so no draw
    }
    // determine wheather mate or stalemate
    int in_check=square_attacked(pos->king_sq[pos->side],pos->side^1,pos);

    if(in_check==true){
        if(pos->side==white){// white to move
            cout<<"0-1 {black mates (claimed by vice)}"<<endl;// black checkmated us
            return true;
        }
        else{
            cout<<"0-1 {white mates (claimed by vice)}"<<endl;
            return true;
        }
    }
    else{
        cout<<endl<<"1/2-1/2 {stalemate (claimed by vice)}"<<endl;
        return true;
    }
    return false;
}

void print_options(){
    cout<<"feature ping=1 setboard=1 colors=0 usermove=1 memory=1"<<endl;
    cout<<"feature done=1"<<endl;// we are done telling the gui what feature we support
}

void xboard_loop(s_board * pos, s_search_info * info){
    info->game_mode=xboard_mode;
    info->post_thinking=true;
    
    print_options();//hack

    int depth =-1, move_time=-1;
    //int moves_togo=30;
    vector<int>moves_togo={30,30};// fro white and black // cz we dont know how many moves left so we track by search

    int time=-1,inc=0;
    int engine_side=both;
    int time_left=0,sec=0,mps=0;// moves per session
    int move=nomove;
    int i=0,score=0,mb=0;
    // thse all will be set at the start of each game 
    //we will receive level command
    //sd command  for setting the depth
    // level  or st and sd dono commadns ek sdath aa skte h to to engine should observe both limitations

    string line,command;
    size_t index=0;

    engine_side=black;
    parse_fen(start_fen,pos);
    depth=-1;
    time=-1;

    while(1){
        if(pos->side==engine_side && check_result(pos)==false){
            // think cz in this protocol we are not totally dependent on gui
            info->start_time=get_time_ms();
            info->depth=depth;

            if(time != -1){
				info->time_set = true;
				time /= moves_togo[pos->side];
				time -= 50;
				info->stop_time = info->start_time + time + inc;
			}
            if(depth == -1 || depth > maxdepth){
				info->depth = maxdepth;
			}

            cout<<"time: "<<time<<" start: "<<info->start_time<<" stop: "<<info->stop_time<<
                  " depth: "<<info->depth<<" timeset: "<<info->time_set<<"movestogo: "<<
                  moves_togo[pos->side]<<" mps: "<<mps<<endl;
 
            search_pos(pos,info);

			if(mps != 0){
				moves_togo[pos->side^1]--;// otherwise decrement after engine has actually searched 
				if(moves_togo[pos->side^1] < 1){
					moves_togo[pos->side^1] = mps;
				}
			}
        }
        cout<<"enter line"<<endl;
        getline(cin,line);
        // command ko 1st word dena h line ka
        index=line.find(' ');//1st space find
        command=line.substr(0,index);

        cout<<"command seen: "<<line<<endl;

        if(command=="quit"){
            break;
        }
        if(command=="force"){// if engine is in force mode we will not think
            engine_side=both;
            continue;
        }
        // color 0 means nwe will never receive a white or black command
        // usermove e2e4  means user makes a move
        if(command=="protover"){
            print_options();
            continue;
        }
        if(command=="sd"){
            index=line.find("sd");
            depth=stoi(line.substr(index+3));
            cout<<"debug depth: "<<depth<<endl;
            continue;
        }
        if(command=="st"){
            index=line.find("st");
            move_time=stoi(line.substr(index+3));
            cout<<"debug movetime: "<<move_time<<endl;
            continue;
        }

        if(command=="time"){
            index=line.find("time");
            move_time=stoi(line.substr(index+5));
            time*=10;
            cout<<"debug time: "<<time<<endl;
            continue;
        }

        if(command=="level"){
            sec=0;
            move_time=-1;
            const char * line_p=line.c_str();

            if(sscanf(line_p, "level %d %d %d", &mps, &time_left, &inc) != 3){
                sscanf(line_p, "level %d %d:%d %d", &mps, &time_left,&sec, &inc);
                cout<<"debug level without: "<<endl;
            }
            else{
                cout<<"debug level without: "<<endl;
            }
            time_left *= 60000;
			time_left += sec * 1000;
			moves_togo[0] = moves_togo[1] = 30;
			if(mps != 0){
				moves_togo[0] = moves_togo[1] = mps;
			}
			time = -1;
            cout<<"debug level timeleft: "<<time_left<<" movestogo: "<<moves_togo[0]<<
                  " inc: "<<inc<<" mps: "<<mps<<endl;
            continue;
        }

        // gui will send ping 3 we will reply pong 3 (use for sync)
        if(command=="ping"){
            string s=line.substr(4);
            cout<<"pong"<<s<<endl;
            continue;
        }
        if(command=="new"){
            engine_side=black;
            parse_fen(start_fen,pos);
            depth=-1;
            time-=1;
            continue;
        }
        // setboard fen string
        if(command=="setboard"){
            engine_side=both;// in force mode cz side both set kr di
            string s=line.substr(9);
            const char * fen=s.c_str();
            parse_fen(fen,pos);
            continue;
        }
        if(command=="go"){
            engine_side=pos->side;// set to curr side 
            continue;// now we will start thinking looop me dubara and  think vale if me
        }
        if(command=="usermove"){
            moves_togo[pos->side]--;// decrement for the side to move  before making this move

            string s=line.substr(9);
            const char * fen=s.c_str();
            move=parse_move(fen,pos);

            if(move==nomove)
                continue;
            make_move(pos,move);
            pos->ply=0;
        }
    }
}

void console_loop(s_board* pos, s_search_info *info){

    cout<<"welcome to Vice in console mode!"<<endl;
    cout<<"type help for commands"<<endl;

    info->game_mode=console_mode;
    info->post_thinking=true;

    int depth=maxdepth,move_time=3000;
    int engine_side=both;
    int move=nomove;

    string line,command;
    engine_side=black;
    parse_fen(start_fen,pos);

    size_t index=0;

    while(1){
        if(pos->side==engine_side && check_result(pos)==false){
            // think cz in this protocol we are not totally dependent on gui
            info->start_time=get_time_ms();
            info->depth=depth;

            if(move_time!=0){
                info->time_set=true;
                info->stop_time=info->start_time+move_time;
            }
            search_pos(pos,info);
        }
        cout<<"Vice >"<<endl;
        
        cout<<"enter line"<<endl;
        getline(cin,line);
        // command ko 1st word dena h line ka
        index=line.find(' ');//1st space find
        command=line.substr(0,index);

        if(command=="help"){
            cout<<"Commands:"<<endl;
            cout<<"quit - quit game"<<endl;
            cout<<"force - computer will not think"<<endl;
            cout<<"print - show board"<<endl;
            cout<<"post - show thinking"<<endl;
            cout<<"nopost - do not show thinking"<<endl;
            cout<<"new - start new game"<<endl;
            cout<<"go - set computer thinking"<<endl;
            cout<<"depth x - set depth to x"<<endl;
            cout<<"time x - set thinking time to x seconds (depth still applies if set)"<<endl;
            cout<<"view - show current depth and movetime settings"<<endl;
            cout<<"setboard x - set position to fen x"<<endl;
            cout<<"** note ** - to reset time and depth, set to 0"<<endl;
            cout<<"enter moves using b7b8q notation"<<endl;
            continue;
        }

        if(command=="quit"){
            info->quit=true;
            break;
        }
        if(command=="post"){
            info->post_thinking=true;
            continue;
        }
        if(command=="print"){
            print_board(pos);
            continue;
        }
        if(command=="nopost"){
            info->post_thinking=false;
            continue;
        }
        if(command=="force"){
            engine_side=both;
            continue;
        }
        if(command=="view"){
            if(depth==maxdepth)
                cout<<"depth not set"<<endl;
            else
                cout<<"depth: "<<depth<<endl;
            if(move_time!=0)
                cout<<"movetime: "<<move_time/1000<<"s"<<endl;
            else
                cout<<"movetime not set"<<endl;
            continue;
        }
        if(command=="depth"){
            index=line.find("depth");
            depth=stoi(line.substr(index+6));
            if(depth==0){
                depth=maxdepth;
            }
            continue;
        }
        if(command=="time"){
            index=line.find("time");
            move_time=stoi(line.substr(index+5));
            move_time*=1000;
            continue;
        }
        if(command=="new"){
            engine_side=black;
            parse_fen(start_fen,pos);
            continue;
        }
        if(command=="go"){
            engine_side=pos->side;
            continue;
        }
        
        move=parse_move(line,pos);
        if(move==nomove){
            cout<<"command unknown: "<<line<<endl;
            continue;
        }
        make_move(pos,move);
        pos->ply=0;
    }
}