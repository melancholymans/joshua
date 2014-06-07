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

const extern char EMPTY;
const extern char EDGE;
const extern char NOT_PMOTO;

const extern char BP_PAWN;
const extern char BP_LANCE;
const extern char BP_KNIGHT;
const extern char BP_SILVER;
const extern char BP_BISHOP;
const extern char BP_ROOK;
const extern char B_KING;
const extern char B_GOLD;
const extern char B_PAWN;
const extern char B_LANCE;
const extern char B_KNIGHT;
const extern char B_SILVER;
const extern char B_BISHOP;
const extern char B_ROOK;

const extern char WP_PAWN;
const extern char WP_LANCE;
const extern char WP_KNIGHT;
const extern char WP_SILVER;
const extern char WP_BISHOP;
const extern char WP_ROOK;
const extern char W_KING;
const extern char W_GOLD;
const extern char W_PAWN;
const extern char W_LANCE;
const extern char W_KNIGHT;
const extern char W_SILVER;
const extern char W_BISHOP;
const extern char W_ROOK;

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

inline color color_of_piece(int p)
{
    //符号ありchar型を右シフトしているのでANSI Cでは挙動が定義されていない
    //VC++2010で算術シフトを採用しているようなのでこのままとするが処理系が
    //変更になったらテストでエラーが」でるようにしておく
    return p >> 4;
}

void from_sfen(string &sfen);
string to_sfen(void);
void put_piece(char p,int sq,int num);

#endif

