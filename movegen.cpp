﻿#include <memory>
#include "bitboard.h"
#include "move.h"
#include "usi.h"
#include "movegen.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using Movens::string_from_move;
/*
NonCapture,			//駒を取らない手
Capture,			//駒を取る手
CapturePlusPro,		//Capture+歩香桂飛角（金銀王以外）で取る手＋取らずに成る手(aperyはここのコメントとプログラムの定義が異なる）
NonCaptureMinusPro,	//NonCapture-歩香桂飛角を取らない成る手-香の３段目への駒をとらない不成(移動不可の打ち駒は禁じ手ではあるがそれ以上移動不可能な進む手は禁じ手と明示してある資料がない）
*/

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
TEST(movegen, movegen_all_white)
{
	//問題図は自作
	using namespace MoveGeneratens;
	string ss("4k4/4gP3/pNbp1pB2/4R1ppL/1P5n1/lrP1P1PlP/2p1+nS1P1/5+s2K/7NL w G4P2g2sp 1");
	Position pos(ss);
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;
	Move ans;
	//NonCapture ALL=false
	ml = generate_moves<NonCapture>(ml, pos);
	ans = make_move(G6, G5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F7, F6, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(D7, D6, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C3, C2, 1, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(A7, A6, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(A4, A3, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(A4, A3, 0, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(A4, A2, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(A4, A2, 0, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(A4, A1, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H5, I3, 1, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H5, I3, 0, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H5, G3, 1, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H5, G3, 0, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E8, E7, 0, Gold, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E8, D8, 0, Gold, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E3, E2, 0, ProNight, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E3, D3, 0, ProNight, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E3, D2, 0, ProNight, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F2, G2, 0, ProSilver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F2, G1, 0, ProSilver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F2, F1, 0, ProSilver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F2, E1, 0, ProSilver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F2, E2, 0, ProSilver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, D6, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, E5, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, F4, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, G3, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, H2, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, B6, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, A5, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, D8, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, B8, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, A9, 0, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B4, B3, 1, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B4, B2, 1, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B4, B1, 1, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E9, D8, 0, King, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E9, D9, 0, King, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E9, F9, 0, King, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	//Capture ALL=false
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化
	ml = generate_moves<Capture>(ml, pos);
	ans = make_move(H4, H3, 1, Lance, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H4, H3, 0, Lance, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E8, F8, 0, Gold, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E3, E4, 0, ProNight, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E3, F3, 0, ProNight, Silver);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F2, F3, 0, ProSilver, Silver);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C7, I1, 1, Bishop, Lance);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B4, C4, 0, Rook, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B4, B5, 0, Rook, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E9, F8, 0, King, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	//Drop
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化
	ml = generate_moves<Drop>(ml, pos);
	ans = make_move(Square(Pawn + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), I5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), E1, 0, PieceType(0), EmptyPiece);
	EXPECT_FALSE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), E2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), E5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), E7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), B3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), B6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), B8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), B9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), I5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), H2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), G1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), G2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), G3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), G5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), G8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), G9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), F1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), F4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), F5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), F6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), F9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), D1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), D9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), C1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), C5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), C6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), C8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), C9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), B1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), B3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), B6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), B8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), B9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Silver + SquareNum - 1), A1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), A2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Silver + SquareNum - 1), A9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), H2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), G1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), F1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), D1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), C1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), B1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), A1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
}
TEST(movegen, movegen_all_black)
{
	//問題図は自作
	using namespace MoveGeneratens;
	string ss("4k4/4gP3/pNbp1pB2/4R1ppL/1P5n1/lrP1P1PlP/2p1+nS1P1/5+s2K/7NL b G4P2g2sp 1");
	Position pos(ss);
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;
	Move ans;

	//TODO:全ての手生成がテストできていない。全ての手が生成できる局面ではないため。別の局面を用意すること
	//Lance,Nightの打つ手、silverの打つ手など
	//NonCapture ALL=false
	ml = generate_moves<NonCapture>(ml, pos);
	ans = make_move(I4, I5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G4, G5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F8, F9, 1, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E4 ,E5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(C4, C5, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B5, B6, 0, Pawn, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I7, 0, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I7, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I8, 0, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I8, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I6, I9, 1, Lance, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));	
	ans = make_move(H1, G3, 0, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(H1, I3, 0, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B7, A9, 1, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(B7, C9, 1, Night, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F3, F4, 0, Silver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F3, E2, 0, Silver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(F3, G2, 0, Silver, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, H8, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, I9, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, F6, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, E5, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, D4, 1, Bishop, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, F6, 0, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, D6, 0, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, C6, 0, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, B6, 0, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, A6, 0, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, E7, 1, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, E5, 0, Rook, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I2, I3, 0, King, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(I2, H2, 0, King, EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	//Capture ALL=false
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化
	
	ml = generate_moves<Capture>(ml, pos);
	ans = make_move(H3, H4, 0, Pawn, Lance);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, H6, 1, Bishop, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(G7, C3, 1, Bishop, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, E8, 1, Rook, Gold);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(E6, G6, 0, Rook, Pawn);
	EXPECT_TRUE(array_check(ans, ms));
	//Drop
	memset(ms, 0, sizeof(ms));
	ml = ms;	//初期化
	
	ml = generate_moves<Drop>(ml, pos);
	ans = make_move(Square(Pawn + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), A2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), A1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Pawn + SquareNum - 1), D1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));

	ans = make_move(Square(Gold + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), H2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), G1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), F1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), E7, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), E5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), E2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), E1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D4, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), D1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), C1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), B1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A9, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A2, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
	ans = make_move(Square(Gold + SquareNum - 1), A1, 0, PieceType(0), EmptyPiece);
	EXPECT_TRUE(array_check(ans, ms));
}


