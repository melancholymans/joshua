#include <string>
#include <ctype.h>

using namespace std;

#include "gtest\gtest.h"
#include "usi.h"
#include "usioption.h"
#include "position.h"

//駒コード（GPSを参考にした）
const char EMPTY = 0;
const char EDGE = 1;
const char NOT_PMOTO = 8;

const char PPAWN = 2;
const char PLANCE = 3;
const char PKNIGHT = 4;
const char PSILVER = 5;
const char PBISHOP = 6;
const char PROOK = 7;
const char KING = 8;
const char GOLD = 9;
const char PAWN = 10;
const char LANCE = 11;
const char KNIGHT = 12;
const char SILVER = 13;
const char BISHOP = 14;
const char ROOK = 15;

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

const char WP_PAWN = BP_PAWN - 16;
const char WP_LANCE = BP_LANCE - 16;
const char WP_KNIGHT = BP_KNIGHT - 16;
const char WP_SILVER = BP_SILVER - 16;
const char WP_BISHOP = BP_BISHOP - 16;
const char WP_ROOK = BP_ROOK - 16;
const char W_KING = B_KING - 16;
const char W_GOLD = B_GOLD - 16;
const char W_PAWN = B_PAWN - 16;
const char W_LANCE = B_LANCE - 16;
const char W_KNIGHT = B_KNIGHT - 16;
const char W_SILVER = B_SILVER - 16;
const char W_BISHOP = B_BISHOP - 16;
const char W_ROOK = B_ROOK - 16;
//player(手番） BLACKが先手、WHITEが後手
const int BLACK = 0;
const int WHITE = -1;
int turn;

//先手大文字、後手小文字、数字は空白、/は行区切り
string start_position = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
string begin_poition;   //どんな局面を受け付けたのか保持
position_t root_position;

void from_sfen(string &sfen)
{
    //from_sfenに制約をもうける、受け付けるのは初期局面（駒落ち初期局面可）のみで途中図は受け付けない、従って駒台の処理もない
    begin_poition = sfen;
    USIInputParser uip(sfen);
    string token = uip.get_next_token();
    int index;

    for(index = 0;index < LIMIT;index++){
        root_position.board[index] = EMPTY;
    }
    int col = 1,row = 1;
    for(index = 0;token.size() > index;index++){
        if(isdigit(token[index])){
            col += (token[index] - '1' + 1);
        }
        else{
            int sq = make_square(col,row);
            switch(token[index]){
            case 'k': put_piece(W_KING,sq,0); col++; break;
            case 'g': put_piece(W_GOLD,sq,0); col++; break;
            case 'p': put_piece(W_PAWN,sq,0); col++; break;
            case 'l': put_piece(W_LANCE,sq,0); col++; break;
            case 'n': put_piece(W_KNIGHT,sq,0); col++; break;
            case 's': put_piece(W_SILVER,sq,0); col++; break; 
            case 'b': put_piece(W_BISHOP,sq,0); col++; break;
            case 'r': put_piece(W_ROOK,sq,0); col++; break;
            
            case 'K': put_piece(B_KING,sq,0); col++; break;
            case 'G': put_piece(B_GOLD,sq,0); col++; break;
            case 'P': put_piece(B_PAWN,sq,0); col++; break;
            case 'L': put_piece(B_LANCE,sq,0); col++; break;
            case 'N': put_piece(B_KNIGHT,sq,0); col++; break;
            case 'S': put_piece(B_SILVER,sq,0); col++; break; 
            case 'B': put_piece(B_BISHOP,sq,0); col++; break;
            case 'R': put_piece(B_ROOK,sq,0); col++; break;
            case '/': col = 1; row++; break;
            case ' ': break;
            default: 
                cout << "Error in SFEN charracter" << endl;
                EXPECT_TRUE(false);
                return;
            }
        }
    }
    //手番設定
    token = uip.get_next_token();

    if(token[0] == 'b'){
        turn = BLACK;
    }
    else if(token[0] == 'w'){
        turn = WHITE;
    }
    else{
        cout << "Error in SFEN charracter" << endl;
        ASSERT_TRUE(false);
        return; 
    }
    //持ち駒(-),手数が入っているが無視してよい
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
    result += (turn == WHITE) ? 'w'  : 'b';
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
TEST(position,to_sfen)
{
    from_sfen(start_position);
    EXPECT_STREQ("lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1",to_sfen(root_position).c_str());

    for(int sq = 0;sq < 222;sq++){
        root_position.board[sq] = EMPTY;
    }
    root_position.board[SQ_F9] = W_GOLD;
    root_position.board[SQ_G9] = W_KING;

    root_position.board[SQ_A8] = BP_PAWN;
    root_position.board[SQ_B8] = BP_LANCE;
    root_position.board[SQ_C8] = BP_KNIGHT;
    root_position.board[SQ_D8] = BP_SILVER;

    root_position.board[SQ_A7] = W_PAWN;
    root_position.board[SQ_B7] = W_PAWN;
    root_position.board[SQ_C7] = W_PAWN;
    root_position.board[SQ_D7] = W_PAWN;
    root_position.board[SQ_E7] = W_PAWN;
    root_position.board[SQ_F7] = W_PAWN;
    root_position.board[SQ_G7] = W_PAWN;

    root_position.board[SQ_A3] = B_PAWN;
    root_position.board[SQ_B3] = B_PAWN;
    root_position.board[SQ_C3] = B_PAWN;
    root_position.board[SQ_D3] = B_PAWN;

    root_position.board[SQ_F2] = WP_SILVER;
    root_position.board[SQ_G2] = WP_KNIGHT;
    root_position.board[SQ_H2] = WP_LANCE;
    root_position.board[SQ_I2] = WP_PAWN;

    root_position.board[SQ_C1] = B_KING;
    root_position.board[SQ_D1] = B_GOLD;

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


//sfen文字列からpositionを設定
void put_piece(char p,int sq,int num)
{
    char pt = type_of_piece(p);
    Color c = color_of_piece(p);
    if(sq < BOARD_UPPER){
        //MAIN boardに駒コードを入れる
        root_position.board[sq] = p;
        if(pt == KING){
            root_position.king_square[c] = sq;
        }
    }
    else{
        //こちらは駒台、駒種によって座標は固定されている、格納するのは枚数
        root_position.board[sq] = num;
    }
}

void print_board(void)
{

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
    EXPECT_FALSE(BP_PAWN & NOT_PMOTO);  //BP_PAWNは成っていないよね->成っているのでFALSE
    EXPECT_FALSE(BP_LANCE & NOT_PMOTO);
    EXPECT_FALSE(BP_KNIGHT & NOT_PMOTO);
    EXPECT_FALSE(BP_SILVER & NOT_PMOTO);
    EXPECT_FALSE(BP_BISHOP & NOT_PMOTO);
    EXPECT_FALSE(BP_ROOK & NOT_PMOTO);
    EXPECT_TRUE(B_PAWN & NOT_PMOTO);    //B_PAWNは成っていないよね->成っていないのでTRUE
    EXPECT_TRUE(B_LANCE & NOT_PMOTO);
    EXPECT_TRUE(B_KNIGHT & NOT_PMOTO);
    EXPECT_TRUE(B_SILVER & NOT_PMOTO);
    EXPECT_TRUE(B_BISHOP & NOT_PMOTO);
    EXPECT_TRUE(B_ROOK & NOT_PMOTO);
    
    EXPECT_FALSE(WP_PAWN & NOT_PMOTO);
    EXPECT_FALSE(WP_LANCE & NOT_PMOTO);
    EXPECT_FALSE(WP_KNIGHT & NOT_PMOTO);
    EXPECT_FALSE(WP_SILVER & NOT_PMOTO);
    EXPECT_FALSE(WP_BISHOP & NOT_PMOTO);
    EXPECT_FALSE(WP_ROOK & NOT_PMOTO);
    EXPECT_TRUE(W_PAWN & NOT_PMOTO);
    EXPECT_TRUE(W_LANCE & NOT_PMOTO);
    EXPECT_TRUE(W_KNIGHT & NOT_PMOTO);
    EXPECT_TRUE(W_SILVER & NOT_PMOTO);
    EXPECT_TRUE(W_BISHOP & NOT_PMOTO);
    EXPECT_TRUE(W_ROOK & NOT_PMOTO);
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
    p = BP_PAWN;
    EXPECT_EQ(WP_PAWN,p |= 0xF0);
    p = BP_LANCE;
    EXPECT_EQ(WP_LANCE,p |= 0xF0);
    p = BP_KNIGHT;
    EXPECT_EQ(WP_KNIGHT,p |= 0xF0);
    p = BP_SILVER;
    EXPECT_EQ(WP_SILVER,p |= 0xF0);
    p = BP_SILVER;
    EXPECT_EQ(WP_SILVER,p |= 0xF0);
    p = BP_BISHOP;
    EXPECT_EQ(WP_BISHOP,p |= 0xF0);
    p = BP_ROOK;
    EXPECT_EQ(WP_ROOK,p |= 0xF0);
    p = B_KING;
    EXPECT_EQ(W_KING,p |= 0xF0);
    p = B_GOLD;
    EXPECT_EQ(W_GOLD,p |= 0xF0);
    p = B_PAWN;
    EXPECT_EQ(W_PAWN,p |= 0xF0);
    p = B_LANCE;
    EXPECT_EQ(W_LANCE,p |= 0xF0);
    p = B_KNIGHT;
    EXPECT_EQ(W_KNIGHT,p |= 0xF0);
    p = B_SILVER;
    EXPECT_EQ(W_SILVER,p |= 0xF0);
    p = B_BISHOP;
    EXPECT_EQ(W_BISHOP,p |= 0xF0);
    p = B_ROOK;
    EXPECT_EQ(W_ROOK,p |= 0xF0);
    //後手->先手
    p = WP_PAWN;
    EXPECT_EQ(BP_PAWN,p &= 0x0F);
    p = WP_LANCE;
    EXPECT_EQ(BP_LANCE,p &= 0x0F);
    p = WP_KNIGHT;
    EXPECT_EQ(BP_KNIGHT,p &= 0x0F);
    p = WP_SILVER;
    EXPECT_EQ(BP_SILVER,p &= 0x0F);
    p = WP_SILVER;
    EXPECT_EQ(BP_SILVER,p &= 0x0F);
    p = WP_BISHOP;
    EXPECT_EQ(BP_BISHOP,p &= 0x0F);
    p = WP_ROOK;
    EXPECT_EQ(BP_ROOK,p &= 0x0F);
    p = W_KING;
    EXPECT_EQ(B_KING,p &= 0x0F);
    p = W_GOLD;
    EXPECT_EQ(B_GOLD,p &= 0x0F);
    p = W_PAWN;
    EXPECT_EQ(B_PAWN,p &= 0x0F);
    p = W_LANCE;
    EXPECT_EQ(B_LANCE,p &= 0x0F);
    p = W_KNIGHT;
    EXPECT_EQ(B_KNIGHT,p &= 0x0F);
    p = W_SILVER;
    EXPECT_EQ(B_SILVER,p &= 0x0F);
    p = W_BISHOP;
    EXPECT_EQ(B_BISHOP,p &= 0x0F);
    p = W_ROOK;
    EXPECT_EQ(B_ROOK,p &= 0x0F);
}

TEST(position,from_sfen)
{
    from_sfen(start_position);
    EXPECT_EQ(W_LANCE,root_position.board[SQ_A9]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_B9]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_C9]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_D9]);
    EXPECT_EQ(W_KING,root_position.board[SQ_E9]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_F9]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_G9]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_H9]);
    EXPECT_EQ(W_LANCE,root_position.board[SQ_I9]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A8]);
    EXPECT_EQ(W_ROOK,root_position.board[SQ_B8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G8]);
    EXPECT_EQ(W_BISHOP,root_position.board[SQ_H8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I8]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_A7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_B7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_C7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_D7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_E7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_F7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_G7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_H7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_I7]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I6]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I5]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I4]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_A3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_B3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_C3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_D3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_E3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_F3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_G3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_H3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_I3]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A2]);
    EXPECT_EQ(B_BISHOP,root_position.board[SQ_B2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G2]);
    EXPECT_EQ(B_ROOK,root_position.board[SQ_H2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I2]);

    EXPECT_EQ(B_LANCE,root_position.board[SQ_A1]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_B1]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_C1]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_D1]);
    EXPECT_EQ(B_KING,root_position.board[SQ_E1]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_F1]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_G1]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_H1]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_I1]);
    
    from_sfen(string("lnsgkgsn1/1r5b1/pppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL w - 1"));
    EXPECT_EQ(W_LANCE,root_position.board[SQ_A9]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I9]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_A1]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_I1]);
    
}
