#include <string>

using namespace std;

#include "position.h"
#include "movegen.h"

Move mlist[16384];  //bonanza�����ꂭ�炢����Ă���2^14
next_move_t next_move[PLY_MAX];
int DIRECT_WHITE[16][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {DL,D,DR,L,R,U,0,0},  //WP_PAWN
    {DL,D,DR,L,R,U,0,0}, //WP_LANCE
    {DL,D,DR,L,R,U,0,0}, //WP_KNIGHT
    {DL,D,DR,L,R,U,0,0}, //WP_SILVER
    {DL,DR,UL,UR,U,L,R,D}, //WP_BISHOP �ŏ��̂S�͔�ѕ����A�c��S�����ѕ���
    {D,L,R,U,DL,DR,UL,UR}, //WP_ROOK �ŏ��̂S�͔�ѕ����A�c��S�����ѕ���
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
    {UL,UR,DL,DR,U,L,R,D}, //BP_BISHOP �ŏ��̂S�͔�ѕ����A�c��S�����ѕ���
    {U,L,R,D,UL,UR,DL,DR}, //BP_ROOK �ŏ��̂S�͔�ѕ����A�c��S�����ѕ���
    {UL,U,UR,L,R,DL,D,DR},  //B_KING
    {UL,U,UR,L,R,D,0,0},  //B_GOLD
    {U,0,0,0,0,0,0,0},  //B_PAWN
    {U,0,0,0,0,0,0,0},  //B_LANCE
    {UUL,UUR,0,0,0,0,0,0},  //B_KNIGHT
    {UL,U,UR,DL,DR,0,0,0},  //B_SILVER
    {UL,UR,DL,DR,0,0,0,0},  //B_BISHOP
    {U,L,R,D,0,0,0,0},  //B_ROOK
};

Move *generate_moves(const Position &pos,Move *ml)
{
    Move *m = NULL;
    char p,cp;
    Color c;

    for(int sq = SQ_A9;sq < SQ_LIMIT;sq++){
        p = pos.board[sq];
        c = color_of_piece(p);
        if(turn == WHITE){
            switch(type_of_piece(p)){
            case PAWN:ml=generate_pawn_moves_w(pos,ml,sq); break;
            case GOLD:ml=generate_gold_moves_w(pos,ml,sq); break;
            case SILVER:ml=generate_silver_moves_w(pos,ml,sq); break;
            case KNIGHT:ml=generate_knight_moves_w(pos,ml,sq); break;
            case KING:ml=generate_king_moves_w(pos,ml,sq); break;
            }
        }
        else{
            switch(type_of_piece(p)){
            case PAWN:ml=generate_pawn_moves_b(pos,ml,sq); break;
            case GOLD:ml=generate_gold_moves_b(pos,ml,sq); break;
            case SILVER:ml=generate_silver_moves_b(pos,ml,sq); break;
            case KNIGHT:ml=generate_knight_moves_b(pos,ml,sq); break;
            case KING:ml=generate_king_moves_b(pos,ml,sq); break;
            }
        }
    }
    return m;
}

Move *generate_pawn_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;
    
    to = from + DIRECT_WHITE[PAWN][0];
    cp = pos.board[to];
    if(cp >= 0){
        p = pos.board[from];
        pmoto = to > SQ_I4 ? 1 : 0;
        *(ml++) = make_move(from,to,pmoto,p,cp);
    }
    return ml;
}

Move *generate_gold_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    for(int i = 0;i < 6;i++){
        to = from + DIRECT_WHITE[GOLD][i];
        cp = pos.board[to];
        p = pos.board[from];
        if(cp >= 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_silver_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    for(int i = 0;i < 5;i++){
        to = from + DIRECT_WHITE[SILVER][i];
        cp = pos.board[to];
        p = pos.board[from];
        if(cp >= 0){
            //�{���͐���łP��A����Ȃ��łP��Ȃ̂ŁA���̌��ߑł��͂悭�Ȃ�
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_knight_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    for(int i = 0;i < 2;i++){
        to = from + DIRECT_WHITE[KNIGHT][i];
        cp = pos.board[to];
        p = pos.board[from];
        if(cp >= 0){
            //�{���͐���łP��A����Ȃ��łP��Ȃ̂ŁA���̌��ߑł��͂悭�Ȃ�,����Ɍj�n�͐���K�{�̃��C�������邱�Ƃ�Y��Ȃ��悤��
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_king_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    for(int i = 0;i < 8;i++){
        to = from + DIRECT_WHITE[KING][i];
        cp = pos.board[to];
        p = pos.board[from];
        //KING�Ȃ̂œG�̗����̂���Ƃ���ɂ͂����Ȃ����܂������܂ł̃f�[�^�������ĂȂ��̂�PASS
        if(cp >= 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}


Move *generate_pawn_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;
    
    to = from + DIRECT_BLACK[PAWN][0];
    cp = pos.board[to];
    if(cp <= 0){
        p = pos.board[from];
        pmoto = to < SQ_A6 ? 1 : 0;
        *(ml++) = make_move(from,to,pmoto,p,cp);
    }
    return ml;
}

Move *generate_gold_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    for(int i = 0;i < 6;i++){
        to = from + DIRECT_BLACK[GOLD][i];
        cp = pos.board[to];
        p = pos.board[from];
        if(cp <= 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_silver_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    for(int i = 0;i < 5;i++){
        to = from + DIRECT_BLACK[SILVER][i];
        cp = pos.board[to];
        p = pos.board[from];
        if(cp <= 0){
            //�{���͐���łP��A����Ȃ��łP��Ȃ̂ŁA���̌��ߑł��͂悭�Ȃ�
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_knight_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    for(int i = 0;i < 2;i++){
        to = from + DIRECT_BLACK[KNIGHT][i];
        cp = pos.board[to];
        p = pos.board[from];
        if(cp <= 0){
            //�{���͐���łP��A����Ȃ��łP��Ȃ̂ŁA���̌��ߑł��͂悭�Ȃ�,����Ɍj�n�͐���K�{�̃��C�������邱�Ƃ�Y��Ȃ��悤��
            pmoto = to > SQ_I4 ? 1 : 0;
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_king_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    for(int i = 0;i < 8;i++){
        to = from + DIRECT_WHITE[KING][i];
        cp = pos.board[to];
        p = pos.board[from];
        //KING�Ȃ̂œG�̗����̂���Ƃ���ɂ͂����Ȃ����܂������܂ł̃f�[�^�������ĂȂ��̂�PASS
        if(cp <= 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
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