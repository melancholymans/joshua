#include <memory>
#include "bitboard.h"
#include "move.h"
#include "usi.h"
#include "movegen.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using Movens::string_from_move;


//TESTのためのヘルパー関数
bool array_check(Move anser, MoveStack *m)
{
	for (int i=0; i < 256; i++){
		if (m++->move == anser){
			return true;
		}
	}
	return false;
}

TEST(movegen, movegen_all)
{
	//問題図は自作
	using namespace MoveGeneratens;
	string ss("4k4/4gP3/pNbp1pB2/4R1ppL/1P5n1/lrP1P1PlP/2p1+nS1P1/5+s2K/7NL b G4P2g2sp 1");
	Position pos(ss);
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;
	Move ans;

	//NonCapture
	ml = generate_moves<NonCapture>(ml, pos);
	ans = make_move(I4, I5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G4, G5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F8, F9, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E4 ,E5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C4, C5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B5, B6, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	
	ans = make_move(I6, I7, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I8, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I9, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));	
	//Capture
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化

	ml = generate_moves<Capture>(ml, pos);
	ans = make_move(H3, H4, 0, Pawn, Lance);
	EXPECT_TRUE(array_check(ans, ms));
	//CapturePlusPro
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化

	ml = generate_moves<CapturePlusPro>(ml, pos);
	ans = make_move(F8, F9, 1, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H3, H4, 0, Pawn, Lance);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(I6, I7, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I8, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I9, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	//NonCaptureMinusPro
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化

	ml = generate_moves<NonCaptureMinusPro>(ml, pos);
	ans = make_move(I4, I5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G4, G5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E4, E5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C4, C5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B5, B6, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
}


