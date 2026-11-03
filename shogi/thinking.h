#pragma once

int do_thinking(usi_handler_t* hd);
int* possible_moves(usi_handler_t* hd, int* ml);
_Bool is_double_pawn(usi_handler_t* hd, int sq, int pc);
_Bool canmove_without_promotion(int to, int pt, int c);
_Bool can_promote(int sq, int pt);
int* attacks_from(usi_handler_t* hd, int pc, int sq, int turn);
int* attacks_bsilver(int sq, int* m);
int* attacks_bbishop(usi_handler_t* hd, int sq, int* m);
int* attacks_brook(usi_handler_t* hd, int sq, int* m);
int* attacks_bgold(int sq, int* m);
int* attaks_bking(int sq, int* m);
int* attacks_bhorse(usi_handler_t* hd, int sq, int* m);
int* attacks_bdragon(usi_handler_t* hd, int sq, int* m);
