#include"def.hpp"
#include <fstream>
#include<iostream>
using namespace std;

int movelist_ok(const s_movelist *list,  const s_board *pos) {
	if(list->count < 0 || list->count >= max_pos_moves) {
		return false;
	}

	int move_num;
	int from = 0;
	int to = 0;
	for(move_num = 0; move_num < list->count; ++move_num) {
		to = to_sq(list->moves[move_num].move);
		from = from_sq(list->moves[move_num].move);
		if(!sq_on_board(to) || !sq_on_board(from)) {
			return false;
		}
		if(!piece_valid(pos->pieces[from])) {
			print_board(pos);
			return false;
		}
	}
	return true;
}

int is_sq_120(const int sq) {
	return (sq>=0 && sq<120);
}

int piece_valid_empty_off(const int pce) {
	return (piece_valid_empty(pce) || pce == off_board);
}

int  sq_on_board(const int sq){
    return file_board[sq]==off_board?0:1;
}

int side_valid(const int side){
    return (side==white|| side ==black)?1:0;
}

int file_rank_valid(const int fr){
    return (fr>=0 && fr<=7)?1:0;// fr>=0 means file fa >=0 and rank >=1  and so on
}

int piece_valid_empty(const int piece){
    return (piece>=emptyy && piece<=bk)?1:0;
}

int piece_valid(const int piece){
    return (piece>=wp && piece<=bk)?1:0;
}

void mirror_eval_test(s_board *pos) {
    ifstream file("mirror.epd");

    string line;
    int ev1 =0 ,ev2 = 0;
    int positions = 0;

    if(!file.is_open()){
        cout<< "file not found" << endl;
        return;
    }  
    else {
        while(getline(file,line)){
            const char * s=line.c_str();
            parse_fen(s, pos);
            positions++;
            ev1 = eval_pos(pos);
            mirror_board(pos);
            ev2 = eval_pos(pos);

            if(ev1 != ev2) {
                cout<<endl;
                parse_fen(s, pos);
                
                print_board(pos);
                mirror_board(pos);
                print_board(pos);

                cout<<endl<<"mirror fail:"<<line<<endl;

                cin.get();
                return;
            }
            if( (positions % 3) == 0)   {//1000
                cout<<"position "<<positions<<endl;
            }
        }
    }
}

// void debug_analysis_test(s_board *pos, s_search_info *info) {

// 	ifstream file("lct2.epd");
//     string line;

// 	info->depth = maxdepth;
// 	info->time_set = true;
// 	int time = 1140000;


//     if(!file.is_open()) {
//          cout<< "file not found" << endl;
//         return;
//     }  
//     else {
//         while(getline(file,line)){

// 			info->start_time = get_time_ms();
// 			info->stop_time = info->start_time + time;

// 			clear_hash_table(pos->hash_table);

//             const char * fen=line.c_str();
//             parse_fen(fen, pos);

//             cout<<line<<endl;
// 			cout<<"time: "<<time<<" start: "<<info->start_time<< " stop: "<<info->stop_time<<
//                   " depth: "<<info->depth<< "timeset: "<<info->time_set<<endl;
// 			search_pos(pos, info);
//         }
//     }
// }

