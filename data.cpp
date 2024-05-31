#include<iostream>
#include"def.hpp"
#include<vector>

using namespace std;

string  piece_char=".PNBRQKpnbrqk";// dot for empty
string  side_char="wb-";
string  rank_char="12345678";
string  file_char="abcdefgh";


vector<bool> piece_big{false,false,true,true,true,true,true,false,true,true,true,true,true};

// jo piece big h 12 me se vo true,  jo piece major h vo true,mnor vale true
// eg for big piece
// empty false, white pawwn is big this is false like this
// knight,bishop,rook king queen  are big true h

vector<bool>piece_major{false,false,false,false,true,true,true,false,false,false,true,true,true};
vector<bool>piece_minor{false,false,true,true,false,false,false,false,true,true,false,false,false};
vector<int>piece_value{0,100,325,325,550,1000,50000,100,325,325,550,1000,50000};
// pawn worth 1 point ,bishop,knight 3 point, rook 5 point , queen 9 point
//but in computer chess we quote values in hundreds  so king 50000  (doesnt matter cz it will never get captured)  
//pawn 100 3 se 3.25 shi rhta h to 325  900 ko 1000  like this
vector<int>piece_color{both,white,white,white,white,white,white,black,black,black,black,black,black};


vector<bool>piece_pawn{ false, true, false, false, false, false, false, true, false, false, false, false, false };
	
vector<bool>piece_knight{ false, false, true, false, false, false, false, false, true, false, false, false, false };
vector<bool>piece_king{ false, false, false, false, false, false, true, false, false, false, false, false, true };
vector<bool>piece_rook_queen{ false, false, false, false, true, true, false, false, false, false, true, true, false };
vector<bool>piece_bishop_queen{ false, false, false, true, false, true, false, false, false, true, false, true, false };
vector<bool>piece_slide{ false, false, false, true, true, true, false, false, false, true, true, true, false };
// it is true where th epiece is sliding piece like queen rook bishop




vector<int>mirror64{
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};