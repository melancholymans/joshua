#if !defined(MOVE_H_INCLUDE)
#define MOVE_H_INCLUDE

#include "types.h"

/*
Move構造体からfrom情報を取出す
*/
inline int move_from(Move m)
{
    return (m >> 8) & 0xFF;
}

/*
Move構造体からto情報を取出す
*/
inline int move_to(Move m)
{
    return m & 0xFF;
}

/*
Move構造体からpiece情報を取出す
Move構造体に格納されているpieceはColor情報を
除去された駒種情報だけなので、盤上移動の時には
使用されないが打つ手の時に使用する.
打つ時Color情報を付与してboardを更新すること
*/
inline char move_piece(Move m)
{
    return (m >> 17) & 0x0F; 
}

/*
Move構造体からcap_piece情報を取出す
Move構造体に格納されているpieceはColor情報を
除去された駒種情報だけ.
*/
inline char move_cap_piece(Move m)
{
    return (m >> 21) & 0X0F;
}

/*
Move構造体からpmoto情報を取出す
成るなら0x10000を返す（PMOTOで定義されている）
成らないなら0を返す
*/
inline int move_pmoto(Move m)
{
    return m & 0x10000;
}

Move move_from_string(const Position &pos,const string &cmd);
int square_from_string(const string sq);
string string_from_move(const Move m);
string string_from_square(int sq);

#endif
