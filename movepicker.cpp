#include <array>	//generate_evasions TESTのため
#include "movegen.h"
#include "movepicker.h"
#include "move.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using MoveGeneratens::generate_moves;

MovePicker::MovePicker(const Position& pos, const int depth) :m_pos(pos)
{	
	m_current_move = m_last_move = m_legal_moves;
	//あといろいろ設定してあるがわからない
	if (pos.checker_bb().is_not_zero()){
		m_phase = EvasionSearch;
	}
	else{
		m_phase = MainSearch;
	}
	//killerMoveがあるがパス
	//わからない処理
	//置換表の手の処理らしきもあり、パス
	m_ttmove = MoveNone;
}

//MovePikerは全部で３つコンストラクタがあるがとりあえずは１つだけ作る

void MovePicker::go_next_phase()
{
	m_current_move = m_legal_moves;
	m_phase++;
	switch (m_phase){
	case PhTacticalMove0: case PhTacticalMove1:
		m_last_move = generate_moves<Capture>(m_current_move, m_pos);
		//ここにscroeCaptures関数があるがまだ実装できていないのでPASS
		return;
	case PhKiller:
		//killerの準備ができていないのでreturnするだけ
		return;
	case PhNonTactionMove0:
		m_last_move = generate_moves<NonCapture>(m_current_move, m_pos);
		//scro関係の関数があるが実装していないのでPASS
		m_current_move = m_last_move;
		m_last_move = generate_moves<Drop>(m_current_move, m_pos);
		//scro関係の関数があるが実装していないのでPASS
		m_current_move = m_first_move;
		////std::partionアルゴリズムをつかったなにかがある
		//insertion_sort(curr_move, last_move);
		return;
	case PhNonTactionMove1:
		//わからない操作
		return;
	case PhBadCapture:
		//わからない操作
		return;
	case PhEvasion:case PhQEvasion:
		m_last_move = generate_moves<Evasion>(m_current_move, m_pos);
		//わからない操作
		return;
	case PhQCapture0:
		//わからない操作
		return;
	case PhQCapture1:
		//わからない操作
		return;
	case EvasionSearch:case QSearch:case QEvasionSearch:case QRecapture:case ProCut:
		m_phase = PhStop;
	case PhStop:
		m_last_move = m_current_move + 1;
		return;
	default:
		_ASSERT(false);
	}
}

Move MovePicker::next_move()
{
	MoveStack* ms;
	Move move;

	do{
		while (m_current_move == m_last_move){
			go_next_phase();
		}
		switch (m_phase){
		case MainSearch: case EvasionSearch: case QSearch: case QEvasionSearch: case ProCut:
			m_current_move++;
			return m_ttmove;
		case PhTacticalMove0:
			ms = pick_best(m_current_move++, m_last_move);
			//いろいろと処理があるが実装できていないのでパス
			return ms->move;
			break;
		case PhKiller:
			//実装できていないのでパス
			break;
		case PhNonTactionMove0: case PhNonTactionMove1:
			move = (m_current_move++)->move;
			//いろいろ条件があるがパス
			return move;
			break;
		case PhBadCapture:
			//いろいろあるがいまのところはこれ
			return MoveNone;
		case PhEvasion: case PhQEvasion: case PhQCapture0:
			move = pick_best(m_current_move++, m_last_move)->move;
			return move;
			break;
		case PhTacticalMove1:
			ms = pick_best(m_current_move++, m_last_move);
			//いろいろ処理しているが今はパス
			return ms->move;
			break;
		case PhQCapture1:
			move = pick_best(m_current_move++, m_last_move)->move;
			return move;
		case PhStop:
			return MoveNone;
		default:
			_ASSERT(false);
		}
	} while (true);	
}
#ifdef _DEBUG
//TESTのためのヘルパー関数
static bool array_check(Move anser, MoveStack *m)
{
	for (int i = 0; i < 512; i++){
		if (m++->move == anser){
			return true;
		}
	}
	return false;
}
//ms[512]を直前にゼロクリアすることを利用してどれくらい
//データを積み上げたかカウントアップする。
static int array_count(MoveStack *m)
{
	int count = 0;
	for (int i = 0; i < 512; i++){
		if (m++->move == 0){
			return count;
		}
		count++;
	}
	return count;
}

TEST(movepicker, movepicker_POS39_white)
{
	//問題局面=受けとしのぎNo39
	string ss("1g3l2l/2+B6/k2p5/2pP3p1/1p1g1p1n1/N1s1psPNp/6pP1/R2+n1P1K1/5SG1L b Brgsl6p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m,old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(A4, B6, 0, Night, EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);

	ml->move = make_move(A7, B6, 0, King, Night); ml++;
	ml->move = make_move(Square(Rook + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Rook + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Rook + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Rook + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);  ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 21; i++){
		m = mp.next_move();
		if (m == old_m){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movepicker, movepicker_POS37_black)
{
	//問題局面=受けとしのぎNo37
	string ss("7nl/2g2Gsk1/+P3pPpp1/2pp4P/1p2PpSN1/2PS3P1/1P2+p4/K1b6/LRB5L w GSN3Prgnl 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(Square(Lance + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);


	//move
	ml->move = make_move(A2, B2, 0, King, EmptyPiece); ml++;
	ml->move = make_move(C1, A3, 0, Bishop, EmptyPiece); ml++;
	//drop
	ml->move = make_move(Square(Gold + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), A3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 10; i++){
		m = mp.next_move();
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movepicker, movepicker_POS35_white)
{
	//問題局面=受けとしのぎNo35
	string ss("l3gB1nl/7kp/4PP3/2p2ps1P/3ps1P1+R/pPP2GN2/2N3N2/P2+p+p4/LKG6 b S3Prbgslp 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(I5, H6, 0, Dragon, EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);
	//move
	ml->move = make_move(H8, G9, 0, King, EmptyPiece); ml++;
	ml->move = make_move(G6, H7, 0, Silver, EmptyPiece); ml++;
	//drop
	ml->move = make_move(Square(Rook + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Bishop + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 8; i++){
		m = mp.next_move();
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movepicker, movepicker_POS33_black)
{
	//問題局面=受けとしのぎNo33
	using namespace MoveGeneratens;
	string ss("1n4kn1/4+S1g1G/l3G1p1l/p4psp1/4P3p/Pp1PS1P2/4b4/1PKp1+r3/LN6L w BGSN2Pr4p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(Square(Rook + SquareNum - 1), C4, 0, PieceType(0), EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);
	//move
	ml->move = make_move(C2, D3, 0, King, EmptyPiece); ml++;
	ml->move = make_move(C2, C1, 0, King, EmptyPiece); ml++;
	ml->move = make_move(B1, C3, 0, Night, EmptyPiece); ml++;
	//drop
	ml->move = make_move(Square(Bishop + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 7+1; i++){
		m = mp.next_move();
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS31_white)
{
	//問題局面=受けとしのぎNo31
	string ss("l5k2/4p1l2/4g4/p2p2p+Bp/2P2R1p1/1np+b2P1P/PP1+rP2+n1/2G4P1/LNK1G4 b 2S2Pg2snl2p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(H6, I7, 0, Horse, EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);

	//move
	ml->move = make_move(G9, H9, 0, King, EmptyPiece); ml++;
	ml->move = make_move(D4, H8, 0, Horse, EmptyPiece); ml++;
	//drop
	ml->move = make_move(Square(Gold + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 6; i++){
		m = mp.next_move();
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS29_black)
{
	//問題局面=受けとしのぎNo29
	string ss("4+R1sn1/4G1kp+P/2P2g2l/2p2bpS1/pPsPPp1Pp/3p+r1P2/NS1g2B2/PK7/L1L5L w GNPn2p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(E4, E2, 0, Dragon, EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);
	//move
	ml->move = make_move(B2, B1, 0, King, EmptyPiece); ml++;
	ml->move = make_move(B3, C2, 0, Silver, EmptyPiece); ml++;
	ml->move = make_move(C1, C2, 0, Lance, EmptyPiece); ml++;
	//Drop
	ml->move = make_move(Square(Gold + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), D2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), D2, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 7; i++){
		m = mp.next_move();
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS27_white)
{
	//問題局面=受けとしのぎNo27　
	//本来black局面であるが手番反転してwhite側手番でテストしている
	string ss("l4G1n1/6pkl/6s1p/2p1p4/pp1PbpN2/2Sp2s1P/PPP6/1KGg5/LN2+r3L b RS3Pbgn2p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(Square(Rook + SquareNum - 1), H4, 0, PieceType(0), EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);
	//move
	ml->move = make_move(H8, I9, 0, King, EmptyPiece); ml++;
	ml->move = make_move(G7, H6, 0, Silver, EmptyPiece); ml++;
	ml->move = make_move(G4, H5, 0, Silver, EmptyPiece); ml++;
	//Drop
	ml->move = make_move(Square(Bishop + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Bishop + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Bishop + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), H7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 15; i++){
		m = mp.next_move();
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS25_black)
{
	//問題局面=受けとしのぎNo25　D5->C4竜移動前の局面を現している
	string ss("6snl/+P4gk2/4+P1p2/1P3+B1P1/BN1+rp1Npp/p2P1PP2/4Rp3/N1K2g3/L1s5L w G2SL2Pg3p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(D5, C4, 0, Dragon, EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);

	ml->move = make_move(C2, B1, 0, King, EmptyPiece); ml++;
	ml->move = make_move(C2, D1, 0, King, EmptyPiece); ml++;
	ml->move = make_move(C2, C1, 0, King, Silver); ml++;	//本当は非合法
	ml->move = make_move(A5, C3, 0, Bishop, EmptyPiece); ml++;
	ml->move = make_move(E3, C3, 0, Rook, EmptyPiece); ml++;
	//Drop
	ml->move = make_move(Square(Gold + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 9; i++){
		m = mp.next_move();
		std::cout << Movens::string_from_move_apery_format(m) << std::endl;
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS23_white)
{
	//問題局面=受けとしのぎNo23　91飛車を打つ前の局面を現している
	//本来black局面であるが手番反転してwhite側手番でテストしている
	string ss("l6n1/6+P+L1/2np1p3/p3psS2/1pPPP1P1g/2GS1+B1pk/PKN+r1P3/3+p5/L4+s3 b RGbgnl4p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(Square(Rook + SquareNum - 1), I1, 0, PieceType(0), EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);
	ml->move = make_move(I4, H3, 0, King, EmptyPiece); ml++;
	//drop 
	ml->move = make_move(Square(Bishop + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Bishop + SquareNum - 1), I2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), I2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), I2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), I3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Pawn + SquareNum - 1), I2, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 10; i++){
		m = mp.next_move();
		std::cout << Movens::string_from_move_apery_format(m) << std::endl;
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS21_black)
{
	/*問題局面=受けとしのぎNo21　66角を打つ前の局面を現している*/
	string ss("5+Prnl/4+R4/1p1p2gkp/P1pl1p1p1/1N4pPP/p1P4G1/4P1N2/4G4/K3S3L w BGSNLPb2s3p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	pos.do_move(make_move(Square(Bishop + SquareNum - 1), D4, 0, PieceType(0), EmptyPiece), st_stack[0]);
	MovePicker mp(pos, 1);

	ml->move = make_move(A1, A2, 0, King, EmptyPiece); ml++;
	ml->move = make_move(A1, B1, 0, King, EmptyPiece); ml++;
	ml->move = make_move(A1, B2, 0, King, EmptyPiece); ml++;
	//TODO:ここでは全ても駒に対して、全ての位置に対して合いごまを生成しているが、柿木将棋のように合いごま対策をすること
	ml->move = make_move(Square(Pawn + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Bishop + SquareNum - 1), B2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Lance + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Night + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Silver + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Gold + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move = make_move(Square(Bishop + SquareNum - 1), C3, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 13; i++){
		m = mp.next_move();
		std::cout << Movens::string_from_move_apery_format(m) << std::endl;
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
TEST(movegen, movepicker_POS3_white)
{
	string ss("2+s2g3/1G3g3/2+L1+Np1K1/6p1+n/2GS1+PP1+S/1pPpp4/+PPkPP1+l1+p/1+b1+r+P4/2+P6 w RBSNLPnl2p 1");
	Position pos(ss);
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	Move m, old_m = MoveNone;
	MoveStack ms[512];
	memset(ms, 0, sizeof(ms));
	MoveStack* ml = ms;

	MovePicker mp(pos, 1);
	//NonCapture ALL=false
	//pawn 1
	ml->move = make_move(F7, F6, 0, Pawn, EmptyPiece); ml++;
	ml->move = make_move(F8, G7, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(F8, G8, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(F8, E8, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(F9, G8, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(F9, E8, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(F9, G9, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(F9, E9, 0, Gold, EmptyPiece); ml++;
	ml->move = make_move(I3, I2, 0, ProPawn, EmptyPiece); ml++;
	ml->move = make_move(I3, H2, 0, ProPawn, EmptyPiece); ml++;
	ml->move = make_move(I3, H3, 0, ProPawn, EmptyPiece); ml++;
	ml->move = make_move(I3, I4, 0, ProPawn, EmptyPiece); ml++;
	ml->move = make_move(G3, H3, 0, ProLance, EmptyPiece); ml++;
	ml->move = make_move(G3, H2, 0, ProLance, EmptyPiece); ml++;
	ml->move = make_move(G3, G4, 0, ProLance, EmptyPiece); ml++;
	ml->move = make_move(G3, G2, 0, ProLance, EmptyPiece); ml++;
	ml->move = make_move(G3, F3, 0, ProLance, EmptyPiece); ml++;
	ml->move= make_move(G3, F2, 0, ProLance, EmptyPiece); ml++;
	ml->move= make_move(I6, H5, 0, ProNight, EmptyPiece); ml++;
	ml->move= make_move(I6, I7, 0, ProNight, EmptyPiece); ml++;
	ml->move= make_move(I6, H6, 0, ProNight, EmptyPiece); ml++;
	ml->move= make_move(C9, D9, 0, ProSilver, EmptyPiece); ml++;
	ml->move= make_move(C9, D8, 0, ProSilver, EmptyPiece); ml++;
	ml->move= make_move(C9, C8, 0, ProSilver, EmptyPiece); ml++;
	ml->move= make_move(C9, B9, 0, ProSilver, EmptyPiece); ml++;
	ml->move= make_move(B2, C2, 0, Horse, EmptyPiece); ml++;
	ml->move= make_move(B2, B1, 0, Horse, EmptyPiece); ml++;
	ml->move= make_move(B2, A1, 0, Horse, EmptyPiece); ml++;
	ml->move= make_move(B2, A2, 0, Horse, EmptyPiece); ml++;
	ml->move= make_move(D2, E1, 0, Dragon, EmptyPiece); ml++;
	ml->move= make_move(D2, D1, 0, Dragon, EmptyPiece); ml++;
	ml->move= make_move(D2, C2, 0, Dragon, EmptyPiece); ml++;
	ml->move= make_move(C3, C2, 0, King, EmptyPiece); ml++;	//本当は合法手ではない王の移動にthem駒の利き範囲への移動をロックする実装がまだないため
	ml->move= make_move(G6, G5, 0, Pawn, Pawn); ml++;
	ml->move= make_move(E4, E3, 1, Pawn, Pawn); ml++;
	ml->move= make_move(D4, D3, 1, Pawn, Pawn); ml++;
	ml->move= make_move(B4, B3, 0, Pawn, Pawn); ml++;
	ml->move= make_move(B4, B3, 1, Pawn, Pawn); ml++;
	ml->move= make_move(F8, E7, 0, Gold, ProNight); ml++;
	ml->move= make_move(C3, C4, 0, King, Pawn); ml++;	//本当は合法手ではない、王手の移動に相手利きがあるところに移動しない機能が実装されていないため
	ml->move= make_move(C3, B3, 0, King, Pawn); ml++;	//本当は合法手ではない、王手の移動に相手利きがあるところに移動しない機能が実装されていないため
	ml->move= make_move(C3, D3, 0, King, Pawn); ml++;	//本当は合法手ではない、王手の移動に相手利きがあるところに移動しない機能が実装されていないため
	ml->move= make_move(I6, I5, 0, ProNight, ProSilver); ml++;
	ml->move= make_move(C9, B8, 0, ProSilver, Gold); ml++;
	ml->move= make_move(B2, B3, 0, Horse, Pawn); ml++;
	ml->move= make_move(B2, A3, 0, Horse, ProPawn); ml++;
	ml->move= make_move(B2, C1, 0, Horse, ProPawn); ml++;
	ml->move= make_move(D2, E2, 0, Dragon, ProPawn); ml++;
	ml->move= make_move(D2, C1, 0, Dragon, ProPawn); ml++;
	ml->move= make_move(D2, D3, 0, Dragon, Pawn); ml++;
	ml->move= make_move(D2, E3, 0, Dragon, Pawn); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), I2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), I4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), H9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), C6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), C8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Pawn + SquareNum - 1), A9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), I2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), I4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), H9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), G2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), G4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), G8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), G7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), G9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), F2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), F3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), F4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), F6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), E5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), E6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), E8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), E9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), D6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), D7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), D8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), D9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), C2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), C6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), C8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), B5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), B6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), B7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), B9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A2, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Lance + SquareNum - 1), A9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), I4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), I7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), I8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), I9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), H3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), H4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), H5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), H6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), H8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), H9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), G4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), G8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), G7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), G9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), F3, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), F4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), F6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), E5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), E6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), E8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), E9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), D6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), D7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), D8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), D9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), C6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), C8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), B5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), B6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), B7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), B9, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), A4, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), A5, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), A6, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), A7, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), A8, 0, PieceType(0), EmptyPiece); ml++;
	ml->move= make_move(Square(Night + SquareNum - 1), A9, 0, PieceType(0), EmptyPiece);
	ml = ms;
	for (int i = 0; i < 153; i++){
		m = mp.next_move();
		std::cout << Movens::string_from_move_apery_format(m) << std::endl;
		if (m == old_m){	//next_move関数が同じ手を返していないことのcheck
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(array_check(m, ml));
		old_m = m;
	}
}
#endif