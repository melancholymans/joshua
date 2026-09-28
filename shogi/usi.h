#pragma once
#include "position.h"

//‚Æ‚è‚ ‚¦‚¸‚±‚Ì\‘¢‘Ì‚É‚µ‚Ä‚¨‚­Aint32_t‚Ì•Ï”‚É‚·‚é
typedef struct {
	int from;
	int to;
	int drop_piece;
	_Bool promotion;
}move_t;

typedef struct {
	board_t* bd;
	FILE* stream;
	char* name;
	char* author;
	move_t* mv;
}usi_handler_t;

usi_handler_t new_usi_handler(FILE* out);
errno_t handle(usi_handler_t* usi_hd,char* buf);
errno_t handle_usi(usi_handler_t* hd,char* cmds);
errno_t handle_is_ready(usi_handler_t* hd,char* cmds);
errno_t handle_setoption(char* cmds);
errno_t handle_usi_newgame(char* cmds);
errno_t handle_position(char* cmds);
errno_t handle_go(char* cmds);
errno_t handle_stop(char* cmds);
errno_t handle_ponder_hit(char* cmds);
errno_t handle_quit(char* cmds);
errno_t handle_gameover(char* cmds);
errno_t handle_debug(usi_handler_t* hd, char** msg[], int size);
void put_send(usi_handler_t* hd, char* msg);
void put_debug(usi_handler_t* hd, char* msg);
void shorten_space(char* buf);
