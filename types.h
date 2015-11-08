#if !defined(TYPES_H_INCLUDE)
#define TYPES_H_INCLUDE

#include <inttypes.h>
#include <string>

const int MAX_LEGAL_MOVE = 1024;	//局面の最大合法手数が593手なので一番近い２のべき乗

enum Color
{
	Black,White,ColorNum
};

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
	DeltaNW = DeltaN + DeltaW,
};
//駒種は1-14なので4bitまで
enum PieceType{
	AllPieces = 0,EmptyPiece = 0,
	Pawn = 1,Lance,Night,Silver,Bishop,Rook,Gold,King=8,
	ProPawn,ProLance,ProNight,ProSilver,Horse,Dragon,
	PieceTypeNum, HandPieceNum = 8
};
//駒コードはblack側が1-14、white側が17-30なので5bit=>PieceType | 0x10で駒コードになる
enum Piece{
	PieceNone = 0,UnPromoted = 0, Promoted = 8,
	BPawn = 1,BLance,BNight,BSilver,BBishop,BRook,BGold,BKing,
	BProPawn, BProLance, BProNight, BProSilver, BHorse,BDragon,
	WPawn = 17, WLance, WNight, WSilver, WBishop, WRook, WGold, WKing,
	WProPawn, WProLance, WProNight, WProSilver, WHorse, WDragon,
	PieceNum = 31
};
//方向子、方向を決めているだけで座標の移動には使用しない。
enum Directtion{
	DirectMisc = 0,		//縦、横斜めの位置関係にない
	DirectFile = 2,		//縦
	DirectRank = 3,		//横
	DirectDiagNESW = 4,	//右上から左下
	DirectDiagNWSE = 5,	//左上から右下
	DirectDiagCross = 2,	//縦横
	DirectDiag = 4			//斜め
};

enum MoveType{
	Capture,			//駒を取る手
	NonCapture,			//駒を取らない手
	Promoto,			//成る手
	Drop,				//駒打ち
	Evasion,			//王手回避
	Legal,				//合法手
	MoveTypeNum
};


const Rank SQUARE_RANK[SquareNum] = {
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1
};
const File SQUARE_FILE[SquareNum] = {
	FileI, FileI, FileI, FileI, FileI, FileI, FileI, FileI, FileI,
	FileH, FileH, FileH, FileH, FileH, FileH, FileH, FileH, FileH,
	FileG, FileG, FileG, FileG, FileG, FileG, FileG, FileG, FileG,
	FileF, FileF, FileF, FileF, FileF, FileF, FileF, FileF, FileF,
	FileE, FileE, FileE, FileE, FileE, FileE, FileE, FileE, FileE,
	FileD, FileD, FileD, FileD, FileD, FileD, FileD, FileD, FileD,
	FileC, FileC, FileC, FileC, FileC, FileC, FileC, FileC, FileC,
	FileB, FileB, FileB, FileB, FileB, FileB, FileB, FileB, FileB,
	FileA, FileA, FileA, FileA, FileA, FileA, FileA, FileA, FileA
};
//指し手データ定義
typedef uint32_t Move;
typedef uint64_t Key;	//boardの駒配置状態を一意に表す数値に使用されたりする

//指し手情報を入れておくクラス
struct MoveStack{
	Move move;
	int score;
};

//駒コードから駒種を取り出す
inline PieceType type_of_piece(Piece piece)
{
	return PieceType(piece & 0x0F);
}
//駒コードからカラーを取り出す
inline Color color_of_piece(Piece piece)
{
	return Color(piece >> 4);
}
//座標値が盤内に収まっているかチエック
inline bool is_square(const Square sq)
{
	return (0 <= sq) && (sq < SquareNum);
}
//座標が属しているrankを返す
inline Rank make_rank(const Square sq)
{
	return SQUARE_RANK[sq];
}
//座標が属しているfileを返す
inline File make_file(const Square sq)
{
	return SQUARE_FILE[sq];
}
//ランクが正常値かチエックして、正常であればtrueを返す
inline bool is_rank(Rank r)
{
	return (0 <= r) && (r < RankNum);
}
//ファイルが正常値かチエックして、正常であればtrueを返す
inline bool is_file(File f)
{
	return (0 <= f) && (f < RankNum);
}
//File,Rank座標からsq座標を計算 TEST OK
inline Square make_square(File f, Rank r)
{
	return Square(f * 9 + r);
}
//引数のカラーをひっくり返す
inline Color over_turn(Color c)
{
	return Color(c ^ 1);
}

#endif