#if !defined(MOVEGEN_H_INCLUDE)
#define MOVEGEN_H_INCLUDE

#include "types.h"

extern Move mlist[16384];
extern next_move_t next_move[PLY_MAX];

Move *generate_moves(const Position &pos,Move *ml);

#endif
