#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"
#include "bitboard.h"

using std::string;

class Position{
public:
	Position(){}
	Position(const Position& p);

	Position(const string& sfen)
	{ 
		position_from_sfen(sfen);
	}
	void position_from_sfen(const string &sfen);
	void set_color_turn(Color c);
	Color get_color_turn();
	void flip_color();
	int get_board(int sq) const;
	int get_hand(Color c,PieceType pt) const;
	bool is_hand(Color c,PieceType pt);
	BitBoard all_bb()
	{
		return by_type_bb[AllPieces];
	}
	BitBoard color_of_bb(const Color c)
	{
		return by_color_bb[c];
	}
	BitBoard piece_type_of_bb(const PieceType pt)
	{
		return by_type_bb[pt];
	}
private:
	void Position::clear();
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
Move
31-30-29-28-27-26-25-24-23-22-21-20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01-00
                     | cap piece |src piece |pm |       from square      |       to square    |

cap piece 取った駒（colorはつかない駒種のみ　メモリ節約のため）     21-24bit
from piece 動いた駒（colorはつかない駒種のみ　メモリ節約のため）    17-20bit
pm->pmotoのフラグ１なら成り動作    1bit 16bit
from square 移動元の座標 8bit 8-15bit
to square 移動先の座標 8bit 0-7bit
25-31bitは空き
*/
/*
inline Move make_move(int from,int to,int pmoto,char piece,char cap_piece)
{
    return (unsigned int(to) | unsigned int(from) << 8 | unsigned int(pmoto) << 16 | unsigned int(piece & 0x0F) << 17 | unsigned int(cap_piece & 0x0F) << 21);
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

