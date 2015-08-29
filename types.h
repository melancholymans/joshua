#if !defined(TYPES_H_INCLUDE)
#define TYPES_H_INCLUDE

#include <inttypes.h>
#include <string>

using namespace std;

enum Square
{
	I9, I8, I7, I6, I5, I4, I3, I2, I1,
	H9, H8, H7, H6, H5, H4, H3, H2, H1,
	G9, G8, G7, G6, G5, G4, G3, G2, G1,
	F9, F8, F7, F6, F5, F4, F3, F2, F1,
	E9, E8, E7, E6, E5, E4, E3, E2, E1,
	D9, D8, D7, D6, D5, D4, D3, D2, D1,
	C9, C8, C7, C6, C5, C4, C3, C2, C1,
	B9, B8, B7, B6, B5, B4, B3, B2, B1,
	A9, A8, A7, A6, A5, A4, A3, A2, A1,
	SquareNum
};

enum File
{
	FileI,FileH,FileG,FileF,FileE,FileD,FileC,FileB,FileA,FileNum
};
enum Rank
{
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1, RankNum
};

enum SquareDelta{
	DeltaNothing = 0,
	DeltaN = -1, DeltaE = -9, DeltaS = 1, DeltaW = 9,
	DeltaNE = DeltaN + DeltaE,
	DeltaSE = DeltaS + DeltaE,
	DeltaSW = DeltaS + DeltaW,
	DeltaNW = DeltaN + DeltaW
};

enum Piece{
	PieceNone = 0,UnPromoted = 0, Promoted = 8,
	BPawn = 1,BLance,BNight,BSilver,BBishop,BRook,BGold,BKing,
	BProPawn, BProLance, BProKnight, BProSilver, BHorse,BDragon,
	WPawn = 17, WLance, WNight, WSilver, WBishop, WRook, WGold, WKing,
	WProPawn, WProLance, WProKnight, WProSilver, WHorse, WDragon,
	PieceNum = 31
};
//関数を呼び分けるマクロ群
//#define DoMove(turn,pos,m,mf) ((turn) ? do_move_w(pos,m,mf) : do_move_b(pos,m,mf))

//const int PLY_MAX = 1;//48; //bonanzaから 

//typedef int Color;
//typedef unsigned int Move;

/*
typedef struct Position{
    char board[16*13 + 7*2 + 2 + 32];  //最後の３２は配列の大きさを256にするためのもの意味はない
    int turn;
}position_t;

typedef struct Search{
    int material;
}search_t;

typedef struct {
    Move *next_move;
    Move *last_move;
}next_move_t;

typedef struct next_modify{
    short *next_dirty;
    short *last_dirty;
}next_modify_t;

typedef struct status{
    long long search_node;    //展開された探索ノード数、922京までカウントできる   
}status_t;

typedef struct backup_info{
    int material;
}backup_info_t;
*/
/*
駒の判定方法一覧
char p;

inline is_not_pmoto(char p)に登録
p & NOT_PMOTO       trueとでたら不成
p & NOT_PMOTO       falseとでたら成

p > 1               trueなら先手
p > 1 || p == 0     trueなら先手または空白
p < 0               trueなら後手
p <= 0              trueなら後手または空白

inline do_white(char p)
先手を強制的に後手に
inline do_black(char p)
p |= 0xF0
後手を強制的に先手に
p &= 0x0F

//駒コードから駒タイプを計算
p &= 0x0F;
これはインライン関数として登録してある
inline char type_of_piece(char p)

駒コードからカラーを判定している
p >> 4;これはボツ
これはインライン関数として登録してある
inline Color color_of_piece(int p)

row,col座標からsq座標に変換
postion.hにあるmake_square　inline関数を
利用のこと

汎用性があるものはここに書く、煩雑に書くようになったら
inline関数へすること
*/

#endif