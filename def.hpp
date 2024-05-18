#ifndef def_h
#define def_h

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

#define name "vice 1.0"

#define board_sq_num 120
#define maxmoves 2048// half moves

#define max_position_on_moves 256

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

class s_undo{
    int moves,castle_perm,enpass,fiftymove;
    u64 poskey;// pos key of the move that was played

    public:

    int get_moves(){return moves;}
    int get_castle_perm(){return castle_perm;}
    int get_enpass(){return enpass;}
    int get_fiftymove(){return fiftymove;}
    u64 get_poskey(){return poskey;}

    void set_moves(int mv){moves=mv;}
    void set_castle_perm(int cp){castle_perm=cp;}
    void set_enpass(int en){ enpass=en;}
    void set_fiftymove(int fm){fiftymove=fm;}
    void set_poskey(int ps){poskey=ps;}

};

class s_board {
    
    vector<int>pieces=vector<int>(board_sq_num);
    vector<u64>pawns=vector<u64>(3);
    vector<int>king_sq=vector<int>(2);
    
    int side,enpass,fiftymove,ply,hisply,castle_perm;
    u64 poskey;
    
    vector<int>piece_num=vector<int>(13);

    vector<int>big_pieces=vector<int>(3);
    vector<int>major_pieces=vector<int>(3);
    vector<int>minor_pieces=vector<int>(3);
    vector<int>material=vector<int>(3);

    vector<s_undo>history=vector<s_undo>(maxmoves);
   
    vector<vector<int>>piece_list=vector<vector<int>>(13,vector<int>(10));
    // to search which color piece is on board or not faster method no need to loop complete 120 board

    public:

    int get_side(){return side;}
    int get_enpass(){return enpass;}
    int get_fiftymove(){return fiftymove;}
    int get_ply(){return ply;}
    int get_hisply(){return hisply;}
    int get_castle_perm(){return castle_perm;}
    u64 get_poskey(){return poskey;}

    void set_side(int sd){side=sd;}
    void set_enpass(int en){enpass=en;}
    void set_fiftymove(int fm){fiftymove=fm;}
    void set_ply(int pl){ply=pl;}
    void set_hisply(int hp){hisply=hp;}
    void set_castle_perm(int cp){castle_perm=cp;}
    void set_poskey(int ps){poskey=ps;}

};

class s_move{
    int move;
    int score;

    public:

    int get_move(){return move;}
    int get_score(){return score;}

    void set_move(int mv){move=mv;}
    void set_score(int sc){score=sc;}

};

class s_move_list{
    int count;

    public:

    vector<s_move>moves=vector<s_move>(max_position_on_moves);
    int get_count(){return count;}
    void set_count(int ct){count=ct;}
};

#define from_sq(m)    ((m) & 0x7F )
#define to_sq(m)    (((m)>>7) & 0x7F )

#define captured(m)    (((m)>>14) & 0xF )
#define promoted(m)    (((m)>>20) & 0xF )

#define move_flag_ep 0x40000
#define move_flag_ps 0x80000
#define move_flag_cs 0x1000000

#define move_flag_cap 0x7C000
#define move_flag_prom 0xF00000

#define fr_to_sq(f,r)((21+(f)) + ((r)*10))

#define SQ64(sq120) (sq120to64[(sq120)])
#define SQ120(sq64) (sq64to120[(sq64)])

#define pop(b)  popbit(b)
#define cnt(b)  countbit(b)

#define setbit(bb,sq)  ((bb) |=setmask[(sq)])
#define clearbit(bb,sq)((bb) &=clearmask[(sq)])

#define is_bq(p) (piece_bishop_queen[(p)])
#define is_rq(p) (piece_rook_queen[(p)])
#define is_kn(p) (piece_knight[(p)])
#define is_ki(p) (piece_king[(p)])

extern vector<int> sq120to64;
extern vector<int> sq64to120;

extern vector<u64> setmask;
extern vector<u64> clearmask;

extern vector<vector<u64>> piece_keys;

extern vector<vector<unsigned long long >> piece_keys2;
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
//extern void print_move_list(const s_movelist* list);

// movegen.cpp
//extern void generate_all_moves(const s_board * pos,s_movelist * list);

//validate.cpp
extern int  sq_on_board(const int sq);
extern int side_valid(const int side);
extern int file_rank_valid(const int fr);
extern int piece_valid_empty(const int piece);
extern int piece_valid(const int piece);

//makemove.cpp
extern int make_move(s_board * pos,int move);
extern void take_move(s_board *pos);

//perft.cpp
extern void perft_test(int depth, s_board *pos);

#endif