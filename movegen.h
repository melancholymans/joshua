#if !defined(MOVEGEN_H_INCLUDE)
#define MOVEGEN_H_INCLUDE

#include "types.h"
#include "movepicker.h"

enum MoveType{
	Capture,			//駒を取る手
	NonCapture,			//駒を取らない手
	Promoto,			//成る手
	Drop,				//駒打ち
	Evasion,			//王手回避
	Legal,				//合法手
	MoveTypeNum
};

namespace MoveGeneratens
{
	/*
	//王手回避手を生成
	template <Color US>
	MoveStack* generate_evasions(MoveStack* ml, const Position &pos);
	//ここから全ての指し手生成関数を呼び出してMoveStackを返す。
	*/
	template <MoveType MT, Color US>
	MoveStack* generate_pawn_moves(MoveStack* ml, const Position &pos, BitBoard& tar, Square ksq);
	template <MoveType MT, PieceType PT, Color US>
	MoveStack* generate_moves(MoveStack* ml, const Position &pos);
}

/*

inline int is_pmoto_w(int to)
{
    return to > SQ_1F ? 1 : 0;
}

inline int is_pmoto_b(int to)
{
    return to < SQ_9D ? 1 : 0;
}

Move *generate_king_moves_w(const Position &pos,Move *ml,int from);
Move *generate_gold_moves_w(const Position &pos,Move *ml,int from);
Move *generate_pawn_moves_w(const Position &pos,Move *ml,int from);
Move *generate_lance_moves_w(const Position &pos,Move *ml,int from);
Move *generate_knight_moves_w(const Position &pos,Move *ml,int from);
Move *generate_silver_moves_w(const Position &pos,Move *ml,int from);
Move *generate_bishop_moves_w(const Position &pos,Move *ml,int from);
Move *generate_rook_moves_w(const Position &pos,Move *ml,int from);
Move *generate_pbishop_moves_w(const Position &pos,Move *ml,int from);
Move *generate_prook_moves_w(const Position &pos,Move *ml,int from);
Move *generate_gold_drop_w(const Position &pos,Move *ml);
Move *generate_pawn_drop_w(const Position &pos,Move *ml);
Move *generate_lance_drop_w(const Position &pos,Move *ml);
Move *generate_knight_drop_w(const Position &pos,Move *ml);
Move *generate_silver_drop_w(const Position &pos,Move *ml);
Move *generate_bishop_drop_w(const Position &pos,Move *ml);
Move *generate_rook_drop_w(const Position &pos,Move *ml);
bool is_checkmate_w(const Position &pos);

Move *generate_king_moves_b(const Position &pos,Move *ml,int from);
Move *generate_gold_moves_b(const Position &pos,Move *ml,int from);
Move *generate_pawn_moves_b(const Position &pos,Move *ml,int from);
Move *generate_lance_moves_b(const Position &pos,Move *ml,int from);
Move *generate_knight_moves_b(const Position &pos,Move *ml,int from);
Move *generate_silver_moves_b(const Position &pos,Move *ml,int from);
Move *generate_bishop_moves_b(const Position &pos,Move *ml,int from);
Move *generate_rook_moves_b(const Position &pos,Move *ml,int from);
Move *generate_pbishop_moves_b(const Position &pos,Move *ml,int from);
Move *generate_prook_moves_b(const Position &pos,Move *ml,int from);
Move *generate_gold_drop_b(const Position &pos,Move *ml);
Move *generate_pawn_drop_b(const Position &pos,Move *ml);
Move *generate_lance_drop_b(const Position &pos,Move *ml);
Move *generate_knight_drop_b(const Position &pos,Move *ml);
Move *generate_silver_drop_b(const Position &pos,Move *ml);
Move *generate_bishop_drop_b(const Position &pos,Move *ml);
Move *generate_rook_drop_b(const Position &pos,Move *ml);
bool is_checkmate_b(const Position &pos);

Move *generate_evasions(const Position &pos,Move *ml);
*/
/*
2014/7/7時点での問題点
KINGが敵の利き領域に進む
is_checkmate_XXで手生成時点で王手がかかっている判断しているが
KING自身が動く時敵の利き領域に入るかどうかはまったくチエックしていない
*/
#endif
