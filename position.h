﻿#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"
#include "bitboard.h"
//ヘッダファイルで名前空間を宣言しているが例外として認める
using BitBoardns::make_pawn_attack;
using BitBoardns::make_lance_attack;
using BitBoardns::make_night_attack;
using BitBoardns::make_silver_attack;
using BitBoardns::make_bishop_attack;
using BitBoardns::make_rook_attack;
using BitBoardns::make_gold_attack;
using BitBoardns::make_king_attack;
using BitBoardns::make_horse_attack;
using BitBoardns::make_dragon_attack;
using BitBoardns::make_between_bb;
using BitBoardns::make_square_bb;
using BitBoardns::get_lance_attack_no_occ;
using BitBoardns::get_bishop_attack_no_occ;
using BitBoardns::get_rook_attack_no_occ;
//Positionクラスの前方宣言
class Position;
class Thread;
struct Searcher;
//check関係のクラス
struct CheckInfo{
	explicit CheckInfo(const Position&);
	std::pair<BitBoard, BitBoard> dc_bb;		//王手を止めているus側の駒（開き王手）と射抜いている駒の両方を保持
	BitBoard pinned_bb;	//pinされているus側の駒
	BitBoard check_bb[PieceTypeNum];
};
//do_moveによって変更された局面をundo_moveで復元するときに必要な情報
struct StateInfo{
public:
	//ひとつ上のレベルへのリンク
	StateInfo* previous;
	//手番側のKINGにチエックをかけている敵側駒のbitboard
	BitBoard checker_bb;
	Key_t board_key;
	Key_t hand_key;
};
//StateInfo構造体をstackに保持したデータをユニークポインタ(unique_ptr)の別定義
typedef std::unique_ptr<std::stack<StateInfo> > StateStackPtr;

class Position{
public:
	Position(){}
	Position(const Position& p);
	Position(const Position& pos, Thread* th)
	{
		*this = pos;
		this_thread = th;
	}
	Position(const string& sfen, Thread* th,Searcher* sech)
	{
		position_from_sfen(sfen,th,sech);
	}
	/*
	Position(const string& sfen)
	{ 
		position_from_sfen(sfen);
	}
	*/
	void position_from_sfen(const string &sfen, Thread* th, Searcher* sech);
	//手番を設定
	void set_color_turn(Color c)
	{
		color_turn = c;
	}
	//現在の手番
	Color get_color_turn(void) const
	{
		return Color(color_turn);
	}
	//手番を変える
	void flip_color(void)
	{
		color_turn = color_turn ^ 1;
	}
	//指定した座標の駒コードを返す
	int get_board(int sq) const
	{
		_ASSERT(sq < 81);
		return board[sq];
	}
	//指定したカラーのKINGの座標を返す
	Square get_king_square(Color c) const
	{
		return king_square[c];
	}
	//駒台の駒数を加減算する			駒種		マスクbit				シフトなしのマスクbit
	//xxxxxxxx xxxxxxxx xxx11111	pawn	0x1F					0x1F
	//xxxxxxxx xxxxxxxx 111xxxxx	lance	0x07(シフトしているので)	0xE0
	//xxxxxxxx xxxxx111 xxxxxxxx	night	0x07(シフトしているので)	0x700
	//xxxxxxxx xx111xxx xxxxxxxx	silver	0x07(シフトしているので)	0x3800
	//xxxxxxxx 11xxxxxx xxxxxxxx	bishop　0x03(シフトしているので)	0xC000
	//xxxxxx11 xxxxxxxx xxxxxxxx	rook	0x03(シフトしているので)	0x30000
	//xxx111xx xxxxxxxx xxxxxxxx	gold	0x07(シフトしているので)	0x1C0000
	//指定したカラー、駒種の駒数を取得
	int get_hand(Color c, PieceType pt) const;
	//指定したカラー、駒種の有無を取得,駒数は不要
	bool is_hand(Color c, PieceType pt) const;
	//カラー、駒種に関係なく、全ての局面bitboardを返す
	BitBoard all_bb() const
	{
		return by_type_bb[AllPieces];
	}
	//指定したカラーの局面bitboardを返す
	BitBoard color_of_bb(const Color c) const
	{
		return by_color_bb[c];
	}
	//カラーに関係なく指定した駒種の局面bitboardを返す
	BitBoard piece_type_of_bb(const PieceType pt) const
	{
		return by_type_bb[pt];
	}
	//指定したカラー、指定した駒種の局面bitboardを返す
	BitBoard color_type_of_bb(const Color c, const PieceType pt) const
	{
		return by_color_bb[c] & by_type_bb[pt];
	}
	//駒がない座標のbitをonにして駒がある座標のbitをoffにする
	BitBoard inver_bit_bb() const
	{
		return ~all_bb();
	}
	//カラーに関係なく座標sqに移動可能な局面bitboardを返す
	BitBoard attackers_to(const Square sq, const BitBoard& occ) const;
	//指定したカラーで座標sqに移動可能な局面bitboardを返す
	BitBoard attackers_to(const Color c, const Square sq, const BitBoard& occ) const;
	//指定したカラーで座標sqに移動可能な局面bitboardを返す(但しkingは除く)
	BitBoard attackers_to_excluded_of_king(const Color c, const Square sq, const BitBoard& occ) const;
	//座標sqにいる駒種ptからの利きbitboardを返す。bitboardクラスに直接アクセスできない場合このラッパー関数群を呼び出す
	//飛び駒用の関数は２とうりの実装がある。occ bitboardを引数にする関数と、positionクラスのby_type_bb[AllPiece]を使い引数はとらない関数
	BitBoard attackers_from_pawn(const Color c, const Square sq) const {return make_pawn_attack(c, sq);}
	//Pawnのbitboardを一斉に動かす。Black側Pawnは左シフト、White側Pawnは右シフト
	BitBoard attackers_from_pawns(const Color c, BitBoard& from_bb) const { return (c == Black) ? (from_bb >> 1) : (from_bb << 1); }
	//指定したカラー指定した座標,指定した局面bitboardでのlanceの利きbitboardを返す
	BitBoard attackers_from_lance(const Color c, const Square sq, const BitBoard& occ) const { return make_lance_attack(c, sq, occ); }
	//指定したカラー指定した座標,でのnightの利きbitboardを返す
	BitBoard attackers_from_night(const Color c, const Square sq) const{ return make_night_attack(c, sq); }
	//指定したカラー指定した座標,でのsilverの利きbitboardを返す
	BitBoard attackers_from_silver(const Color c, const Square sq) const{return make_silver_attack(c, sq);}
	//現在の局面で、指定した座標でのbishopの利きbitboardを返す
	BitBoard attackers_from_bishop(const Square sq) const
	{
		return make_bishop_attack(sq,all_bb());
	}
	//指定した座標,指定した局面bitboardでのbishopの利きbitboardを返す
	BitBoard attackers_from_bishop(const Square sq, const BitBoard& occ) const{ return make_bishop_attack(sq, occ); }
	//現在の局面で、指定した座標でのrookの利きbitboardを返す
	BitBoard attackers_from_rook(const Square sq) const
	{ 
		return make_rook_attack(sq, all_bb()); 
	}
	//指定した座標,指定した局面bitboardでのrookの利きbitboardを返す
	BitBoard attackers_from_rook(const Square sq, const BitBoard& occ) const{ return make_rook_attack(sq, occ); }
	//指定したカラー指定した座標でのgoldの利きbitboardを返す
	BitBoard attackers_from_gold(const Color c, const Square sq) const{ return make_gold_attack(c, sq); }
	//指定した座標でのkingの利きbitboardを返す
	BitBoard attackers_from_king(const Square sq) const{ return make_king_attack(sq); }
	//指定したカラー指定した座標でのpropawnの利きbitboardを返す
	BitBoard attackers_from_propawn(const Color c, const Square sq) const{ return make_gold_attack(c, sq); }
	//指定したカラー指定した座標でのprolanceの利きbitboardを返す
	BitBoard attackers_from_prolance(const Color c, const Square sq) const{ return make_gold_attack(c, sq); }
	//指定したカラー指定した座標でのprolanceの利きbitboardを返す
	BitBoard attackers_from_pronight(const Color c, const Square sq) const{ return make_gold_attack(c, sq); }
	//指定したカラー指定した座標でのprosilverの利きbitboardを返す
	BitBoard attackers_from_prosilver(const Color c, const Square sq) const{ return make_gold_attack(c, sq); }
	//指定した座標でのhorseの利きbitboardを返す
	BitBoard attackers_from_horse(const Square sq) const
	{
		return make_horse_attack(sq, all_bb()); 
	}
	//指定した座標,指定した局面bitboardでのhorseの利きbitboardを返す
	BitBoard attackers_from_horse(const Square sq, const BitBoard& occ) const{ return make_horse_attack(sq, occ); }
	//指定した座標でのdragonの利きbitboardを返す
	BitBoard attackers_from_dragon(const Square sq) const
	{
		return make_dragon_attack(sq, all_bb());
	}
	//指定した座標,指定した局面bitboardでのdragonの利きbitboardを返す
	BitBoard attackers_from_dragon(const Square sq, const BitBoard& occ) const{ return make_dragon_attack(sq, occ); }
	//全ての駒種に対応するattacks_from関数(汎用）
	BitBoard attacks_from(const Color c, const Square sq, const PieceType pt, const BitBoard& occ);
	//局面を更新
	void do_move(const Move m, StateInfo& st);
	//局面を復元
	void undo_move(const Move m);
	void add_hand(Color c, PieceType pt);
	void sub_hand(Color c, PieceType pt);
	Key_t get_board_key() const
	{
		return m_st->board_key;
	}
	Key_t get_hand_key() const
	{
		return m_st->hand_key;
	}
	//us側でpinされている駒の局面bitboardを返す
	//動けない駒のことをpin,kingとpin駒を射抜いている駒をpierce(them側)と呼称する
	BitBoard pinned_bb() const
	{
		BitBoard pin(0x00, 0x00);
		const Color us = Color(color_turn);
		const Color them = over_turn(us);

		const Square ksq = king_square[us];
		BitBoard pierce = ((by_type_bb[Lance] & get_lance_attack_no_occ(us, ksq)) |
			((by_type_bb[Bishop] | by_type_bb[Horse]) & get_bishop_attack_no_occ(ksq)) |
			((by_type_bb[Rook] | by_type_bb[Dragon]) & get_rook_attack_no_occ(ksq))) & by_color_bb[them];
		while (pierce.is_not_zero()){
			const Square sq = pierce.first_one();
			const BitBoard between = make_between_bb(sq, ksq) & by_type_bb[AllPieces];
			if (between.is_one_bit()){
				pin |= between & color_of_bb(us);
			}
		}
		return pin;
	}
	//us側駒(pin駒と呼称）が動くとthem kingに王手できるpin駒の局面bitboardを返す。開き王手(discovered check)のこと
	//動けない駒のことをpin(us),kingとpin駒を射抜いている駒をpierce(us)と呼称する.このpin駒(pair.first)とpierce駒(pair.second)を同時に返す
	std::pair<BitBoard, BitBoard> discovered_bb() const
	{
		BitBoard a(0x00, 0x00); BitBoard b(0x00, 0x00);
		std::pair<BitBoard, BitBoard> pin_pierce(a,b);
		const Color us = Color(color_turn);
		const Color them = over_turn(us);

		const Square ksq = king_square[them];
		BitBoard pierce = ((by_type_bb[Lance] & get_lance_attack_no_occ(them, ksq)) |
			((by_type_bb[Bishop] | by_type_bb[Horse]) & get_bishop_attack_no_occ(ksq)) |
			((by_type_bb[Rook] | by_type_bb[Dragon]) & get_rook_attack_no_occ(ksq))) & by_color_bb[us];
		while(pierce.is_not_zero()){
			const Square sq = pierce.first_one();
			const BitBoard between = make_between_bb(sq, ksq) & by_color_bb[us];
			if (between.is_one_bit()){
				pin_pierce.first |= between;
				pin_pierce.second |= make_square_bb(sq);
			}

		}
		return pin_pierce;
	}
	//指定されたMoveが王手となる手ならtrueを返す
	bool move_gives_check(const Move m, const CheckInfo& ci) const;
	//指定されたさし手が開き王手ならtrueを返す
	bool is_discovered_check(const Square from, const Square to, const Square ksq, const BitBoard& dc_bb) const;
	//do_move関数で検出した王手をかけている駒のbitboardを返す
	BitBoard checker_bb() const
	{
		return m_st->checker_bb;
	}
	//打歩詰のときtrueを返す
	bool is_pawn_drop_checkmate(const Color c, const Square to, const Square ksq) const;
	//m_nodeを返す
	uint64_t nodes_searched() const
	{
		return m_nodes;
	}
	//postionクラス内に保持しているSearcherポインタを返す
	Searcher* get_searcher() const
	{
		return m_searcher;
	}
#ifdef _DEBUG
	bool get_color_bit(const Color c, const Square sq);
	bool get_piece_bit(const PieceType pt, const Square sq);
	void print_color_bb(Color c,string msg);
	void print_piece_bb(PieceType pt,string msg);
	void print_bb(BitBoard& bb, string msg) const;
#endif
private:
	//positionクラスをクリアにする
	void Position::clear()
	{
		memset(this, 0, sizeof(Position));
		m_st = &start_state;
	}
	void put_piece(Piece piece, Square sq);
	void put_hand(Piece pt, int num);
	//指定の駒種、カラーの駒のbitboardを更新する。指定座標が駒がいればoffにする。指定座標に駒がいなければonにする（打ち駒）
	void drop_piece_bb(const PieceType pt, const Square sq, const Color c);
	//指定の駒種、カラーの駒のbitboardを更新する。指定座標が駒がいればoffにする。指定座標に駒がいなければonにする（盤上の移動）
	void Position::move_piece_bb(const PieceType pt_from, const PieceType pt_to, const Square from, const Square to, const Color c);
	//指定の駒種、カラーの駒のbitboardを更新する。指定座標が駒がいればoffにする。指定座標に駒がいなければonにする（駒の捕獲）
	void Position::cap_piece_bb(const PieceType,const Square sq,const Color c);
	int board[SquareNum];
	unsigned int hand[ColorNum];
	Square king_square[ColorNum];
	BitBoard by_type_bb[PieceTypeNum];
	BitBoard by_color_bb[ColorNum];
	int color_turn;
	StateInfo start_state;
	StateInfo* m_st;
	//positionごとのスレッドを保管
	Thread* this_thread;
	//展開したnode数をカウントアップするためもの？
	uint64_t m_nodes;
	//Searcherを保持している
	Searcher* m_searcher;
};

namespace Positionns
{
	void init();
	void is_ok(Position& pos);
	void print_board(const Position &pos);
	Key_t make_board_key(const Position& pos);
	Key_t make_hand_key(const Position& pos);
	Key_t get_zobrist(const PieceType pt, const Square sq, const Color c);
	Key_t get_zob_hand(const PieceType hp, const Color c);
	Key_t get_zob_turn();
}
#endif

