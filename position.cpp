﻿/*
#include <string>
#include <ctype.h>
#include <crtdbg.h>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "usioption.h"
#include "usi.h"
#include "movegen.h"
#include "move.h"
#include "search.h"
#include "evaluate.h"

//駒コード（GPSを参考にした）
const char EMPTY = 0;
const char EDGE = 1;

const char BP_PAWN = 2;
const char BP_LANCE = 3;
const char BP_KNIGHT = 4;
const char BP_SILVER = 5;
const char BP_BISHOP = 6;
const char BP_ROOK = 7;
const char B_KING = 8;
const char B_GOLD = 9;
const char B_PAWN = 10;
const char B_LANCE = 11;
const char B_KNIGHT = 12;
const char B_SILVER = 13;
const char B_BISHOP = 14;
const char B_ROOK = 15;

const char WP_PAWN = BP_PAWN - 16;      //-14
const char WP_LANCE = BP_LANCE - 16;    //-13
const char WP_KNIGHT = BP_KNIGHT - 16;  //-12
const char WP_SILVER = BP_SILVER - 16;  //-11
const char WP_BISHOP = BP_BISHOP - 16;  //-10   
const char WP_ROOK = BP_ROOK - 16;      //-9
const char W_KING = B_KING - 16;        //-8
const char W_GOLD = B_GOLD - 16;        //-7
const char W_PAWN = B_PAWN - 16;        //-6
const char W_LANCE = B_LANCE - 16;      //-5
const char W_KNIGHT = B_KNIGHT - 16;    //-4
const char W_SILVER = B_SILVER - 16;    //-3
const char W_BISHOP = B_BISHOP - 16;    //-2
const char W_ROOK = B_ROOK - 16;        //-1
//player(手番） BLACKが先手、WHITEが後手
const int BLACK = 0;
const int WHITE = -1;
const int NO_COLOR = 16;

//先手大文字、後手小文字、数字は空白、/は行区切り +は成駒の印
string start_position = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
string begin_poition;   //どんな局面を受付けたのか保持
position_t root_position;

void from_sfen(string &sfen)
{
    begin_poition = sfen;
    USIInputParser uip(sfen);
    string token = uip.get_next_token();
    int index;
    char pmoto = 0;

    //全体を盤外に設定
    for(index = 0;index < LIMIT;index++){
        root_position.board[index] = EDGE;
    }
    //盤上はEMPTYに設定
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row); 
            root_position.board[sq] = EMPTY;
        }
    }
    //駒台もEMPTYに設定
    for(int sq = 208;sq < 222;sq++){
        root_position.board[sq] = EMPTY;
    }
    //盤上設定
    int col = 1,row = 1;
    for(index = 0;token.size() > index;index++){
        if(isdigit(token[index])){
            col += (token[index] - '1' + 1);
        }
        else{
            int sq = make_square(col,row);
            switch(token[index]){
            case '+': pmoto = -8; break; 
            case 'k': put_piece(W_KING,sq,0); col++; break;
            case 'g': put_piece(W_GOLD,sq,0); col++; break;
            case 'p': put_piece(W_PAWN+pmoto,sq,0); col++; pmoto = 0;break;
            case 'l': put_piece(W_LANCE+pmoto,sq,0); col++; pmoto = 0;break;
            case 'n': put_piece(W_KNIGHT+pmoto,sq,0); col++; pmoto = 0;break;
            case 's': put_piece(W_SILVER+pmoto,sq,0); col++; pmoto = 0;break; 
            case 'b': put_piece(W_BISHOP+pmoto,sq,0); col++; pmoto = 0;break;
            case 'r': put_piece(W_ROOK+pmoto,sq,0); col++; pmoto = 0;break;
            
            case 'K': put_piece(B_KING,sq,0); col++; break;
            case 'G': put_piece(B_GOLD,sq,0); col++; break;
            case 'P': put_piece(B_PAWN+pmoto,sq,0); col++; pmoto = 0;break;
            case 'L': put_piece(B_LANCE+pmoto,sq,0); col++; pmoto = 0;break;
            case 'N': put_piece(B_KNIGHT+pmoto,sq,0); col++; pmoto = 0;break;
            case 'S': put_piece(B_SILVER+pmoto,sq,0); col++; pmoto = 0;break; 
            case 'B': put_piece(B_BISHOP+pmoto,sq,0); col++; pmoto = 0;break;
            case 'R': put_piece(B_ROOK+pmoto,sq,0); col++; pmoto = 0;break;
            case '/': col = 1; row++; break;
            case ' ': break;
            default: 
                cout << "Error in SFEN charracter" << endl;
                _ASSERT(false);
                return;
            }
        }
    }
    //手番設定
    token = uip.get_next_token();

    if(token[0] == 'b'){
        root_position.turn = BLACK;
    }
    else if(token[0] == 'w'){
        root_position.turn = WHITE;
    }
    else{
        cout << "Error in SFEN charracter" << endl;
        _ASSERT(false);
        return;
    }
    //持ち駒設定(サンプル S2Pb18p）
    token = uip.get_next_token();
    for(index = 0;token.size() > index;index++){
        char num;
        if(isdigit(token[index]) && isalnum(token[index+1])){   //最初に数字が来るパターンに対応
            if(isdigit(token[index+1])){    //２桁の数字に対応
                num = atoi(token.substr(index,2).c_str());
                index += 2;
            }
            else{                           //１桁の数字に対応
                num = atoi(token.substr(index,1).c_str());
                index +=1;
            }
        }
        else if(isalpha(token[index])){  //数字がないパターンに対応
            num = 1;
        }
        else if(token[index] == '-'){   //持ち駒がないパターンに対応
            break;
        }
        else{       //どれでもない
            cout << "Error in SFEN charracter" << endl;
            _ASSERT(false);
            return; 
        }
        switch(token[index]){
        case 'g': put_piece(W_GOLD,215,num); break;
        case 'p': put_piece(W_PAWN,216,num); break;
        case 'l': put_piece(W_LANCE,217,num); break;
        case 'n': put_piece(W_KNIGHT,218,num); break;
        case 's': put_piece(W_SILVER,219,num); break;
        case 'b': put_piece(W_BISHOP,220,num); break;
        case 'r': put_piece(W_ROOK,221,num); break;

        case 'G': put_piece(B_GOLD,208,num); break;
        case 'P': put_piece(B_PAWN,209,num); break;
        case 'L': put_piece(B_LANCE,210,num); break;
        case 'N': put_piece(B_KNIGHT,211,num); break;
        case 'S': put_piece(B_SILVER,212,num); break; 
        case 'B': put_piece(B_BISHOP,213,num); break;
        case 'R': put_piece(B_ROOK,214,num); break;
        }
    }
    //持ち駒の次は手数が入っているが無視してよい
}

string to_sfen(const Position &pos)
{
    string result;
    int skip;
    char p;

    for(int row = 1;row < 10;row++){
        skip = 0;
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            p = pos.board[sq];
            if(p != EMPTY){
                if(skip > 0){
                    result += (char)skip + '0';
                }
                result += piece_letters[p];
                skip = 0;
            }
            else{
                skip++;
            }
        }
        if(skip > 0){ 
            result += (char)skip + '0';
        }
        result += row < 9 ? '/' : ' ';
    }
    result += (pos.turn == WHITE) ? 'w'  : 'b';
    result += ' ';
    //ここは駒台の処理
    bool is_stand = false;
    for(int sq = 208;sq < 215;sq++){
        int num = pos.board[sq];
        if(num > 0){
            is_stand = true;
            if(num >1){
                result += (char)num + '0';
            }
            result += piece_letters[sq - 199];
        }
    }
    for(int sq = 215;sq < 222;sq++){
        int num = pos.board[sq];
        if(num > 0){
            is_stand = true;
            if(num >1){
                result += (char)num + '0';
            }
            result += piece_letters[sq - 222];
        }
    }
    if(!is_stand){
        //持ち駒がなかった場合
        result += '-';
    }
    result += ' ';
    result += '1';
    return result;
}

//sfen文字列からpositionを設定
void put_piece(char p,int sq,int num)
{
    char pt = type_of_piece(p);
    Color c = color_of_piece(p);
    if(sq < BOARD_UPPER){
        //MAIN boardに駒コードを入れる
        root_position.board[sq] = p;
        if(pt == KING){
            //ColorはWHITE=-1,BLACK=0で配列に収まらないので+1にしてWHITE=0,BLACK=1にしている
            //root_position.king_square[c+1] = sq;
            root_position.board[223+c] = sq;    //強制的にintからcharに入れている
        }
    }
    else{
        //こちらは駒台、駒種によって座標は固定されている、格納するのは枚数
        root_position.board[sq] = num;
    }
}
*/
/*
void print_board(const Position &pos)
{
    for(int sq = 215;sq < 222;sq++){
        int num = pos.board[sq];
        if(num > 0){
            cout << piece_letters[sq - 222] << ":" << num << " ";
        }
    }
    cout << endl;
    for(int row = 1;row <10;row++){
        cout << "+--+--+--+--+--+--+--+--+--+" << endl;
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            char p = pos.board[sq];
            if(p != EMPTY){
                cout << "|" << setw(2) << piece_letters[p];
            }
            else{
                cout << "|  ";
            }
        }
        cout << "|" << endl;
    }
    cout << "+--+--+--+--+--+--+--+--+--+" << endl;
    for(int sq = 208;sq < 215;sq++){
        int num = pos.board[sq];
        if(num > 0){
            cout << piece_letters[sq - 199] << ":" << num << " ";
        }
    }
    cout << endl;
    cout << to_sfen(pos) << endl;
}
*/
/*
BLACK,WHITEと分かれていることを忘れないように
*/
/*
short *do_move_b(Position &pos,Move m,short *mf)
{
    int from = move_from(m);
    int to = move_to(m);
    char cp;
    char p;

    if(from != 0){
        //盤上の手
        p = pos.board[from];
        char pt = type_of_piece(p);
        if(pt == KING){
            *(mf++) = 223;
            *(mf++) = pos.board[223];
            pos.board[223] = to;  //強制的にintからcharに入れている
        }
        if(pos.board[to] != EMPTY){
            cp = pos.board[to] & 0x0F;
            int of = 208 + (cp | 0x08) - 9;
            *(mf++) = of;
            *(mf++) = pos.board[of];
            pos.board[of] += 1;
            sech.material += cap_piece_value[cp];
        }
        *(mf++) = to; 
        *(mf++) = pos.board[to];    //変更を場所、内容の順に登録しておく
        //成処理
        if(m & 0x10000){
            pos.board[to] = p-8;
            sech.material += pmoto_piece_value[p-8];
        }
        else{
            pos.board[to] = p;
        }
        *(mf++) = from; 
        *(mf++) = p;                //変更を場所、内容の順に登録しておく
        pos.board[from] = EMPTY;
    }
    else{
        //打つ手　打つ手はmaterialは変化しない（盤上の駒と駒台の駒の価値が一緒のためBonanza方式）
        p = move_piece(m);  //打つ駒種を取り出す
        *(mf++) = to; 
        *(mf++) = EMPTY;    //変更を場所、内容の順に登録しておく
        pos.board[to] = pos.turn ? 0xF0 | p : p;    //駒コードに変換
        int of = 208 + p - 9;
        *(mf++) = of;
        *(mf++) = pos.board[of];
        pos.board[of] -= 1;
    }
    pos.turn = ~pos.turn;
    return mf;
}
*/
/*
BLACK,WHITEと分かれていることを忘れないように
*/
/*
short *do_move_w(Position &pos,Move m,short *mf)
{
    int from = move_from(m);
    int to = move_to(m);
    char cp;
    char p;

    if(from != 0){
        //盤上の手
        p = pos.board[from];
        char pt = type_of_piece(p);
        if(pt == KING){
            *(mf++) = 222;;
            *(mf++) = pos.board[222];
            pos.board[222] = to;  //強制的にintからcharに入れている
        }
        if(pos.board[to] != EMPTY){
            cp = pos.board[to] & 0x0F;
            int of = 215 + (cp | 0x08) - 9;
            *(mf++) = of;
            *(mf++) = pos.board[of];
            pos.board[of] += 1;
            sech.material -= cap_piece_value[cp];
        }
        *(mf++) = to; 
        *(mf++) = pos.board[to];    //変更を場所、内容の順に登録しておく
        //成処理
        if(m & 0x10000){
            pos.board[to] = p-8;
            sech.material -= pmoto_piece_value[(p-8) & 0x0F];
        }
        else{
            pos.board[to] = p;
        }
        *(mf++) = from; 
        *(mf++) = p;                //変更を場所、内容の順に登録しておく
        pos.board[from] = EMPTY;
    }
    else{
        //打つ手 打つ手はmaterialは変化しない（盤上の駒と駒台の駒の価値が一緒のためBonanza方式）
        p = move_piece(m);  //打つ駒種を取り出す
        *(mf++) = to; 
        *(mf++) = EMPTY;    //変更を場所、内容の順に登録しておく
        pos.board[to] = pos.turn ? 0xF0 | p : p;    //駒コードに変換
        int of = 215 + p - 9;
        *(mf++) = of;
        *(mf++) = pos.board[of];
        pos.board[of] -= 1;
    }
    pos.turn = ~pos.turn;
    return mf;
}

void undo_move(Position &pos,int ply)
{
    int sq;

    for(short *mf = next_modify[ply].next_dirty;mf != next_modify[ply].last_dirty;){
        sq = *(mf++);
        pos.board[sq] = *(mf++);
    }
    pos.turn = ~pos.turn;
}

void is_ok(Position &pos)
{
    char p;
    int sq;
    int piece_count[16];

    for(int i = 0;i < 16;i++){
        piece_count[i] = 0;
    }
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            sq = make_square(col,row);
            p = pos.board[sq];
            if(p == EMPTY){
                continue;
            }
            piece_count[type_of_piece(p)]++;
        }
    }
    for(int sq = 208;sq < 215;sq++){
        piece_count[sq-208+9] += pos.board[sq];
    }
    for(int sq = 215;sq < 222;sq++){
        piece_count[sq-215+9] += pos.board[sq];
    }

    int sum = 0;
    for(int i = 0;i < 16;i++){
        sum += piece_count[i];
    }
    _ASSERT(sum == 40);
    _ASSERT((piece_count[PAWN] + piece_count[PPAWN]) == 18);
    _ASSERT((piece_count[LANCE] + piece_count[PLANCE]) == 4);
    _ASSERT((piece_count[KNIGHT] + piece_count[PKNIGHT]) == 4);
    _ASSERT((piece_count[SILVER] + piece_count[PSILVER]) == 4);
    _ASSERT(piece_count[GOLD] == 4);
    _ASSERT((piece_count[BISHOP] + piece_count[PBISHOP]) == 2);
    _ASSERT((piece_count[ROOK] + piece_count[PROOK]) == 2);
    _ASSERT(piece_count[KING] == 2);
    //BLACK側死に駒の有無
    for(int sq = SQ_9A;sq <= SQ_1A;sq++){
        char p = pos.board[sq];
        if((p == B_PAWN) || (p == B_LANCE) || (p == B_KNIGHT)){
            _ASSERT(false);
        }
    }
    for(int sq = SQ_9B;sq <= SQ_1B;sq++){
        char p = pos.board[sq];
        if(p == B_KNIGHT){
            _ASSERT(false);
        }
    }
    //WHITE側死に駒の有無
    for(int sq = SQ_9I;sq <= SQ_1I;sq++){
        char p = pos.board[sq];
        if((p == W_PAWN) || (p == W_LANCE) || (p == W_KNIGHT)){
            _ASSERT(false);
        }
    }
    for(int sq = SQ_9H;sq <= SQ_1H;sq++){
        char p = pos.board[sq];
        if(p == W_KNIGHT){
            _ASSERT(false);
        }
    }
}

TEST(position,color_of_piece)
{
    char p = W_LANCE;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = W_PAWN;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = W_LANCE;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = W_KING;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = WP_PAWN;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = WP_SILVER;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = WP_ROOK;
    ASSERT_EQ(WHITE,color_of_piece(p));
    p = WP_BISHOP;
    ASSERT_EQ(WHITE,color_of_piece(p));

    p = B_LANCE;
    ASSERT_EQ(BLACK,color_of_piece(p));
    p = B_KING;
    ASSERT_EQ(BLACK,color_of_piece(p));
    p = BP_PAWN;
    ASSERT_EQ(BLACK,color_of_piece(p));
    p = BP_SILVER;
    ASSERT_EQ(BLACK,color_of_piece(p));
    p = BP_ROOK;
    ASSERT_EQ(BLACK,color_of_piece(p));
    p = B_BISHOP;
    ASSERT_EQ(BLACK,color_of_piece(p));
}

TEST(position,piece_type)
{
    //不成り、成り判定
    EXPECT_FALSE(is_not_pmoto(BP_PAWN));  //BP_PAWNは成っていないよね->成っているのでFALSE
    EXPECT_FALSE(is_not_pmoto(BP_LANCE));
    EXPECT_FALSE(is_not_pmoto(BP_KNIGHT));
    EXPECT_FALSE(is_not_pmoto(BP_SILVER));
    EXPECT_FALSE(is_not_pmoto(BP_BISHOP));
    EXPECT_FALSE(is_not_pmoto(BP_ROOK));
    EXPECT_TRUE(is_not_pmoto(B_PAWN));    //B_PAWNは成っていないよね->成っていないのでTRUE
    EXPECT_TRUE(is_not_pmoto(B_LANCE));
    EXPECT_TRUE(is_not_pmoto(B_KNIGHT));
    EXPECT_TRUE(is_not_pmoto(B_SILVER));
    EXPECT_TRUE(is_not_pmoto(B_BISHOP));
    EXPECT_TRUE(is_not_pmoto(B_ROOK));
    
    EXPECT_FALSE(is_not_pmoto(WP_PAWN));
    EXPECT_FALSE(is_not_pmoto(WP_LANCE));
    EXPECT_FALSE(is_not_pmoto(WP_KNIGHT));
    EXPECT_FALSE(is_not_pmoto(WP_SILVER));
    EXPECT_FALSE(is_not_pmoto(WP_BISHOP));
    EXPECT_FALSE(is_not_pmoto(WP_ROOK));
    EXPECT_TRUE(is_not_pmoto(W_PAWN));
    EXPECT_TRUE(is_not_pmoto(W_LANCE));
    EXPECT_TRUE(is_not_pmoto(W_KNIGHT));
    EXPECT_TRUE(is_not_pmoto(W_SILVER));
    EXPECT_TRUE(is_not_pmoto(W_BISHOP));
    EXPECT_TRUE(is_not_pmoto(W_ROOK));
    //先手、後手判別
    char p;
    p = BP_PAWN;
    EXPECT_EQ(true,p > 1);  //先手判定
    EXPECT_EQ(true,p > 1 || p == 0); //先手＆空白判定
    p = WP_PAWN;
    EXPECT_EQ(false,p > 1);  //先手判定
    EXPECT_EQ(false,p > 1 || p == 0); //先手＆空白判定

    p = BP_PAWN;
    EXPECT_EQ(false,p < 0);  //後手判定
    EXPECT_EQ(false,p <= 0);  //後手＆空白判定
    p = WP_PAWN;
    EXPECT_EQ(true,p < 0);  //後手判定
    EXPECT_EQ(true,p <= 0);  //後手＆空白判定

    //先手->後手
    EXPECT_EQ(WP_PAWN,do_white(BP_PAWN));
    EXPECT_EQ(WP_LANCE,do_white(BP_LANCE));
    EXPECT_EQ(WP_KNIGHT,do_white(BP_KNIGHT));
    EXPECT_EQ(WP_SILVER,do_white(BP_SILVER));
    EXPECT_EQ(WP_SILVER,do_white(BP_SILVER));
    EXPECT_EQ(WP_BISHOP,do_white(BP_BISHOP));
    EXPECT_EQ(WP_ROOK,do_white(BP_ROOK));
    EXPECT_EQ(W_KING,do_white(B_KING));
    EXPECT_EQ(W_GOLD,do_white(B_GOLD));
    EXPECT_EQ(W_PAWN,do_white(B_PAWN));
    EXPECT_EQ(W_LANCE,do_white(B_LANCE));
    EXPECT_EQ(W_KNIGHT,do_white(B_KNIGHT));
    EXPECT_EQ(W_SILVER,do_white(B_SILVER));
    EXPECT_EQ(W_BISHOP,do_white(B_BISHOP));
    EXPECT_EQ(W_ROOK,do_white(B_ROOK));
    //後手->先手
    p = WP_PAWN;
    EXPECT_EQ(BP_PAWN,do_black(WP_PAWN));
    EXPECT_EQ(BP_LANCE,do_black(WP_LANCE));
    EXPECT_EQ(BP_KNIGHT,do_black(WP_KNIGHT));
    EXPECT_EQ(BP_SILVER,do_black(WP_SILVER));
    EXPECT_EQ(BP_SILVER,do_black(WP_SILVER));
    EXPECT_EQ(BP_BISHOP,do_black(WP_BISHOP));
    EXPECT_EQ(BP_ROOK,do_black(WP_ROOK));
    EXPECT_EQ(B_KING,do_black(W_KING));
    EXPECT_EQ(B_GOLD,do_black(W_GOLD));
    EXPECT_EQ(B_PAWN,do_black(W_PAWN));
    EXPECT_EQ(B_LANCE,do_black(W_LANCE));
    EXPECT_EQ(B_KNIGHT,do_black(W_KNIGHT));
    EXPECT_EQ(B_SILVER,do_black(W_SILVER));
    EXPECT_EQ(B_BISHOP,do_black(W_BISHOP));
    EXPECT_EQ(B_ROOK,do_black(W_ROOK));
}

TEST(position,from_sfen)
{
    from_sfen(start_position);
    EXPECT_EQ(W_LANCE,root_position.board[SQ_9A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_8A]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_7A]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_6A]);
    EXPECT_EQ(W_KING,root_position.board[SQ_5A]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_4A]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_3A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_2A]);
    EXPECT_EQ(W_LANCE,root_position.board[SQ_1A]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9B]);
    EXPECT_EQ(W_ROOK,root_position.board[SQ_8B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3B]);
    EXPECT_EQ(W_BISHOP,root_position.board[SQ_2B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1B]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_9C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_8C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_7C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_6C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_5C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_4C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_3C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_2C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_1C]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1D]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1E]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1F]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_9G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_8G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_7G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_6G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_5G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_4G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_3G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_2G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_1G]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9H]);
    EXPECT_EQ(B_BISHOP,root_position.board[SQ_8H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3H]);
    EXPECT_EQ(B_ROOK,root_position.board[SQ_2H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1H]);

    EXPECT_EQ(B_LANCE,root_position.board[SQ_9I]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_8I]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_7I]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_6I]);
    EXPECT_EQ(B_KING,root_position.board[SQ_5I]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_4I]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_3I]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_2I]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_1I]);
    EXPECT_EQ(SQ_5A,(unsigned char)root_position.board[222]);
    EXPECT_EQ(SQ_5I,(unsigned char)root_position.board[223]);

    from_sfen(string("lnsgkgsn1/1r5b1/pppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1"));
    EXPECT_EQ(W_LANCE,root_position.board[SQ_9A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1A]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_9I]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_1I]);
    
    //拡張したfrom_sfen用のテスト
    from_sfen(string("5gk2/+P+L+N+S5/ppppppp2/9/9/9/PPPP5/5+s+n+l+p/2KG5 b PL2SBR2g4pl2nbr 1"));
    EXPECT_EQ(EMPTY,root_position.board[SQ_9A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5A]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_4A]);
    EXPECT_EQ(W_KING,root_position.board[SQ_3A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1A]);

    EXPECT_EQ(BP_PAWN,root_position.board[SQ_9B]);
    EXPECT_EQ(BP_LANCE,root_position.board[SQ_8B]);
    EXPECT_EQ(BP_KNIGHT,root_position.board[SQ_7B]);
    EXPECT_EQ(BP_SILVER,root_position.board[SQ_6B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1B]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_9C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_8C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_7C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_6C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_5C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_4C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_3C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1C]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1D]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1E]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1F]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_9G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_8G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_7G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_6G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1G]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5H]);
    EXPECT_EQ(WP_SILVER,root_position.board[SQ_4H]);
    EXPECT_EQ(WP_KNIGHT,root_position.board[SQ_3H]);
    EXPECT_EQ(WP_LANCE,root_position.board[SQ_2H]);
    EXPECT_EQ(WP_PAWN,root_position.board[SQ_1H]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8I]);
    EXPECT_EQ(B_KING,root_position.board[SQ_7I]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_6I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1I]);
    //持ち駒のチエック
    EXPECT_EQ(0,root_position.board[208]);  //black gold
    EXPECT_EQ(1,root_position.board[209]);  //black pawn
    EXPECT_EQ(1,root_position.board[210]);  //black lance
    EXPECT_EQ(0,root_position.board[211]);  //black knight
    EXPECT_EQ(2,root_position.board[212]);  //black silver
    EXPECT_EQ(1,root_position.board[213]);  //black bishop
    EXPECT_EQ(1,root_position.board[214]);  //black rook

    EXPECT_EQ(2,root_position.board[215]);  //white gold
    EXPECT_EQ(4,root_position.board[216]);  //white pawn
    EXPECT_EQ(1,root_position.board[217]);  //white lance
    EXPECT_EQ(2,root_position.board[218]);  //white knight
    EXPECT_EQ(0,root_position.board[219]);  //white silver
    EXPECT_EQ(1,root_position.board[220]);  //white bishop
    EXPECT_EQ(1,root_position.board[221]);  //white rook

    //EXPECT_EQ(SQ_3A,(unsigned char)root_position.board[223+WHITE] );
    //EXPECT_EQ(SQ_7I,(unsigned char)root_position.board[223+BLACK] );
}
*/
/*
TEST(position,to_sfen)
{
    from_sfen(start_position);
    EXPECT_STREQ("lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1",to_sfen(root_position).c_str());

    for(int sq = 0;sq < 222;sq++){
        root_position.board[sq] = EMPTY;
    }
    root_position.board[SQ_4A] = W_GOLD;
    root_position.board[SQ_3A] = W_KING;

    root_position.board[SQ_9B] = BP_PAWN;
    root_position.board[SQ_8B] = BP_LANCE;
    root_position.board[SQ_7B] = BP_KNIGHT;
    root_position.board[SQ_6B] = BP_SILVER;

    root_position.board[SQ_9C] = W_PAWN;
    root_position.board[SQ_8C] = W_PAWN;
    root_position.board[SQ_7C] = W_PAWN;
    root_position.board[SQ_6C] = W_PAWN;
    root_position.board[SQ_5C] = W_PAWN;
    root_position.board[SQ_4C] = W_PAWN;
    root_position.board[SQ_3C] = W_PAWN;

    root_position.board[SQ_9G] = B_PAWN;
    root_position.board[SQ_8G] = B_PAWN;
    root_position.board[SQ_7G] = B_PAWN;
    root_position.board[SQ_6G] = B_PAWN;

    root_position.board[SQ_4H] = WP_SILVER;
    root_position.board[SQ_3H] = WP_KNIGHT;
    root_position.board[SQ_2H] = WP_LANCE;
    root_position.board[SQ_1H] = WP_PAWN;

    root_position.board[SQ_7I] = B_KING;
    root_position.board[SQ_6I] = B_GOLD;

    root_position.board[208] = 0;   //B_GOLD
    root_position.board[209] = 1;   //B_PAWN
    root_position.board[210] = 1;   //B_LANCE
    root_position.board[211] = 0;   //B_KNIGHT
    root_position.board[212] = 2;   //B_SILVER
    root_position.board[213] = 1;   //B_BISHOP
    root_position.board[214] = 1;   //B_ROOK

    root_position.board[215] = 2;   //W_GOLD
    root_position.board[216] = 4;   //W_PAWN
    root_position.board[217] = 1;   //W_LANCE
    root_position.board[218] = 2;   //W_KNIGHT
    root_position.board[219] = 0;   //W_SILVER
    root_position.board[220] = 1;   //W_BISHOP
    root_position.board[221] = 1;   //W_ROOK

    EXPECT_STREQ("5gk2/+P+L+N+S5/ppppppp2/9/9/9/PPPP5/5+s+n+l+p/2KG5 b PL2SBR2g4pl2nbr 1",to_sfen(root_position).c_str());
}

TEST(position,make_square)
{
    //make_square(col,row)で指定
    EXPECT_EQ(166,make_square(6,9));
    EXPECT_EQ(72,make_square(8,3));
    EXPECT_EQ(116,make_square(4,6));
    EXPECT_EQ(41,make_square(9,1));
    EXPECT_EQ(33,make_square(1,1));
    EXPECT_EQ(169,make_square(9,9));
    EXPECT_EQ(161,make_square(1,9));
    EXPECT_EQ(101,make_square(5,5));
    EXPECT_EQ(120,make_square(8,6));
    EXPECT_EQ(68,make_square(4,3));
}

TEST(position,make_col_row)
{
    int col,row;

    make_col_row(SQ_9A,&col,&row);
    EXPECT_EQ(1,col);
    EXPECT_EQ(1,row);

    make_col_row(SQ_8B,&col,&row);
    EXPECT_EQ(2,col);
    EXPECT_EQ(2,row);

    make_col_row(SQ_7C,&col,&row);
    EXPECT_EQ(3,col);
    EXPECT_EQ(3,row);

    make_col_row(SQ_6D,&col,&row);
    EXPECT_EQ(4,col);
    EXPECT_EQ(4,row);

    make_col_row(SQ_5E,&col,&row);
    EXPECT_EQ(5,col);
    EXPECT_EQ(5,row);

    make_col_row(SQ_4F,&col,&row);
    EXPECT_EQ(6,col);
    EXPECT_EQ(6,row);

    make_col_row(SQ_3G,&col,&row);
    EXPECT_EQ(7,col);
    EXPECT_EQ(7,row);

    make_col_row(SQ_2H,&col,&row);
    EXPECT_EQ(8,col);
    EXPECT_EQ(8,row);

    make_col_row(SQ_1I,&col,&row);
    EXPECT_EQ(9,col);
    EXPECT_EQ(9,row);
}

int update_board(USIInputParser &uip);      //TESTのヘルパー関数
int update_board_material(USIInputParser &uip,int material[],int row,int col);
void is_eq_board(void);                     //TESTのヘルパー関数

TEST(position,undo_move)
{
    int ply = 0;
    string q_str[11] = {
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6g6f 7a6b 5g5f 5c5d 3i4h 3a4b 4i5h 4a3b 5h6g 5a4a 6i7h 6a5b 5i6i 4b3c 6h7g 2b3a 8h7i 4c4d 3g3f 5b4c 7i6h 7c7d 6i7i 8d8e 2h3h 6b7c 7i8h 4c5c 6g5g 5c5b 5g5h 7c8d 6h4f 8b9b 4h3g 4d4e 4f5g 4a4b 3g2f 7d7e 7f7e 8d7e P*7f 7e8d 2i3g 3c4d 6f6e 9b8b 4g4f 5d5e 4f4e 4d3c 3g2e 3c2b 5g4f 2c2d 4f2d 2a3c 2e3c+ 2b3c 2d4f 8a7c N*4d 3c4d 4e4d P*4e S*4c 3b4c 4d4c+ 4b4c 4f5e N*4f P*4d 4c5d 3h1h 4f5h+ 1h5h S*6i 7h7i 6i5h+ N*7d 8b8a G*6d 3a6d 6e6d P*2e 7d6b+ 2e2f 6b5b G*3b B*7d N*6a 7d6c+ 5d6e G*6f",
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6g6f 7a6b 5g5f 5c5d 3i4h 3a4b 4i5h 4a3b 5h6g 5a4a 6i7h 6a5b 5i6i 4b3c 6h7g 2b3a 8h7i 4c4d 3g3f 5b4c 7i6h 7c7d 6i7i 3a6d 2i3g 4a3a 2g2f 3a2b 7i8h 8d8e 2h3h 6b5c 1g1f 5d5e 6f6e 6d7c 5f5e 6c6d 6e6d 5c6d P*6e P*6f 7g6f 6d5e 6f5e 7c5e S*6f 5e7c 3h3i S*2h 3i3h 2h1i+ 4h5g L*8c 5g5f 9c9d 7f7e 8e8f 7e7d 8f8g+ 8h7i 8g8h 7i6i 8h7h 6i7h 8c8h+ 7h6i G*7h 6i5i 7h6h 6g6h 7c9e 3h3i P*6g 5f6g B*2h 3i3h 2h1g+ G*2h 1g2f 3h3i 8h9i P*8c 8b5b P*5c 5b5c P*5d 5c5d P*5e 5d7d P*7e 7d8d 6g7h P*6g 6h7g 6g6h+ 5i6h P*8h 9g9f L*2g 2h2g 2f2g 9f9e G*2i 3i2i 1i2i L*8f R*1h P*2h 8d8f 7g8f 8h8i+ 8c8b+ 1h2h+ 6h7g 2g3g 8b8a 8i7i 7h6g 3g5i 7g7f N*8d 7f8e 2h8h B*8g 8h9g N*7g N*7c 8e7d 9g8f G*8h G*9f R*5f 5i3g 8a9a 3d3e 8g9f 8d9f G*8g B*8c 7d7c P*7b 7c8b 8f8d 8g9f P*8g 9f8e 8d7c 8b9c 8c7d L*8c 7c8c",
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6h7g 7a6b 5g5f 5c5d 3i4h 3a4b 4i5h 4a3b 6g6f 5a4a 5h6g 6a5b 8h7i 8d8e 3g3f 4b3c 5i6h 7c7d 6i7h 4c4d 2g2f 5b4c 2f2e 4a3a 9g9f 6c6d 9f9e 1c1d 7i9g 2b1c 4h5g 6b5c 7g8f 3c2d 8i7g 2a3c 6f6e 8b6b 8f8e 8a7c 8e7d 7c6e 7g6e 6d6e P*6f N*4e 2h5h 4e5g+ 6g5g 2d2e P*2f 2e3f 5h3h 3f4g+ 9g5c+ 4c5c 7d7c+ 1c5g+ 6h7g 5g6f 7g6h B*4f 6h6i S*5h 3h5h 4g5h 6i5h 4f5g+ 5h6i G*5h",
        "startpos moves 7g7f 8c8d 2h6h 3c3d 6g6f 7a6b 5i4h 5a4b 4h3h 5c5d 3h2h 4b3b 3i3h 6a5b 7i7h 3a4b 7h6g 7c7d 1g1f 4b5c 5g5f 4a4b 6i5h 8d8e 8h7g 6b7c 9i9h 1c1d 4g4f 7c8d 6h7h 6c6d 3g3f 7d7e 5h4g 7e7f 6g7f 8b7b 6f6e 2b7g+ 7h7g B*8h 7f8e 8h7g+ 8i7g 7b7g+ 8e8d 7g8g S*7e P*7d P*8h 8g8h B*6f N*7g 7e7d R*6h B*5g 6h6i 6f1a+ 6i4i+ P*7h 8h7h L*6h 4i6i P*8b 7h8i 8b8a+ G*5h 5g6f 5c4d 7d7c 6d6e 6f7g P*7f 7g9e 9c9d 8a9a 9d9e L*2b 6i3i 2h1g B*2h 1g2f 3i3h 4f4e 4d3e 2f2e 3e2d 2e1d S*1c",
        "startpos moves 7g7f 3c3d 6g6f 8c8d 2h6h 7a6b 5i4h 5c5d 4h3h 5a4b 7i7h 4b3b 8h7g 6a5b 3h2h 8d8e 6i5h 3a4b 3i3h 7c7d 1g1f 1c1d 7h6g 4b5c 5g5f 6c6d 4g4f 6d6e 5h4g 8a7c 3g3f 8e8f 7g8f 6e6f 6g6f P*6e 6f7g 6b6c 2i3g 5c6d 2g2f 4a4b 9g9f 9c9d 9i9h 8b8a 6h5h 8a8f 8g8f 2b4d R*8a 6e6f 7g6h B*7h 4f4e 4d6b P*6e 6f6g+ 6e6d 6g5h 6d6c 6b2f 4i5h 7h8i+ P*2b 3b2b S*3a 2b1b 3a4b+ 5b4b 8a9a+ 8i9h 9a8b 2f5c 6c6b+ L*2d P*2e 9h7f 6h6g R*2f L*2g P*6a 6g7f N*3e 3f3e S*3f 8b7c 2f2g 3h2g 3f2g+ 2h2g S*4i 6b6c L*7a 7c8c P*8b 8c9c 4i5h 6c5c G*8c S*8d 5h4g 5c4b G*3f 2g2h 4g4h R*3b 1b1c B*3a",
        "startpos moves 7g7f 8c8d 2g2f 3c3d 2f2e 8d8e 6i7h 8e8f 8g8f 8b8f P*8g 8f8d 2e2d 2c2d 2h2d 4a3b 2d2f P*2c 1g1f 1c1d 5i6h 9c9d 9g9f 5a4b 3i3h 7a7b 4g4f P*8f 8g8f 2b8h+ 7i8h 8d8f B*7g B*4d 2f2c+ 4d7g+ 8h7g 8f8i+ 2c2a 3a2b 7g8h 3b3a B*2d 4b4a 2a3a 4a3a 7h7i 8i7i 8h7i N*1b G*4b 3a2a N*1c 2b1c R*3a 2a2b 2d3c+",
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6h7g 7a6b 3i4h 5c5d 5g5f 6b5c 6i7h 5c6d 2g2f 3a4b 2f2e 4b3c 4h5g 4a3b 5g4f 5a4a 5i6i 8d8e 3g3f 3c4d 7g6f 7c7d 2e2d 2c2d 2h2d P*2c 2d2f 8e8f 8g8f 8b8f P*8g 8f8d 2i3g 6a5b 4i5h 8a7c 1g1f 1c1d 9g9f 9c9d 3f3e 3d3e 5f5e 5d5e 1f1e 1d1e 4f4e 4d4e 3g4e P*8f 8g8f P*8g 7h8g 4c4d P*5c 5b6b P*2d 2c2d S*7a S*3g 2f2g 6b6a P*3c 2a3c 5c5b+ 4a5b 4e3c+ 2b3c 2g3g 6a7a 3g3e P*8e 3e3c+ 3b3c S*2b R*3i P*5i 8e8f B*5g S*7h 6i6h N*5f 6h7h 8f8g+ 7h7i G*7h",
        "startpos moves 7g7f 3c3d 2g2f 8c8d 2f2e 8d8e 6i7h 4a3b 2e2d 2c2d 2h2d 8e8f 8g8f 8b8f 2d3d 2a3c 3d3f 8f8d 5i5h 1c1d 4i3h 5a6b 9g9f 6b7b 8i7g 8d2d P*2g 6a6b 7f7e 3a4b 8h9g 2b1c 3i4h 2d5d 7h6h P*8g 7i7h 3c4e 3f3b+ 4e5g+ 6h5g 8g8h+ P*5e 5d5e N*8d 7b8b 3b4b 8h7h 4b6b 7a6b P*8c 8b8c S*7b 8c8d G*8e",
        "startpos moves 7g7f 8c8d 7i7h 3c3d 7h7g 7a6b 2g2f 3a4b 3i4h 5c5d 5g5f 4a3b 6i7h 5a4a 5i6i 6a5b 4i5h 4b3c 3g3f 2b3a 8h7i 4c4d 6g6f 7c7d 2f2e 3a6d 7i4f 6d4f 4g4f 6b5c 4h5g 4a3a 6i7i 3a2b 7i8h 5b4c 5h6g 8d8e 1g1f B*4g B*5h 4g5h+ 2h5h B*4g 5h2h 4g3f+ 1f1e 9c9d 9g9f 5c6b 9i9h 8a7c 8h9i 6b5c 8i9g 9d9e 9f9e P*9f P*3g 3f3e B*7a 8b8a 3g3f 3e3f 7a5c+ 4c5c S*6b 9f9g+ 6b5c+ 9g9h 9i9h 9a9e P*9g P*9f 9g9f 9e9f P*9g 9f9g 9h8h N*4a 5c6c L*2f 2h3h 3f4g 3h3g 4g2i 6c7b 8a9a P*9b 9g9h+ 8h7i 9a9b G*8c 2i1i 3g1g 1i2h 8c9b 2h1g 7b7c 2f2g+ R*5a R*9i 7i6h B*5i 6h5h 5i3g+ 5g4h 3g4h 5h4h 1g2f 4h5g 9i5i+ 5g4g 2f3g",
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6h7g 7a6b 2g2f 3a4b 3i4h 7c7d 6i7h 5c5d 5i6i 4a3b 4i5h 5a4a 5g5f 6a5b 3g3f 4b3c 8h7i 2b3a 6g6f 4c4d 1g1f 5b4c 1f1e 6b7c 1i1g 7c6d 5h6g 8b5b 4g4f 8a7c 4h4g 9c9d 9g9f 3a4b 2i3g 6d5c 7h6h 4b3a 6i7h 7c8e 7g8h 5c6b 4g5h 2c2d 5h5g 3a5c 6h5h 3c4b 7i6h 7d7e 7f7e 5c7e 8g8f 7e6d 8f8e 6b5c 8e8d 5b7b N*7f P*8b 8d8c+ 8b8c 2f2e 2d2e P*2b 3b2b P*2c 2b2c 2h2e 4c3c P*2b 4a3a 2e8e 2c2b 8e8c+ 7b7e P*7d 5c6b 8c8a 3a3b 8i7g 6d5c 7g6e 5c6d 7d7c+ 6b7c 6e7c+ P*8g S*4a 3b4c 7c6c 8g8h+ 7h8h S*9b 7f6d 3c2d 8a9a P*6b 6c6b P*6a B*5b 4c3c L*7h 7e7h+ 8h7h L*7c P*7d 6a6b 7d7c+ 6b6c 6d7b+ P*7a 7b7a 4b4c R*5c 3c2c 5b4c+ P*7b 4c2a N*3c S*3b 2b3b 4a3b",
        "startpos moves 2g2f 8c8d 2f2e 8d8e 6i7h 4a3b 2e2d 2c2d 2h2d P*2c 2d2f 7a7b 9g9f 9c9d 1g1f 1c1d 3i3h 6c6d 7g7f 8e8f 8g8f 8b8f P*8g 8f8b 3g3f 3c3d 3h3g 7b6c 3f3e 3d3e 3g4f 2b4d 4i3h 6c5d 2i3g 3a2b P*3d 6a5b 5i5h 5a4b 8h4d 4c4d 4f3e 5b4c 7f7e 8b8e 2f7f 7c7d B*1g 1d1e 3e4d 1e1f 4d4c 5d4c 1g2f B*2i 3h3i 1f1g 3i2i 1g1h+ G*8f 8e8f 7f8f P*8e R*8b 4c5b 8f4f P*4e 4f4e P*4d 4e4d S*4c 4d6d 1h2i 6d6b+ 1a1i+ 2f5c+ 4b4a 6b5b 4c5b 8b5b+"
    };
    for(int i = 0;i < 11;i++){
        USIInputParser uip(q_str[i]);
        ply = update_board(uip);    //do_moveで局面を更新
        for(int i = ply-1;i >= 0;i--){
            undo_move(root_position,i); //undo_moveで局面を復元
            is_ok(root_position);
        }
        is_eq_board();
    }
}
*/
/*
手を動かしてmaterialがちゃんと更新されているかチエックする
*/
/*
TEST(position,do_move_material)
{
    int ply;
    string q_str[2] = {
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6g6f 7a6b 5g5f 5c5d 3i4h 3a4b 4i5h 4a3b 5h6g 5a4a 6i7h 6a5b 5i6i 4b3c 6h7g 2b3a 8h7i 4c4d 3g3f 5b4c 7i6h 7c7d 6i7i 8d8e 2h3h 6b7c 7i8h 4c5c 6g5g 5c5b 5g5h 7c8d 6h4f 8b9b 4h3g 4d4e 4f5g 4a4b 3g2f 7d7e 7f7e 8d7e P*7f 7e8d 2i3g 3c4d 6f6e 9b8b 4g4f 5d5e 4f4e 4d3c 3g2e 3c2b 5g4f 2c2d 4f2d 2a3c 2e3c+ 2b3c 2d4f 8a7c N*4d 3c4d 4e4d P*4e S*4c 3b4c 4d4c+ 4b4c 4f5e N*4f P*4d 4c5d 3h1h 4f5h+ 1h5h S*6i 7h7i 6i5h+ N*7d 8b8a G*6d 3a6d 6e6d P*2e 7d6b+ 2e2f 6b5b G*3b B*7d N*6a 7d6c+ 5d6e G*6f",  //MOVE_TEST_Q1.csaで棋譜登録してある
        "startpos moves 7g7f 8c8d 7i6h 3c3d 6g6f 7a6b 5g5f 5c5d 3i4h 3a4b 4i5h 4a3b 5h6g 5a4a 6i7h 6a5b 5i6i 4b3c 6h7g 2b3a 8h7i 4c4d 3g3f 5b4c 7i6h 7c7d 6i7i 3a6d 2i3g 4a3a 2g2f 3a2b 7i8h 8d8e 2h3h 6b5c 1g1f 5d5e 6f6e 6d7c 5f5e 6c6d 6e6d 5c6d P*6e P*6f 7g6f 6d5e 6f5e 7c5e S*6f 5e7c 3h3i S*2h 3i3h 2h1i+ 4h5g L*8c 5g5f 9c9d 7f7e 8e8f 7e7d 8f8g+ 8h7i 8g8h 7i6i 8h7h 6i7h 8c8h+ 7h6i G*7h 6i5i 7h6h 6g6h 7c9e 3h3i P*6g 5f6g B*2h 3i3h 2h1g+ G*2h 1g2f 3h3i 8h9i P*8c 8b5b P*5c 5b5c P*5d 5c5d P*5e 5d7d P*7e 7d8d 6g7h P*6g 6h7g 6g6h+ 5i6h P*8h 9g9f L*2g 2h2g 2f2g 9f9e G*2i 3i2i 1i2i L*8f R*1h P*2h 8d8f 7g8f 8h8i+ 8c8b+ 1h2h+ 6h7g 2g3g 8b8a 8i7i 7h6g 3g5i 7g7f N*8d 7f8e 2h8h B*8g 8h9g N*7g N*7c 8e7d 9g8f G*8h G*9f R*5f 5i3g 8a9a 3d3e 8g9f 8d9f G*8g B*8c 7d7c P*7b 7c8b 8f8d 8g9f P*8g 9f8e 8d7c 8b9c 8c7d L*8c 7c8c",   //MOVE_TEST_Q2.csa
    };
    int material_answer[3][200] = {
    */
     //   {
     //       0,/*7g7f*/ 0,/*8c8d*/ 0,/*7i6h*/ 0,/*3c3d*/ 0,/*6g6f*/ 0,/*7a6b*/ 
     //       0,/*5g5f*/ 0,/*5c5d*/ 0,/*3i4h*/ 0,/*3a4b*/ 0,/*4i5h*/ 0,/*4a3b*/ 
     //       0,/*5h6g*/ 0,/*5a4a*/ 0,/*6i7h*/ 0,/*6a5b*/ 0,/*5i6i*/ 0,/*4b3c*/ 
     //       0,/*6h7g*/ 0,/*2b3a*/ 0,/*8h7i*/ 0,/*4c4d*/ 0,/*3g3f*/ 0,/*5b4c*/ 
     //       0,/*7i6h*/ 0,/*7c7d*/ 0,/*6i7i*/ 0,/*8d8e*/ 0,/*2h3h*/ 0,/*6b7c*/ 
     //       0,/*7i8h*/ 0,/*4c5c*/ 0,/*6g5g*/ 0,/*5c5b*/ 0,/*5g5h*/ 0,/*7c8d*/ 
     //       0,/*6h4f*/ 0,/*8b9b*/ 0,/*4h3g*/ 0,/*4d4e*/ 0,/*4f5g*/ 0,/*4a4b*/ 
     //       0,/*3g2f*/ 0,/*7d7e*/ DPawn+DPawn,/*7f7e*/ -DPawn-DPawn,/*8d7e*/ 0,/*P*7f*/ 0,/*7e8d*/ 
     //       0,/*2i3g*/ 0,/*3c4d*/ 0,/*6f6e*/ 0,/*9b8b*/ 0,/*4g4f*/ 0,/*5d5e*/ 
     //       DPawn+DPawn,/*4f4e*/ 0,/*4d3c*/ 0,/*3g2e*/ 0,/*3c2b*/ 0,/*5g4f*/ 0,/*2c2d*/ 
     //       DPawn+DPawn,/*4f2d*/ 0,/*2a3c*/ DKnight+DKnight+DPKnight-DKnight,/*2e3c+*/ -DPKnight-DKnight,/*2b3c*/ 0,/*2d4f*/ 0,/*8a7c*/ 
     //       0,/*N*4d*/ -DKnight-DKnight,/*3c4d*/ DSilver+DSilver,/*4e4d*/ 0,/*P*4e*/ 0,/*S*4c*/ -DSilver-DSilver,/*3b4c*/ 
     //       DGold+DGold+DPPawn-DPawn,/*4d4c+*/ -DPPawn-DPawn,/*4b4c*/ DPawn+DPawn,/*4f5e*/ 0,/*N*4f*/ 0,/*P*4d*/ 0,/*4c5d*/ 
     //       0,/*3h1h*/ -DGold-DGold-DPKnight+DKnight,/*4f5h+*/ DPKnight+DKnight,/*1h5h*/ 0,/*S*6i*/ 0,/*7h7i*/ -DRook-DRook-DPSilver+DSilver,/*6i5h+*/ 
     //       0,/*N*7d*/ 0,/*8b8a*/ 0,/*G*6d*/ -DGold-DGold,/*3a6d*/ DBishop+DBishop,/*6e6d*/ 0,/*P*2e*/
     //       DPKnight-DKnight,/*7d6b+*/ -DSilver-DSilver,/*2e2f*/ DGold+DGold,/*6b5b*/ 0,/*G*3b*/ 0,/*B*7d*/ 0,/*N*6a*/ 
     //       DPawn+DPawn+DPBishop-DBishop,/*7d6c+*/ 0,/*5d6e*/ 0/*G*6f*/
     //       //MOVE_TEST_Q1.csaで棋譜登録してある
     //   },
     //   {
     //       0,/*7g7f*/ 0,/*8c8d*/ 0,/*7i6h*/ 0,/*3c3d*/ 0,/*6g6f*/ 0,/*7a6b*/ 
     //       0,/*5g5f*/ 0,/*5c5d*/ 0,/*3i4h*/ 0,/*3a4b*/ 0,/*4i5h*/ 0,/*4a3b*/ 
     //       0,/*5h6g*/ 0,/*5a4a*/ 0,/*6i7h*/ 0,/*6a5b*/ 0,/*5i6i*/ 0,/*4b3c*/ 
     //       0,/*6h7g*/ 0,/*2b3a*/ 0,/*8h7i*/ 0,/*4c4d*/ 0,/*3g3f*/ 0,/*5b4c*/ 
     //       0,/*7i6h*/ 0,/*7c7d*/ 0,/*6i7i*/ 0,/*3a6d*/ 0,/*2i3g*/ 0,/*4a3a*/ 
     //       0,/*2g2f*/ 0,/*3a2b*/ 0,/*7i8h*/ 0,/*8d8e*/ 0,/*2h3h*/ 0,/*6b5c*/ 
     //       0,/*1g1f*/ 0,/*5d5e*/ 0,/*6f6e*/ 0,/*6d7c*/ DPawn+DPawn,/*5f5e*/ 0,/*6c6d*/ 
     //       DPawn+DPawn,/*6e6d*/ -DPawn-DPawn,/*5c6d*/ 0,/*P*6e*/ 0,/*P*6f*/ DPawn+DPawn,/*7g6f*/ -DPawn-DPawn,/*6d5e*/ 
     //       DSilver+DSilver,/*6f5e*/ -DSilver-DSilver,/*7c5e*/ 0,/*S*6f*/ 0,/*5e7c*/ 0,/*3h3i*/ 0,/*S*2h*/ 
     //       0,/*3i3h*/ -DLance-DLance-DPSilver+DSilver,/*2h1i+*/ 0,/*4h5g*/ 0,/*L*8c*/ 0,/*5g5f*/ 0,/*9c9d*/ 
     //       0,/*7f7e*/ 0,/*8e8f*/ DPawn+DPawn,/*7e7d*/ -DPawn-DPawn-DPPawn+DPawn,/*8f8g+*/ 0,/*8h7i*/ 0,/*8g8h*/ 
     //       0,/*7i6i*/ -DGold-DGold,/*8h7h*/ DPPawn+DPawn,/*6i7h*/ -DPLance+DLance,/*8c8h+*/ 0,/*7h6i*/ 0,/*G*7h*/ 
     //       0,/*6i5i*/ -DBishop-DBishop,/*7h6h*/ DGold+DGold,/*6g6h*/ 0,/*7c9e*/ 0,/*3h3i*/ 0,/*P*6g*/ 
     //       DPawn+DPawn,/*5f6g*/ 0,/*B*2h*/ 0,/*3i3h*/ -DPBishop+DBishop,/*2h1g+*/ 0,/*G*2h*/ -DPawn-DPawn,/*1g2f*/ 
     //       0,/*3h3i*/ -DLance-DLance,/*8h9i*/ 0,/*P*8c*/ 0,/*8b5b*/ 0,/*P*5c*/ -DPawn-DPawn,/*5b5c*/ 
     //       0,/*P*5d*/ -DPawn-DPawn,/*5c5d*/ 0,/*P*5e*/ -DPawn-DPawn,/*5d7d*/ 0,/*P*7e*/ 0,/*7d8d*/ 
     //       0,/*6g7h*/ 0,/*P*6g*/ 0,/*6h7g*/ -DPPawn+DPawn,/*6g6h+*/ DPPawn+DPawn,/*5i6h*/ 0,/*P*8h*/ 
     //       0,/*9g9f*/ 0,/*L*2g*/ DLance+DLance,/*2h2g*/ -DGold-DGold,/*2f2g*/ DBishop+DBishop,/*9f9e*/ 0,/*G*2i*/ 
     //       DGold+DGold,/*3i2i*/ -DRook-DRook,/*1i2i*/ 0,/*L*8f*/ 0,/*R*1h*/ 0,/*P*2h*/ -DLance-DLance,/*8d8f*/ 
     //       DRook+DRook,/*7g8f*/ -DKnight-DKnight-DPPawn+DPawn,/*8h8i+*/ DPPawn-DPawn,/*8c8b+*/ -DPawn-DPawn-DPRook+DRook,/*1h2h+*/ 0,/*6h7g*/ -DKnight-DKnight,/*2g3g*/ 
     //       DKnight+DKnight,/*8b8a*/ 0,/*8i7i*/ 0,/*7h6g*/ 0,/*3g5i*/ 0,/*7g7f*/ 0,/*N*8d*/ 
     //       0,/*7f8e*/ 0,/*2h8h*/ 0,/*B*8g*/ 0,/*8h9g*/ 0,/*N*7g*/ 0,/*N*7c*/ 
     //       0,/*8e7d*/ -DGold-DGold,/*9g8f*/ 0,/*G*8h*/ 0,/*G*9f*/ 0,/*R*5f*/ 0,/*5i3g*/ 
     //       DLance+DLance,/*8a9a*/ 0,/*3d3e*/ DGold+DGold,/*8g9f*/ -DBishop-DBishop,/*8d9f*/ 0,/*G*8g*/ 0,/*B*8c*/ 
     //       DKnight+DKnight,/*7d7c*/ 0,/*P*7b*/ 0,/*7c8b*/ 0,/*8f8d*/ DKnight+DKnight,/*8g9f*/ 0,/*P*8g*/ 
     //       0,/*9f8e*/ 0,/*8d7c*/ 0,/*8b9c*/ 0,/*8c7d*/ 0,/*L*8c*/ -DLance-DLance/*7c8c*/   
     //       //MOVE_TEST_Q2.csaで棋譜登録してある
     //   }
    //};
    /*
    for(int i = 0;i < 2;i++){
        USIInputParser uip(q_str[i]);
        ply = update_board_material(uip,material_answer[i],3,200);    //do_moveで局面を更新
        for(int i = ply-1;i >= 0;i--){
            undo_move(root_position,i); //undo_moveで局面を復元
            is_ok(root_position);
        }
        is_eq_board();
        EXPECT_EQ(0,sech.material);
    }
}
*/
/*
int update_board(USIInputParser &uip)
{
    string cmd;
    int ply = 0;
    short *mf;

    next_modify[ply].next_dirty = next_modify[ply].last_dirty = modifylist;   //念のため
    cmd = uip.get_next_token();

    //平手初期局面
    if(cmd == "startpos"){
        from_sfen(start_position);
    }
    //駒落ち初期局面
    else if(cmd == "sfen"){
        string sfen;
        while(cmd != "moves" && !uip.at_end_of_line()){
            cmd = uip.get_next_token();
            sfen += cmd;
            sfen += ' ';
        }
        from_sfen(sfen);
    }
    game_init(root_position);
    //指し手再現,局面更新
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token(); //cmdには指し手ごと分割されて渡す
                Move m = move_from_string(root_position,cmd);
                mf = next_modify[ply].next_dirty;
                next_modify[ply].last_dirty = DoMove(root_position.turn,root_position,m,mf);
                next_modify[ply+1].next_dirty = next_modify[ply].last_dirty;
                ply += 1;
            }
        }
    }
    return ply;
}

int update_board_material(USIInputParser &uip,int material[],int row,int col)
{
    string cmd;
    int ply = 0;
    short *mf;
    int mater_total = 0;

    next_modify[ply].next_dirty = next_modify[ply].last_dirty = modifylist;   //念のため
    cmd = uip.get_next_token();

    //平手初期局面
    if(cmd == "startpos"){
        from_sfen(start_position);
    }
    //駒落ち初期局面
    else if(cmd == "sfen"){
        string sfen;
        while(cmd != "moves" && !uip.at_end_of_line()){
            cmd = uip.get_next_token();
            sfen += cmd;
            sfen += ' ';
        }
        from_sfen(sfen);
    }
    game_init(root_position);
    //指し手再現,局面更新
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token(); //cmdには指し手ごと分割されて渡す
                Move m = move_from_string(root_position,cmd);
                mf = next_modify[ply].next_dirty;
                next_modify[ply].last_dirty = DoMove(root_position.turn,root_position,m,mf);
                next_modify[ply+1].next_dirty = next_modify[ply].last_dirty;
                mater_total += *material;
                EXPECT_EQ(mater_total,sech.material);
                material++;
                ply += 1;
            }
        }
    }
    return ply;
}
*/
/*
初期局面の配置との比較
*/
/*
void is_eq_board(void)
{
    EXPECT_EQ(W_LANCE,root_position.board[SQ_9A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_8A]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_7A]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_6A]);
    EXPECT_EQ(W_KING,root_position.board[SQ_5A]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_4A]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_3A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_2A]);
    EXPECT_EQ(W_LANCE,root_position.board[SQ_1A]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9B]);
    EXPECT_EQ(W_ROOK,root_position.board[SQ_8B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3B]);
    EXPECT_EQ(W_BISHOP,root_position.board[SQ_2B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1B]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_9C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_8C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_7C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_6C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_5C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_4C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_3C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_2C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_1C]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1D]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1E]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1F]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_9G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_8G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_7G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_6G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_5G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_4G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_3G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_2G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_1G]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9H]);
    EXPECT_EQ(B_BISHOP,root_position.board[SQ_8H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3H]);
    EXPECT_EQ(B_ROOK,root_position.board[SQ_2H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1H]);

    EXPECT_EQ(B_LANCE,root_position.board[SQ_9I]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_8I]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_7I]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_6I]);
    EXPECT_EQ(B_KING,root_position.board[SQ_5I]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_4I]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_3I]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_2I]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_1I]);
}
*/