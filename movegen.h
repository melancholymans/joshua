#if !defined(MOVEGEN_H_INCLUDE)
#define MOVEGEN_H_INCLUDE

#include "types.h"

extern Move mlist[16384];
extern next_move_t next_move[PLY_MAX];

Move *generate_moves(const Position &pos,Move *ml);
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

#endif
