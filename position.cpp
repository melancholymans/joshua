#include <string>
#include <sstream>
#include <array>	//do_move TEST

#include "position.h"
#include "bitboard.h"
#include "usi.h"
#include "move.h"
#include "mt64bit.h"
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
using Positionns::get_zobrist;
using Positionns::get_zob_hand;
using Positionns::get_zob_turn;
using BitBoardns::make_square_relation;
using BitBoardns::make_square_bb;
using BitBoardns::make_between_bb;
//局所定数宣言・定義
//駒の枚数をunsigned int 32bitにパッキンするための駒１つの定数
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
static Key zobrist[PieceNum][SquareNum][ColorNum];
static Key zob_hand[HandPieceNum][ColorNum];	//zob_hand配列の大きさは8とする。zob_hand[0]は使わない。配列へのindexをPieceTypeで統一するため
static Key zob_turn;

//＜ここからCheckInfoクラスの定義域＞
CheckInfo::CheckInfo(const Position& pos)
{
	const Color them = over_turn(pos.get_color_turn());
	const Square ksq = pos.get_king_square(them);
	BitBoard zero_bb(0x00, 0x00);

	//us側のpinされている駒のbitboardを返す
	pinned = pos.pinned_us_bb();
	//us側の駒が動いたらthem kingに王手をかけられる（空き王手）駒を探している
	dc_bb = pos.pinned_them_bb();
	//them kingに王手かけられるかもしれない利きbitboardを駒種ごど配列に格納している。
	//具体的な使い方としては指し手リストのto座標と、このcheck_bb配列の座標が合致していればその指し手は王手として有効
	//つまりこの配列は王手候補bitboardである。
	check_bb[Pawn] = pos.attackers_from_pawn(them, ksq);
	check_bb[Lance] = pos.attackers_from_lance(them, ksq,pos.all_bb());
	check_bb[Night] = pos.attackers_from_night(them, ksq);
	check_bb[Silver] = pos.attackers_from_silver(them, ksq);
	check_bb[Bishop] = pos.attackers_from_bishop(ksq, pos.all_bb());
	check_bb[Rook] = pos.attackers_from_rook(ksq, pos.all_bb());
	check_bb[Gold] = pos.attackers_from_gold(them, ksq);
	check_bb[King] = zero_bb;
	check_bb[ProPawn] = check_bb[Gold];
	check_bb[ProLance] = check_bb[Gold];
	check_bb[ProNight] = check_bb[Gold];
	check_bb[ProSilver] = check_bb[Gold];
	check_bb[Horse] = check_bb[Bishop] | pos.attackers_from_king(ksq);
	check_bb[Dragon] = check_bb[Rook] | pos.attackers_from_king(ksq);
}
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
	m_st->board_key = Positionns::make_board_key(*this);
	m_st->hand_key = Positionns::make_hand_key(*this);
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
//局面の更新(board,hand,king_square,color_turnだけ更新するdo_move)
void Position::do_move(const Move m, StateInfo& st)
{
#ifdef _DEBUG
	Positionns::is_ok(*this);
#endif
	Key bod_key = get_board_key();
	Key had_key = get_hand_key();
	bod_key ^= get_zob_turn();	
	const Color us = get_color_turn();
	const Square to = move_to(m);
	const PieceType pt_cap = move_cap_piece(m);
	PieceType pt_to;

	memcpy(&st, m_st, sizeof(StateInfo));
	st.previous = m_st;
	m_st = &st;
	if (is_drop(m)){	//打つ手		
		pt_to = drop_piece(m);  //打つ駒種を取り出す
		had_key -= get_zob_hand(pt_to, us);
		bod_key += get_zobrist(pt_to, to, us);
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
		bod_key -= get_zobrist(pt_from, from, us);
		bod_key += get_zobrist(pt_to, to, us);
		if (pt_cap){
			//駒をとった場合の処理
			Color them = over_turn(us);
			bod_key -= get_zobrist(pt_cap, to, them);
			had_key += get_zob_hand(PieceType(pt_cap & 0x07), us);	//pt_capが成り駒だと正常に手駒に登録されないので成りbitを削っている
			cap_piece_bb(pt_cap, to, them);
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
		if (1){
		}
		else{
		}
	}
	m_st->board_key = bod_key;
	m_st->hand_key = had_key;
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
		drop_piece_bb(pt_to, to, us);
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
		move_piece_bb(pt_from, pt_to, from, to, us);
		if (pt_cap){	//駒をとっていたときの戻し
			//bitboardの処理
			cap_piece_bb(pt_cap, to, them);
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
BitBoard Position::attacks_from(const Color c, const Square sq, const PieceType pt, const BitBoard& occ)
{
	switch (pt){
	case Pawn:return make_pawn_attack(c, sq);
	case Lance:return make_lance_attack(c, sq, occ);
	case Night:return make_night_attack(c, sq);
	case Silver:return make_silver_attack(c, sq);
	case Bishop:return make_bishop_attack(sq, occ);
	case Rook:return make_rook_attack(sq, occ);
	case Gold:
	case ProPawn:
	case ProLance:
	case ProNight:
	case ProSilver:
		return make_gold_attack(c,sq);
	case King:return make_king_attack(sq);
	case Horse:return make_horse_attack(sq, occ);
	case Dragon:return make_dragon_attack(sq, occ);
	default:_ASSERT(false);
	}
}
BitBoard Position::attackers_to(const Square sq, const BitBoard& occ) const
{
	//aperyではgoldsBBという変数が定義されておりdoMove関数でも管理されており一つの局面bitboardで全てのGold系が一括で処理できるようになっているが、現在全て把握できていないのでこの関数内だけでまとめておくにとどめ将来の改造ネタとする
	BitBoard gold_bb = piece_type_of_bb(Gold) | piece_type_of_bb(ProPawn) | piece_type_of_bb(ProLance) | piece_type_of_bb(ProNight) | piece_type_of_bb(ProSilver);

	return (((make_pawn_attack(Black, sq) & piece_type_of_bb(Pawn)) |
		(make_lance_attack(Black, sq, occ) & piece_type_of_bb(Lance)) |
		(make_night_attack(Black, sq) & piece_type_of_bb(Night)) |
		(make_silver_attack(Black, sq) & piece_type_of_bb(Silver)) |
		(make_gold_attack(Black, sq) & gold_bb)) & color_of_bb(White)) |

		(((make_pawn_attack(White, sq) & piece_type_of_bb(Pawn)) |
		(make_lance_attack(White, sq, occ) & piece_type_of_bb(Lance)) |
		(make_night_attack(White, sq) & piece_type_of_bb(Night)) |
		(make_silver_attack(White, sq) & piece_type_of_bb(Silver)) |
		(make_gold_attack(White, sq) & gold_bb)) & color_of_bb(Black)) |

		(make_bishop_attack(sq, occ) & (piece_type_of_bb(Bishop) | piece_type_of_bb(Horse))) | 
		(make_rook_attack(sq, occ) & (piece_type_of_bb(Rook) | piece_type_of_bb(Dragon))) |
		(make_king_attack(sq) & (piece_type_of_bb(King) | piece_type_of_bb(Horse) | piece_type_of_bb(Dragon)));
}
BitBoard Position::attackers_to(const Color c, const Square sq, const BitBoard& occ) const
{
	//aperyではgoldsBBという変数が定義されておりdoMove関数でも管理されており一つの局面bitboardで全てのGold系が一括で処理できるようになっているが、現在全て把握できていないのでこの関数内だけでまとめておくにとどめ将来の改造ネタとする
	BitBoard gold_bb = piece_type_of_bb(Gold) | piece_type_of_bb(ProPawn) | piece_type_of_bb(ProLance) | piece_type_of_bb(ProNight) | piece_type_of_bb(ProSilver);
	const Color them = over_turn(c);

	return ((make_pawn_attack(them, sq) & piece_type_of_bb(Pawn)) |
		(make_lance_attack(them, sq, occ) & piece_type_of_bb(Lance)) |
		(make_night_attack(them, sq) & piece_type_of_bb(Night)) |
		(make_silver_attack(them, sq) & piece_type_of_bb(Silver)) |
		(make_gold_attack(them, sq) & gold_bb) |
		(make_bishop_attack(sq, occ) & (piece_type_of_bb(Bishop) | piece_type_of_bb(Horse))) |
		(make_rook_attack(sq, occ) & (piece_type_of_bb(Rook) | piece_type_of_bb(Dragon))) | 
		(make_king_attack(sq)& (piece_type_of_bb(King) | piece_type_of_bb(Horse) | piece_type_of_bb(Dragon)))) &
		color_of_bb(c);
}
BitBoard Position::attackers_to_excluded_of_king(const Color c, const Square sq, const BitBoard& occ) const
{
	//aperyではgoldsBBという変数が定義されておりdoMove関数でも管理されており一つの局面bitboardで全てのGold系が一括で処理できるようになっているが、現在全て把握できていないのでこの関数内だけでまとめておくにとどめ将来の改造ネタとする
	BitBoard gold_bb = piece_type_of_bb(Gold) | piece_type_of_bb(ProPawn) | piece_type_of_bb(ProLance) | piece_type_of_bb(ProNight) | piece_type_of_bb(ProSilver);
	const Color them = over_turn(c);

	return ((make_pawn_attack(them, sq) & piece_type_of_bb(Pawn)) |
		(make_lance_attack(them, sq, occ) & piece_type_of_bb(Lance)) |
		(make_night_attack(them, sq) & piece_type_of_bb(Night)) |
		(make_silver_attack(them, sq) & piece_type_of_bb(Silver)) |
		(make_gold_attack(them, sq) & gold_bb) |
		(make_bishop_attack(sq, occ) & (piece_type_of_bb(Bishop) | piece_type_of_bb(Horse))) |
		(make_rook_attack(sq, occ) & (piece_type_of_bb(Rook) | piece_type_of_bb(Dragon)))) &
		color_of_bb(c);
}

#ifdef _DEBUG
//Postionクラスの指定した駒種の指定した座標のbitがonになっていればtrueを返す
bool Position::get_piece_bit(const PieceType pt, const Square sq)
{
	return by_type_bb[pt].is_bit_on(sq);
}
//Postionクラスの指定したカラーの指定した座標のbitがonになっていればtrueを返す
bool Position::get_color_bit(const Color c, const Square sq)
{
	return by_color_bb[c].is_bit_on(sq);
}
//Postionクラスの指定した駒種のbitboardを表示する
void Position::print_piece_bb(const PieceType pt, string msg)
{
	printf("mssage: %s\n", msg.c_str());
	BitBoardns::print(by_type_bb[pt]);
}
//Postionクラスの指定したカラーのbitboardを表示する
void Position::print_color_bb(Color c,string msg)
{
	printf("mssage:%s\n", msg.c_str());
	BitBoardns::print(by_color_bb[c]);
}
//指定したBitBoardを表示する
void Position::print_bb(BitBoard& bb,string msg)
{
	printf("mssage:%s\n", msg.c_str());
	BitBoardns::print(bb);
}
#endif
//＜ここからnamespace Positionnsの定義領域＞
void Positionns::init()
{
	MT64bit mt64;	//シード固定でクラス生成

	//zobrist初期化,zob_trun用にbit0番目はあけておくということらしい
	for (int pt = 0; pt < PieceNum; pt++){
		for (int sq = I9; sq < SquareNum; sq++){
			for (int c = Black; c < ColorNum; c++){
				zobrist[pt][sq][c] = mt64.random() & 0x7FFFFFFFFFFFFFFE;
			}
		}
	}
	for (int hp = Pawn; hp < HandPieceNum; hp++){
		zob_hand[hp][Black] = mt64.random() & 0x7FFFFFFFFFFFFFFE;
		zob_hand[hp][White] = mt64.random() & 0x7FFFFFFFFFFFFFFE;
	}
	zob_turn = 1ULL;
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
Key Positionns::get_zobrist(const PieceType pt, const Square sq, const Color c)
{
	return zobrist[pt][sq][c];
}
Key Positionns::get_zob_hand(const PieceType hp, const Color c)
{
	return zob_hand[hp][c];
}
Key Positionns::get_zob_turn()
{
	return zob_turn;
}
//局面のハッシュキーの初期化
Key Positionns::make_board_key(const Position& pos)
{
	
	Key result = 0;
	
	for (int sq = I9; sq < SquareNum; sq++){
		if (pos.get_board(sq) != EmptyPiece){
			result += get_zobrist(type_of_piece(Piece(pos.get_board(sq))), Square(sq),color_of_piece(Piece(pos.get_board(sq))));
		}
	}
	if (pos.get_color_turn() == White){
		result ^= get_zob_turn();
	}
	return result;
}
Key Positionns::make_hand_key(const Position& pos)
{
	Key result = 0;
	for (int hp = Pawn; hp < HandPieceNum; hp++){
		for (int c = Black; c < ColorNum; c++){
			const int hand_num = pos.get_hand(Color(c), PieceType(hp));
			result += get_zob_hand(PieceType(hp), Color(c));
		}
	}
	return result;
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
TEST(position, pinned_us_bb)
{
	//問題図は自作
	string ss("l3k4/4gP3/pNbp1pB2/2p3ppp/1P2R2n1/2P3PlP/P3+nS1P1/2r2+s1K1/L6NL b G4Pg2s2 1");
	Position pos(ss);

	CheckInfo cib(pos);
	EXPECT_EQ(cib.pinned.p(0), 0x10000008000);
	EXPECT_EQ(cib.pinned.p(1), 0x00);
	EXPECT_EQ(cib.dc_bb.p(0), 0x10000000);
	EXPECT_EQ(cib.dc_bb.p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Pawn].p(0), 0x2000000000);
	EXPECT_EQ(cib.check_bb[Pawn].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Lance].p(0), 0x2000000000);
	EXPECT_EQ(cib.check_bb[Lance].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Night].p(0), 0x800020000000);
	EXPECT_EQ(cib.check_bb[Night].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Silver].p(0), 0x402010000000);
	EXPECT_EQ(cib.check_bb[Silver].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Bishop].p(0), 0x100400010000000);
	EXPECT_EQ(cib.check_bb[Bishop].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Rook].p(0), 0x40202008040201);
	EXPECT_EQ(cib.check_bb[Rook].p(1), 0x201);
	EXPECT_EQ(cib.check_bb[Gold].p(0), 0x602018000000);
	EXPECT_EQ(cib.check_bb[Gold].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Horse].p(0), 0x100602018000000);
	EXPECT_EQ(cib.check_bb[Horse].p(1), 0x00);
	EXPECT_EQ(cib.check_bb[Dragon].p(0), 0x40602018040201);
	EXPECT_EQ(cib.check_bb[Dragon].p(1), 0x201);

	pos.flip_color();
	CheckInfo ciw(pos);
	EXPECT_EQ(ciw.pinned.p(0), 0x2000000000);
	EXPECT_EQ(ciw.pinned.p(1), 0x00);
	EXPECT_EQ(ciw.dc_bb.p(0), 0x400000000);
	EXPECT_EQ(ciw.dc_bb.p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Pawn].p(0), 0x8000);
	EXPECT_EQ(ciw.check_bb[Pawn].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Lance].p(0), 0x8000);
	EXPECT_EQ(ciw.check_bb[Lance].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Night].p(0), 0x800020);
	EXPECT_EQ(ciw.check_bb[Night].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Silver].p(0), 0x5008140);
	EXPECT_EQ(ciw.check_bb[Silver].p(1), 0x00);
	//BitBoardns::print(ciw.check_bb[Silver]);
	EXPECT_EQ(ciw.check_bb[Bishop].p(0), 0x10105000140);
	EXPECT_EQ(ciw.check_bb[Bishop].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Rook].p(0), 0x402028080);
	EXPECT_EQ(ciw.check_bb[Rook].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Gold].p(0), 0x30280C0);
	EXPECT_EQ(ciw.check_bb[Gold].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Horse].p(0), 0x101070281C0);
	EXPECT_EQ(ciw.check_bb[Horse].p(1), 0x00);
	EXPECT_EQ(ciw.check_bb[Dragon].p(0), 0x4070281C0);
	EXPECT_EQ(ciw.check_bb[Dragon].p(1), 0x00);
}
TEST(position, pinned_them_bb)
{
	EXPECT_EQ(1, 1);
}
TEST(position, attackers_to)
{
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	ack = pos.attackers_to(C9, occ);
	EXPECT_EQ(ack.p(0), 0x600000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(E9, occ);
	EXPECT_EQ(ack.p(0), 0x600000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(G9, occ);
	EXPECT_EQ(ack.p(0), 0x80200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(I9, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(A8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x200);
	ack = pos.attackers_to(B8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(C8, occ);
	EXPECT_EQ(ack.p(0), 0x600000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(F8, occ);
	EXPECT_EQ(ack.p(0), 0x8080000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H8, occ);
	EXPECT_EQ(ack.p(0), 0x80200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(I8, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(A7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x201);
	ack = pos.attackers_to(B7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(D7, occ);
	EXPECT_EQ(ack.p(0), 0x402000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H7, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(B6, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(C6, occ);
	EXPECT_EQ(ack.p(0), 0x500002000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(E6, occ);
	EXPECT_EQ(ack.p(0), 0x4040000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(G6, occ);
	EXPECT_EQ(ack.p(0), 0x2040000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H6, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(I6, occ);
	EXPECT_EQ(ack.p(0), 0x04);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(A5, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x5020);
	ack = pos.attackers_to(D5, occ);
	EXPECT_EQ(ack.p(0), 0x1020000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(E5, occ);
	EXPECT_EQ(ack.p(0), 0x1020040000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(F5, occ);
	EXPECT_EQ(ack.p(0), 0x20040100000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H5, occ);
	EXPECT_EQ(ack.p(0), 0x2000100200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(I5, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(D4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(F4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(G4, occ);
	EXPECT_EQ(ack.p(0), 0x1400000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H4, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(I4, occ);
	EXPECT_EQ(ack.p(0), 0x2000000040);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(A3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x20030);
	ack = pos.attackers_to(C3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x30);
	ack = pos.attackers_to(F3, occ);
	EXPECT_EQ(ack.p(0), 0xA0000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H3, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(A2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x20000);
	ack = pos.attackers_to(B2, occ);
	EXPECT_EQ(ack.p(0), 0x4000000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(C2, occ);
	EXPECT_EQ(ack.p(0), 0x4000000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(D2, occ);
	EXPECT_EQ(ack.p(0), 0x4008080000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(F2, occ);
	EXPECT_EQ(ack.p(0), 0x80004000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(G2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(H2, occ);
	EXPECT_EQ(ack.p(0), 0x4020200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(I2, occ);
	EXPECT_EQ(ack.p(0), 0x100);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(B1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(D1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(E1, occ);
	EXPECT_EQ(ack.p(0), 0x80000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(F1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(position, attackers_to2)
{
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	ack = pos.attackers_to(White,C9, occ);
	EXPECT_EQ(ack.p(0), 0x600000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, E9, occ);
	EXPECT_EQ(ack.p(0), 0x600000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, G9, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, I9, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, A8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x200);
	ack = pos.attackers_to(White, B8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, C8, occ);
	EXPECT_EQ(ack.p(0), 0x600000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, F8, occ);
	EXPECT_EQ(ack.p(0), 0x8000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, I8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, A7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x201);
	ack = pos.attackers_to(White, B7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, D7, occ);
	EXPECT_EQ(ack.p(0), 0x402000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, B6, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, C6, occ);
	EXPECT_EQ(ack.p(0), 0x100002000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, E6, occ);
	EXPECT_EQ(ack.p(0), 0x4040000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, G6, occ);
	EXPECT_EQ(ack.p(0), 0x2040000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H6, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, I6, occ);
	EXPECT_EQ(ack.p(0), 0x04);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, A5, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x1000);
	ack = pos.attackers_to(White, D5, occ);
	EXPECT_EQ(ack.p(0), 0x1000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, E5, occ);
	EXPECT_EQ(ack.p(0), 0x1000040000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, F5, occ);
	EXPECT_EQ(ack.p(0), 0x40100000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H5, occ);
	EXPECT_EQ(ack.p(0), 0x2000100000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, I5, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, D4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, F4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, G4, occ);
	EXPECT_EQ(ack.p(0), 0x400000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, I4, occ);
	EXPECT_EQ(ack.p(0), 0x2000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, A3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10);
	ack = pos.attackers_to(White, C3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10);
	ack = pos.attackers_to(White, F3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, A2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, B2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, C2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, D2, occ);
	EXPECT_EQ(ack.p(0), 0x8000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, F2, occ);
	EXPECT_EQ(ack.p(0), 0x4000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, G2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, H2, occ);
	EXPECT_EQ(ack.p(0), 0x4020000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, I2, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, B1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, D1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, E1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to(White, F1, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(position, attackers_to_excluded_of_king)
{
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	ack = pos.attackers_to_excluded_of_king(White, C9, occ);
	EXPECT_EQ(ack.p(0), 0x200000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, E9, occ);
	EXPECT_EQ(ack.p(0), 0x200000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, G9, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, I9, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, A8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x200);
	ack = pos.attackers_to_excluded_of_king(White, B8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, C8, occ);
	EXPECT_EQ(ack.p(0), 0x200000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, F8, occ);
	EXPECT_EQ(ack.p(0), 0x8000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, H8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, I8, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, A7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x201);
	ack = pos.attackers_to_excluded_of_king(White, B7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, D7, occ);
	EXPECT_EQ(ack.p(0), 0x2000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(White, H7, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);

	ack = pos.attackers_to_excluded_of_king(Black, A5, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x4000);
	ack = pos.attackers_to_excluded_of_king(Black, D5, occ);
	EXPECT_EQ(ack.p(0), 0x20000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, E5, occ);
	EXPECT_EQ(ack.p(0), 0x20000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, F5, occ);
	EXPECT_EQ(ack.p(0), 0x20000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, H5, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, I5, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, D4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, F4, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, G4, occ);
	EXPECT_EQ(ack.p(0), 0x1000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, H4, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, I4, occ);
	EXPECT_EQ(ack.p(0), 0x40);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, A3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x20000);
	ack = pos.attackers_to_excluded_of_king(Black, C3, occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, F3, occ);
	EXPECT_EQ(ack.p(0), 0xA0000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	ack = pos.attackers_to_excluded_of_king(Black, H3, occ);
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(position, attackers_from_pawn)
{
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	//black
	Color c = Black;
	sq = I9;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x200);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x80000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x20000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x8000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x800000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = B2;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x40);
	sq = A1;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10000);

	c = White;
	sq = I9;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x02);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x800);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x200000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x80000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x20000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x8000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2000000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = B2;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x100);
	sq = A1;
	ack = pos.attackers_from_pawn(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(position, attackers_from_night)
{
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	//black
	Color c = Black;
	sq = G7;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x8000200);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2000080000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x800020000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x200008000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2000000000000);
	EXPECT_EQ(ack.p(1), 0x10);
	sq = B2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x800000000000000);
	EXPECT_EQ(ack.p(1), 0x4000);
	sq = A1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x40);
	//できないことを再確認
	sq = I9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = B9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);

	sq = I8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = B8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	//white
	c = White;
	sq = I9;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x800);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x200008);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x80002000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x20000800000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x8000200000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2000080000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x20000000000000);
	EXPECT_EQ(ack.p(1), 0x100);
	//できないことを再確認
	sq = I2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = B2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A2;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);

	sq = I1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = B1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A1;
	ack = pos.attackers_from_night(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, attackers_from_silver)
{
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	//black
	Color c = Black;
	sq = I9;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x400);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x140205);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x50081400);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x14020500000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x5008140000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x1402050000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x814000000000000);
	EXPECT_EQ(ack.p(1), 0xA0);
	sq = B2;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x5000000000000000);
	EXPECT_EQ(ack.p(1), 0x28040);
	sq = A1;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10080);
	//white
	c = White;
	sq = I9;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x402);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x140805);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x50201400);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x14080500000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x5020140000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x1408050000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2014000000000000);
	EXPECT_EQ(ack.p(1), 0xA0);
	sq = B2;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x5000000000000000);
	EXPECT_EQ(ack.p(1), 0x28100);
	sq = A1;
	ack = pos.attackers_from_silver(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x80);
}
/*
テストを書き換える
*/

TEST(bitboard, attackers_from_gold)
{
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	//black
	Color c = Black;
	sq = I9;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x202);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0xC0A03);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x30280C00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0xC0A0300000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x30280C0000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0xC0A030000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x280C000000000000);
	EXPECT_EQ(ack.p(1), 0x60);
	sq = B2;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x3000000000000000);
	EXPECT_EQ(ack.p(1), 0x18140);
	sq = A1;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10180);
	//white
	c = White;
	sq = I9;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x602);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x180A06);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x60281800);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x180A0600000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x6028180000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x180A060000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x2818000000000000);
	EXPECT_EQ(ack.p(1), 0xC0);
	sq = B2;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x6000000000000000);
	EXPECT_EQ(ack.p(1), 0x30140);
	sq = A1;
	ack = pos.attackers_from_gold(c, Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10100);
}
TEST(bitboard, attackers_from_king)
{
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	//kingにはblack/whiteは関係ない
	sq = I9;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x602);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = H8;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x1C0A07);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G7;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x70281C00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F6;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x1C0A0700000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x70281C0000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = D4;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x1C0A070000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C3;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x281C000000000000);
	EXPECT_EQ(ack.p(1), 0xE0);
	sq = B2;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x7000000000000000);
	EXPECT_EQ(ack.p(1), 0x38140);
	sq = A1;
	ack = pos.attackers_from_king(Square(sq));
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x10180);
}
TEST(bitboard, attackers_from_lance)
{
	//lance_attack配列はlance_mask配列,lance_attack_index配列から作られる最終成果物で
	//局面bitboardの指定した座標にlanceをおいた時得られる利きbitboardを持っている
	//配列数は4,599個あるので全数テストするのは困難であるのでサンプリングしてテストする
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	//lance black
	sq = C9;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = I8;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x01);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x18000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x100000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0xC000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G4;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x400000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A3;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x4000);
	sq = C2;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x1800000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F1;
	ack = pos.attackers_from_lance(Black, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x7C0000000);
	EXPECT_EQ(ack.p(1), 0x00);
	//lance white
	sq = C9;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x180000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = I8;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x04);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x40000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x400000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = E5;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x20000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G4;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x1000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A3;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x30000);
	sq = C2;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x4000000000000000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F1;
	ack = pos.attackers_from_lance(White, Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, attackers_from_rook)
{
	//rook_attack配列はrook_mask配列,rook_attack_index配列から作られる最終成果物で
	//局面bitboardの指定した座標にrookをおいた時得られる利きbitboardを持っている
	//配列数は495,616個あるので全数テストするのは困難であるのでサンプリングしてテストする
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	sq = C9;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x180200000000000);
	EXPECT_EQ(ack.p(1), 0x01);
	sq = I8;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80405);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x4058100000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x501000000000000);
	EXPECT_EQ(ack.p(1), 0x1008);
	sq = E5;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x40202C080400000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = G4;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x20101404020);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = A3;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x00);
	EXPECT_EQ(ack.p(1), 0x34040);
	sq = C2;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x5810080000000000);
	EXPECT_EQ(ack.p(1), 0x10080);
	sq = F1;
	ack = pos.attackers_from_rook(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x40201007C4000000);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, attackers_from_bishop)
{
	//bishop_attack配列はbishop_mask配列,bishop_attack_index配列から作られる最終成果物で
	//局面bitboardの指定した座標にbishopをおいた時得られる利きbitboardを持っている
	//配列数は20,224個あるので全数テストするのは困難であるのでサンプリングしてテストする
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	sq = C9;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x400000000000);
	EXPECT_EQ(ack.p(1), 0x802);
	sq = I8;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x20080200A00);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80200A000282020);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x2822000000000);
	EXPECT_EQ(ack.p(1), 0x414);
	sq = E5;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x1005000141000000);
	EXPECT_EQ(ack.p(1), 0x20080);
	sq = G4;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x8088828000A088);
	EXPECT_EQ(ack.p(1), 0x01);
	sq = A3;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x4000000000000000);
	EXPECT_EQ(ack.p(1), 0xA0);
	sq = C2;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x28000000000000);
	EXPECT_EQ(ack.p(1), 0x140);
	sq = F1;
	ack = pos.attackers_from_bishop(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80002008020);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(bitboard, attackers_from_horse)
{
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	sq = C9;
	ack = pos.attackers_from_horse(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80600000000000);
	EXPECT_EQ(ack.p(1), 0x803);
	sq = I8;
	ack = pos.attackers_from_horse(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x20080200E05);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = pos.attackers_from_horse(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80200E050382020);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = pos.attackers_from_horse(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x503822000000000);
	EXPECT_EQ(ack.p(1), 0x41C);
	sq = E5;
	ack = pos.attackers_from_horse(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x10070281C1000000);
	EXPECT_EQ(ack.p(1), 0x20080);
}
TEST(bitboard, attackers_from_dragon)
{
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	//-+-- + -- + -- + -- + -- + -- + -- + -- + -- +
	//	A  B  C  D  E  F  G  H  I
	//9  *  *  .  *  .  *  .  * .
	//8  .  .  .  *  *  .  *  . .
	//7  .  .  *  .  *  .  *  . *
	//6  *  .  .  *  .  *  .  . .
	//5  .  *  *  .  .  .  *  . .
	//4  *  *  *  .  *  .  .  . .
	//3  .  *  .  *  *  .  *  . *
	//2	 .  .  .  .  *  .  .  . .
	//1  *  .  *  .  .  .  *  * *
	//テストはbitboard.cppと同じ
	int sq;
	BitBoard ack;
	BitBoard occ(0x4D096E604D5A0344, 0x25271);
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	Position pos(ss);

	BitBoardns::init();
	sq = C9;
	ack = pos.attackers_from_dragon(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x180600000000000);
	EXPECT_EQ(ack.p(1), 0x03);
	sq = I8;
	ack = pos.attackers_from_dragon(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x80E05);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = F7;
	ack = pos.attackers_from_dragon(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0xE058380000);
	EXPECT_EQ(ack.p(1), 0x00);
	sq = C6;
	ack = pos.attackers_from_dragon(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x503800000000000);
	EXPECT_EQ(ack.p(1), 0x101C);
	sq = E5;
	ack = pos.attackers_from_dragon(Square(sq), occ);
	EXPECT_EQ(ack.p(0), 0x40702C1C0400000);
	EXPECT_EQ(ack.p(1), 0x00);
}
TEST(position, inver_bit_bb)
{
	//bitboardのbit反転する関数
	//テスト問題は加藤一二三実践集より
	string ss("ln1g3n1/1ks1gr2l/1p3sbp1/p1ppppp1p/5P1P1/P1P1P1P2/1P1PS1N1P/1BKGGS1R1/LN6L b - 1");
	Position pos(ss);

	BitBoard bb = pos.inver_bit_bb();
	EXPECT_EQ(false, bb.is_bit_on(A9));
	EXPECT_EQ(false, bb.is_bit_on(B9));
	EXPECT_EQ(true, bb.is_bit_on(C9));
	EXPECT_EQ(false, bb.is_bit_on(D9));
	EXPECT_EQ(true, bb.is_bit_on(E9));
	EXPECT_EQ(true, bb.is_bit_on(F9));
	EXPECT_EQ(true, bb.is_bit_on(G9));
	EXPECT_EQ(false, bb.is_bit_on(H9));
	EXPECT_EQ(true, bb.is_bit_on(I9));
	EXPECT_EQ(true, bb.is_bit_on(A8));
	EXPECT_EQ(false, bb.is_bit_on(B8));
	EXPECT_EQ(false, bb.is_bit_on(C8));
	EXPECT_EQ(true, bb.is_bit_on(D8));
	EXPECT_EQ(false, bb.is_bit_on(E8));
	EXPECT_EQ(false, bb.is_bit_on(F8));
	EXPECT_EQ(true, bb.is_bit_on(G8));
	EXPECT_EQ(true, bb.is_bit_on(H8));
	EXPECT_EQ(false, bb.is_bit_on(I8));
	EXPECT_EQ(true, bb.is_bit_on(A7));
	EXPECT_EQ(false, bb.is_bit_on(B7));
	EXPECT_EQ(true, bb.is_bit_on(C7));
	EXPECT_EQ(true, bb.is_bit_on(D7));
	EXPECT_EQ(true, bb.is_bit_on(E7));
	EXPECT_EQ(false, bb.is_bit_on(F7));
	EXPECT_EQ(false, bb.is_bit_on(G7));
	EXPECT_EQ(false, bb.is_bit_on(H7));
	EXPECT_EQ(true, bb.is_bit_on(I7));
	EXPECT_EQ(false, bb.is_bit_on(A6));
	EXPECT_EQ(true, bb.is_bit_on(B6));
	EXPECT_EQ(false, bb.is_bit_on(C6));
	EXPECT_EQ(false, bb.is_bit_on(D6));
	EXPECT_EQ(false, bb.is_bit_on(E6));
	EXPECT_EQ(false, bb.is_bit_on(F6));
	EXPECT_EQ(false, bb.is_bit_on(G6));
	EXPECT_EQ(true, bb.is_bit_on(H6));
	EXPECT_EQ(false, bb.is_bit_on(I6));
	EXPECT_EQ(true, bb.is_bit_on(A5));
	EXPECT_EQ(true, bb.is_bit_on(B5));
	EXPECT_EQ(true, bb.is_bit_on(C5));
	EXPECT_EQ(true, bb.is_bit_on(D5));
	EXPECT_EQ(true, bb.is_bit_on(E5));
	EXPECT_EQ(false, bb.is_bit_on(F5));
	EXPECT_EQ(true, bb.is_bit_on(G5));
	EXPECT_EQ(false, bb.is_bit_on(H5));
	EXPECT_EQ(true, bb.is_bit_on(I5));
	EXPECT_EQ(false, bb.is_bit_on(A4));
	EXPECT_EQ(true, bb.is_bit_on(B4));
	EXPECT_EQ(false, bb.is_bit_on(C4));
	EXPECT_EQ(true, bb.is_bit_on(D4));
	EXPECT_EQ(false, bb.is_bit_on(E4));
	EXPECT_EQ(true, bb.is_bit_on(F4));
	EXPECT_EQ(false, bb.is_bit_on(G4));
	EXPECT_EQ(true, bb.is_bit_on(H4));
	EXPECT_EQ(true, bb.is_bit_on(I4));
	EXPECT_EQ(true, bb.is_bit_on(A3));
	EXPECT_EQ(false, bb.is_bit_on(B3));
	EXPECT_EQ(true, bb.is_bit_on(C3));
	EXPECT_EQ(false, bb.is_bit_on(D3));
	EXPECT_EQ(false, bb.is_bit_on(E3));
	EXPECT_EQ(true, bb.is_bit_on(F3));
	EXPECT_EQ(false, bb.is_bit_on(G3));
	EXPECT_EQ(true, bb.is_bit_on(H3));
	EXPECT_EQ(false, bb.is_bit_on(I3));
	EXPECT_EQ(true, bb.is_bit_on(A2));
	EXPECT_EQ(false, bb.is_bit_on(B2));
	EXPECT_EQ(false, bb.is_bit_on(C2));
	EXPECT_EQ(false, bb.is_bit_on(D2));
	EXPECT_EQ(false, bb.is_bit_on(E2));
	EXPECT_EQ(false, bb.is_bit_on(F2));
	EXPECT_EQ(true, bb.is_bit_on(G2));
	EXPECT_EQ(false, bb.is_bit_on(H2));
	EXPECT_EQ(true, bb.is_bit_on(I2));
	EXPECT_EQ(false, bb.is_bit_on(A1));
	EXPECT_EQ(false, bb.is_bit_on(B1));
	EXPECT_EQ(true, bb.is_bit_on(C1));
	EXPECT_EQ(true, bb.is_bit_on(D1));
	EXPECT_EQ(true, bb.is_bit_on(E1));
	EXPECT_EQ(true, bb.is_bit_on(F1));
	EXPECT_EQ(true, bb.is_bit_on(G1));
	EXPECT_EQ(true, bb.is_bit_on(H1));
	EXPECT_EQ(false, bb.is_bit_on(I1));
}
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
	Positionns::init();		//StateInfoのテストのため
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
	using std::array;
	array<StateInfo, 80> st_stack;		//stack代わりのメモリ
	int index = 0;
	//1g1f
	from = square_from_string("1g");
	to = square_from_string("1f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//1
	pos.do_move(m, st_stack[index]);
	index++;
	//8a7c night
	from = square_from_string("8a");
	to = square_from_string("7c");
	m = make_move(from, to, 0, Night, EmptyPiece);	//2
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);	//結果が乱数なので値を直接比較することができないので前の値と異なっている、更新されていること（または更新されていないこと）を確認して良好とする。
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4h4g silver
	from = square_from_string("4h");
	to = square_from_string("4g");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//3
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5b6c gold
	from = square_from_string("5b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Gold, EmptyPiece);	//4
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	//2e2d pawn
	from = square_from_string("2e");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Pawn, EmptyPiece);		//5
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	//2c2d pawn
	from = square_from_string("2c");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Pawn, Pawn);		//6
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3f3e pawn
	from = square_from_string("3f");
	to = square_from_string("3e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);		//7
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3d3e pawn
	from = square_from_string("3d");
	to = square_from_string("3e");
	m = make_move(from, to, 0, Pawn, Pawn);		//8
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4e4d pawn
	from = square_from_string("4e");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Pawn, Pawn);		//9
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4c3d silver
	from = square_from_string("4c");
	to = square_from_string("3d");
	m = make_move(from, to, 0, Silver, EmptyPiece);		//10
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5g4f silver
	from = square_from_string("5g");
	to = square_from_string("4f");
	m = make_move(from, to, 0, Silver, EmptyPiece);		//11
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4b4d rook
	from = square_from_string("4b");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Rook, Pawn);		//12
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*4e pawn
	from = square_from_string("4b");
	to = square_from_string("4e");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//13
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4d4c rook
	from = square_from_string("4d");
	to = square_from_string("4c");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//14
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//8h3c bishop bishop
	from = square_from_string("8h");
	to = square_from_string("3c");
	m = make_move(from, to, 0, Bishop, Bishop);	//15
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4c3c rook bishop
	from = square_from_string("4c");
	to = square_from_string("3c");
	m = make_move(from, to, 0, Rook, Bishop);	//16
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2h2d rook pawn
	from = square_from_string("2h");
	to = square_from_string("2d");
	m = make_move(from, to, 0, Rook, Pawn);	//17
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*2c pawn
	from = square_from_string("2h");
	to = square_from_string("2c");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//18
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2d2f rook
	from = square_from_string("2d");
	to = square_from_string("2f");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//19
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	
	//3e3f pawn
	from = square_from_string("3e");
	to = square_from_string("3f");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//20
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4g3f silver pawn
	from = square_from_string("4g");
	to = square_from_string("3f");
	m = make_move(from, to, 0, Silver, Pawn);	//21
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5d5e pawn
	from = square_from_string("5d");
	to = square_from_string("5e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//22
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*3e pawn
	to = square_from_string("3e");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//23
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3d4c silver
	from = square_from_string("3d");
	to = square_from_string("4c");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//24
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//B*1a bishop
	from = square_from_string("3d");
	to = square_from_string("1a");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//25
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5e5f pawn pawn
	from = square_from_string("5e");
	to = square_from_string("5f");
	m = make_move(from, to, 0, Pawn, Pawn);	//26
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*2b pawn
	from = square_from_string("3d");
	to = square_from_string("2b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//27
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4c5b silver
	from = square_from_string("4c");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//28
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2b2a pawn pmoto Night
	from = square_from_string("2b");
	to = square_from_string("2a");
	m = make_move(from, to, 1, Pawn, Night);	//29
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3c5c rook
	from = square_from_string("3c");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Rook, EmptyPiece);	//30
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//1a4d bishop pmoto 
	from = square_from_string("1a");
	to = square_from_string("4d");
	m = make_move(from, to, 1, Bishop, EmptyPiece);	//31
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//7c6e night
	from = square_from_string("7c");
	to = square_from_string("6e");
	m = make_move(from, to, 0, Night, EmptyPiece);	//32
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//N*6i night
	from = square_from_string("7c");
	to = square_from_string("6i");
	m = make_move(drop_piece_from(Night), to, 0, Night, EmptyPiece);	//33
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//B*3i bishop
	from = square_from_string("7c");
	to = square_from_string("3i");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//34
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4d5c Horse rook
	from = square_from_string("4d");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Horse, Rook);	//35
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5b5c silver Horse
	from = square_from_string("5b");
	to = square_from_string("5c");
	m = make_move(from, to, 0, Silver, Horse);	//36
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2f2c rook pmoto pawn
	from = square_from_string("2f");
	to = square_from_string("2c");
	m = make_move(from, to, 1, Rook, Pawn);	//37
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//9d9e pawn
	from = square_from_string("9d");
	to = square_from_string("9e");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//38
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2c1b doragon lance 
	from = square_from_string("2c");
	to = square_from_string("1b");
	m = make_move(from, to, 0, Dragon, Lance);	//39
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//9e9f pawn pawn
	from = square_from_string("9e");
	to = square_from_string("9f");
	m = make_move(from, to, 0, Pawn, Pawn);	//40
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*9h pawn
	from = square_from_string("9e");
	to = square_from_string("9h");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//41
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3i8d bishop pmoto 
	from = square_from_string("3i");
	to = square_from_string("8d");
	m = make_move(from, to, 1, Bishop, EmptyPiece);	//42
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//L*8f lance
	from = square_from_string("3i");
	to = square_from_string("8f");
	m = make_move(drop_piece_from(Lance), to, 0, Lance, EmptyPiece);	//43
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//8d9c Horse
	from = square_from_string("8d");
	to = square_from_string("9c");
	m = make_move(from, to, 1, Horse, EmptyPiece);	//44
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4e4d pawn
	from = square_from_string("4e");
	to = square_from_string("4d");
	m = make_move(from, to, 0, Pawn, EmptyPiece);	//45
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*4b pawn
	from = square_from_string("4e");
	to = square_from_string("4b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//46
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3g4e night
	from = square_from_string("3g");
	to = square_from_string("4e");
	m = make_move(from, to, 0, Night, EmptyPiece);	//47
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5c6b silver
	from = square_from_string("5c");
	to = square_from_string("6b");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//48
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//R*2c rook
	from = square_from_string("5c");
	to = square_from_string("2c");
	m = make_move(drop_piece_from(Rook), to, 0, Rook, EmptyPiece);	//49
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*3g pawn
	from = square_from_string("5c");
	to = square_from_string("3g");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//50
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3f4g silver
	from = square_from_string("3f");
	to = square_from_string("4g");
	m = make_move(from, to, 0, Silver, EmptyPiece);	//51
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//B*4i bishop
	from = square_from_string("3f");
	to = square_from_string("4i");
	m = make_move(drop_piece_from(Bishop), to, 0, Bishop, EmptyPiece);	//52
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//1b4b dragon pawn
	from = square_from_string("1b");
	to = square_from_string("4b");
	m = make_move(from, to, 0, Dragon, Pawn);	//53
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3g3h pawn pmoto
	from = square_from_string("3g");
	to = square_from_string("3h");
	m = make_move(from, to, 1, Pawn, EmptyPiece);	//54
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2c6c rook pmoto gold
	from = square_from_string("2c");
	to = square_from_string("6c");
	m = make_move(from, to, 1, Rook, Gold);	//55
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//6b6c silver Dragon
	from = square_from_string("6b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Silver, Dragon);	//56
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4e5c night pmoto
	from = square_from_string("4e");
	to = square_from_string("5c");
	m = make_move(from, to, 1, Night, EmptyPiece);	//57
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*4a pawn
	from = square_from_string("4e");
	to = square_from_string("4a");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//58
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4b2b dragon
	from = square_from_string("4b");
	to = square_from_string("2b");
	m = make_move(from, to, 0, Dragon, EmptyPiece);	//59
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4i5h bishop pmoto gold
	from = square_from_string("4i");
	to = square_from_string("5h");
	m = make_move(from, to, 1, Bishop, Gold);	//60
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4g5h silver bishop
	from = square_from_string("4g");
	to = square_from_string("5h");
	m = make_move(from, to, 0, Silver, Horse);	//61
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//P*3b Pawn
	from = square_from_string("5c");
	to = square_from_string("3b");
	m = make_move(drop_piece_from(Pawn), to, 0, Pawn, EmptyPiece);	//62
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5c6c PRoNight Silver
	from = square_from_string("5c");
	to = square_from_string("6c");
	m = make_move(from, to, 0, ProNight, Silver);	//63
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//7b6c Silver PRoNight
	from = square_from_string("7b");
	to = square_from_string("6c");
	m = make_move(from, to, 0, Silver, ProNight);	//64
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//2b3b dragon pawn
	from = square_from_string("2b");
	to = square_from_string("3b");
	m = make_move(from, to, 0, Dragon, Pawn);	//65
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//R*5b
	from = square_from_string("2b");
	to = square_from_string("5b");
	m = make_move(drop_piece_from(Rook), to, 0, Rook, EmptyPiece);	//66
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//3b4a dragon pawn
	from = square_from_string("3b");
	to = square_from_string("4a");
	m = make_move(from, to, 0, Dragon, Pawn);	//67
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//G*5a
	from = square_from_string("3b");
	to = square_from_string("5a");
	m = make_move(drop_piece_from(Gold), to, 0, Gold, EmptyPiece);	//68
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4a5b dragon rook
	from = square_from_string("4a");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Dragon, Rook);	//69
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//6c5b silver dragon
	from = square_from_string("6c");
	to = square_from_string("5b");
	m = make_move(from, to, 0, Silver, Dragon);	//70
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//B*4e bishop
	from = square_from_string("6c");
	to = square_from_string("4e");
	m = make_move(drop_piece_from(Bishop), to, 0, Silver, EmptyPiece);	//71
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//6a7b gold
	from = square_from_string("6a");
	to = square_from_string("7b");
	m = make_move(from, to, 0, Gold, EmptyPiece);	//72
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//8i7g night
	from = square_from_string("8i");
	to = square_from_string("7g");
	m = make_move(from, to, 0, Night, EmptyPiece);	//73
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//6e5g night pmoto
	from = square_from_string("6e");
	to = square_from_string("5g");
	m = make_move(from, to, 1, Night, EmptyPiece);	//74
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_EQ(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//6i5g night night
	from = square_from_string("6i");
	to = square_from_string("5g");
	m = make_move(from, to, 0, Night, ProNight);	//75
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//5f5g pawn pmoto night
	from = square_from_string("5f");
	to = square_from_string("5g");
	m = make_move(from, to, 1, Pawn, Night);	//76
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//4f5g silver proPawn
	from = square_from_string("4f");
	to = square_from_string("5g");
	m = make_move(from, to, 0, Silver, ProPawn);	//77
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//R*5e rook
	from = square_from_string("4f");
	to = square_from_string("5e");
	m = make_move(drop_piece_from(Rook), to, 0, Silver, EmptyPiece);	//78
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	index++;
	//S*7c silver
	from = square_from_string("4f");
	to = square_from_string("7c");
	m = make_move(drop_piece_from(Silver), to, 0, Silver, EmptyPiece);	//79
	pos.do_move(m, st_stack[index]);
	EXPECT_NE(st_stack[index].board_key, st_stack[index - 1].board_key);
	EXPECT_NE(st_stack[index].hand_key, st_stack[index - 1].hand_key);
	
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