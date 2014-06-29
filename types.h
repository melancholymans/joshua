#if !defined(TYPES_H_INCLUDE)
#define TYPES_H_INCLUDE

#include <stdlib.h>

const int PLY_MAX = 48; //bonanzaから 

typedef int Color;
typedef unsigned int Move;

//将棋所からのコマンドをパース
class USIInputParser{
public:
    USIInputParser(const string &line);
    string get_next_token(void);
    string get_rest_of_line(void);
    bool at_end_of_line(void);
private:
    const string &input_line;
    int length,current_index;
    void skip_whitespace(void);
};

typedef struct{
    Move *next_move;
    Move *last_move;
}next_move_t;

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