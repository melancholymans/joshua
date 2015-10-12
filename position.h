#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"
#include "bitboard.h"
//ヘッダファイルで名前空間を宣言しているが例外として認める
using std::string;
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

//do_moveによって変更された局面をundo_moveで復元するときに必要な情報
struct StateInfo{
public:
	//ひとつ上のレベルへのリンク
	StateInfo* previous;
	//手番側のKINGにチエックをかけている敵側駒のbitboard
	BitBoard checkers_bb;
	Key board_key;
	Key hand_key;
};

extern const int hand_packed[8];
extern const int hand_shift[8];
extern const int hand_masking[8];

class Position{
public:
	Position(){}
	Position(const Position& p);

	Position(const string& sfen)
	{ 
		position_from_sfen(sfen);
	}
	void position_from_sfen(const string &sfen);
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
	Square get_king_square(Color c)
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
	int get_hand(Color c, PieceType pt) const
	{
		_ASSERT(pt < 8);
		return (hand[c] & hand_masking[pt]) >> hand_shift[pt];
	}
	//指定したカラー、駒種の有無を取得,駒数は不要
	bool is_hand(Color c, PieceType pt)
	{
		_ASSERT(pt < 8);
		return static_cast<bool>(hand[c] & hand_masking[pt]);
	}
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
	//駒がない座標のbitをonにして駒がある座標のbitをoffにする
	BitBoard inver_bit_bb() const
	{
		return ~all_bb();
	}
	//座標sqに移動可能な局面bitboardを返す
	BitBoard attackers_to(const Square sq, const BitBoard& occ) const;
	//座標sqにいる駒種ptからの利きbitboardを返す。bitboardクラスに直接アクセスできない場合このラッパー関数群を呼び出す
	BitBoard attackers_from_pawn(const Color c, Square sq) const {return make_pawn_attack(c, sq);}
	BitBoard attackers_from_lance(const Color c, Square sq, BitBoard& occ) const { return make_lance_attack(c, sq, occ); }
	BitBoard attackers_from_night(const Color c, Square sq) const{return make_night_attack(c, sq);}
	BitBoard attackers_from_silver(const Color c, Square sq) const{return make_silver_attack(c, sq);}
	BitBoard attackers_from_bishop(Square sq) const
	{
		return make_bishop_attack(sq,all_bb());
	}
	BitBoard attackers_from_bishop(Square sq, BitBoard& occ) const{ return make_bishop_attack(sq, occ); }
	BitBoard attackers_from_rook(Square sq) const
	{ 
		return make_rook_attack(sq, all_bb()); 
	}
	BitBoard attackers_from_rook(Square sq, BitBoard& occ) const{ return make_rook_attack(sq, occ); }
	BitBoard attackers_from_gold(const Color c, Square sq) const{return make_gold_attack(c,sq);}
	BitBoard attackers_from_king(Square sq) const{return make_king_attack(sq);}
	BitBoard attackers_from_propawn(const Color c, Square sq) const{return make_gold_attack(c, sq);}
	BitBoard attackers_from_prolance(const Color c, Square sq) const{return make_gold_attack(c, sq);}
	BitBoard attackers_from_pronight(const Color c, Square sq) const{return make_gold_attack(c, sq);}
	BitBoard attackers_from_prosilver(const Color c, Square sq) const{return make_gold_attack(c, sq);}
	BitBoard attackers_from_horse(Square sq) const
	{
		return make_horse_attack(sq, all_bb()); 
	}
	BitBoard attackers_from_horse(Square sq, BitBoard& occ) const{ return make_horse_attack(sq, occ); }
	BitBoard attackers_from_dragon(Square sq) const
	{
		return make_dragon_attack(sq, all_bb());
	}
	BitBoard attackers_from_dragon(Square sq, BitBoard& occ) const{ return make_dragon_attack(sq, occ); }
	//全ての駒種に対応するattacks_from関数(汎用）
	BitBoard attacks_from(const Color c, const Square sq, const PieceType pt, const BitBoard& occ);
	//局面を更新
	void do_move(const Move m, StateInfo& st);
	//局面を復元
	void undo_move(const Move m);
	void add_hand(Color c, PieceType pt)
	{
		_ASSERT(pt < 8);
		hand[c] += hand_packed[pt];
	}
	void sub_hand(Color c, PieceType pt)
	{
		_ASSERT(pt < 8);
		hand[c] -= hand_packed[pt];
	}
	Key get_board_key() const
	{
		return m_st->board_key;
	}
	Key get_hand_key() const
	{
		return m_st->hand_key;
	}
#ifdef _DEBUG
	bool get_color_bit(const Color c, const Square sq);
	bool get_piece_bit(const PieceType pt, const Square sq);
	void print_color_bb(Color c,string msg);
	void print_piece_bb(PieceType pt,string msg);
	void print_bb(BitBoard& bb, string msg);
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
};

namespace Positionns
{
	void init();
	void is_ok(Position& pos);
	void print_board(const Position &pos);
	Key make_board_key(const Position& pos);
	Key make_hand_key(const Position& pos);
	Key get_zobrist(const PieceType pt, const Square sq, const Color c);
	Key get_zob_hand(const PieceType hp, const Color c);
	Key get_zob_turn();
}
#endif

