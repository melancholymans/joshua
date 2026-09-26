#pragma once

typedef struct {
	int mb[81];
	int hb[2][8];
	int turn;
}board_t;

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

usi_handler_t* new_usi_handler(FILE* out);
errno_t handle(usi_handler_t* usi_hd,char* buf);
void shorten_space(char* buf);
