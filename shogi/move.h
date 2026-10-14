#pragma once

#include "init.h"

int initial_to_piecetype(char p);
void do_usi_move(usi_handler_t* hd, char* usi_move);
void do_move(usi_handler_t* hd, const int move);
void undo_move(usi_handler_t* hd, const int move);
void get_move(const int move, int* to, int* from, int* ispmoto, int* morh, int* piece, int* cap);
int strip_pmoto(int pc);
void move_to_usi(int mv, char* usi_move);
/*int* move_list(void);*/
/*void legal_moves(usi_handler_t* hd, int* m)*/
int* attacks_from(usi_handler_t* hd, int pc, int sq, int turn);
