#include <string>
#include <ctype.h>

using namespace std;

#include "gtest\gtest.h"
#include "usioption.h"
#include "position.h"
#include "move.h"

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
                assert(false);
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
        assert(false);
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
            assert(false);
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
            root_position.king_square[c+1] = sq;
        }
    }
    else{
        //こちらは駒台、駒種によって座標は固定されている、格納するのは枚数
        root_position.board[sq] = num;
    }
}

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

void do_move(Position &pos,Move m)
{
    int from = move_from(m);    //(m >> 8) & 0xFF;
    int to = m & 0xFF;
    char cp;
    char p;
    char *base_address;

    if(from != 0){
        //盤上の手
        p = pos.board[from];
        char pt = type_of_piece(p);
        if(pt == KING){
            Color c = color_of_piece(p);
            root_position.king_square[c+1] = to;
        }
        if(pos.board[to] != EMPTY){
            cp = pos.board[to] & 0x0F;
            base_address = pos.turn ? &pos.board[215] : &pos.board[208];
            *(base_address + ((cp | 0x08) - 9)) += 1;
        }
        pos.board[to] = m & 0x10000 ? p-8:p;
        pos.board[from] = EMPTY;
    }
    else{
        //打つ手
        p = move_piece(m);  //(m >> 17) & 0x0F;   //打つ駒種を取り出す
        pos.board[to] = pos.turn ? 0xF0 | p : p;    //駒コードに変換
        base_address = pos.turn ? &pos.board[215] : &pos.board[208];
        *(base_address + ((p | 0x08) - 9)) -= 1;
    }
    pos.turn = ~pos.turn;
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
    EXPECT_EQ(SQ_5A,root_position.king_square[WHITE+1]);
    EXPECT_EQ(SQ_5I,root_position.king_square[BLACK+1]);

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

    EXPECT_EQ(SQ_3A,root_position.king_square[WHITE+1]);
    EXPECT_EQ(SQ_7I,root_position.king_square[BLACK+1]);
}

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
