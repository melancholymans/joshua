#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "misc.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

/*
WHILE(後手)								BLACK(先手)
			+--+--+--+--+--+--+--+--+--+
FILEI		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
FILEH		|H9|H8|H7|H6|H5|H4|H3|H2|H1|
			|09|10|11|12|13|14|15|16|17|
			+--+--+--+--+--+--+--+--+--+
FILEG		|G9|G8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
FILEF		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
FILEE		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
FILED		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
FILEC		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+  ↑ BitBoard.p_[0]	63座標
FILEB		|I9|I8|I7|I6|I5|I4|I3|I2|I1|  ↓ BitBoard.p_[1]	18座標
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
FILEA		|I9|I8|I7|I6|I5|I4|I3|I2|I1|
			|00|01|02|03|04|05|06|07|08|
			+--+--+--+--+--+--+--+--+--+
	RANK	  9  8  7  6  5  4  3  2  1

	将棋盤を真横から見ているような配置にしてある、将棋は先手側から、後手側から先手側に移動する手が多いので
	このような配置にしてある、と思う。
*/
const BitBoard SquareBB[SquareNum] = {		//bitboard index		board square
	BitBoard(1ULL << 0, 0),					//0						I9			
	BitBoard(1ULL << 1, 0),					//1						I8
	BitBoard(1ULL << 2, 0),					//2						I7
	BitBoard(1ULL << 3, 0),					//3						I6
	BitBoard(1ULL << 4, 0),					//4						I5
	BitBoard(1ULL << 5, 0),					//5						I4
	BitBoard(1ULL << 6, 0),					//6						I3
	BitBoard(1ULL << 7, 0),					//7						I2
	BitBoard(1ULL << 8, 0),					//8						I1
	BitBoard(1ULL << 9, 0),					//9						H9
	BitBoard(1ULL << 10, 0),				//10					H8
	BitBoard(1ULL << 11, 0),				//11					H7
	BitBoard(1ULL << 12, 0),				//12					H6
	BitBoard(1ULL << 13, 0),				//13					H5
	BitBoard(1ULL << 14, 0),				//14					H4
	BitBoard(1ULL << 15, 0),				//15					H3
	BitBoard(1ULL << 16, 0),				//16					H2
	BitBoard(1ULL << 17, 0),				//17					H1
	BitBoard(1ULL << 18, 0),				//18					G9
	BitBoard(1ULL << 19, 0),				//19					G8
	BitBoard(1ULL << 20, 0),				//20					G7
	BitBoard(1ULL << 21, 0),				//21					G6
	BitBoard(1ULL << 22, 0),				//22					G5
	BitBoard(1ULL << 23, 0),				//23					G4
	BitBoard(1ULL << 24, 0),				//24					G3
	BitBoard(1ULL << 25, 0),				//25					G2
	BitBoard(1ULL << 26, 0),				//26					G1
	BitBoard(1ULL << 27, 0),				//27					F9
	BitBoard(1ULL << 28, 0),				//28					F8
	BitBoard(1ULL << 29, 0),				//29					F7
	BitBoard(1ULL << 30, 0),				//30					F6
	BitBoard(1ULL << 31, 0),				//31					F5
	BitBoard(1ULL << 32, 0),				//32					F4
	BitBoard(1ULL << 33, 0),				//33					F3
	BitBoard(1ULL << 34, 0),				//34					F2
	BitBoard(1ULL << 35, 0),				//35					F1
	BitBoard(1ULL << 36, 0),				//36					E9
	BitBoard(1ULL << 37, 0),				//37					E8
	BitBoard(1ULL << 38, 0),				//38					E7
	BitBoard(1ULL << 39, 0),				//39					E6
	BitBoard(1ULL << 40, 0),				//40					E5
	BitBoard(1ULL << 41, 0),				//41					E4
	BitBoard(1ULL << 42, 0),				//42					E3
	BitBoard(1ULL << 43, 0),				//43					E2
	BitBoard(1ULL << 44, 0),				//44					E1
	BitBoard(1ULL << 45, 0),				//45					D9
	BitBoard(1ULL << 46, 0),				//46					D8
	BitBoard(1ULL << 47, 0),				//47					D7
	BitBoard(1ULL << 48, 0),				//48					D6
	BitBoard(1ULL << 49, 0),				//49					D5
	BitBoard(1ULL << 50, 0),				//50					D4
	BitBoard(1ULL << 51, 0),				//51					D3
	BitBoard(1ULL << 52, 0),				//52					D2
	BitBoard(1ULL << 53, 0),				//53					D1
	BitBoard(1ULL << 54, 0),				//54					C9
	BitBoard(1ULL << 55, 0),				//55					C8
	BitBoard(1ULL << 56, 0),				//56					C7
	BitBoard(1ULL << 57, 0),				//57					C6
	BitBoard(1ULL << 58, 0),				//58					C5
	BitBoard(1ULL << 59, 0),				//59					C4
	BitBoard(1ULL << 60, 0),				//60					C3
	BitBoard(1ULL << 61, 0),				//61					C2
	BitBoard(1ULL << 62, 0),				//62					C1
	BitBoard(0, 1ULL << 0),					//63					B9
	BitBoard(0, 1ULL << 1),					//64					B8
	BitBoard(0, 1ULL << 2),					//65					B7
	BitBoard(0, 1ULL << 3),					//66					B6
	BitBoard(0, 1ULL << 4),					//67					B5
	BitBoard(0, 1ULL << 5),					//68					B4
	BitBoard(0, 1ULL << 6),					//69					B3
	BitBoard(0, 1ULL << 7),					//70					B2
	BitBoard(0, 1ULL << 8),					//71					B1
	BitBoard(0, 1ULL << 9),					//72					A9
	BitBoard(0, 1ULL << 10),				//73					A8
	BitBoard(0, 1ULL << 11),				//74					A7
	BitBoard(0, 1ULL << 12),				//75					A6
	BitBoard(0, 1ULL << 13),				//76					A5
	BitBoard(0, 1ULL << 14),				//77					A4
	BitBoard(0, 1ULL << 15),				//78					A3
	BitBoard(0, 1ULL << 16),				//79					A2
	BitBoard(0, 1ULL << 17)					//80					A1
};

void BitBoards::init()
{

}

void BitBoards::print(BitBoard bb)
{
	sync_cout;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "   A  B  C  D  E  F  G  H  I " << std::endl;
	for (int r = Rank9; r < RankNum; r++){
		std::cout << (9 - r) << " ";
		for (int f = FileA; FileI <= f; f--){
			std::cout << setw(2) << make_square(f, r) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------------------------------" << sync_endl;
}

#ifdef _DEBUG
TEST(bitboard_case, bitboard)
{
	//pop_count function
	uint64_t b0 = 0xCB87;
	uint64_t b1 = 0x3b;

	BitBoard bb(b0, b1);
	EXPECT_EQ(bb.pop_count(), 14);

}
#endif