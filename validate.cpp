#include"def.hpp"
#include <fstream>
#include<iostream>
using namespace std;

int  sq_on_board(const int sq){
    return file_board[sq]==OFFBOARD?0:1;
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
            if( (positions % 1000) == 0)   {
                cout<<"position "<<positions<<endl;
            }
        }
    }
}
