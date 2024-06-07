all:
	g++ -std=c++20 vice.cpp init.cpp bitboard.cpp hashkey.cpp board.cpp data.cpp attack.cpp validate.cpp io.cpp movegen.cpp -o vice