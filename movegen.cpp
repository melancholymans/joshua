#include <string>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "movegen.h"

Move mlist[16384];  //bonanzaもこれくらい取っている2^14
next_move_t next_move[256];
short modifylist[1024];    //do_moveでおこなった変更を記録し、undo_moveでこの情報を使ってboardを復元する
next_modify_t next_modify[256]; //modifylistの管理

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

Move *generate_moves(const Position &pos,Move *ml)
{
    char p;
    Color c;

    if(pos.turn ? is_checkmate_w(pos):is_checkmate_b(pos)){
        //自王に王手がかかっているなら王手回避手を生成して返す
        generate_evasions(pos,ml);
        return ml;
    }
    for(int sq = SQ_9A;sq < SQ_LIMIT;sq++){
        p = pos.board[sq];
        c = color_of_piece(p);
        if(c != pos.turn){
            continue;
        }
        if(WHITE == pos.turn){
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
        else if(BLACK == pos.turn){
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
    if(WHITE == pos.turn){
        for(int sq = 215;sq < 222;sq++){
            if(pos.board[sq] > 0){
                switch(sq){
                case 215:ml=generate_gold_drop_w(pos,ml); break;
                case 216:ml=generate_pawn_drop_w(pos,ml); break;
                case 217:ml=generate_lance_drop_w(pos,ml); break;
                case 218:ml=generate_knight_drop_w(pos,ml); break;
                case 219:ml=generate_silver_drop_w(pos,ml); break;
                case 220:ml=generate_bishop_drop_w(pos,ml); break;
                case 221:ml=generate_rook_drop_w(pos,ml); break;
                }
            }
        }
    }
    else if(BLACK == pos.turn){
        for(int sq = 208;sq < 215;sq++){
            if(pos.board[sq] > 0){
                switch(sq){
                case 208:ml=generate_gold_drop_b(pos,ml); break;
                case 209:ml=generate_pawn_drop_b(pos,ml); break;
                case 210:ml=generate_lance_drop_b(pos,ml); break;
                case 211:ml=generate_knight_drop_b(pos,ml); break;
                case 212:ml=generate_silver_drop_b(pos,ml); break;
                case 213:ml=generate_bishop_drop_b(pos,ml); break;
                case 214:ml=generate_rook_drop_b(pos,ml); break;
                }
            }
        }
    }
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
        pmoto = is_pmoto_w(to);
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
            if(cp == 0 || cp > 1){ 
                pmoto = is_pmoto_w(to);
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
            pmoto = is_pmoto_w(to);
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
            pmoto = is_pmoto_w(to);
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
            if(cp == 0 || cp > 1){
                pmoto = is_pmoto_w(to);
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
            if(cp == 0 || cp > 1){
                pmoto = is_pmoto_w(to);
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

Move *generate_gold_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_GOLD,0);
            }
        }
    }
    return ml;
}

Move *generate_pawn_drop_w(const Position &pos,Move *ml)
{
    //２歩判定ができていない、
    for(int row = 1;row < 9;row++){ //死駒対策
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_PAWN,0);
            }
        }
    }
    return ml;
}

Move *generate_lance_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 9;row++){ //死駒対策
        for(int col = 1;col < 10;col++){ 
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_LANCE,0);
            }
        }
    }
    return ml;
}

Move *generate_knight_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 8;row++){ //死駒対策
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_KNIGHT,0);
            }
        }
    }
    return ml;
}

Move *generate_silver_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_SILVER,0);
            }
        }
    }
    return ml;
}

Move *generate_bishop_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_BISHOP,0);
            }
        }
    }
    return ml;
}

Move *generate_rook_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_ROOK,0);
            }
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
        pmoto = is_pmoto_b(to);
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
            if(cp <= 0){ 
                pmoto = is_pmoto_b(to);
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
            pmoto = is_pmoto_b(to);
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
            pmoto = is_pmoto_b(to);
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
            if(cp <= 0){ 
                pmoto = is_pmoto_b(to);
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
            if(cp <= 0){ 
                pmoto = is_pmoto_b(to);
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

Move *generate_gold_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_GOLD,0);
            }
        }
    }
    return ml;
}

Move *generate_pawn_drop_b(const Position &pos,Move *ml)
{
    //２歩判定ができていない、
    for(int row = 2;row < 10;row++){ //死駒対策
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_PAWN,0);
            }
        }
    }
    return ml;
}

Move *generate_lance_drop_b(const Position &pos,Move *ml)
{
    for(int row = 2;row < 10;row++){ //死駒対策
        for(int col = 1;col < 10;col++){ 
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_LANCE,0);
            }
        }
    }
    return ml;
}

Move *generate_knight_drop_b(const Position &pos,Move *ml)
{
    for(int row = 3;row < 10;row++){ //死駒対策
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_KNIGHT,0);
            }
        }
    }
    return ml;
}

Move *generate_silver_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_SILVER,0);
            }
        }
    }
    return ml;
}

Move *generate_bishop_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_BISHOP,0);
            }
        }
    }
    return ml;
}

Move *generate_rook_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_ROOK,0);
            }
        }
    }
    return ml;
}

bool is_checkmate_w(const Position &pos)
{
    //利きデータをもっていないので逐一調べる
    int from,to,i;
    Color c;
    char p,cp;
    
    //from = pos.king_square[pos.turn+1];
    from = (unsigned char)pos.board[223+pos.turn];  //boardはcharデータなので127までの数しか扱えない、board[222],[223]をkingの座標をいれることにしたのでunsigned charでキャストすることで255までの数を扱えるようにした。
    //8方向に敵駒がいないかサーチ
    for(i = 0;i < 8;i++){
        to = from + DIRECT_WHITE[KING][i];
        p = pos.board[to];
        c = color_of_piece(p);
        if(c == ~pos.turn){
            return true;
        }
    }
    for(i = 0;i < 2;i++){
        to = from + DIRECT_WHITE[KNIGHT][i];
        p = pos.board[to];
        c = color_of_piece(p);
        if(c == ~pos.turn && p==B_KNIGHT){
            return true;
        }
    }
    //８方向に敵飛び駒がいるか調べる
    for(to = from,i = 0;i < 8;i++,to = from){
        do{
            to = to + DIRECT_WHITE[KING][i];
            cp = pos.board[to];
            if((cp==B_LANCE) &&
                (DIRECT_WHITE[KING][i]==D)){
                return true;
            }
            if((cp==B_ROOK || cp==BP_ROOK) &&
                (DIRECT_WHITE[KING][i]==U || 
                DIRECT_WHITE[KING][i]==D || 
                DIRECT_WHITE[KING][i]==L || 
                DIRECT_WHITE[KING][i]==R) 
                ){
                return true;
            }
            if((cp==B_BISHOP || cp==BP_BISHOP) &&
                (DIRECT_WHITE[KING][i]==UL ||
                DIRECT_WHITE[KING][i]==UR ||
                DIRECT_WHITE[KING][i]==DL || 
                DIRECT_WHITE[KING][i]==DR)
                ){
                return true;
            }
        }while(cp == EMPTY);
    }
    return false;
}

bool is_checkmate_b(const Position &pos)
{
    //利きデータをもっていないので逐一調べる
    int from,to,i;
    Color c;
    char p,cp;
    
    //from = pos.king_square[pos.turn+1];
    from = (unsigned char)pos.board[223+pos.turn];
    //8方向に敵駒がいないかサーチ
    for(i = 0;i < 8;i++){
        to = from + DIRECT_BLACK[KING][i];
        p = pos.board[to];
        c = color_of_piece(p);
        if(c == ~pos.turn){
            return true;
        }
    }
    //桂馬の方向
    for(i = 0;i < 2;i++){
        to = from + DIRECT_BLACK[KNIGHT][i];
        p = pos.board[to];
        c = color_of_piece(p);
        if(c == ~pos.turn && p==W_KNIGHT){
            return true;
        }
    }
    //８方向に敵飛び駒がいるか調べる
    for(to = from,i = 0;i < 8;i++,to = from){
        do{
            to = to + DIRECT_BLACK[KING][i];
            cp = pos.board[to];
            if((cp==W_LANCE) && (DIRECT_BLACK[KING][i]==U)){
                return true;
            }
            if((cp==W_ROOK || cp==WP_ROOK) &&
                (DIRECT_BLACK[KING][i]==U || 
                DIRECT_BLACK[KING][i]==D || 
                DIRECT_BLACK[KING][i]==L || 
                DIRECT_BLACK[KING][i]==R) 
                ){
                return true;
            }
            if((cp==W_BISHOP || cp==WP_BISHOP) &&
                (DIRECT_BLACK[KING][i]==UL ||
                DIRECT_BLACK[KING][i]==UR ||
                DIRECT_BLACK[KING][i]==DL || 
                DIRECT_BLACK[KING][i]==DR)
                ){
                return true;
            }
        }while(cp == EMPTY);
    }
    return false;
}

Move *generate_evasions(const Position &pos,Move *ml)
{
    //今はなにもしない
    for(int i = 0;i < 8;i++){
    }
    return ml;
}

/*
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

bool array_check(Move anser,Move *m,int n);

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
    Move anser = make_move(SQ_9G,SQ_9F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8G,SQ_8F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7G,SQ_7F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6G,SQ_6F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5G,SQ_5F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4G,SQ_4F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3G,SQ_3F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2G,SQ_2F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1G,SQ_1F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance
    anser = make_move(SQ_9I,SQ_9H,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1I,SQ_1H,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver
    anser = make_move(SQ_7I,SQ_7H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7I,SQ_6H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3I,SQ_4H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3I,SQ_3H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold
    anser = make_move(SQ_6I,SQ_7H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6I,SQ_6H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6I,SQ_5H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4I,SQ_5H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4I,SQ_4H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4I,SQ_3H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //king
    anser = make_move(SQ_5I,SQ_6H,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5I,SQ_5H,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5I,SQ_4H,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //rook
    anser = make_move(SQ_2H,SQ_1H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_3H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_4H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_5H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_6H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_7H,0,B_ROOK,EMPTY);
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
    anser = make_move(SQ_9G,SQ_9F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8F,SQ_8E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3G,SQ_3F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1G,SQ_1F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2C,SQ_2B,1,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance 1
    anser = make_move(SQ_1I,SQ_1H,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //knight    1
    anser = make_move(SQ_8I,SQ_7G,0,B_KNIGHT,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver    5
    anser = make_move(SQ_4E,SQ_5D,0,B_SILVER,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_4D,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_3D,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_5F,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_3F,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold  7
    anser = make_move(SQ_8H,SQ_8G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8H,SQ_7G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_8G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_7G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_6G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_6H,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_7I,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    //bishop
    anser = make_move(SQ_6A,SQ_7B,1,B_BISHOP,W_GOLD);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6A,SQ_5B,1,B_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6A,SQ_4C,1,B_BISHOP,W_SILVER);
    EXPECT_TRUE(array_check(anser,m,n));
    //pbishop
    anser = make_move(SQ_5C,SQ_6B,0,BP_BISHOP,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_4B,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_3A,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_6D,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_7E,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_4D,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_3E,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_2F,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_5B,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_6C,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_4C,0,BP_BISHOP,W_SILVER);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_5D,0,BP_BISHOP,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    //rook  4
    anser = make_move(SQ_8A,SQ_7A,1,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8A,SQ_8B,1,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8A,SQ_8C,1,B_ROOK,W_GOLD);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8A,SQ_9A,1,B_ROOK,W_LANCE);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop pawn
    anser = make_move(0,SQ_7D,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7I,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6C,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6D,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6H,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6I,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5B,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5H,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5I,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4I,0,B_PAWN,EMPTY);
    ASSERT_TRUE(array_check(anser,m,n));

    //Q1の局面でテスト,WHITE側
    from_sfen(expect);
    root_position.turn = WHITE;
    next_move[0].last_move = mlist;
    m = generate_moves(root_position,mlist);
    n = m - next_move[0].last_move;
    EXPECT_EQ(133,n);
    m = mlist;
    //pawn  7
    anser = make_move(SQ_9E,SQ_9F,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7F,SQ_7G,1,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6B,SQ_6C,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5D,SQ_5E,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3C,SQ_3D,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2D,SQ_2E,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1C,SQ_1D,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance 2
    anser = make_move(SQ_9A,SQ_9B,0,W_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1A,SQ_1B,0,W_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //knight
    anser = make_move(SQ_9C,SQ_8E,0,W_KNIGHT,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver    6
    anser = make_move(SQ_8D,SQ_8E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8D,SQ_7E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_4D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_3D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_5B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_3B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold
    anser = make_move(SQ_8C,SQ_9D,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8C,SQ_7D,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8C,SQ_8B,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7B,SQ_6C,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7B,SQ_8B,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7B,SQ_7A,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //king
    anser = make_move(SQ_7C,SQ_7D,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7C,SQ_6D,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7C,SQ_6C,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7C,SQ_8B,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop pawn 9
    anser = make_move(0,SQ_8B,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8E,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8G,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4A,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop silver
    anser = make_move(0,SQ_9B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6C,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop rook
    anser = make_move(0,SQ_9B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6C,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1H,0,W_ROOK,EMPTY);
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