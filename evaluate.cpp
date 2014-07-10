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

/*
‰Šú‹Ç–Ê‚Ì‹îŠ„•]‰¿’l‚ğZ’è
*/
int eval_material(Position &pos)
{
    int material,pc;
    int piece_count[2][16];
    char p;
    Color c;

    memset(piece_count,0,sizeof(int)*2*16);
    for(int row =1;row < 10;row++){
        for(int col;col < 10;col++){
            int sq = make_square(col,row);
            p = pos.board[sq];
            c = color_of_piece(p);
            switch(type_of_piece(p)){
            case KING:
            }
        }
    }
    return 1;
}

int evaluate(Position &pos)
{
    return 1;
}
