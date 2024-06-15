#ifndef def_hpp
#define def_hpp

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)){\
    cout<<#n<<" - failed ";\
    cout<<" on "<<__DATE__;\
    cout<<" at "<<__TIME__;\
    cout<<" in file "<<__FILE__;\
    cout<<" at line "<<__LINE__;\
    exit(1);\
}
#endif

typedef unsigned long long u64;

#define NAME "vice 1.0" // __Apple__ me koi name already h// for time in ms ke liye use

#define board_sq_num 120
#define maxmoves 2048// half moves

#define maxdepth 64

#define max_position_on_moves 256// 1 given pos pr max move itne ho skte h

#define start_fen "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum piece{emptyy,wp,wn,wb,wr,wq,wk,bp,bn,bb,br,bq,bk};
enum file{fa,fb,fc,fd,fe,ff,fg,fh,fn};
enum rank{r1,r2,r3,r4,r5,r6,r7,r8,rn};
enum color{white,black,both};

enum board120{
    a1=21,b1,c1,d1,e1,f1,g1,h1,
    a2=31,b2,c2,d2,e2,f2,g2,h2,
    a3=41,b3,c3,d3,e3,f3,g3,h3,
    a4=51,b4,c4,d4,e4,f4,g4,h4,
    a5=61,b5,c5,d5,e5,f5,g5,h5,
    a6=71,b6,c6,d6,e6,f6,g6,h6,
    a7=81,b7,c7,d7,e7,f7,g7,h7,
    a8=91,b8,c8,d8,e8,f8,g8,h8,no_sq,OFFBOARD
};

enum castle{wkca=1,wqca=2,bkca=4,bqca=8};


class s_move{
    public:

    int move;
    int score;
};

class s_movelist{
    public:
    
    int count;//count of the number of move on the movelist
    vector<s_move>moves=vector<s_move>(max_position_on_moves);
};

class s_pventry{// if we find move that beat alpha then we store move and poskey that led to that move
    public:

    u64 poskey;
    int move;
};

class s_pvtable{// actual class for the table 
    public:

    s_pventry * ptable;// we will allocate momeory to this pointer depending on how many entries we want to store
    int num_entries;
};

class s_undo{
    public:

    int moves,castle_perm,enpass,fiftymove;
    u64 poskey;
};

class s_search_info{
    public:

    // time westart searching,depth set means we only search to certain depth acc to protocol
    //
    int start_time=0,stop_time=0,depth=0,depth_set=0,time_set=0;
    int moves_to_go=0,infinite=0;
    
    long nodes;// count of the pos that engine visit in search tree
    int quit=0,stopped=0;
    // get intrupted during a search and gui send us a quit then quit will set to true 
    // and we will do all cleanup and exit right inside the search and close the program

    //if stop is sent then will stop searching and  stop will set true and simplely recursively
    // back out of search wihtout updating any score or something   and let these value maintain its best from 
    // previous iteration
};

class s_board {
    public:

    int side,enpass,fiftymove,ply,hisply,castle_perm;
    u64 poskey;
    
    vector<int>pieces=vector<int>(board_sq_num);
    vector<u64>pawns=vector<u64>(3);
    vector<int>king_sq=vector<int>(2);

    vector<int>piece_num=vector<int>(13);

    vector<int>big_pieces=vector<int>(2);
    vector<int>major_pieces=vector<int>(2);
    vector<int>minor_pieces=vector<int>(2);
    vector<int>material=vector<int>(2);

    vector<s_undo>history=vector<s_undo>(maxmoves);
   
    vector<vector<int>>piece_list=vector<vector<int>>(13,vector<int>(10));

    s_pvtable pvtable[1];
    //vector<s_pvtable>pvtable=vector<s_pvtable>(1);
    vector<int>pvarray=vector<int>(maxdepth);
    //fill the pv array upto maxdepth with the moves from the table 
    // jo move alpha ko beat kre vo to hona hi chihiye isme 
    // best line in the tree other line bhi hogi store  way through the tree
    //getpvline fun se()  pvtable.cpp ki
    // we will get our arrray out of pvtable into pvarray before printing


    //history heuristic related to search
    vector<vector<int>>search_history=vector<vector<int>>(13,vector<int>(board_sq_num));
    // this basically set values 0 if move beat alpha
   // when a piece move beat alpha  for that piece type  its to sq we will increment this array by 1
    vector<vector<int>>search_killer=vector<vector<int>>(2,vector<int>(maxdepth));
    // it stores 2 moves that recently caused beta cutoff
    // non captyre moves ke liye h ye 2 array bs
    // we will use these 2 array for move ordering in alpha beta
};

#define from_sq(m)    ((m) & 0x7F )
#define to_sq(m)    (((m)>>7) & 0x7F )

#define captured(m)    (((m)>>14) & 0xF )// to check what piece get captured in our move
//simply put it in captured which will >> by 14 and & with 0xF  to tell what is get captured in the move
#define promoted(m)    (((m)>>20) & 0xF )

#define move_flag_ep 0x40000
#define move_flag_ps 0x80000
#define move_flag_cs 0x1000000

#define move_flag_cap 0x7C000  // to check is something get captured in our move
//ep + cap ka   ep 100  vala us 7 * 0000 val ebox ka 
//cap 0011 1100  to total 111 1100   which is 7 and 12  7 C  bahce sare 3  0000  to 000
#define move_flag_prom 0xF00000// to check wheather anything was promoted during move

#define nomove 0

#define fr_to_sq(f,r) ((21+(f)) + ((r)*10))

#define SQ64(sq120) (sq120to64[(sq120)])
#define SQ120(sq64) (sq64to120[(sq64)])

#define pop(b)  popbit(b)
#define cnt(b)  countbit(b)

#define setbit(bb,sq)   ((bb) |=setmask[(sq)])
#define clearbit(bb,sq) ((bb) &=clearmask[(sq)])

#define is_bq(p) (piece_bishop_queen[(p)])
#define is_rq(p) (piece_rook_queen[(p)])
#define is_kn(p) (piece_knight[(p)])
#define is_ki(p) (piece_king[(p)])

extern vector<int> sq120to64;
extern vector<int> sq64to120;

extern vector<u64> setmask;// 64 size but not used here
extern vector<u64> clearmask;

extern vector<vector<u64>> piece_keys;
extern u64 side_key;
extern vector<u64> castle_keys;

extern string piece_char;
extern string side_char;
extern string rank_char;
extern string file_char;

extern vector<bool> piece_big;
extern vector<bool> piece_major;
extern vector<bool> piece_minor;
extern vector<int> piece_value;
extern vector<int> piece_color;

extern vector<bool> piece_pawn;

extern vector<int> file_board;
extern vector<int> rank_board;

extern vector<bool> piece_knight;
extern vector<bool> piece_king;
extern vector<bool> piece_rook_queen;
extern vector<bool> piece_bishop_queen;
extern vector<bool> piece_slide;

//init.cpp
extern void AllInit();
extern void init_file_rank_board();

//bitboard.cpp
extern void print_bit_board(u64 bb);
extern int popbit(u64 *bb);
extern int countbit(u64 b);

// hashkey.cpp
extern u64 generate_pos_key(const s_board *pos);

//board.cpp
extern void reset_board(s_board *pos);
extern int parse_fen(const char * fen ,s_board * pos);
extern void print_board(const s_board *pos);
extern void update_list_material(s_board *pos);
extern int check_board(const s_board *pos);

// attack.cpp
extern int square_attacked(const int sq,const int side, const s_board *pos);

//io.cpp
extern char * print_move(const int move);
extern char * print_sq(const int sq);
extern void print_move_list(const s_movelist* list);
extern int parse_move(string ptr_char,s_board* pos);

// movegen.cpp
extern void generate_all_moves(const s_board * pos,s_movelist * list);
extern int move_exist(s_board * pos,const int move);

//validate.cpp
extern int sq_on_board(const int sq);
extern int side_valid(const int side);
extern int file_rank_valid(const int fr);
extern int piece_valid_empty(const int piece);
extern int piece_valid(const int piece);

//makemove.cpp
extern int make_move(s_board * pos,int move);
extern void take_move(s_board *pos);

//perft.cpp
extern void perft_test(int depth, s_board *pos);

//search.cpp
extern void search_pos(s_board * pos,s_search_info * info);

//misc.cpp
extern int get_time_ms();

//pvtable.cpp
extern void init_pvtable(s_pvtable * table);
//extern void clear_pvtable(s_pvtable * table);
extern int probe_pvtable(const s_board* pos);
extern void store_pvmove(const s_board* pos, const int move);
extern int get_pvline(const int depth,s_board* pos);

//evaluate.cpp
extern int eval_pos(const s_board * pos);

#endif