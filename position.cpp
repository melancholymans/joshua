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
//指定の駒種、カラーの駒のbitboardを更新する。指定座標が駒がいればoffにする。指定座標に駒がいなければonにする（打ち駒）
inline void Position::drop_piece_bb(const PieceType pt, const Square sq, const Color c)
{
	by_type_bb[AllPieces].xor_bit(sq);
	by_type_bb[pt].xor_bit(sq);
	by_color_bb[c].xor_bit(sq);
}
//指定の駒種、カラーの駒のbitboardを更新する。指定座標が駒がいればoffにする。指定座標に駒がいなければonにする（移動の表現）
inline void Position::move_piece_bb(const PieceType pt_from, const PieceType pt_to, const Square from, const Square to, const Color c)
{
	by_type_bb[AllPieces].xor_bit(from,to);
	by_type_bb[pt_from].xor_bit(from);
	by_type_bb[pt_to].xor_bit(to);
	by_color_bb[c].xor_bit(from, to);
}
//指定の駒種、カラーの駒のbitboardを更新する。指定座標が駒がいればoffにする。指定座標に駒がいなければonにする（移動の表現）
inline void Position::cap_piece_bb(const PieceType pt, const Square sq,const Color c)
{
	by_type_bb[AllPieces].xor_bit(sq);
	by_type_bb[pt].xor_bit(sq);
	by_color_bb[c].xor_bit(sq);
}

#ifdef _DEBUG
bool Position::get_piece_bit(const PieceType pt, const Square sq)
{
	return by_type_bb[pt].is_bit_on(sq);
}
bool Position::get_color_bit(const Color c, const Square sq)
{
	return by_color_bb[c].is_bit_on(sq);
}
void Position::print_piece_bb(const PieceType pt,string msg)
{
	printf("mssage: %s\n", msg.c_str());
	BitBoardns::print(by_type_bb[pt]);
}
void Position::print_color_bb(Color c,string msg)
{
	printf("mssage:%s\n", msg.c_str());
	BitBoardns::print(by_color_bb[c]);
}
#endif
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
		drop_piece_bb(pt_to, to, us);
		board[to] = (us << 4) | pt_to;    //駒コードに変換（駒種は4bitまで）
		//ここでもいろいろ処理、不明
		//この打ち駒によって詰みが発生した場合の処理
	}
	else{				//盤上の手
		const Square from = move_from(m);
		const PieceType pt_from = move_piece(m);
		board[from] = EmptyPiece;
		pt_to = PieceType(pt_from | (is_pmoto(m) << 3));
		board[to] = int((us << 4) | pt_to);
		//ここでboard keyなど更新
		if (pt_cap){
			//駒をとった場合の処理
			Color them = over_turn(us);
			//ここでboard keyなどの処理
			cap_piece_bb(pt_cap,to,them);
			add_hand(us, PieceType(pt_cap & 0x07));	//pt_capが成り駒だと正常に手駒に登録されないので成りbitを削っている
			//ここでいろいろな処理、不明
			const int hand_num = get_hand(us, PieceType(pt_cap & 0x07));	//一時的に更新後の手駒数が必要？
			//ここでいろいろ処理、不明
		}
		move_piece_bb(pt_from, pt_to, from, to, us);
		if (pt_from == King){
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
		drop_piece_bb(pt_to, to,us);
		board[to] = EmptyPiece;
		add_hand(us, pt_to);
		const int hand_num = get_hand(us, pt_to);	//一時的に更新後のhand?numは必要
		//いろいろわからない処理
	}
	else{				//盤上の手の戻し
		const Square from = move_from(m);
		const PieceType pt_from = move_piece(m);
		const PieceType pt_to = PieceType(pt_from | (is_pmoto(m) << 3));
		const PieceType pt_cap = move_cap_piece(m);
		if (pt_from == King){	//移動させた駒がkingなら
			king_square[us] = from;
		}
		else{	//king以外の駒
			//いろいろとわからない処理
		}
		move_piece_bb(pt_from,pt_to,from,to,us);
		if (pt_cap){	//駒をとっていたときの戻し
			//bitboardの処理
			cap_piece_bb(pt_cap,to,them);
			board[to] = (them << 4) | pt_cap;
			const int hand_num = get_hand(us, PieceType(pt_cap & 0x07));	//一時的に現手駒数を取得
			//いろいろわからない処理
			sub_hand(us, PieceType(pt_cap & 0x07));
		}
		else{
			board[to] = EmptyPiece;
		}
		board[from] = (us << 4) | pt_from;
	}
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

	//Black側とWhite側の＆をとって0にならなければエラー
	EXPECT_EQ(false, (pos.color_of_bb(Black) & pos.color_of_bb(White)).is_not_zero());
	//Black Whiteを合算したものと駒種を合算したもの(AllPiece)が同じでなければエラー
	EXPECT_EQ(false, (pos.color_of_bb(Black) | pos.color_of_bb(White)) != pos.piece_type_of_bb(AllPieces));
	BitBoard all_piece = pos.piece_type_of_bb(Pawn) | pos.piece_type_of_bb(Lance) | pos.piece_type_of_bb(Night) | pos.piece_type_of_bb(Silver) |
		pos.piece_type_of_bb(Bishop) | pos.piece_type_of_bb(Rook) | pos.piece_type_of_bb(Gold) | pos.piece_type_of_bb(King) | pos.piece_type_of_bb(ProPawn) |
		pos.piece_type_of_bb(ProLance) | pos.piece_type_of_bb(ProNight) | pos.piece_type_of_bb(ProSilver) | pos.piece_type_of_bb(Horse) | pos.piece_type_of_bb(Dragon);
	EXPECT_EQ(false,all_piece != pos.piece_type_of_bb(AllPieces));
	//王の数が２である
	EXPECT_EQ(2, pos.piece_type_of_bb(King).pop_count());
	EXPECT_EQ(1, (pos.piece_type_of_bb(King) & pos.color_of_bb(Black)).pop_count());
	EXPECT_EQ(1, (pos.piece_type_of_bb(King) & pos.color_of_bb(White)).pop_count());
	//駒数のカウント(bitboard側）
	for (int i = 0; i < 16; i++){
		piece_count[i] = 0;
	}
	for (int p = Pawn; p < PieceTypeNum; p++){
		piece_count[p] = pos.piece_type_of_bb(PieceType(p)).pop_count();
	}
	for (int hp = Pawn; hp < King; hp++){
		piece_count[hp] += pos.get_hand(Black, PieceType(hp));
	}
	for (int hp = Pawn; hp < King; hp++){
		piece_count[hp] += pos.get_hand(White, PieceType(hp));
	}
	int sum = 0;
	for (int i = 0; i < 16; i++){
		sum += piece_count[i];
	}
	EXPECT_EQ(40, sum);
	EXPECT_EQ(18, piece_count[Pawn] + piece_count[ProPawn]);
	EXPECT_EQ(4, piece_count[Lance] + piece_count[ProLance]);
	EXPECT_EQ(4, piece_count[Night] + piece_count[ProNight]);
	EXPECT_EQ(4, piece_count[Silver] + piece_count[ProSilver]);
	EXPECT_EQ(4, piece_count[Gold]);
	EXPECT_EQ(2, piece_count[Bishop] + piece_count[Horse]);
	EXPECT_EQ(2, piece_count[Rook] + piece_count[Dragon]);
	EXPECT_EQ(2, piece_count[King]);
	//2歩をチエック
	for (int f = FileI; f < FileNum; f++){
		EXPECT_EQ(true, (pos.piece_type_of_bb(Pawn) & pos.color_of_bb(Black) & BitBoardns::FILE_MASK[f]).pop_count() < 2);
	}
	for (int f = FileI; f < FileNum; f++){
		EXPECT_EQ(true, (pos.piece_type_of_bb(Pawn) & pos.color_of_bb(White) & BitBoardns::FILE_MASK[f]).pop_count() < 2);
	}
	//駒数のカウント（board側）
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
	for (int hp = Pawn; hp < King; hp++){
        piece_count[hp] += pos.get_hand(Black,PieceType(hp));
    }
	for (int hp = Pawn; hp < King; hp++){
        piece_count[hp] += pos.get_hand(White,PieceType(hp));
    }
    sum = 0;
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
	//2歩をチエック
	int count[2] = {0,0};
	for (int sq = A9; sq <= A1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = B9; sq <= B1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = C9; sq <= C1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = D9; sq <= D1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = E9; sq <= E1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = F9; sq <= F1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = G9; sq <= G1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = H9; sq <= H1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
	count[0] = 0; count[1] = 0;
	for (int sq = I9; sq <= I1; sq++){
		Piece p = Piece(pos.get_board(sq));
		if (type_of_piece(p) == Pawn){
			count[color_of_piece(p)]++;
		}
	}
	if (count[Black] > 1 || count[White] > 1){
		EXPECT_TRUE(false);
	}
}
#endif

#ifdef _DEBUG
TEST(postion, undo_move)
{
	Square from, to;
	Move m;
	StateInfo st;
	using Positionns::print_board;

	string ss("l3g2X1/1kg1s4/jpS6/2pp1P2p/4rBP2/pLP5P/1PNPS4/P1KGS1x2/L7L w RGN4P2n 1");
	Position pos(ss);
	
	EXPECT_EQ(1, pos.get_color_bit(Black, H9));
	EXPECT_EQ(1, pos.get_color_bit(Black, C7));
	EXPECT_EQ(1, pos.get_color_bit(Black, F6));
	EXPECT_EQ(1, pos.get_color_bit(Black, F5));
	EXPECT_EQ(1, pos.get_color_bit(Black, G5));
	EXPECT_EQ(1, pos.get_color_bit(Black, B4));
	EXPECT_EQ(1, pos.get_color_bit(Black, C4));
	EXPECT_EQ(1, pos.get_color_bit(Black, I4));
	EXPECT_EQ(1, pos.get_color_bit(Black, B3));
	EXPECT_EQ(1, pos.get_color_bit(Black, C3));
	EXPECT_EQ(1, pos.get_color_bit(Black, D3));
	EXPECT_EQ(1, pos.get_color_bit(Black, E3));
	EXPECT_EQ(1, pos.get_color_bit(Black, A2));
	EXPECT_EQ(1, pos.get_color_bit(Black, C2));
	EXPECT_EQ(1, pos.get_color_bit(Black, D2));
	EXPECT_EQ(1, pos.get_color_bit(Black, E2));
	EXPECT_EQ(1, pos.get_color_bit(Black, A1));
	EXPECT_EQ(1, pos.get_color_bit(Black, I1));

	EXPECT_EQ(1, pos.get_color_bit(White, A9));
	EXPECT_EQ(1, pos.get_color_bit(White, E9));
	EXPECT_EQ(1, pos.get_color_bit(White, B8));
	EXPECT_EQ(1, pos.get_color_bit(White, C8));
	EXPECT_EQ(1, pos.get_color_bit(White, E8));
	EXPECT_EQ(1, pos.get_color_bit(White, A7));
	EXPECT_EQ(1, pos.get_color_bit(White, B7));
	EXPECT_EQ(1, pos.get_color_bit(White, C6));
	EXPECT_EQ(1, pos.get_color_bit(White, D6));
	EXPECT_EQ(1, pos.get_color_bit(White, I6));
	EXPECT_EQ(1, pos.get_color_bit(White, E5));
	EXPECT_EQ(1, pos.get_color_bit(White, A4));
	EXPECT_EQ(1, pos.get_color_bit(White, G2));

	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G2));
	/*
	//他の駒種は目視で確認
	printf("");
	pos.print_piece_bb(Pawn,"Pawn");
	pos.print_piece_bb(Lance,"Lance");
	pos.print_piece_bb(Night,"Night");
	pos.print_piece_bb(Silver,"Silver");
	pos.print_piece_bb(Bishop,"Bishop");
	pos.print_piece_bb(Rook,"Rook");
	pos.print_piece_bb(Gold,"Gold");
	pos.print_piece_bb(King,"King");
	pos.print_piece_bb(ProPawn,"ProPawn");
	pos.print_piece_bb(ProLance,"ProLance");
	pos.print_piece_bb(ProNight,"ProNight");
	pos.print_piece_bb(ProSilver,"ProSilver");
	pos.print_piece_bb(Horse,"Horse");
	pos.print_piece_bb(Dragon,"Dragon");
	*/

	EXPECT_EQ(WLance, pos.get_board(A9));
	EXPECT_EQ(WGold, pos.get_board(E9));
	EXPECT_EQ(BProPawn, pos.get_board(H9));
	EXPECT_EQ(WKing, pos.get_board(B8));
	EXPECT_EQ(WGold, pos.get_board(C8));
	EXPECT_EQ(WSilver, pos.get_board(E8));
	EXPECT_EQ(WHorse, pos.get_board(A7));
	EXPECT_EQ(WPawn, pos.get_board(B7));
	EXPECT_EQ(BSilver, pos.get_board(C7));
	EXPECT_EQ(WPawn, pos.get_board(C6));
	EXPECT_EQ(WPawn, pos.get_board(D6));
	EXPECT_EQ(BPawn, pos.get_board(F6));
	EXPECT_EQ(WPawn, pos.get_board(I6));
	EXPECT_EQ(WRook, pos.get_board(E5));
	EXPECT_EQ(BBishop, pos.get_board(F5));
	EXPECT_EQ(BPawn, pos.get_board(G5));
	EXPECT_EQ(WPawn, pos.get_board(A4));
	EXPECT_EQ(BLance, pos.get_board(B4));
	EXPECT_EQ(BPawn, pos.get_board(C4));
	EXPECT_EQ(BPawn, pos.get_board(I4));
	EXPECT_EQ(BPawn, pos.get_board(B3));
	EXPECT_EQ(BNight, pos.get_board(C3));
	EXPECT_EQ(BPawn, pos.get_board(D3));
	EXPECT_EQ(BSilver, pos.get_board(E3));
	EXPECT_EQ(BPawn, pos.get_board(A2));
	EXPECT_EQ(BKing, pos.get_board(C2));
	EXPECT_EQ(BGold, pos.get_board(D2));
	EXPECT_EQ(BSilver, pos.get_board(E2));
	EXPECT_EQ(WProPawn, pos.get_board(G2));
	EXPECT_EQ(BLance, pos.get_board(A1));
	EXPECT_EQ(BLance, pos.get_board(I1));

	EXPECT_EQ(1, pos.get_hand(Black, Rook));
	EXPECT_EQ(1, pos.get_hand(Black, Gold));
	EXPECT_EQ(1, pos.get_hand(Black, Night));
	EXPECT_EQ(4, pos.get_hand(Black, Pawn));
	EXPECT_EQ(2, pos.get_hand(White, Night));

	//S*7c silver
	to = square_from_string("7c");
	m = make_move(drop_piece_from(Silver), to, 0, Silver, EmptyPiece);	//79
	pos.undo_move(m);
	//R*5e rook
	from = square_from_string("4f");
	to = square_from_string("5e");
	m = make_move(drop_piece_from(Rook), to, 0, Silver, EmptyPiece);	//78
	pos.undo_move(m);
	//4f5g silver proPawn
	from = square_from_string("4f");
	to = square_from_string("5g");
	m = make_move(from, to, 0, Silver, ProPawn);	//77	
	pos.undo_move(m);
	//5f5g pawn pmoto night
	from = square_from_string("5f");
	to = square_from_string("5g");
	m = make_move(from, to, 1, Pawn, Night);	//76
	pos.undo_move(m);
	//6i5g night ProNight
	from = square_from_string("6i");
	to = square_from_string("5g");
	m = make_move(from, to, 0, Night, ProNight);	//75
	pos.undo_move(m);
	//6e5g night pmoto
	from = square_from_string("6e");
	to = square_from_string("5g");
	m = make_move(from, to, 1, Night, EmptyPiece);	//74
	pos.undo_move(m);
	//8i7g night
	from = square_from_string("8i");
	to = square_from_string("7g");
	m = make_move(from, to, 0, Night, EmptyPiece);	//73
	pos.undo_move(m);
	//6a7b gold
	from = square_from_string("6a");
	to = square_from_string("7b");
	m = make_move(from, to, 0, Gold, EmptyPiece);	//72
	pos.undo_move(m);
	//B*4e bishop
	from = square_from_string("6c");
	to = square_from_string("4e");
	m = make_move(drop_piece_from(Bishop), to, 0, Silver, EmptyPiece);	//71
	pos.undo_move(m);
	//6c5b silver dragon
	from = square_from_string("6c");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Silver, Dragon);	//70
	pos.undo_move(m);
	//4a5b dragon rook
	from = square_from_string("4a");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Dragon, Rook);	//69
	pos.undo_move(m);
	//G*5a
	from = square_from_string("3b");
	to = square_from_string("5a");
	m = make_move(drop_piece_from(Gold), to, 0, Gold, EmptyPiece);	//68
	pos.undo_move(m);
	//3b4a dragon pawn
	from = square_from_string("3b");
	to = square_from_string("4a");
	m = make_move(from, to, 0, Dragon, Pawn);	//67
	pos.undo_move(m);
	//R*5b
	from = square_from_string("2b");
	to = square_from_string("5b");
	m = make_move(drop_piece_from(Rook), to, 0, Rook, EmptyPiece);	//66
	pos.undo_move(m);
	//2b3b dragon pawn
	from = square_from_string("2b");
	to = square_from_string("3b");
	m = make_move(from, to, 0, Dragon, Pawn);	//65
	pos.undo_move(m);
	//7b6c Silver PRoNight
	from = square_from_string("7b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Silver, ProNight);	//64
	pos.undo_move(m);
	//5c6c PRoNight Silver
	from = square_from_string("5c");
	to = square_from_string("6c");
	m = make_move(from, to, 0, ProNight, Silver);	//63
	pos.undo_move(m);
	//P*3b Pawn
	from = square_from_string("5c");
	to = square_from_string("3b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//62
	pos.undo_move(m);
	//4g5h silver bishop
	from = square_from_string("4g");
	to = square_from_string("5h");
	m = make_move(from, to, 0, Silver, Horse);	//61
	pos.undo_move(m);	
	//4i5h bishop pmoto gold
	from = square_from_string("4i");
	to = square_from_string("5h");
	m = make_move(from, to, 1, Bishop, Gold);	//60
	pos.undo_move(m);
	//4b2b dragon
	from = square_from_string("4b");
	to = square_from_string("2b");
	m = make_move(from, to, 0, Dragon, EmptyPiece);	//59
	pos.undo_move(m);
	//P*4a pawn
	from = square_from_string("4e");
	to = square_from_string("4a");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//58
	pos.undo_move(m);
	//4e5c night pmoto
	from = square_from_string("4e");
	to = square_from_string("5c");
	m = make_move(from, to, 1, Night, EmptyPiece);	//57
	pos.undo_move(m);
	//6b6c silver Dragon
	from = square_from_string("6b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Silver, Dragon);	//56
	pos.undo_move(m);
	//2c6c rook pmoto gold
	from = square_from_string("2c");
	to = square_from_string("6c");
	m = make_move(from, to, 1, Rook, Gold);	//55
	pos.undo_move(m);
	//3g3h pawn pmoto
	from = square_from_string("3g");
	to = square_from_string("3h");
	m = make_move(from, to, 1, Pawn, EmptyPiece);	//54
	pos.undo_move(m);
	//1b4b dragon pawn
	from = square_from_string("1b");
	to = square_from_string("4b");
	m = make_move(from, to, 0, Dragon, Pawn);	//53
	pos.undo_move(m);
	//B*4i bishop
	from = square_from_string("3f");
	to = square_from_string("4i");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//52
	pos.undo_move(m);
	//3f4g silver
	from = square_from_string("3f");
	to = square_from_string("4g");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//51
	pos.undo_move(m);
	//P*3g pawn
	from = square_from_string("5c");
	to = square_from_string("3g");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//50
	pos.undo_move(m);
	//R*2c rook
	from = square_from_string("5c");
	to = square_from_string("2c");
	m = make_move(drop_piece_from(Rook), to, 0, Rook, EmptyPiece);	//49
	pos.undo_move(m);
	//5c6b silver
	from = square_from_string("5c");
	to = square_from_string("6b");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//48
	pos.undo_move(m);
	//3g4e night
	from = square_from_string("3g");
	to = square_from_string("4e");
	m = make_move(from, to, 0, Night, EmptyPiece);	//47
	pos.undo_move(m);
	//P*4b pawn
	from = square_from_string("4e");
	to = square_from_string("4b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//46
	pos.undo_move(m);
	//4e4d pawn
	from = square_from_string("4e");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//45
	pos.undo_move(m);
	//8d9c Horse
	from = square_from_string("8d");
	to = square_from_string("9c");
	m = make_move(from, to, 1, Horse, EmptyPiece);	//44
	pos.undo_move(m);
	//L*8f lance
	from = square_from_string("3i");
	to = square_from_string("8f");
	m = make_move(drop_piece_from(Lance), to, 0, Lance, EmptyPiece);	//43
	pos.undo_move(m);
	//3i8d bishop pmoto 
	from = square_from_string("3i");
	to = square_from_string("8d");
	m = make_move(from, to, 1, Bishop, EmptyPiece);	//42
	pos.undo_move(m);
	//P*9h pawn
	from = square_from_string("9e");
	to = square_from_string("9h");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//41
	pos.undo_move(m);
	//9e9f pawn pawn
	from = square_from_string("9e");
	to = square_from_string("9f");
	m = make_move(from, to, 0, Pawn, Pawn);	//40
	pos.undo_move(m);
	//2c1b doragon lance 
	from = square_from_string("2c");
	to = square_from_string("1b");
	m = make_move(from, to, 0, Dragon, Lance);	//39
	pos.undo_move(m);
	//9d9e pawn
	from = square_from_string("9d");
	to = square_from_string("9e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//38
	pos.undo_move(m);
	//2f2c rook pmoto pawn
	from = square_from_string("2f");
	to = square_from_string("2c");
	m = make_move(from, to, 1, Rook, Pawn);	//37
	pos.undo_move(m);
	//5b5c silver Horse
	from = square_from_string("5b");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Silver, Horse);	//36
	pos.undo_move(m);
	//4d5c Horse rook
	from = square_from_string("4d");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Horse, Rook);	//35
	pos.undo_move(m);
	//B*3i bishop
	from = square_from_string("7c");
	to = square_from_string("3i");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//34
	pos.undo_move(m);
	//N*6i night
	from = square_from_string("7c");
	to = square_from_string("6i");
	m = make_move(drop_piece_from(Night), to, 0, Night, EmptyPiece);	//33
	pos.undo_move(m);
	//7c6e night
	from = square_from_string("7c");
	to = square_from_string("6e");
	m = make_move(from, to, 0, Night, EmptyPiece);	//32
	pos.undo_move(m);
	//1a4d bishop pmoto 
	from = square_from_string("1a");
	to = square_from_string("4d");
	m = make_move(from, to, 1, Bishop, EmptyPiece);	//31
	pos.undo_move(m);
	//3c5c rook
	from = square_from_string("3c");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//30
	pos.undo_move(m);
	//2b2a pawn pmoto Night
	from = square_from_string("2b");
	to = square_from_string("2a");
	m = make_move(from, to, 1, Pawn, Night);	//29
	pos.undo_move(m);
	//4c5b silver
	from = square_from_string("4c");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//28
	pos.undo_move(m);
	//P*2b pawn
	from = square_from_string("3d");
	to = square_from_string("2b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//27
	pos.undo_move(m);
	//5e5f pawn pawn
	from = square_from_string("5e");
	to = square_from_string("5f");
	m = make_move(from, to, 0, Pawn, Pawn);	//26
	pos.undo_move(m);
	//B*1a bishop
	from = square_from_string("3d");
	to = square_from_string("1a");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//25
	pos.undo_move(m);
	//3d4c silver
	from = square_from_string("3d");
	to = square_from_string("4c");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//24
	pos.undo_move(m);
	//P*3e pawn
	to = square_from_string("3e");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//23
	pos.undo_move(m);
	//5d5e pawn
	from = square_from_string("5d");
	to = square_from_string("5e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//22
	pos.undo_move(m);
	//4g3f silver pawn
	from = square_from_string("4g");
	to = square_from_string("3f");
	m = make_move(from, to, 0, Silver, Pawn);	//21
	pos.undo_move(m);
	//3e3f pawn
	from = square_from_string("3e");
	to = square_from_string("3f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//20
	pos.undo_move(m);
	//2d2f rook
	from = square_from_string("2d");
	to = square_from_string("2f");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//19
	pos.undo_move(m);
	//P*2c pawn
	from = square_from_string("2h");
	to = square_from_string("2c");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//18
	pos.undo_move(m);
	//2h2d rook pawn
	from = square_from_string("2h");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Rook, Pawn);	//17
	pos.undo_move(m);
	//4c3c rook bishop
	from = square_from_string("4c");
	to = square_from_string("3c");
	m = make_move(from, to, 0, Rook, Bishop);	//16
	pos.undo_move(m);
	//8h3c bishop bishop
	from = square_from_string("8h");
	to = square_from_string("3c");
	m = make_move(from, to, 0, Bishop, Bishop);	//15
	pos.undo_move(m);
	//4d4c rook
	from = square_from_string("4d");
	to = square_from_string("4c");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//14
	pos.undo_move(m);
	//P*4e pawn
	from = square_from_string("4b");
	to = square_from_string("4e");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//13
	pos.undo_move(m);
	//4b4d rook
	from = square_from_string("4b");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Rook, Pawn);		//12
	pos.undo_move(m);
	//5g4f silver
	from = square_from_string("5g");
	to = square_from_string("4f");
	m = make_move(from, to, 0, Silver, EmptyPiece);		//11
	pos.undo_move(m);
	//4c3d silver
	from = square_from_string("4c");
	to = square_from_string("3d");
	m = make_move(from, to, 0, Silver, EmptyPiece);		//10
	pos.undo_move(m);
	//4e4d pawn
	from = square_from_string("4e");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Pawn, Pawn);		//9
	pos.undo_move(m);
	//3d3e pawn
	from = square_from_string("3d");
	to = square_from_string("3e");
	m = make_move(from, to, 0, Pawn, Pawn);		//8
	pos.undo_move(m);
	//3f3e pawn
	from = square_from_string("3f");
	to = square_from_string("3e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);		//7
	pos.undo_move(m);
	//2c2d pawn
	from = square_from_string("2c");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Pawn, Pawn);		//6
	pos.undo_move(m);
	//2e2d pawn
	from = square_from_string("2e");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Pawn, EmptyPiece);		//5
	pos.undo_move(m);
	//5b6c gold
	from = square_from_string("5b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Gold, EmptyPiece);	//4
	pos.undo_move(m);
	//4h4g silver
	from = square_from_string("4h");
	to = square_from_string("4g");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//3
	pos.undo_move(m);
	//8a7c night
	from = square_from_string("8a");
	to = square_from_string("7c");
	m = make_move(from, to, 0, Night, EmptyPiece);	//2
	pos.undo_move(m);
	//1g1f
	from = square_from_string("1g");
	to = square_from_string("1f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//1
	pos.undo_move(m);
	
	EXPECT_EQ(WLance, pos.get_board(A9));
	EXPECT_EQ(WNight, pos.get_board(B9));
	EXPECT_EQ(EmptyPiece, pos.get_board(C9));
	EXPECT_EQ(WGold, pos.get_board(D9));
	EXPECT_EQ(EmptyPiece, pos.get_board(E9));
	EXPECT_EQ(EmptyPiece, pos.get_board(F9));
	EXPECT_EQ(EmptyPiece, pos.get_board(G9));
	EXPECT_EQ(WNight, pos.get_board(H9));
	EXPECT_EQ(EmptyPiece, pos.get_board(I9));
	EXPECT_EQ(WKing, pos.get_board(B8));
	EXPECT_EQ(WSilver, pos.get_board(C8));
	EXPECT_EQ(WGold, pos.get_board(E8));
	EXPECT_EQ(WRook, pos.get_board(F8));
	EXPECT_EQ(WLance, pos.get_board(I8));
	EXPECT_EQ(WPawn, pos.get_board(B7));
	EXPECT_EQ(WSilver, pos.get_board(F7));
	EXPECT_EQ(WBishop, pos.get_board(G7));
	EXPECT_EQ(WPawn, pos.get_board(H7));
	EXPECT_EQ(WPawn, pos.get_board(A6));
	EXPECT_EQ(WPawn, pos.get_board(C6));
	EXPECT_EQ(WPawn, pos.get_board(D6));
	EXPECT_EQ(WPawn, pos.get_board(E6));
	EXPECT_EQ(WPawn, pos.get_board(F6));
	EXPECT_EQ(WPawn, pos.get_board(G6));
	EXPECT_EQ(WPawn, pos.get_board(I6));
	EXPECT_EQ(BPawn, pos.get_board(F5));
	EXPECT_EQ(BPawn, pos.get_board(H5));
	EXPECT_EQ(BPawn, pos.get_board(A4));
	EXPECT_EQ(BPawn, pos.get_board(C4));
	EXPECT_EQ(BPawn, pos.get_board(E4));
	EXPECT_EQ(BPawn, pos.get_board(G4));
	EXPECT_EQ(BPawn, pos.get_board(B3));
	EXPECT_EQ(BPawn, pos.get_board(D3));
	EXPECT_EQ(BPawn, pos.get_board(I3));
	EXPECT_EQ(BSilver, pos.get_board(E3));
	EXPECT_EQ(BNight, pos.get_board(G3));
	EXPECT_EQ(BBishop, pos.get_board(B2));
	EXPECT_EQ(BKing, pos.get_board(C2));
	EXPECT_EQ(BGold, pos.get_board(E2));
	EXPECT_EQ(BGold, pos.get_board(E2));
	EXPECT_EQ(BSilver, pos.get_board(F2));
	EXPECT_EQ(BRook, pos.get_board(H2));
	EXPECT_EQ(BLance, pos.get_board(A1));
	EXPECT_EQ(BNight, pos.get_board(B1));
	EXPECT_EQ(BLance, pos.get_board(I1));
	EXPECT_EQ(0, pos.get_hand(Black, Pawn));
	EXPECT_EQ(0, pos.get_hand(Black, Lance));
	EXPECT_EQ(0, pos.get_hand(Black, Night));
	EXPECT_EQ(0, pos.get_hand(Black, Silver));
	EXPECT_EQ(0, pos.get_hand(Black, Gold));
	EXPECT_EQ(0, pos.get_hand(Black, Bishop));
	EXPECT_EQ(0, pos.get_hand(Black, Rook));
	EXPECT_EQ(0, pos.get_hand(White, Pawn));
	EXPECT_EQ(0, pos.get_hand(White, Lance));
	EXPECT_EQ(0, pos.get_hand(White, Night));
	EXPECT_EQ(0, pos.get_hand(White, Silver));
	EXPECT_EQ(0, pos.get_hand(White, Gold));
	EXPECT_EQ(0, pos.get_hand(White, Bishop));
	EXPECT_EQ(0, pos.get_hand(White, Rook));
	
	EXPECT_EQ(1, pos.get_color_bit(Black, A1));
	EXPECT_EQ(1, pos.get_color_bit(Black, B1));
	EXPECT_EQ(1, pos.get_color_bit(Black, I1));
	EXPECT_EQ(1, pos.get_color_bit(Black, B2));
	EXPECT_EQ(1, pos.get_color_bit(Black, C2));
	EXPECT_EQ(1, pos.get_color_bit(Black, D2));
	EXPECT_EQ(1, pos.get_color_bit(Black, E2));
	EXPECT_EQ(1, pos.get_color_bit(Black, F2));
	EXPECT_EQ(1, pos.get_color_bit(Black, H2));
	EXPECT_EQ(1, pos.get_color_bit(Black, B3));
	EXPECT_EQ(1, pos.get_color_bit(Black, D3));
	EXPECT_EQ(1, pos.get_color_bit(Black, E3));
	EXPECT_EQ(1, pos.get_color_bit(Black, G3));
	EXPECT_EQ(1, pos.get_color_bit(Black, I3));
	EXPECT_EQ(1, pos.get_color_bit(Black, A4));
	EXPECT_EQ(1, pos.get_color_bit(Black, C4));
	EXPECT_EQ(1, pos.get_color_bit(Black, E4));
	EXPECT_EQ(1, pos.get_color_bit(Black, G4));
	EXPECT_EQ(1, pos.get_color_bit(Black, F5));
	EXPECT_EQ(1, pos.get_color_bit(Black, H5));

	EXPECT_EQ(1, pos.get_color_bit(White, A9));
	EXPECT_EQ(1, pos.get_color_bit(White, B9));
	EXPECT_EQ(1, pos.get_color_bit(White, D9));
	EXPECT_EQ(1, pos.get_color_bit(White, H9));
	EXPECT_EQ(1, pos.get_color_bit(White, B8));
	EXPECT_EQ(1, pos.get_color_bit(White, C8));
	EXPECT_EQ(1, pos.get_color_bit(White, E8));
	EXPECT_EQ(1, pos.get_color_bit(White, F8));
	EXPECT_EQ(1, pos.get_color_bit(White, I8));
	EXPECT_EQ(1, pos.get_color_bit(White, B7));
	EXPECT_EQ(1, pos.get_color_bit(White, F7));
	EXPECT_EQ(1, pos.get_color_bit(White, G7));
	EXPECT_EQ(1, pos.get_color_bit(White, H7));
	EXPECT_EQ(1, pos.get_color_bit(White, A6));
	EXPECT_EQ(1, pos.get_color_bit(White, C6));
	EXPECT_EQ(1, pos.get_color_bit(White, D6));
	EXPECT_EQ(1, pos.get_color_bit(White, E6));
	EXPECT_EQ(1, pos.get_color_bit(White, F6));
	EXPECT_EQ(1, pos.get_color_bit(White, G6));
	EXPECT_EQ(1, pos.get_color_bit(White, I6));

	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I6));
	/*
	//他の駒種は目視で確認
	pos.print_piece_bb(Pawn,"Pawn");
	pos.print_piece_bb(Lance,"Lance");
	pos.print_piece_bb(Night,"Night");
	pos.print_piece_bb(Silver,"Silver");
	pos.print_piece_bb(Bishop,"Bishop");
	pos.print_piece_bb(Rook,"Rook");
	pos.print_piece_bb(Gold,"Gold");
	pos.print_piece_bb(King,"King");
	*/
}
TEST(position, do_move)
{
	Square from,to;
	Move m;
	StateInfo st;
	using Positionns::print_board;

	//テスト問題は加藤一二三実践集より
	string ss("ln1g3n1/1ks1gr2l/1p3sbp1/p1ppppp1p/5P1P1/P1P1P1P2/1P1PS1N1P/1BKGGS1R1/LN6L b - 1");
	Position pos(ss);

	EXPECT_EQ(1, pos.get_color_bit(Black, A1));
	EXPECT_EQ(1, pos.get_color_bit(Black, B1));
	EXPECT_EQ(1, pos.get_color_bit(Black, I1));
	EXPECT_EQ(1, pos.get_color_bit(Black, B2));
	EXPECT_EQ(1, pos.get_color_bit(Black, C2));
	EXPECT_EQ(1, pos.get_color_bit(Black, D2));
	EXPECT_EQ(1, pos.get_color_bit(Black, E2));
	EXPECT_EQ(1, pos.get_color_bit(Black, F2));
	EXPECT_EQ(1, pos.get_color_bit(Black, H2));
	EXPECT_EQ(1, pos.get_color_bit(Black, B3));
	EXPECT_EQ(1, pos.get_color_bit(Black, D3));
	EXPECT_EQ(1, pos.get_color_bit(Black, E3));
	EXPECT_EQ(1, pos.get_color_bit(Black, G3));
	EXPECT_EQ(1, pos.get_color_bit(Black, I3));
	EXPECT_EQ(1, pos.get_color_bit(Black, A4));
	EXPECT_EQ(1, pos.get_color_bit(Black, C4));
	EXPECT_EQ(1, pos.get_color_bit(Black, E4));
	EXPECT_EQ(1, pos.get_color_bit(Black, G4));
	EXPECT_EQ(1, pos.get_color_bit(Black, F5));
	EXPECT_EQ(1, pos.get_color_bit(Black, H5));

	EXPECT_EQ(1, pos.get_color_bit(White, A9));
	EXPECT_EQ(1, pos.get_color_bit(White, B9));
	EXPECT_EQ(1, pos.get_color_bit(White, D9));
	EXPECT_EQ(1, pos.get_color_bit(White, H9));
	EXPECT_EQ(1, pos.get_color_bit(White, B8));
	EXPECT_EQ(1, pos.get_color_bit(White, C8));
	EXPECT_EQ(1, pos.get_color_bit(White, E8));
	EXPECT_EQ(1, pos.get_color_bit(White, F8));
	EXPECT_EQ(1, pos.get_color_bit(White, I8));
	EXPECT_EQ(1, pos.get_color_bit(White, B7));
	EXPECT_EQ(1, pos.get_color_bit(White, F7));
	EXPECT_EQ(1, pos.get_color_bit(White, G7));
	EXPECT_EQ(1, pos.get_color_bit(White, H7));
	EXPECT_EQ(1, pos.get_color_bit(White, A6));
	EXPECT_EQ(1, pos.get_color_bit(White, C6));
	EXPECT_EQ(1, pos.get_color_bit(White, D6));
	EXPECT_EQ(1, pos.get_color_bit(White, E6));
	EXPECT_EQ(1, pos.get_color_bit(White, F6));
	EXPECT_EQ(1, pos.get_color_bit(White, G6));
	EXPECT_EQ(1, pos.get_color_bit(White, I6));

	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I6));
	/*	
	他の駒種は目視で確認
	pos.print_piece_bb(Pawn,"Pawn");
	pos.print_piece_bb(Lance,"Lance");
	pos.print_piece_bb(Night,"Night");
	pos.print_piece_bb(Silver,"Silver");
	pos.print_piece_bb(Bishop,"Bishop");
	pos.print_piece_bb(Rook,"Rook");
	pos.print_piece_bb(Gold,"Gold");
	pos.print_piece_bb(King,"King");
	*/
	//1g1f
	from = square_from_string("1g");
	to = square_from_string("1f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//1
	pos.do_move(m,st);
	//8a7c night
	from = square_from_string("8a");
	to = square_from_string("7c");
	m = make_move(from, to, 0, Night, EmptyPiece);	//2
	pos.do_move(m, st);
	//4h4g silver
	from = square_from_string("4h");
	to = square_from_string("4g");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//3
	pos.do_move(m, st);
	//5b6c gold
	from = square_from_string("5b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Gold, EmptyPiece);	//4
	pos.do_move(m, st);
	//2e2d pawn
	from = square_from_string("2e");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Pawn, EmptyPiece);		//5
	pos.do_move(m, st);
	//2c2d pawn
	from = square_from_string("2c");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Pawn, Pawn);		//6
	pos.do_move(m, st);
	//3f3e pawn
	from = square_from_string("3f");
	to = square_from_string("3e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);		//7
	pos.do_move(m, st);
	//3d3e pawn
	from = square_from_string("3d");
	to = square_from_string("3e");
	m = make_move(from, to, 0, Pawn, Pawn);		//8
	pos.do_move(m, st);
	//4e4d pawn
	from = square_from_string("4e");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Pawn, Pawn);		//9
	pos.do_move(m, st);
	//4c3d silver
	from = square_from_string("4c");
	to = square_from_string("3d");
	m = make_move(from, to, 0, Silver, EmptyPiece);		//10
	pos.do_move(m, st);
	//5g4f silver
	from = square_from_string("5g");
	to = square_from_string("4f");
	m = make_move(from, to, 0, Silver, EmptyPiece);		//11
	pos.do_move(m, st);
	//4b4d rook
	from = square_from_string("4b");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Rook, Pawn);		//12
	pos.do_move(m, st);
	//P*4e pawn
	from = square_from_string("4b");
	to = square_from_string("4e");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//13
	pos.do_move(m, st);
	//4d4c rook
	from = square_from_string("4d");
	to = square_from_string("4c");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//14
	pos.do_move(m, st);
	//8h3c bishop bishop
	from = square_from_string("8h");
	to = square_from_string("3c");
	m = make_move(from, to, 0, Bishop, Bishop);	//15
	pos.do_move(m, st);
	//4c3c rook bishop
	from = square_from_string("4c");
	to = square_from_string("3c");
	m = make_move(from, to, 0, Rook, Bishop);	//16
	pos.do_move(m, st);
	//2h2d rook pawn
	from = square_from_string("2h");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Rook, Pawn);	//17
	pos.do_move(m, st);
	//P*2c pawn
	from = square_from_string("2h");
	to = square_from_string("2c");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//18
	pos.do_move(m, st);
	//2d2f rook
	from = square_from_string("2d");
	to = square_from_string("2f");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//19
	pos.do_move(m, st);
	//3e3f pawn
	from = square_from_string("3e");
	to = square_from_string("3f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//20
	pos.do_move(m, st);
	//4g3f silver pawn
	from = square_from_string("4g");
	to = square_from_string("3f");
	m = make_move(from, to, 0, Silver, Pawn);	//21
	pos.do_move(m, st);
	//5d5e pawn
	from = square_from_string("5d");
	to = square_from_string("5e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//22
	pos.do_move(m, st);
	//P*3e pawn
	to = square_from_string("3e");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//23
	pos.do_move(m, st);
	//3d4c silver
	from = square_from_string("3d");
	to = square_from_string("4c");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//24
	pos.do_move(m, st);
	//B*1a bishop
	from = square_from_string("3d");
	to = square_from_string("1a");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//25
	pos.do_move(m, st);
	//5e5f pawn pawn
	from = square_from_string("5e");
	to = square_from_string("5f");
	m = make_move(from, to, 0, Pawn, Pawn);	//26
	pos.do_move(m, st);
	//P*2b pawn
	from = square_from_string("3d");
	to = square_from_string("2b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//27
	pos.do_move(m, st);
	//4c5b silver
	from = square_from_string("4c");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//28
	pos.do_move(m, st);
	//2b2a pawn pmoto Night
	from = square_from_string("2b");
	to = square_from_string("2a");
	m = make_move(from, to, 1, Pawn, Night);	//29
	pos.do_move(m, st);
	//3c5c rook
	from = square_from_string("3c");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//30
	pos.do_move(m, st);
	//1a4d bishop pmoto 
	from = square_from_string("1a");
	to = square_from_string("4d");
	m = make_move(from, to, 1, Bishop, EmptyPiece);	//31
	pos.do_move(m, st);
	//7c6e night
	from = square_from_string("7c");
	to = square_from_string("6e");
	m = make_move(from, to, 0, Night, EmptyPiece);	//32
	pos.do_move(m, st);
	//N*6i night
	from = square_from_string("7c");
	to = square_from_string("6i");
	m = make_move(drop_piece_from(Night), to, 0, Night, EmptyPiece);	//33
	pos.do_move(m, st);
	//B*3i bishop
	from = square_from_string("7c");
	to = square_from_string("3i");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//34
	pos.do_move(m, st);
	//4d5c Horse rook
	from = square_from_string("4d");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Horse, Rook);	//35
	pos.do_move(m, st);
	//5b5c silver Horse
	from = square_from_string("5b");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Silver, Horse);	//36
	pos.do_move(m, st);
	//2f2c rook pmoto pawn
	from = square_from_string("2f");
	to = square_from_string("2c");
	m = make_move(from, to, 1, Rook, Pawn);	//37
	pos.do_move(m, st);
	//9d9e pawn
	from = square_from_string("9d");
	to = square_from_string("9e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//38
	pos.do_move(m, st);
	//2c1b doragon lance 
	from = square_from_string("2c");
	to = square_from_string("1b");
	m = make_move(from, to, 0, Dragon, Lance);	//39
	pos.do_move(m, st);
	//9e9f pawn pawn
	from = square_from_string("9e");
	to = square_from_string("9f");
	m = make_move(from, to, 0, Pawn, Pawn);	//40
	pos.do_move(m, st);
	//P*9h pawn
	from = square_from_string("9e");
	to = square_from_string("9h");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//41
	pos.do_move(m, st);
	//3i8d bishop pmoto 
	from = square_from_string("3i");
	to = square_from_string("8d");
	m = make_move(from, to, 1, Bishop, EmptyPiece);	//42
	pos.do_move(m, st);
	//L*8f lance
	from = square_from_string("3i");
	to = square_from_string("8f");
	m = make_move(drop_piece_from(Lance), to, 0, Lance, EmptyPiece);	//43
	pos.do_move(m, st);
	//8d9c Horse
	from = square_from_string("8d");
	to = square_from_string("9c");
	m = make_move(from, to, 1, Horse, EmptyPiece);	//44
	pos.do_move(m, st);
	//4e4d pawn
	from = square_from_string("4e");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//45
	pos.do_move(m, st);
	//P*4b pawn
	from = square_from_string("4e");
	to = square_from_string("4b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//46
	pos.do_move(m, st);
	//3g4e night
	from = square_from_string("3g");
	to = square_from_string("4e");
	m = make_move(from, to, 0, Night, EmptyPiece);	//47
	pos.do_move(m, st);
	//5c6b silver
	from = square_from_string("5c");
	to = square_from_string("6b");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//48
	pos.do_move(m, st);
	//R*2c rook
	from = square_from_string("5c");
	to = square_from_string("2c");
	m = make_move(drop_piece_from(Rook), to, 0, Rook, EmptyPiece);	//49
	pos.do_move(m, st);
	//P*3g pawn
	from = square_from_string("5c");
	to = square_from_string("3g");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//50
	pos.do_move(m, st);
	//3f4g silver
	from = square_from_string("3f");
	to = square_from_string("4g");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//51
	pos.do_move(m, st);
	//B*4i bishop
	from = square_from_string("3f");
	to = square_from_string("4i");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//52
	pos.do_move(m, st);
	//1b4b dragon pawn
	from = square_from_string("1b");
	to = square_from_string("4b");
	m = make_move(from, to, 0, Dragon, Pawn);	//53
	pos.do_move(m, st);
	//3g3h pawn pmoto
	from = square_from_string("3g");
	to = square_from_string("3h");
	m = make_move(from, to, 1, Pawn, EmptyPiece);	//54
	pos.do_move(m, st);
	//2c6c rook pmoto gold
	from = square_from_string("2c");
	to = square_from_string("6c");
	m = make_move(from, to, 1, Rook, Gold);	//55
	pos.do_move(m, st);
	//6b6c silver Dragon
	from = square_from_string("6b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Silver, Dragon);	//56
	pos.do_move(m, st);
	//4e5c night pmoto
	from = square_from_string("4e");
	to = square_from_string("5c");
	m = make_move(from, to, 1, Night, EmptyPiece);	//57
	pos.do_move(m, st);
	//P*4a pawn
	from = square_from_string("4e");
	to = square_from_string("4a");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//58
	pos.do_move(m, st);
	//4b2b dragon
	from = square_from_string("4b");
	to = square_from_string("2b");
	m = make_move(from, to, 0, Dragon, EmptyPiece);	//59
	pos.do_move(m, st);
	//4i5h bishop pmoto gold
	from = square_from_string("4i");
	to = square_from_string("5h");
	m = make_move(from, to, 1, Bishop, Gold);	//60
	pos.do_move(m, st);
	//4g5h silver bishop
	from = square_from_string("4g");
	to = square_from_string("5h");
	m = make_move(from, to, 0, Silver, Horse);	//61
	pos.do_move(m, st);
	//P*3b Pawn
	from = square_from_string("5c");
	to = square_from_string("3b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//62
	pos.do_move(m, st);
	//5c6c PRoNight Silver
	from = square_from_string("5c");
	to = square_from_string("6c");
	m = make_move(from, to, 0, ProNight, Silver);	//63
	pos.do_move(m, st);	
	//7b6c Silver PRoNight
	from = square_from_string("7b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Silver, ProNight);	//64
	pos.do_move(m, st);
	//2b3b dragon pawn
	from = square_from_string("2b");
	to = square_from_string("3b");
	m = make_move(from, to, 0, Dragon, Pawn);	//65
	pos.do_move(m, st);
	//R*5b
	from = square_from_string("2b");
	to = square_from_string("5b");
	m = make_move(drop_piece_from(Rook), to, 0, Rook, EmptyPiece);	//66
	pos.do_move(m, st);
	//3b4a dragon pawn
	from = square_from_string("3b");
	to = square_from_string("4a");
	m = make_move(from, to, 0, Dragon, Pawn);	//67
	pos.do_move(m, st);
	//G*5a
	from = square_from_string("3b");
	to = square_from_string("5a");
	m = make_move(drop_piece_from(Gold), to, 0, Gold, EmptyPiece);	//68
	pos.do_move(m, st);
	//4a5b dragon rook
	from = square_from_string("4a");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Dragon, Rook);	//69
	pos.do_move(m, st);	
	//6c5b silver dragon
	from = square_from_string("6c");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Silver, Dragon);	//70
	pos.do_move(m, st);
	//B*4e bishop
	from = square_from_string("6c");
	to = square_from_string("4e");
	m = make_move(drop_piece_from(Bishop), to, 0, Silver, EmptyPiece);	//71
	pos.do_move(m, st);
	//6a7b gold
	from = square_from_string("6a");
	to = square_from_string("7b");
	m = make_move(from, to, 0, Gold, EmptyPiece);	//72
	pos.do_move(m, st);
	//8i7g night
	from = square_from_string("8i");
	to = square_from_string("7g");
	m = make_move(from, to, 0, Night, EmptyPiece);	//73
	pos.do_move(m, st);	
	//6e5g night pmoto
	from = square_from_string("6e");
	to = square_from_string("5g");
	m = make_move(from, to, 1, Night, EmptyPiece);	//74
	pos.do_move(m, st);
	//6i5g night night
	from = square_from_string("6i");
	to = square_from_string("5g");
	m = make_move(from, to, 0, Night, ProNight);	//75
	pos.do_move(m, st);
	//5f5g pawn pmoto night
	from = square_from_string("5f");
	to = square_from_string("5g");
	m = make_move(from, to, 1, Pawn, Night);	//76
	pos.do_move(m, st);
	//4f5g silver proPawn
	from = square_from_string("4f");
	to = square_from_string("5g");
	m = make_move(from, to, 0, Silver, ProPawn);	//77
	pos.do_move(m, st);
	//R*5e rook
	from = square_from_string("4f");
	to = square_from_string("5e");
	m = make_move(drop_piece_from(Rook), to, 0, Silver, EmptyPiece);	//78
	pos.do_move(m, st);
	//S*7c silver
	from = square_from_string("4f");
	to = square_from_string("7c");
	m = make_move(drop_piece_from(Silver), to, 0, Silver, EmptyPiece);	//79
	pos.do_move(m, st);
	EXPECT_EQ(WLance, pos.get_board(A9));
	EXPECT_EQ(WGold, pos.get_board(E9));
	EXPECT_EQ(BProPawn, pos.get_board(H9));
	EXPECT_EQ(WKing, pos.get_board(B8));
	EXPECT_EQ(WGold, pos.get_board(C8));
	EXPECT_EQ(WSilver, pos.get_board(E8));
	EXPECT_EQ(WHorse, pos.get_board(A7));
	EXPECT_EQ(WPawn, pos.get_board(B7));
	EXPECT_EQ(BSilver, pos.get_board(C7));
	EXPECT_EQ(WPawn, pos.get_board(C6));
	EXPECT_EQ(WPawn, pos.get_board(D6));
	EXPECT_EQ(BPawn, pos.get_board(F6));
	EXPECT_EQ(WPawn, pos.get_board(I6));
	EXPECT_EQ(WRook, pos.get_board(E5));
	EXPECT_EQ(BBishop, pos.get_board(F5));
	EXPECT_EQ(BPawn, pos.get_board(G5));
	EXPECT_EQ(WPawn, pos.get_board(A4));
	EXPECT_EQ(BLance, pos.get_board(B4));
	EXPECT_EQ(BPawn, pos.get_board(C4));
	EXPECT_EQ(BPawn, pos.get_board(I4));
	EXPECT_EQ(BPawn, pos.get_board(B3));
	EXPECT_EQ(BNight, pos.get_board(C3));
	EXPECT_EQ(BPawn, pos.get_board(D3));
	EXPECT_EQ(BSilver, pos.get_board(E3));
	EXPECT_EQ(BPawn, pos.get_board(A2));
	EXPECT_EQ(BKing, pos.get_board(C2));
	EXPECT_EQ(BGold, pos.get_board(D2));
	EXPECT_EQ(BSilver, pos.get_board(E2));
	EXPECT_EQ(WProPawn, pos.get_board(G2));
	EXPECT_EQ(BLance, pos.get_board(A1));
	EXPECT_EQ(BLance, pos.get_board(I1));

	EXPECT_EQ(1, pos.get_hand(Black,Rook));
	EXPECT_EQ(1, pos.get_hand(Black, Gold));
	EXPECT_EQ(1, pos.get_hand(Black, Night));
	EXPECT_EQ(4, pos.get_hand(Black, Pawn));
	EXPECT_EQ(2, pos.get_hand(White, Night));
	
	/*ここにbitboardのテストを書く予定*/
	EXPECT_EQ(1, pos.get_color_bit(Black, H9));
	EXPECT_EQ(1, pos.get_color_bit(Black, C7));
	EXPECT_EQ(1, pos.get_color_bit(Black, F6));
	EXPECT_EQ(1, pos.get_color_bit(Black, F5));
	EXPECT_EQ(1, pos.get_color_bit(Black, G5));
	EXPECT_EQ(1, pos.get_color_bit(Black, B4));
	EXPECT_EQ(1, pos.get_color_bit(Black, C4));
	EXPECT_EQ(1, pos.get_color_bit(Black, I4));
	EXPECT_EQ(1, pos.get_color_bit(Black, B3));
	EXPECT_EQ(1, pos.get_color_bit(Black, C3));
	EXPECT_EQ(1, pos.get_color_bit(Black, D3));
	EXPECT_EQ(1, pos.get_color_bit(Black, E3));
	EXPECT_EQ(1, pos.get_color_bit(Black, A2));
	EXPECT_EQ(1, pos.get_color_bit(Black, C2));
	EXPECT_EQ(1, pos.get_color_bit(Black, D2));
	EXPECT_EQ(1, pos.get_color_bit(Black, E2));
	EXPECT_EQ(1, pos.get_color_bit(Black, A1));
	EXPECT_EQ(1, pos.get_color_bit(Black, I1));

	EXPECT_EQ(1, pos.get_color_bit(White, A9));
	EXPECT_EQ(1, pos.get_color_bit(White, E9));
	EXPECT_EQ(1, pos.get_color_bit(White, B8));
	EXPECT_EQ(1, pos.get_color_bit(White, C8));
	EXPECT_EQ(1, pos.get_color_bit(White, E8));
	EXPECT_EQ(1, pos.get_color_bit(White, A7));
	EXPECT_EQ(1, pos.get_color_bit(White, B7));
	EXPECT_EQ(1, pos.get_color_bit(White, C6));
	EXPECT_EQ(1, pos.get_color_bit(White, D6));
	EXPECT_EQ(1, pos.get_color_bit(White, I6));
	EXPECT_EQ(1, pos.get_color_bit(White, E5));
	EXPECT_EQ(1, pos.get_color_bit(White, A4));
	EXPECT_EQ(1, pos.get_color_bit(White, G2));


	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, H9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, F5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E3));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E2));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I1));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E9));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E8));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, B7));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, C6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, D6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, I6));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, E5));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, A4));
	EXPECT_EQ(1, pos.get_piece_bit(AllPieces, G2));
	/*
	//他の駒種は目視で確認
	pos.print_piece_bb(Pawn,"Pawn");
	pos.print_piece_bb(Lance,"Lance");
	pos.print_piece_bb(Night,"Night");
	pos.print_piece_bb(Silver,"Silver");
	pos.print_piece_bb(Bishop,"Bishop");
	pos.print_piece_bb(Rook,"Rook");
	pos.print_piece_bb(Gold,"Gold");
	pos.print_piece_bb(King,"King");
	pos.print_piece_bb(ProPawn,"PRoPawn");
	pos.print_piece_bb(ProLance,"ProLance");
	pos.print_piece_bb(ProNight,"ProNight");
	pos.print_piece_bb(ProSilver,"ProSilver");
	pos.print_piece_bb(Horse,"Horse");
	pos.print_piece_bb(Dragon,"Dragon");
	*/
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
	駒文字（PieceToChar）のインデックスと駒]コードが一致することを確認している
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