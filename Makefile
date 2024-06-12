all:
	g++ -std=c++20 vice.cpp init.cpp bitboard.cpp hashkey.cpp board.cpp data.cpp \
	attack.cpp validate.cpp io.cpp movegen.cpp makemove.cpp perft.cpp search.cpp \
	misc.cpp -o vice