#include <string>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "movegen.h"

Move mlist[16384];  //bonanzaもこれくらい取っている2^14
next_move_t next_move[PLY_MAX];
int DIRECT_WHITE[16][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {DL,D,DR,L,R,U,0,0},  //WP_PAWN
    {DL,D,DR,L,R,U,0,0}, //WP_LANCE
    {DL,D,DR,L,R,U,0,0}, //WP_KNIGHT
    {DL,D,DR,L,R,U,0,0}, //WP_SILVER
    {DL,DR,UL,UR,U,L,R,D}, //WP_BISHOP 最初の４つは飛び方向、残り４つが非飛び方向
    {D,L,R,U,DL,DR,UL,UR}, //WP_ROOK 最初の４つは飛び方向、残り４つが非飛び方向
    {DL,D,DR,L,R,UL,U,UR},  //W_KING
    {DL,D,DR,L,R,U,0,0},  //W_GOLD
    {D,0,0,0,0,0,0,0},  //W_PAWN
    {D,0,0,0,0,0,0,0},  //W_LANCE
    {DDL,DDR,0,0,0,0,0,0},  //W_KNIGHT
    {DL,D,DR,UL,UR,0,0,0},  //W_SILVER
    {DL,DR,UL,UR,0,0,0,0},  //W_BISHOP
    {D,L,R,U,0,0,0,0},  //W_ROOK
};
int DIRECT_BLACK[16][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {UL,U,UR,L,R,D,0,0},  //BPPAWN
    {UL,U,UR,L,R,D,0,0}, //BP_LANCE
    {UL,U,UR,L,R,D,0,0}, //BP_KNIGHT
    {UL,U,UR,L,R,D,0,0}, //BP_SILVER
    {UL,UR,DL,DR,U,L,R,D}, //BP_BISHOP 最初の４つは飛び方向、残り４つが非飛び方向
    {U,L,R,D,UL,UR,DL,DR}, //BP_ROOK 最初の４つは飛び方向、残り４つが非飛び方向
    {UL,U,UR,L,R,DL,D,DR},  //B_KING
    {UL,U,UR,L,R,D,0,0},  //B_GOLD
    {U,0,0,0,0,0,0,0},  //B_PAWN
    {U,0,0,0,0,0,0,0},  //B_LANCE
    {UUL,UUR,0,0,0,0,0,0},  //B_KNIGHT
    {UL,U,UR,DL,DR,0,0,0},  //B_SILVER
    {UL,UR,DL,DR,0,0,0,0},  //B_BISHOP
    {U,L,R,D,0,0,0,0},  //B_ROOK
};

bool array_check(Move anser,Move *m,int n);

Move *generate_moves(const Position &pos,Move *ml)
{
    char p;
    Color c;

    for(int sq = SQ_A9;sq < SQ_LIMIT;sq++){
        p = pos.board[sq];
        c = color_of_piece(p);
        if(c != turn){
            continue;
        }
        if(WHITE == turn){
            switch(type_of_piece(p)){
            case KING:ml=generate_king_moves_w(pos,ml,sq); break;
            case GOLD:case PPAWN:case PLANCE:case PKNIGHT:case PSILVER:ml=generate_gold_moves_w(pos,ml,sq); break;
            case PAWN:ml=generate_pawn_moves_w(pos,ml,sq); break;
            case LANCE:ml=generate_lance_moves_w(pos,ml,sq); break;
            case KNIGHT:ml=generate_knight_moves_w(pos,ml,sq); break;
            case SILVER:ml=generate_silver_moves_w(pos,ml,sq); break;
            case BISHOP:ml=generate_bishop_moves_w(pos,ml,sq); break;
            case ROOK:ml=generate_rook_moves_w(pos,ml,sq); break;
            case PBISHOP:ml=generate_pbishop_moves_w(pos,ml,sq); break;
            case PROOK:ml=generate_prook_moves_w(pos,ml,sq); break;
            }
        }
        else if(BLACK == turn){
            switch(type_of_piece(p)){
            case KING:ml=generate_king_moves_b(pos,ml,sq); break;
            case GOLD:ml=generate_gold_moves_b(pos,ml,sq); break;
            case PAWN:ml=generate_pawn_moves_b(pos,ml,sq); break;
            case LANCE:ml=generate_lance_moves_b(pos,ml,sq); break;
            case KNIGHT:ml=generate_knight_moves_b(pos,ml,sq); break;
            case SILVER:ml=generate_silver_moves_b(pos,ml,sq); break;
            case BISHOP:ml=generate_bishop_moves_b(pos,ml,sq); break;
            case ROOK:ml=generate_rook_moves_b(pos,ml,sq); break;
            case PBISHOP:ml=generate_pbishop_moves_b(pos,ml,sq); break;
            case PROOK:ml=generate_prook_moves_b(pos,ml,sq); break;
            }
        }
    }
    //打つ手

    return ml;
}

Move *generate_king_moves_w(const Position &pos,Move *ml,int from)
{
    int to;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 8;i++){
        to = from + DIRECT_WHITE[KING][i];
        cp = pos.board[to];
        //KINGなので敵の利きのあるところにはいけないがまだそこまでのデータが揃ってないのでPASS
        if(cp > 1 || cp == 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_gold_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 6;i++){
        to = from + DIRECT_WHITE[GOLD][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_pawn_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;
    
    p = pos.board[from];
    to = from + DIRECT_WHITE[PAWN][0];
    cp = pos.board[to];
    if(cp > 1 || cp == 0){
        pmoto = to > SQ_I4 ? 1 : 0;
        *(ml++) = make_move(from,to,pmoto,p,cp);
    }
    return ml;
}

Move *generate_lance_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 1;i++){
        //loopを回す必要がないが他のこまのためにこうしている
        do{
            to = to + DIRECT_WHITE[LANCE][i];
            cp = pos.board[to];
            pmoto = to > SQ_I4 ? 1 : 0;
            if(cp == 0 || cp > 1){ 
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_knight_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 2;i++){
        to = from + DIRECT_WHITE[KNIGHT][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない,さらに桂馬は成る必須のラインがあることを忘れないように
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_silver_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 5;i++){
        to = from + DIRECT_WHITE[SILVER][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_bishop_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[BISHOP][i];
            cp = pos.board[to];
            pmoto = to > SQ_I4 ? 1 : 0;
            if(cp == 0 || cp > 1){
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_rook_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[ROOK][i];
            cp = pos.board[to];
            pmoto = to > SQ_I4 ? 1 : 0;
            if(cp == 0 || cp > 1){
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_pbishop_moves_w(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[BISHOP][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(i = 0;i < 4;i++){
        to = from + DIRECT_WHITE[ROOK][i];
        cp = pos.board[to];
        if(cp == 0 || cp > 1){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_prook_moves_w(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[ROOK][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(i = 0;i < 4;i++){
        to = from + DIRECT_WHITE[BISHOP][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_king_moves_b(const Position &pos,Move *ml,int from)
{
    int to;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 8;i++){
        to = from + DIRECT_BLACK[KING][i];
        cp = pos.board[to];
        //KINGなので敵の利きのあるところにはいけないがまだそこまでのデータが揃ってないのでPASS
        if(cp <= 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_gold_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 6;i++){
        to = from + DIRECT_BLACK[GOLD][i];
        cp = pos.board[to];
        if(cp <= 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_pawn_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;
    
    p = pos.board[from];
    to = from + DIRECT_BLACK[PAWN][0];
    cp = pos.board[to];
    if(cp <= 0){
        pmoto = to < SQ_A6 ? 1 : 0;
        *(ml++) = make_move(from,to,pmoto,p,cp);
    }
    return ml;
}

Move *generate_lance_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 1;i++){
        do{
            to = to + DIRECT_BLACK[LANCE][i];
            cp = pos.board[to];
            pmoto = to < SQ_A6 ? 1 : 0;
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_knight_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 2;i++){
        to = from + DIRECT_BLACK[KNIGHT][i];
        cp = pos.board[to];
        if(cp <= 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない,さらに桂馬は成る必須のラインがあることを忘れないように
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_silver_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 5;i++){
        to = from + DIRECT_BLACK[SILVER][i];
        cp = pos.board[to];
        if(cp <= 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない
            pmoto = to < SQ_A6 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}


Move *generate_bishop_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[BISHOP][i];
            cp = pos.board[to];
            pmoto = to < SQ_A6 ? 1 : 0;
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}


Move *generate_rook_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[ROOK][i];
            cp = pos.board[to];
            pmoto = to < SQ_A6 ? 1 : 0;
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_pbishop_moves_b(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[BISHOP][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(i = 0;i < 4;i++){
        to = from + DIRECT_BLACK[ROOK][i];
        cp = pos.board[to];
        if(cp <= 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_prook_moves_b(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[ROOK][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(int i = 0;i < 4;i++){
        to = from + DIRECT_BLACK[BISHOP][i];
        cp = pos.board[to];
        if(cp <= 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}
/*
Move *generate_evasions(const Position &pos,Move *ml)
{
    Move *m = NULL;
    return m;
}

Move *generate_captures(const Position &pos,Move *ml)
{
    Move *m = NULL;
    return m;
}

Move *generate_checks(const Position &pos,Move *ml)
{
    Move *m = NULL;
    return m;
}

Move *generate_drop(const Position &pos,Move *ml)
{
    Move *m = NULL;
    return m;
}
*/

TEST(movegen,generate_moves)
{
    //指し手のテストは生成される手の数と生成された手そのものを比較する
    from_sfen(start_position);
    next_move[0].last_move = mlist;
    Move *m = generate_moves(root_position,mlist);
    int n = m - next_move[0].last_move;
    EXPECT_EQ(30,n);
    m = mlist;
    //pawn
    Move anser = make_move(SQ_A3,SQ_A4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B3,SQ_B4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C3,SQ_C4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_D3,SQ_D4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E3,SQ_E4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F3,SQ_F4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_G3,SQ_G4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H3,SQ_H4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_I3,SQ_I4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance
    anser = make_move(SQ_A1,SQ_A2,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_I1,SQ_I2,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver
    anser = make_move(SQ_C1,SQ_C2,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C1,SQ_D2,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_G1,SQ_F2,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_G1,SQ_G2,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold
    anser = make_move(SQ_D1,SQ_C2,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_D1,SQ_D2,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_D1,SQ_E2,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F1,SQ_E2,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F1,SQ_F2,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F1,SQ_G2,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //king
    anser = make_move(SQ_E1,SQ_D2,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E1,SQ_E2,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E1,SQ_F2,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //rook
    anser = make_move(SQ_H2,SQ_I2,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H2,SQ_G2,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H2,SQ_F2,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H2,SQ_E2,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H2,SQ_D2,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H2,SQ_C2,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));

    //Q1の局面でテスト,BLACK側
    string expect ="lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b P5psr 1";
    from_sfen(expect);
    next_move[0].last_move = mlist;
    m = generate_moves(root_position,mlist);
    n = m - next_move[0].last_move;
    EXPECT_EQ(57,n);
    m = mlist;
    //pawn  5
    anser = make_move(SQ_A3,SQ_A4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B4,SQ_B5,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_G3,SQ_G4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_I3,SQ_I4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H7,SQ_H8,1,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance 1
    anser = make_move(SQ_B1,SQ_C3,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //knight    1
    anser = make_move(SQ_B1,SQ_C3,0,B_KNIGHT,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver    5
    anser = make_move(SQ_F5,SQ_E6,0,B_SILVER,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F5,SQ_F6,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F5,SQ_G6,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F5,SQ_E4,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F5,SQ_G4,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold  7
    anser = make_move(SQ_B2,SQ_B3,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B2,SQ_C3,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C2,SQ_B3,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C2,SQ_C3,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C2,SQ_D3,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C2,SQ_D2,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C2,SQ_C1,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    //bishop
    anser = make_move(SQ_D9,SQ_C8,1,B_BISHOP,W_GOLD);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_D9,SQ_E8,1,B_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_D9,SQ_F7,1,B_BISHOP,W_SILVER);
    EXPECT_TRUE(array_check(anser,m,n));
    //pbishop
    anser = make_move(SQ_E7,SQ_D8,0,BP_BISHOP,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_F8,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_G9,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_D6,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_C5,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_F6,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_G5,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_H4,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_E8,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_D7,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_F7,0,BP_BISHOP,W_SILVER);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E7,SQ_E6,0,BP_BISHOP,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    //rook  4
    anser = make_move(SQ_B9,SQ_C9,1,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B9,SQ_B8,1,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B9,SQ_B7,1,B_ROOK,W_GOLD);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B9,SQ_A9,1,B_ROOK,W_LANCE);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop pawn
    anser = make_move(0,SQ_C6,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C5,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C3,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C1,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D7,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D6,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D5,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D3,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D2,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D1,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E8,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E5,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E3,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E2,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E1,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F8,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F6,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F4,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F3,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F2,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F1,0,B_PAWN,EMPTY);
    ASSERT_TRUE(array_check(anser,m,n));

    //Q1の局面でテスト,WHITE側
    from_sfen(expect);
    turn = WHITE;
    next_move[0].last_move = mlist;
    m = generate_moves(root_position,mlist);
    n = m - next_move[0].last_move;
    EXPECT_EQ(133,n);
    m = mlist;
    //pawn  7
    anser = make_move(SQ_A5,SQ_A4,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C4,SQ_C3,1,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_D8,SQ_D7,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_E6,SQ_E5,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_G7,SQ_G6,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_H6,SQ_H5,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_I7,SQ_I6,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance 2
    anser = make_move(SQ_A9,SQ_A8,0,W_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_I9,SQ_I8,0,W_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //knight
    anser = make_move(SQ_A7,SQ_B5,0,W_KNIGHT,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver    6
    anser = make_move(SQ_B6,SQ_B5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B6,SQ_C5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F7,SQ_F6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F7,SQ_G6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F7,SQ_E8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_F7,SQ_G8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold
    anser = make_move(SQ_B7,SQ_A6,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B7,SQ_C6,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_B7,SQ_B8,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C8,SQ_D7,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C8,SQ_B8,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C8,SQ_C9,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //king
    anser = make_move(SQ_C7,SQ_C6,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C7,SQ_D6,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C7,SQ_D7,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_C7,SQ_B8,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop pawn 9
    anser = make_move(0,SQ_B8,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B5,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B3,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F9,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F8,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F6,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F4,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F3,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F2,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop silver
    anser = make_move(0,SQ_A8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_A6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_A4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B3,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C9,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C3,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C1,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D7,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D3,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D2,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D1,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E9,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E3,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E2,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E1,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F9,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F3,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F2,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F1,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G9,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G2,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G1,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H3,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H2,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I8,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I6,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I5,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I4,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I2,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop rook
    anser = make_move(0,SQ_A8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_A6,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_A4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_B3,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C9,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C6,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C3,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_C1,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D7,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D6,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D3,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D2,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_D1,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E9,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E3,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E2,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_E1,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F9,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F6,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F3,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F2,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_F1,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G9,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G6,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G2,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_G1,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H3,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_H2,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I8,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I6,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I5,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I4,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_I2,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
}

bool array_check(Move anser,Move *m,int n)
{
    bool flag = false;

    for(int i = 0;i < n;i++){
        if(anser == *(m++)){
            flag = true;
        }
    }
    return flag;
}