#if !defined(MOVE_H_INCLUDE)
#define MOVE_H_INCLUDE

#include "types.h"

namespace Movens{
	// xxxxxxxx xxxxxxxx xxxxxxxx x1111111 移動先(7bit=127) to
	// xxxxxxxx xxxxxxxx xx111111 1xxxxxxx 移動元(7bit=127) from 駒打ちのときはPieceType + SquareNum - 1 (81->87)
	// xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx １なら成 pmoto
	// xxxxxxxx xxxx1111 xxxxxxxx xxxxxxxx 移動する駒の駒種(4bit=15) piece
	// xxxxxxxx 1111xxxx xxxxxxxx xxxxxxxx 獲られた駒の駒種(4bit=15) cap_piece

	//Moveからfrom情報を取出す
	inline Square move_from(Move m)
	{
		return Square((m >> 7) & 0x7F);
	}
	//Moveからto情報を取出す
	inline Square move_to(Move m)
	{
		return Square(m & 0x7F);
	}
	//Moveからpiece情報を取出す
	inline PieceType move_piece(Move m)
	{
		return PieceType((m >> 16) & 0x0F);
	}
	//Moveからcap_piece情報を取出す
	inline PieceType move_cap_piece(Move m)
	{
		return PieceType((m >> 20) & 0X0F);
	}
	//打つ駒の駒種を取り出す
	inline PieceType move_drop_piece(Move m)
	{
		return PieceType(move_from(m) - SquareNum + 1);
	}
	//Moveからpmoto情報を取出す
	inline bool is_pmoto(Move m)
	{
		return bool(m & 0x4000);
	}
	//Moveから打ち手なのか判定する
	inline bool is_drop(Move m)
	{
		return move_from(m) > (SquareNum -1);
	}
	//Moveから駒を獲る手なのか判定する
	inline bool is_capture(Move m)
	{
		return bool(m & 0xF00000);
	}
	//打ち駒の駒種からfromに変換する
	inline Square make_drop(PieceType pt)
	{
		return Square(pt + SquareNum - 1);
	}
	//各引数からMoveを作る
	inline Move make_move(Square from, Square to, int pmoto, PieceType piece, PieceType cap_piece)
	{
		return (uint32_t(to) | uint32_t(from) << 7 | uint32_t(pmoto) << 14 | uint32_t(piece & 0x0F) << 16 | uint32_t(cap_piece & 0x0F) << 20);
	}
	Move move_from_string(const Position &pos, const string &cmd);
	string string_from_move(const Move m);
}
#endif
