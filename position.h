#if !defined(POSITION_H_INCLUDE)
#define POSITION_H_INCLUDE

#include "types.h"

typedef struct Position{
    void set_posiition();
    char board[16*13+38*2];
    int king_square[2];
}position_t;

//positionの定数
const int BOARD_UPPER = 16*13;
const int STAND_BLACK_START = 16*13;
const int STAND_WHITE_START = 16*13 + 7;
const int LIMIT = 16*13 + 7*2;

extern string start_position;
extern position_t root_position;
extern const int BLACK;
extern const int WHITE;
extern int turn;

const extern char EMPTY;
const extern char EDGE;
const extern char NOT_PMOTO;
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

//座標記号
enum{
    SQ_A9=33,SQ_B9,SQ_C9,SQ_D9,SQ_E9,SQ_F9,SQ_G9,SQ_H9,SQ_I9,
    SQ_A8=49,SQ_B8,SQ_C8,SQ_D8,SQ_E8,SQ_F8,SQ_G8,SQ_H8,SQ_I8,
    SQ_A7=65,SQ_B7,SQ_C7,SQ_D7,SQ_E7,SQ_F7,SQ_G7,SQ_H7,SQ_I7,
    SQ_A6=81,SQ_B6,SQ_C6,SQ_D6,SQ_E6,SQ_F6,SQ_G6,SQ_H6,SQ_I6,
    SQ_A5=97,SQ_B5,SQ_C5,SQ_D5,SQ_E5,SQ_F5,SQ_G5,SQ_H5,SQ_I5,
    SQ_A4=113,SQ_B4,SQ_C4,SQ_D4,SQ_E4,SQ_F4,SQ_G4,SQ_H4,SQ_I4,
    SQ_A3=129,SQ_B3,SQ_C3,SQ_D3,SQ_E3,SQ_F3,SQ_G3,SQ_H3,SQ_I3,
    SQ_A2=145,SQ_B2,SQ_C2,SQ_D2,SQ_E2,SQ_F2,SQ_G2,SQ_H2,SQ_I2,
    SQ_A1=161,SQ_B1,SQ_C1,SQ_D1,SQ_E1,SQ_F1,SQ_G1,SQ_H1,SQ_I1,
    SQ_LIMIT
};
//列、行番号から座標を計算
inline int make_square(int col,int row)
{
    return ((row+1) << 4) | col;
}

//駒コードから駒タイプを計算
inline char type_of_piece(char p)
{
    return p & 0x0F;
}

inline Color color_of_piece(int p)
{
    //駒コードからカラーを判定している
    //符号ありchar型を右シフトしているのでANSI Cでは挙動が定義されていない
    //VC++2010で算術シフトを採用しているようなのでこのままとするが処理系が
    //変更になったらテストでエラーがでるようにしておく
    return p >> 4;
}
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
inline Move make_move(int from,int to,int pmoto,char piece,char cap_piece)
{
    return (unsigned int(to) | unsigned int(from) << 8 | unsigned int(pmoto) << 16 | unsigned int(piece) << 17 | unsigned int(cap_piece) << 21);
}

void from_sfen(string &sfen);
string to_sfen(const Position &pos);
void print_board(const Position &pos);
void put_piece(char p,int sq,int num);
void do_move(Position &pos,Move m);

#endif

