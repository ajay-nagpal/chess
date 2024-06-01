#include"def.hpp"

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

// assert me use krnge in ufun oko assert(sq_on_board(jis sq kko hme dekhna h ki vo board pr h ya n hi let sq))
// ye 1,0 return kregea assert ko aur assert us hisab se fail hoga