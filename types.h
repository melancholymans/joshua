#if !defined(TYPES_H_INCLUDE)
#define TYPES_H_INCLUDE

#include <stdlib.h>

const int PLY_MAX = 48; //bonanza‚©‚ç 

typedef int Color;
typedef unsigned int Move;

typedef struct{
    Move *next_move;
    Move *last_move;
}next_move_t;

#endif