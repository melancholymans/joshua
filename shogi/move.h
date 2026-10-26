#pragma once

#include "init.h"

/*
 xxxxxxxx xxxxxxxx xxxxxxxx x1111111 to: 移動元座標
 xxxxxxxx xxxxxxxx xx111111 1xxxxxxx from: 移動先座標 駒打ちの時はpiece(駒番)
 xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx pmoto: 成:1 不成:0,駒打ちの時は0で決め打ち
 xxxxxxxx xxxxxxxx 1xxxxxxx xxxxxxxx morh: main board(0) or hand board(1)
 xxxxxxxx xxx11111 xxxxxxxx xxxxxxxx 移動する駒のpiece(駒番),駒打ちの時は使用しない
 xxx11111 xxxxxxxx xxxxxxxx xxxxxxxx 取った駒の(captured piece)駒番,そのままの駒番を保存colorの切替pmotoの削除は登録の時はしない,駒打ちの時は使用しない
 to = m & 0x7f
 from = (m >> 7) & 0x7f
 ispmoto = (m >> 14) & 0x01
 morh(main or hand) = (m >> 15) & 0x01
 piece = (m >> 16) & 0x1f
 cap = (m >> 24) & 0x1f
*/

int go(usi_handler_t* hd);
int* legel_move(usi_handler_t* hd, int* ml);
int* possible_moves(usi_handler_t* hd, int* ml);
_Bool canmove_without_promotion(int to, int pt, int c);
_Bool can_promote(int sq, int pt);
int* attacks_from(usi_handler_t* hd, int pc, int sq, int turn);	
int* attacks_bsilver(int sq, int* m);
int* attacks_bbishop(usi_handler_t* hd, int sq, int* m);
int* attacks_brook(usi_handler_t* hd, int sq, int* m);
int initial_to_piecetype(char p);
void do_usi_move(usi_handler_t* hd, char* usi_move);
void do_move(usi_handler_t* hd, const int move);
void undo_move(usi_handler_t* hd, const int move);
void get_move(const int move, int* to, int* from, int* ispmoto, int* morh, int* piece, int* cap);
int strip_pmoto(int pc);
void move_to_usi(int mv, char* usi_move);
