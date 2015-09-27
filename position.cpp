#include <string>
#include <sstream>

#include "position.h"
#include "bitboard.h"
#include "usi.h"
#include "move.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

//名前空間宣言
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using std::noskipws;
using namespace Movens;

//局所定数宣言・定義
const int hand_packed[8] = {
	0,
	1 << 0,		//HandPawn
	1 << 5,		//HandLance
	1 << 8,		//HandNight
	1 << 11,	//HandSilver
	1 << 14,	//HandBishop
	1 << 16,	//HandRook
	1 << 18		//HandGold
};
//unsigned int 32bitにパッキングされて駒数を元に戻すためのシフト数
const int hand_shift[8] = {
	0,
	0,			//HandPawn
	5,			//HandLance
	8,			//HandNight
	11,			//HandSilver
	14,			//HandBishop
	16,			//HandRook
	18			//HandGold
};
//unsigned 32bitから駒台の枚数をマスキングするための定数
const int hand_masking[8] = {
	0,
	0x1F,		//pawn
	0xE0,		//lance
	0x700,		//night
	0x3800,		//silver
	0xC000,		//bishop
	0x30000,	//rook
	0x1C0000,	//gold
};
//駒文字(先手大文字、後手小文字）
static const string PieceToChar(" PLNSBRGKXTV[JZ  plnsbrgkxtv{jz");

//＜ここからPositionクラスの定義領域＞
//Position classのコンストラクタから呼ばれる
//sfen文字列からPosition内部の局面情報をセットする。
//標準初期sfen文字列はこう->"lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1"
//小文字が後手駒、大文字が先手駒
void Position::position_from_sfen(const string &sfen)
{
	stringstream uip(sfen);
	char token;
	int sq = A9;
	int pmoto = 0;
	int piece;

	clear();
	uip >> noskipws;	//空白をスキップさせない設定
    //盤上
    while(uip >> token && !isspace(token)){
        if(isdigit(token)){
			sq += DeltaE * (token - '0');
        }
		else if (token == '/'){
			sq += (DeltaW * 9) + DeltaS;
		}
		else if (token == '+'){
			pmoto = Promoted;
		}
		else if ((piece = PieceToChar.find(token)) != string::npos){
			put_piece(Piece(piece + pmoto), Square(sq));
			pmoto = UnPromoted;
			sq += DeltaE;
        }
    }
    //手番
	while (uip >> token){
		if (token == 'b'){
			color_turn = Black;
			break;
		}
		else if (token == 'w'){
			color_turn = White;
			break;
		}
		else{
			cout << "Error in SFEN charracter" << endl;
			_ASSERT(false);
			return;
		}
	}
    //持ち駒(サンプル S2Pb18p）
	while (uip >> token && isspace(token)){}	//space cut
	int digits = 0;
    do{
		if (token == '-'){   //持ち駒がないパターンに対応
			break;
		}
		else if (isdigit(token)){   //最初に数字が来るパターンに対応
			digits = digits * 10 + token - '0';
        }
		else if ((piece = PieceToChar.find(token)) != string::npos){  //数字がないパターンに対応
			put_hand(Piece(piece), digits == 0 ? 1 : digits);
			digits = 0;
        }
		else{//どれでもない
            cout << "Error in SFEN charracter" << endl;
            _ASSERT(false);
            return; 
        }
	} while (uip >> token && !isspace(token));
    //持ち駒の次は手数が入っているが無視してよい
	//このあといろいろ設定する必要があるが準備ができていないのでここまで
}
//sfen文字列から局面の盤上を設定
void Position::put_piece(Piece piece,Square sq)
{
	PieceType pt = type_of_piece(piece);
	Color c = color_of_piece(piece);
	
	board[sq] = piece;
	if (piece == BKing){
		king_square[Black] = sq;	
	}
	if (piece == WKing){
		king_square[White] = sq;
	}

	by_type_bb[AllPieces].set_bit(sq);
	by_type_bb[pt].set_bit(sq);
	by_color_bb[c].set_bit(sq);
}
void Position::put_hand(Piece piece,const int num)
{
	PieceType pt = type_of_piece(piece);
	Color c = color_of_piece(piece);

	for (int i = 0; i < num; i++){
		hand[c] += hand_packed[pt];
	}
}

//＜ここからnamespace Positionnsの定義領域＞
void Positionns::init()
{
}

void Positionns::print_board(const Position& pos)
{
    for(int r = Rank9;r < RankNum;r++){
		cout << "|";
		for (int f = FileA; FileI <= f; f--){
            int sq = make_square(File(f),Rank(r));
            int p = pos.get_board(sq);
			cout << std::setw(3) << PieceToChar[p];
        }
        cout << "  |" << endl;
    }
	cout << "black:";
	for (int pt = Pawn; pt < King; pt++){
		cout << PieceToChar[pt] << " " << pos.get_hand(Black, PieceType(pt)) << " ";
	}
	cout << endl;
	cout << "white:";
	for (int pt = Pawn; pt < King; pt++){
		cout << PieceToChar[pt] << " " << pos.get_hand(White, PieceType(pt)) << " ";
	}
	cout << endl;
}
//局面の更新(board,hand,king_square,color_turnだけ更新するdo_move)
void Position::do_move(const Move m,StateInfo& st)
{
#ifdef _DEBUG
	Positionns::is_ok(*this);
#endif
	Key bod_key = get_board_key();
	Key had_key = get_hand_key();

	const Color us = get_color_turn();
    const Square to = move_to(m);
	const PieceType pt_cap = move_cap_piece(m);
	PieceType pt_to;
	
	if (is_drop(m)){	//打つ手		
		pt_to = drop_piece(m);  //打つ駒種を取り出す
		//hand key || board keyの更新
		//いろいろわからない処理が続いている
		const int hand_num = get_hand(us, pt_to);	//一時的に現手駒数が必要
		sub_hand(us, pt_to);
		//ここでbitboardを更新
		board[to] = (us << 4) | pt_to;    //駒コードに変換（駒種は4bitまで）
		//ここでもいろいろ処理、不明
		//この打ち駒によって詰みが発生した場合の処理
	}
	else{				//盤上の手
		const Square from = move_from(m);
		const PieceType pt_from = move_piece(m);
		//ここでbitboardを更新している
		board[from] = EmptyPiece;
		board[to] = (us << 4) | pt_from | (is_pmoto(m) << 3);
		//ここでboard keyなど更新
		if (pt_cap){
			//駒をとった場合の処理
			Color them = over_turn(us);
			//ここでboard keyなどの処理
			//ここでbitboardの処理
			add_hand(us, pt_cap);
			//ここでいろいろな処理、不明
			const int hand_num = get_hand(us, pt_cap);	//一時的に更新後の手駒数が必要？
			//ここでいろいろ処理、不明
		}
		//ここでbitboardの更新
        if(pt_from == King){
			king_square[us] = to;
        }
		else{	//king以外の駒の場合
			//わからない処理
		}
        //王手がかかっているときの処理
        if(1){
        }
        else{
        }
    }
	flip_color();
#ifdef _DEBUG
	Positionns::is_ok(*this);
#endif
}
//局面の復元
void Position::undo_move(const Move m)
{
#ifdef _DEBUG
	Positionns::is_ok(*this);
#endif
	const Color them = get_color_turn();
	const Color us = over_turn(them);
	const Square to = move_to(m);

	flip_color();
	if (is_drop(m)){	//打つ手の戻し
		const PieceType pt_to = drop_piece(m);
		//ここでbitboardの戻し
		board[to] = EmptyPiece;
		add_hand(us, pt_to);
		const int hand_num = get_hand(us, pt_to);	//一時的に更新後のhand?numは必要
		//いろいろわからない処理
	}
	else{				//盤上の手の戻し
		const Square from = move_from(m);
		const PieceType pt_from = move_piece(m);
		const PieceType pt_cap = move_cap_piece(m);

		if (pt_from == King){	//移動させた駒がkingなら
			king_square[us] = from;
		}
		else{	//king以外の駒
			//いろいろとわからない処理
		}
		if (pt_cap){	//駒をとっていたときの戻し
			//bitboardの処理
			board[to] = (them << 4) | pt_cap;
			const int hand_num = get_hand(us, pt_cap);	//一時的に現手駒数を取得
			//いろいろわからない処理
			sub_hand(us, pt_cap);
		}
		else{
			board[to] = EmptyPiece;
		}
		//bitboardの戻し
		board[from] = (us << 4) | pt_from;
	}
	//bitboardの処理
	//StatInfo関係の処理
#ifdef _DEBUG
	Positionns::is_ok(*this);
#endif
}

//board,hand,bitboardのチエック
#ifdef _DEBUG
void Positionns::is_ok(Position &pos)
{
	int failded_step = 0;
    int p;
    int sq;
    int piece_count[16];

	//ここにbitboard関係のチエックが入る,そのうち実装（do_moveにbitboardを更新するコードを書いたら
	//駒数のカウント
    for(int i = 0;i < 16;i++){
        piece_count[i] = 0;
    }
    for(int f = FileI;f < FileNum;f++){
        for(int r = Rank9;r < RankNum;r++){
            sq = make_square(File(f),Rank(r));
            p = pos.get_board(sq);
			if (p == EmptyPiece){
                continue;
            }
            piece_count[type_of_piece(Piece(p))]++;
        }
    }
	for (int hp = Pawn; hp < PieceTypeNum; hp++){
        piece_count[hp] += pos.get_hand(Black,PieceType(hp));
    }
	for (int hp = Pawn; hp < PieceTypeNum; hp++){
        piece_count[hp] += pos.get_hand(White,PieceType(hp));
    }
    int sum = 0;
    for(int i = 0;i < 16;i++){
        sum += piece_count[i];
    }
	EXPECT_EQ(40,sum);
	EXPECT_EQ(18,piece_count[Pawn] + piece_count[ProPawn]);
	EXPECT_EQ(4,piece_count[Lance] + piece_count[ProLance]);
	EXPECT_EQ(4, piece_count[Night] + piece_count[ProNight]);
	EXPECT_EQ(4, piece_count[Silver] + piece_count[ProSilver]);
	EXPECT_EQ(4, piece_count[Gold]);
	EXPECT_EQ(2,piece_count[Bishop] + piece_count[Horse]);
	EXPECT_EQ(2,piece_count[Rook] + piece_count[Dragon]);
	EXPECT_EQ(2,piece_count[King]);
    //BLACK側死に駒の有無
    for(int sq = I9;sq <= A9;sq += 9){
        int p = pos.get_board(sq);
        if((p == BPawn) || (p == BLance) || (p == BNight)){
			EXPECT_TRUE(false);
        }
    }
    for(int sq = I8;sq <= A8;sq += 9){
        char p = pos.get_board(sq);
        if(p == BNight){
			EXPECT_TRUE(false);
        }
    }
    //WHITE側死に駒の有無
    for(int sq = I1;sq <= A1;sq += 9){
        int p = pos.get_board(sq);
        if((p == WPawn) || (p == WLance) || (p == WNight)){
			EXPECT_TRUE(false);
        }
    }
    for(int sq = I2;sq <= A2;sq += 9){
        int p = pos.get_board(sq);
        if(p == WNight){
			EXPECT_TRUE(false);
		}
    }
	//チエックできるものとしては2歩、kingがとられていないなどがあるが余力があったら実装する
}
#endif

#ifdef _DEBUG
TEST(position, do_move)
{
	Square from,to;
	Move m;
	StateInfo st;

	//テスト問題は加藤一二三実践集より
	string ss("ln1g3n1/1ks1gr2l/1p3sbp1/p1ppppp1p/5P1P1/P1P1P1P2/1P1PS1N1P/1BKGGS1R1/LN6L b  1");
	Position pos(ss);

	Positionns::print_board(pos);
	from = square_from_string("1g");
	to = square_from_string("1f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);
	pos.do_move(m,st);
	Positionns::print_board(pos);

}
TEST(position, get_king_square)
{
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);
	EXPECT_EQ(B4, pos.get_king_square(Black));
	EXPECT_EQ(D8, pos.get_king_square(White));
}
TEST(position, add_hand_sub_hand)
{
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP15/1P1pP1P1P/9/2S3b+pL b GRBSNL2Pgn2p 1");
	Position pos(ss);

	EXPECT_EQ(2, pos.get_hand(Black, Pawn));
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	pos.add_hand(Black, Pawn);
	EXPECT_EQ(18, pos.get_hand(Black, Pawn));

	EXPECT_EQ(1, pos.get_hand(Black, Lance));
	pos.add_hand(Black, Lance);
	pos.add_hand(Black, Lance);
	pos.add_hand(Black, Lance);
	EXPECT_EQ(4, pos.get_hand(Black, Lance));

	EXPECT_EQ(1, pos.get_hand(Black, Night));
	pos.add_hand(Black, Night);
	pos.add_hand(Black, Night);
	pos.add_hand(Black, Night);
	EXPECT_EQ(4, pos.get_hand(Black, Night));

	EXPECT_EQ(1, pos.get_hand(Black, Silver));
	pos.add_hand(Black, Silver);
	pos.add_hand(Black, Silver);
	pos.add_hand(Black, Silver);
	EXPECT_EQ(4, pos.get_hand(Black, Silver));

	EXPECT_EQ(1, pos.get_hand(Black, Bishop));
	pos.add_hand(Black, Bishop);
	EXPECT_EQ(2, pos.get_hand(Black, Bishop));

	EXPECT_EQ(1, pos.get_hand(Black, Rook));
	pos.add_hand(Black, Rook);
	EXPECT_EQ(2, pos.get_hand(Black, Rook));

	EXPECT_EQ(1, pos.get_hand(Black, Gold));
	pos.add_hand(Black, Gold);
	EXPECT_EQ(2, pos.get_hand(Black, Gold));

	pos.sub_hand(Black, Pawn);
	EXPECT_EQ(17, pos.get_hand(Black, Pawn));

	pos.sub_hand(Black, Lance);
	EXPECT_EQ(3, pos.get_hand(Black, Lance));

	pos.sub_hand(Black, Night);
	EXPECT_EQ(3, pos.get_hand(Black, Night));

	pos.sub_hand(Black, Silver);
	EXPECT_EQ(3, pos.get_hand(Black, Silver));

	pos.sub_hand(Black, Bishop);
	EXPECT_EQ(1, pos.get_hand(Black, Bishop));

	pos.sub_hand(Black, Rook);
	pos.sub_hand(Black, Rook);
	EXPECT_EQ(0, pos.get_hand(Black, Rook));

	pos.sub_hand(Black, Gold);
	pos.sub_hand(Black, Gold);
	EXPECT_EQ(0, pos.get_hand(Black, Gold));
}
TEST(position, piece_type_of_bb)
{
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoard bb = pos.piece_type_of_bb(Pawn);
	EXPECT_EQ(bb.p(0), 0x908044009400044);
	EXPECT_EQ(bb.p(1), 0x5040);
	bb = pos.piece_type_of_bb(Lance);
	EXPECT_EQ(bb.p(0), 0x400000000000100);
	EXPECT_EQ(bb.p(1), 0x20200);
	bb = pos.piece_type_of_bb(Night);
	EXPECT_EQ(bb.p(0), 0x100000);
	EXPECT_EQ(bb.p(1), 0x11);
	bb = pos.piece_type_of_bb(Silver);
	EXPECT_EQ(bb.p(0), 0x4001020000000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(Bishop);
	EXPECT_EQ(bb.p(0), 0x2004000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(Rook);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(Gold);
	EXPECT_EQ(bb.p(0), 0x280040000000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(King);
	EXPECT_EQ(bb.p(0), 0x400000000000);
	EXPECT_EQ(bb.p(1), 0x20);
	bb = pos.piece_type_of_bb(ProPawn);
	EXPECT_EQ(bb.p(0), 0x20000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(ProLance);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(ProNight);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(ProSilver);
	EXPECT_EQ(bb.p(0), 0x80000);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(Horse);
	EXPECT_EQ(bb.p(0), 0x00);
	EXPECT_EQ(bb.p(1), 0x00);
	bb = pos.piece_type_of_bb(Dragon);
	EXPECT_EQ(bb.p(0), 0x200);
	EXPECT_EQ(bb.p(1), 0x00);
}
TEST(position, color_of_bb)
{
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoard bb = pos.color_of_bb(Black);
	EXPECT_EQ(bb.p(0), 0x4C000E0001080340);
	EXPECT_EQ(bb.p(1), 0x24060);
	bb = pos.color_of_bb(White);
	EXPECT_EQ(bb.p(0), 0x10960604C520004);
	EXPECT_EQ(bb.p(1), 0x1211);
}
TEST(position, all_bb)
{
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);
	BitBoard bb = pos.all_bb();
	EXPECT_EQ(bb.p(0), 0x4D096E604D5A0344);
	EXPECT_EQ(bb.p(1), 0x25271);
}
TEST(position, print_board)
{
	Position pos(USI::start_sfen);
	//Positionns::print_board(pos);

	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos1(ss);
	//Positionns::print_board(pos1);
}
TEST(position, position_from_sfen)
{
	Position pos(USI::start_sfen);
	EXPECT_EQ(WLance, pos.get_board(A9));
	EXPECT_EQ(WNight, pos.get_board(B9));
	EXPECT_EQ(WSilver, pos.get_board(C9));
	EXPECT_EQ(WGold, pos.get_board(D9));
	EXPECT_EQ(WKing, pos.get_board(E9));
	EXPECT_EQ(WGold, pos.get_board(F9));
	EXPECT_EQ(WSilver, pos.get_board(G9));
	EXPECT_EQ(WNight, pos.get_board(H9));
	EXPECT_EQ(WLance, pos.get_board(I9));

	EXPECT_EQ(PieceNone, pos.get_board(A8));
	EXPECT_EQ(WRook, pos.get_board(B8));
	EXPECT_EQ(PieceNone, pos.get_board(C8));
	EXPECT_EQ(PieceNone, pos.get_board(D8));
	EXPECT_EQ(PieceNone, pos.get_board(E8));
	EXPECT_EQ(PieceNone, pos.get_board(F8));
	EXPECT_EQ(PieceNone, pos.get_board(G8));
	EXPECT_EQ(WBishop, pos.get_board(H8));
	EXPECT_EQ(PieceNone, pos.get_board(I8));

	EXPECT_EQ(WPawn, pos.get_board(A7));
	EXPECT_EQ(WPawn, pos.get_board(B7));
	EXPECT_EQ(WPawn, pos.get_board(C7));
	EXPECT_EQ(WPawn, pos.get_board(D7));
	EXPECT_EQ(WPawn, pos.get_board(E7));
	EXPECT_EQ(WPawn, pos.get_board(F7));
	EXPECT_EQ(WPawn, pos.get_board(G7));
	EXPECT_EQ(WPawn, pos.get_board(H7));
	EXPECT_EQ(WPawn, pos.get_board(I7));

	EXPECT_EQ(PieceNone, pos.get_board(A6));
	EXPECT_EQ(PieceNone, pos.get_board(B6));
	EXPECT_EQ(PieceNone, pos.get_board(C6));
	EXPECT_EQ(PieceNone, pos.get_board(D6));
	EXPECT_EQ(PieceNone, pos.get_board(E6));
	EXPECT_EQ(PieceNone, pos.get_board(F6));
	EXPECT_EQ(PieceNone, pos.get_board(G6));
	EXPECT_EQ(PieceNone, pos.get_board(H6));
	EXPECT_EQ(PieceNone, pos.get_board(I6));

	EXPECT_EQ(PieceNone, pos.get_board(A5));
	EXPECT_EQ(PieceNone, pos.get_board(B5));
	EXPECT_EQ(PieceNone, pos.get_board(C5));
	EXPECT_EQ(PieceNone, pos.get_board(D5));
	EXPECT_EQ(PieceNone, pos.get_board(E5));
	EXPECT_EQ(PieceNone, pos.get_board(F5));
	EXPECT_EQ(PieceNone, pos.get_board(G5));
	EXPECT_EQ(PieceNone, pos.get_board(H5));
	EXPECT_EQ(PieceNone, pos.get_board(I5));

	EXPECT_EQ(PieceNone, pos.get_board(A4));
	EXPECT_EQ(PieceNone, pos.get_board(B4));
	EXPECT_EQ(PieceNone, pos.get_board(C4));
	EXPECT_EQ(PieceNone, pos.get_board(D4));
	EXPECT_EQ(PieceNone, pos.get_board(E4));
	EXPECT_EQ(PieceNone, pos.get_board(F4));
	EXPECT_EQ(PieceNone, pos.get_board(G4));
	EXPECT_EQ(PieceNone, pos.get_board(H4));
	EXPECT_EQ(PieceNone, pos.get_board(I4));

	EXPECT_EQ(BPawn, pos.get_board(A3));
	EXPECT_EQ(BPawn, pos.get_board(B3));
	EXPECT_EQ(BPawn, pos.get_board(C3));
	EXPECT_EQ(BPawn, pos.get_board(D3));
	EXPECT_EQ(BPawn, pos.get_board(E3));
	EXPECT_EQ(BPawn, pos.get_board(F3));
	EXPECT_EQ(BPawn, pos.get_board(G3));
	EXPECT_EQ(BPawn, pos.get_board(H3));
	EXPECT_EQ(BPawn, pos.get_board(I3));

	EXPECT_EQ(PieceNone, pos.get_board(A2));
	EXPECT_EQ(BBishop, pos.get_board(B2));
	EXPECT_EQ(PieceNone, pos.get_board(C2));
	EXPECT_EQ(PieceNone, pos.get_board(D2));
	EXPECT_EQ(PieceNone, pos.get_board(E2));
	EXPECT_EQ(PieceNone, pos.get_board(F2));
	EXPECT_EQ(PieceNone, pos.get_board(G2));
	EXPECT_EQ(BRook, pos.get_board(H2));
	EXPECT_EQ(PieceNone, pos.get_board(I2));

	EXPECT_EQ(BLance, pos.get_board(A1));
	EXPECT_EQ(BNight, pos.get_board(B1));
	EXPECT_EQ(BSilver, pos.get_board(C1));
	EXPECT_EQ(BGold, pos.get_board(D1));
	EXPECT_EQ(BKing, pos.get_board(E1));
	EXPECT_EQ(BGold, pos.get_board(F1));
	EXPECT_EQ(BSilver, pos.get_board(G1));
	EXPECT_EQ(BNight, pos.get_board(H1));
	EXPECT_EQ(BLance, pos.get_board(I1));
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos1(ss);
	EXPECT_EQ(WLance, pos1.get_board(A9));
	EXPECT_EQ(WNight, pos1.get_board(B9));
	EXPECT_EQ(PieceNone, pos1.get_board(C9));
	EXPECT_EQ(WGold, pos1.get_board(D9));
	EXPECT_EQ(PieceNone, pos1.get_board(E9));
	EXPECT_EQ(WPawn, pos1.get_board(F9));
	EXPECT_EQ(PieceNone, pos1.get_board(G9));
	EXPECT_EQ(BDragon, pos1.get_board(H9));
	EXPECT_EQ(PieceNone, pos1.get_board(I9));

	EXPECT_EQ(PieceNone, pos1.get_board(A8));
	EXPECT_EQ(PieceNone, pos1.get_board(B8));
	EXPECT_EQ(PieceNone, pos1.get_board(C8));
	EXPECT_EQ(WKing, pos1.get_board(D8));
	EXPECT_EQ(WBishop, pos1.get_board(E8));
	EXPECT_EQ(PieceNone, pos1.get_board(F8));
	EXPECT_EQ(BProSilver, pos1.get_board(G8));
	EXPECT_EQ(PieceNone, pos1.get_board(H8));
	EXPECT_EQ(PieceNone, pos1.get_board(I8));

	EXPECT_EQ(PieceNone, pos1.get_board(A7));
	EXPECT_EQ(PieceNone, pos1.get_board(B7));
	EXPECT_EQ(WPawn, pos1.get_board(C7));
	EXPECT_EQ(PieceNone, pos1.get_board(D7));
	EXPECT_EQ(WPawn, pos1.get_board(E7));
	EXPECT_EQ(PieceNone, pos1.get_board(F7));
	EXPECT_EQ(WNight, pos1.get_board(G7));
	EXPECT_EQ(PieceNone, pos1.get_board(H7));
	EXPECT_EQ(WPawn, pos1.get_board(I7));

	EXPECT_EQ(WPawn, pos1.get_board(A6));
	EXPECT_EQ(PieceNone, pos1.get_board(B6));
	EXPECT_EQ(PieceNone, pos1.get_board(C6));
	EXPECT_EQ(WSilver, pos1.get_board(D6));
	EXPECT_EQ(PieceNone, pos1.get_board(E6));
	EXPECT_EQ(WGold, pos1.get_board(F6));
	EXPECT_EQ(PieceNone, pos1.get_board(G6));
	EXPECT_EQ(PieceNone, pos1.get_board(H6));
	EXPECT_EQ(PieceNone, pos1.get_board(I6));

	EXPECT_EQ(PieceNone, pos1.get_board(A5));
	EXPECT_EQ(WNight, pos1.get_board(B5));
	EXPECT_EQ(BLance, pos1.get_board(C5));
	EXPECT_EQ(PieceNone, pos1.get_board(D5));
	EXPECT_EQ(PieceNone, pos1.get_board(E5));
	EXPECT_EQ(PieceNone, pos1.get_board(F5));
	EXPECT_EQ(WPawn, pos1.get_board(G5));
	EXPECT_EQ(PieceNone, pos1.get_board(H5));
	EXPECT_EQ(PieceNone, pos1.get_board(I5));

	EXPECT_EQ(BPawn, pos1.get_board(A4));
	EXPECT_EQ(BKing, pos1.get_board(B4));
	EXPECT_EQ(BPawn, pos1.get_board(C4));
	EXPECT_EQ(PieceNone, pos1.get_board(D4));
	EXPECT_EQ(BSilver, pos1.get_board(E4));
	EXPECT_EQ(PieceNone, pos1.get_board(F4));
	EXPECT_EQ(PieceNone, pos1.get_board(G4));
	EXPECT_EQ(PieceNone, pos1.get_board(H4));
	EXPECT_EQ(PieceNone, pos1.get_board(I4));

	EXPECT_EQ(PieceNone, pos1.get_board(A3));
	EXPECT_EQ(BPawn, pos1.get_board(B3));
	EXPECT_EQ(PieceNone, pos1.get_board(C3));
	EXPECT_EQ(WPawn, pos1.get_board(D3));
	EXPECT_EQ(BPawn, pos1.get_board(E3));
	EXPECT_EQ(PieceNone, pos1.get_board(F3));
	EXPECT_EQ(BPawn, pos1.get_board(G3));
	EXPECT_EQ(PieceNone, pos1.get_board(H3));
	EXPECT_EQ(BPawn, pos1.get_board(I3));

	EXPECT_EQ(PieceNone, pos1.get_board(A2));
	EXPECT_EQ(PieceNone, pos1.get_board(B2));
	EXPECT_EQ(PieceNone, pos1.get_board(C2));
	EXPECT_EQ(PieceNone, pos1.get_board(D2));
	EXPECT_EQ(BGold, pos1.get_board(E2));
	EXPECT_EQ(PieceNone, pos1.get_board(F2));
	EXPECT_EQ(PieceNone, pos1.get_board(G2));
	EXPECT_EQ(PieceNone, pos1.get_board(H2));
	EXPECT_EQ(PieceNone, pos1.get_board(I2));

	EXPECT_EQ(BLance, pos1.get_board(A1));
	EXPECT_EQ(PieceNone, pos1.get_board(B1));
	EXPECT_EQ(BSilver, pos1.get_board(C1));
	EXPECT_EQ(PieceNone, pos1.get_board(D1));
	EXPECT_EQ(PieceNone, pos1.get_board(E1));
	EXPECT_EQ(PieceNone, pos1.get_board(F1));
	EXPECT_EQ(WBishop, pos1.get_board(G1));
	EXPECT_EQ(WProPawn, pos1.get_board(H1));
	EXPECT_EQ(BLance, pos1.get_board(I1));
	//get_hand	
	EXPECT_EQ(2,pos1.get_hand(Black, Pawn));
	EXPECT_EQ(0,pos1.get_hand(Black, Lance));
	EXPECT_EQ(0,pos1.get_hand(Black, Night));
	EXPECT_EQ(0,pos1.get_hand(Black, Silver));
	EXPECT_EQ(0,pos1.get_hand(Black, Bishop));
	EXPECT_EQ(1,pos1.get_hand(Black, Rook));
	EXPECT_EQ(0,pos1.get_hand(Black, Gold));
	EXPECT_EQ(2, pos1.get_hand(White, Pawn));
	EXPECT_EQ(0, pos1.get_hand(White, Lance));
	EXPECT_EQ(1, pos1.get_hand(White, Night));
	EXPECT_EQ(0, pos1.get_hand(White, Silver));
	EXPECT_EQ(0, pos1.get_hand(White, Bishop));
	EXPECT_EQ(0, pos1.get_hand(White, Rook));
	EXPECT_EQ(1, pos1.get_hand(White, Gold));
	//is_hand
	EXPECT_TRUE(pos1.is_hand(Black, Pawn));
	EXPECT_FALSE(pos1.is_hand(Black, Lance));
	EXPECT_FALSE(pos1.is_hand(Black, Night));
	EXPECT_FALSE(pos1.is_hand(Black, Silver));
	EXPECT_FALSE(pos1.is_hand(Black, Bishop));
	EXPECT_TRUE(pos1.is_hand(Black, Rook));
	EXPECT_FALSE(pos1.is_hand(Black, Gold));
	EXPECT_TRUE(pos1.is_hand(White, Pawn));
	EXPECT_FALSE(pos1.is_hand(White, Lance));
	EXPECT_TRUE(pos1.is_hand(White, Night));
	EXPECT_FALSE(pos1.is_hand(White, Silver));
	EXPECT_FALSE(pos1.is_hand(White, Bishop));
	EXPECT_FALSE(pos1.is_hand(White, Rook));
	EXPECT_TRUE(pos1.is_hand(White, Gold));
}
TEST(position, color_turn)
{
	Position pos;
	pos.set_color_turn(Black);
	EXPECT_EQ(Black, pos.get_color_turn());
	pos.flip_color();
	EXPECT_EQ(White, pos.get_color_turn());
}


TEST(position, PieceToChar)
{
	/*
	駒文字（PieceToChar）のインデックスと駒コードが一致することを確認している
	Pawn..P p
	Lance..L l
	Night..N n
	Silver..S s
	Bishop..B b
	Rook..R r
	Golad..G g
	King..K k
	ProPawn..X x
	ProLance..T t
	ProNight..V v
	ProSilver..[{
	Horse..J j
	Dragon..Z z
	*/
	EXPECT_EQ(BPawn, PieceToChar.find('P'));
	EXPECT_EQ(BLance, PieceToChar.find('L'));
	EXPECT_EQ(BNight, PieceToChar.find('N'));
	EXPECT_EQ(BSilver, PieceToChar.find('S'));
	EXPECT_EQ(BBishop, PieceToChar.find('B'));
	EXPECT_EQ(BRook, PieceToChar.find('R'));
	EXPECT_EQ(BGold, PieceToChar.find('G'));
	EXPECT_EQ(BKing, PieceToChar.find('K'));
	EXPECT_EQ(BProPawn, PieceToChar.find('X'));
	EXPECT_EQ(BProLance, PieceToChar.find('T'));
	EXPECT_EQ(BProNight, PieceToChar.find('V'));
	EXPECT_EQ(BProSilver, PieceToChar.find('['));
	EXPECT_EQ(BHorse, PieceToChar.find('J'));
	EXPECT_EQ(BDragon, PieceToChar.find('Z'));

	EXPECT_EQ(WPawn, PieceToChar.find('p'));
	EXPECT_EQ(WLance, PieceToChar.find('l'));
	EXPECT_EQ(WNight, PieceToChar.find('n'));
	EXPECT_EQ(WSilver, PieceToChar.find('s'));
	EXPECT_EQ(WBishop, PieceToChar.find('b'));
	EXPECT_EQ(WRook, PieceToChar.find('r'));
	EXPECT_EQ(WGold, PieceToChar.find('g'));
	EXPECT_EQ(WKing, PieceToChar.find('k'));
	EXPECT_EQ(WProPawn, PieceToChar.find('x'));
	EXPECT_EQ(WProLance, PieceToChar.find('t'));
	EXPECT_EQ(WProNight, PieceToChar.find('v'));
	EXPECT_EQ(WProSilver, PieceToChar.find('{'));
	EXPECT_EQ(WHorse, PieceToChar.find('j'));
	EXPECT_EQ(WDragon, PieceToChar.find('z'));
}
#endif