#if !defined(MOVEGEN_H_INCLUDE)
#define MOVEGEN_H_INCLUDE

#include "types.h"

extern Move mlist[16384];
extern next_move_t next_move[PLY_MAX];

Move *generate_moves(const Position &pos,Move *ml);
Move *generate_pawn_moves_w(const Position &pos,Move *ml,int from);
Move *generate_gold_moves_w(const Position &pos,Move *ml,int from);
Move *generate_silver_moves_w(const Position &pos,Move *ml,int from);
Move *generate_knight_moves_w(const Position &pos,Move *ml,int from);
Move *generate_king_moves_w(const Position &pos,Move *ml,int from);
Move *generate_pawn_moves_b(const Position &pos,Move *ml,int from);
Move *generate_gold_moves_b(const Position &pos,Move *ml,int from);
Move *generate_silver_moves_b(const Position &pos,Move *ml,int from);
Move *generate_knight_moves_b(const Position &pos,Move *ml,int from);
Move *generate_king_moves_b(const Position &pos,Move *ml,int from);

#endif
