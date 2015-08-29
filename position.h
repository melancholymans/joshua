﻿#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"

class Position{
public:
	Position(){}
	Position(const Position& p);

	Position(const string& sfen)
	{ 
		position_from_sfen(sfen);
	}
	void position_from_sfen(const string &sfen);
private:
	void Position::clear();
	void put_piece(Piece piece, int sq);
	int board[SquareNum];
};
//positionの定数
/*
const int BOARD_UPPER = 16*13;
const int STAND_BLACK_START = 16*13;
const int STAND_WHITE_START = 16*13 + 7;
const int LIMIT = 16*13 + 7*2 + 2;  //16*13は盤上7*2は駒台、最後の２はKING用の座標

extern string start_position;
extern position_t root_position;
extern const int BLACK;
extern const int WHITE;
extern const int NO_COLOR;
extern int turn;

const extern char EMPTY;
const extern char EDGE;
//const extern char NOT_PMOTO;
enum{
    PPAWN=2,PLANCE,PKNIGHT,PSILVER,PBISHOP,PROOK,
    KING=8,GOLD,PAWN,LANCE,KNIGHT,SILVER,BISHOP,ROOK
};
enum{
    UUL = -33,UUR = -31,UL = -17,U = -16,UR = -15,L = -1,
    R = 1,DL = 15,D = 16,DR = 17,DDL = 31,DDR = 33
};

const extern char BP_PAWN;      //2
const extern char BP_LANCE;     //3
const extern char BP_KNIGHT;    //4   
const extern char BP_SILVER;    //5
const extern char BP_BISHOP;    //6
const extern char BP_ROOK;      //7
const extern char B_KING;       //8
const extern char B_GOLD;       //9
const extern char B_PAWN;       //10
const extern char B_LANCE;      //11
const extern char B_KNIGHT;     //12
const extern char B_SILVER;     //13
const extern char B_BISHOP;     //14
const extern char B_ROOK;       //15

const extern char WP_PAWN;      //2-16=-14
const extern char WP_LANCE;     //3-16=-13
const extern char WP_KNIGHT;    //4-16=-12
const extern char WP_SILVER;    //5-16=-11
const extern char WP_BISHOP;    //6-16=-10
const extern char WP_ROOK;      //7-16=-9
const extern char W_KING;       //8-16=-8
const extern char W_GOLD;       //9-16=-7
const extern char W_PAWN;       //10-16=-6
const extern char W_LANCE;      //11-16=-5
const extern char W_KNIGHT;     //12-16=-4
const extern char W_SILVER;     //13-16=-3
const extern char W_BISHOP;     //14-16=-2
const extern char W_ROOK;       //15-16=-1

*/
//File,Rank座標からsq座標を計算
inline int make_square(int File,int Rank)
{
    return File*9+Rank;
}

/*
sq盤座標からcol,row座標に変換
*/
/*
inline void make_col_row(int sq,int *col,int *row)
{
    *row = sq/16;   //整数の割り算は小数点は切り捨て
    *col = sq - *row*16;
    *row = *row - 1;
}


*/
/*
駒コードを与えて成っていないか判定する
成っていなかったらtrue,成駒ならfalse
*/
/*
inline int is_not_pmoto(char p)
{
    return p & 8;
}
*/
/*
先手駒コードを強制的に後手駒コードにする
*/
/*
inline char do_white(char p)
{
    return p | 0xF0;
}
*/
/*
後手駒コードを強制的に先手駒コードにする
*/
/*
inline char do_black(char p)
{
    return p & 0x0F;
}
*/
/*
Move
31-30-29-28-27-26-25-24-23-22-21-20-19-18-17-16-15-14-13-12-11-10-09-08-07-06-05-04-03-02-01-00
                     | cap piece |src piece |pm |       from square      |       to square    |

cap piece 取った駒（colorはつかない駒種のみ　メモリ節約のため）     21-24bit
from piece 動いた駒（colorはつかない駒種のみ　メモリ節約のため）    17-20bit
pm->pmotoのフラグ１なら成り動作    1bit 16bit
from square 移動元の座標 8bit 8-15bit
to square 移動先の座標 8bit 0-7bit
25-31bitは空き
*/
/*
inline Move make_move(int from,int to,int pmoto,char piece,char cap_piece)
{
    return (unsigned int(to) | unsigned int(from) << 8 | unsigned int(pmoto) << 16 | unsigned int(piece & 0x0F) << 17 | unsigned int(cap_piece & 0x0F) << 21);
}
*/

/*
string to_sfen(const Position &pos);
void print_board(const Position &pos);
*/
/*
short *do_move_b(Position &pos,Move m,short *mf);
short *do_move_w(Position &pos,Move m,short *mf);
void undo_move(Position &pos,int ply);
void is_ok(Position &pos);
*/
#endif

