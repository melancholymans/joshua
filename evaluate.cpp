#include "gtest\gtest.h"
#include "position.h"
#include "evaluate.h"

const int DPawn = 87;
const int DLance = 235;
const int DKnight = 254;
const int DSilver = 371;
const int DGold = 447;
const int DBishop = 571;
const int DRook = 647;
const int Dking = 15000;
const int DPPawn = 530;
const int DPLance = 482;
const int DPKnight = 500;
const int DPSilver = 489;
const int DPBishop = 832;
const int DPRook = 955;

int piece_value[16];
/*
初期局面の駒割評価値を算定
*/
int eval_material(const Position &pos)
{
    int material = 0;
    int piece_count[2][16];
    char p;
    Color c;

    memset(piece_count,0,sizeof(int)*2*16);
    for(int row =1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            p = pos.board[sq];
            c = color_of_piece(p);
            if(c == NO_COLOR){
                continue;
            }
            piece_count[c+1][type_of_piece(p)]++;
        }
    }
    //ここではBLACK=1,WHITE=0
    material += (piece_count[1][GOLD] + pos.board[208] - piece_count[0][GOLD] - pos.board[215])*DGold;
    material += (piece_count[1][PAWN] + pos.board[209] - piece_count[0][PAWN] - pos.board[216])*DPawn;
    material += (piece_count[1][LANCE] + pos.board[210] - piece_count[0][LANCE] - pos.board[217])*DLance;
    material += (piece_count[1][KNIGHT] + pos.board[211] - piece_count[0][KNIGHT] - pos.board[218])*DKnight;
    material += (piece_count[1][SILVER] + pos.board[212] - piece_count[0][SILVER] - pos.board[219])*DSilver;
    material += (piece_count[1][BISHOP] + pos.board[213] - piece_count[0][BISHOP] - pos.board[220])*DBishop;
    material += (piece_count[1][ROOK] + pos.board[214] - piece_count[0][ROOK] - pos.board[221])*DRook;
    material += (piece_count[1][PPAWN] - piece_count[0][PPAWN])*DPPawn;
    material += (piece_count[1][PLANCE] - piece_count[0][PLANCE])*DPLance;
    material += (piece_count[1][PKNIGHT] - piece_count[0][PKNIGHT])*DPKnight;
    material += (piece_count[1][PSILVER] - piece_count[0][PSILVER])*DPSilver;
    material += (piece_count[1][PBISHOP] - piece_count[0][PBISHOP])*DPBishop;
    material += (piece_count[1][PROOK] - piece_count[0][PROOK])*DPRook;
    return material;
}

int evaluate(Position &pos)
{
    return 1;
}

TEST(evaluate,eval_material)
{
    //Q1の局面でテスト
    string expect ="lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b P5psr 1";
    from_sfen(expect);
    int ev = eval_material(root_position);
    int excep = 0;
    excep += (2-2)*DGold;
    excep += (6-12)*DPawn;
    excep += (2-2)*DLance;
    excep += (2-2)*DKnight;
    excep += (1-3)*DSilver;
    excep += (1-0)*DBishop;
    excep += (1-1)*DRook;
    excep += (1-0)*DPBishop;
    EXPECT_EQ(excep,ev);
    //Q2の局面でテスト
    expect ="5S2l/1rP2s1k1/p2+B1gnp1/5np2/3G3n1/5S2p/P1+p1PpPP1/1P1PG2KP/L2+rLPGNL b B3ps 1";
    from_sfen(expect);
    ev = eval_material(root_position);
    excep = 0;
    excep += (3-1)*DGold;
    excep += (9-8)*DPawn;
    excep += (3-1)*DLance;
    excep += (1-3)*DKnight;
    excep += (2-2)*DSilver;
    excep += (1-0)*DBishop;
    excep += (0-1)*DRook;
    excep += (0-1)*DPPawn;
    excep += (1-0)*DPBishop;
    excep += (0-1)*DPRook;
    EXPECT_EQ(excep,ev);

    //Q3の局面でテスト
    expect ="lR6l/1s1g5/1k1s1+P2p/1+bpp1+Bs2/1n1n2Pp1/2P6/S2R4P/K1GG5/9 b P2Ng9p2l 1";
    from_sfen(expect);
    ev = eval_material(root_position);
    excep = 0;
    excep += (2-2)*DGold;
    excep += (4-13)*DPawn;
    excep += (0-4)*DLance;
    excep += (2-2)*DKnight;
    excep += (1-3)*DSilver;
    excep += (0-0)*DBishop;
    excep += (2-0)*DRook;
    excep += (1-0)*DPPawn;
    excep += (1-1)*DPBishop;
    EXPECT_EQ(excep,ev);
}
