#if !defined(USI_H_INCLUDE)
#define USI_H_INCLUDE

#include "types.h"

void usi_main_loop(void);
void game_init(const Position &pos);
bool handle_command(const string &command);
void set_position(USIInputParser &uip);
void go(void);

#endif
