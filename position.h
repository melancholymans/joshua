#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"
#include "bitboard.h"

using std::string;

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
	Color Position::get_color_turn(void)
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
		return board[sq];
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
		return (hand[c] & hand_masking[pt]) >> hand_shift[pt];
	}
	//指定したカラー、駒種の有無を取得,駒数は不要
	bool is_hand(Color c, PieceType pt)
	{
		return bool(hand[c] & hand_masking[pt]);
	}
	//カラー、駒種に関係なく、全ての局面bitboardを返す
	BitBoard all_bb()
	{
		return by_type_bb[AllPieces];
	}
	//指定したカラーの局面bitboardを返す
	BitBoard color_of_bb(const Color c)
	{
		return by_color_bb[c];
	}
	//カラーに関係なく指定した駒種の局面bitboardを返す
	BitBoard piece_type_of_bb(const PieceType pt)
	{
		return by_type_bb[pt];
	}
	//局面を更新
	void do_move();
	//局面を復元
	void undo_move();
private:
	//positionクラスをクリアにする
	void Position::clear()
	{
		memset(this, 0, sizeof(Position));
	}
	void put_piece(Piece piece, int sq);
	void put_hand(Piece pt, int num);
	int board[SquareNum];
	unsigned int hand[ColorNum];
	BitBoard by_type_bb[PieceTypeNum];
	BitBoard by_color_bb[ColorNum];
	int color_turn;
};

namespace Positionns
{
	void init();
	void print_board(const Position &pos);
}
//positionの定数
/*
const int BOARD_UPPER = 16*13;
const int STAND_BLACK_START = 16*13;
const int STAND_WHITE_START = 16*13 + 7;
const int LIMIT = 16*13 + 7*2 + 2;  //16*13は盤上7*2は駒台、最後の２はKING用の座標

extern string start_position;
extern position_t root_position;
extern const int BLACK;
extern const int WHITE;
extern const int NO_COLOR;
extern int turn;

const extern char EMPTY;
const extern char EDGE;
//const extern char NOT_PMOTO;

*/

/*
駒コードを与えて成っていないか判定する
成っていなかったらtrue,成駒ならfalse
*/
/*
inline int is_not_pmoto(char p)
{
    return p & 8;
}
*/
/*
先手駒コードを強制的に後手駒コードにする
*/
/*
inline char do_white(char p)
{
    return p | 0xF0;
}
*/
/*
後手駒コードを強制的に先手駒コードにする
*/
/*
inline char do_black(char p)
{
    return p & 0x0F;
}
*/

/*
string to_sfen(const Position &pos);
void print_board(const Position &pos);
*/
/*
short *do_move_b(Position &pos,Move m,short *mf);
short *do_move_w(Position &pos,Move m,short *mf);
void undo_move(Position &pos,int ply);
void is_ok(Position &pos);
*/
#endif

