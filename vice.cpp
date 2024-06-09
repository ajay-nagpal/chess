#include<iostream>
#include"def.hpp"

#include"cstdlib"
#include<vector>

#include <iomanip>// for %5d soln

using namespace std;

// wiki fen page other 3 fen as example
#define fen1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"

//#define fen1 "8/3q4/8/8/4Q3/8/8/8 w - - 0 2 "
#define fen2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define fen3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define fen4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

#define pawnmoves_w "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define pawnmoves_b "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b - e3 0 1"

#define knight_king_b "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define knight_king_w "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

#define rook "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define queen "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1"
#define bishop "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"

#define castle1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define castle2 "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"

#define castle3 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

#define perftfen "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main(){
    AllInit();
    //return 0;
  
}



