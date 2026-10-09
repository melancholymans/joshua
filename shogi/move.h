#pragma once

#include "init.h"

void do_move(usi_handler_t* hd, int move);
void get_move(const int move, int* to, int* from, int* ispmoto, int* morh, int* piece, int* cap);
void do_usi_move(usi_handler_t* hd, char* usi_move);
int usisq_to_movesq(char* usi_sq);
int initial_to_piecetype(char p);
void do_move(usi_handler_t* hd, const int move);
void undo_move(usi_handler_t* hd, const int move);
