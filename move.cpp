#include <string>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "move.h"

const int PMOTO = 1 << 16;  //65536
const int NOT_PMOTO = 0;

string string_from_move(const Move m)
{
    //Move構造体を座標文字に変換
    int from = move_from(m);
    int to = move_to(m);
    char piece = move_piece(m);
    char cap_piece = move_cap_piece(m);
    int pmoto = move_pmoto(m);
    string result = "";
    string piece_string(" GPLNSBR");

    if(from == 0){
        //打つ手
        result = piece_string[piece-8];   
        result += "*" +string_from_square(to);
    }
    else{
        //盤上
        result = string_from_square(from) + string_from_square(to);
        if(pmoto){
            result += "+";
        }
    }
    return result;
}

string string_from_square(int sq)
{
    int row,col;
    string col_string = " 987654321";
    string row_string = " abcdefghi";

    make_col_row(sq,&col,&row);
    return col_string.substr(col,1) + row_string.substr(row,1);
}

Move move_from_string(const Position &pos,const string &cmd)
{
    //渡される文字列は sample 7g7f 又は G*5b
    string pieceLetters(" GPLNSBRK");   //Gが先頭になっているのは打手の駒コードが+8の処理をしているのでそれにあわせるため
    string square_string = "123456789";
    int from,to;
    bool pmoto;
    char piece,cap_piece;

    if(cmd.size() < 4){
        cout << "Error move string format" << endl;
        exit(0);
    }
    if(cmd[1] == '*'){
        from = 0;   //打手の場合は0
        to = square_from_string(cmd.substr(2,2));
        pmoto = false;
        piece = pieceLetters.find(cmd[0]) + 8;
        cap_piece = 0;
    }
    else if(square_string.find(cmd[0]) != string::npos){
        //盤上の移動
        from = square_from_string(cmd.substr(0,2));
        to = square_from_string(cmd.substr(2,2));
        pmoto = false;
        if(cmd.size() == 5 && cmd[4] == '+'){
            pmoto = true;
        }
        piece = pos.board[from] & 0x0F;     //colorは外す
        cap_piece = pos.board[to] & 0x0F;   //colorは外す
    }
    else{
        cout << "Error move string format" << endl;
        exit(0);
    }
    return make_move(from,to,pmoto,piece,cap_piece);
}

int square_from_string(const string sq)
{
    //座標文字列を渡される(sample 7g)
    int col,row;

    col = 9 - (sq[0] - '1');
    row = (sq[1] - 'a') + 1;
    return make_square(col,row);
}

TEST(move,move_from_string)
{
    //平手初期局面を与え、駒を動かして、正しいMoveが返ってくるか調べる
    from_sfen(start_position);
    string cmd;
    Move m;

    cmd = "7g7f";   //第１手で77歩を76歩へ移動
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1344371,m);
    cmd = "5a6b";   //51王を62王へ移動
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1058100,m);
    //取る手,成るテスト
    cmd = "9g9c+";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(22380865,m);
    //打つテスト
    cmd = "P*9d";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1310801,m);
    cmd = "L*8e";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1441890,m);
    cmd = "N*7f";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1572979,m);
    cmd = "S*6d";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1704020,m);
    cmd = "B*5e";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1835109,m);
    cmd = "R*4f";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1966198,m);
    cmd = "G*3d";
    m = move_from_string(root_position,cmd);
    EXPECT_EQ(1179735,m);
}

TEST(move,square_from_string)
{
    EXPECT_EQ(33,square_from_string(string("9a")));
    EXPECT_EQ(50,square_from_string(string("8b")));
    EXPECT_EQ(67,square_from_string(string("7c")));
    EXPECT_EQ(84,square_from_string(string("6d")));
    EXPECT_EQ(101,square_from_string(string("5e")));
    EXPECT_EQ(118,square_from_string(string("4f")));
    EXPECT_EQ(135,square_from_string(string("3g")));
    EXPECT_EQ(152,square_from_string(string("2h")));
    EXPECT_EQ(169,square_from_string(string("1i")));
}

TEST(move,string_from_square)
{
    EXPECT_STREQ("9a",string_from_square(SQ_9A).c_str());
    EXPECT_STREQ("8b",string_from_square(SQ_8B).c_str());
    EXPECT_STREQ("7c",string_from_square(SQ_7C).c_str());
    EXPECT_STREQ("6d",string_from_square(SQ_6D).c_str());
    EXPECT_STREQ("5e",string_from_square(SQ_5E).c_str());
    EXPECT_STREQ("4f",string_from_square(SQ_4F).c_str());
    EXPECT_STREQ("3g",string_from_square(SQ_3G).c_str());
    EXPECT_STREQ("2h",string_from_square(SQ_2H).c_str());
    EXPECT_STREQ("1i",string_from_square(SQ_1I).c_str());
}


TEST(move,string_from_move)
{
    Move m;
    m = make_move(SQ_9G,SQ_9F,0,B_PAWN,EMPTY);
    EXPECT_STREQ("9g9f",string_from_move(m).c_str());
    m = make_move(SQ_7F,SQ_6D,0,B_KNIGHT,EMPTY);
    EXPECT_STREQ("7f6d",string_from_move(m).c_str());
    m = make_move(SQ_8B,SQ_3B,0,W_ROOK,EMPTY);
    EXPECT_STREQ("8b3b",string_from_move(m).c_str());
    m = make_move(0,SQ_5E,0,W_ROOK,EMPTY);
    EXPECT_STREQ("R*5e",string_from_move(m).c_str());
    m = make_move(SQ_8B,SQ_8H,1,W_ROOK,EMPTY);
    EXPECT_STREQ("8b8h+",string_from_move(m).c_str());
}

TEST(move,move_from)
{
    Move m;
    m = make_move(SQ_9G,SQ_9F,0,B_PAWN,EMPTY);
    EXPECT_EQ(SQ_9G,move_from(m));
    EXPECT_EQ(SQ_9F,move_to(m));
    
    EXPECT_EQ(B_PAWN,move_piece(m));
    EXPECT_EQ(EMPTY,move_cap_piece(m));
    EXPECT_EQ(NOT_PMOTO,move_pmoto(m));

    m = make_move(SQ_7F,SQ_6D,0,B_KNIGHT,EMPTY);
    EXPECT_EQ(SQ_7F,move_from(m));
    EXPECT_EQ(SQ_6D,move_to(m));
    EXPECT_EQ(B_KNIGHT,move_piece(m));
    EXPECT_EQ(EMPTY,move_cap_piece(m));
    EXPECT_EQ(NOT_PMOTO,move_pmoto(m));
    
    m = make_move(SQ_8B,SQ_3B,0,W_LANCE,EMPTY);
    EXPECT_EQ(SQ_8B,move_from(m));
    EXPECT_EQ(SQ_3B,move_to(m));
    EXPECT_EQ(B_LANCE,move_piece(m));   //Color情報がないのでW_LANCEと書けない
    EXPECT_EQ(EMPTY,move_cap_piece(m));
    EXPECT_EQ(NOT_PMOTO,move_pmoto(m));

    m = make_move(0,SQ_5E,0,W_BISHOP,B_PAWN);
    EXPECT_EQ(0,move_from(m));
    EXPECT_EQ(SQ_5E,move_to(m));
    EXPECT_EQ(B_BISHOP,move_piece(m));
    EXPECT_EQ(B_PAWN,move_cap_piece(m));
    EXPECT_EQ(NOT_PMOTO,move_pmoto(m));

    m = make_move(SQ_8B,SQ_8H,1,B_SILVER,W_GOLD);
    EXPECT_EQ(SQ_8B,move_from(m));
    EXPECT_EQ(SQ_8H,move_to(m));
    EXPECT_EQ(B_SILVER,move_piece(m));
    EXPECT_EQ(B_GOLD,move_cap_piece(m));
    EXPECT_EQ(PMOTO,move_pmoto(m));

    m = make_move(SQ_8B,SQ_1I,1,W_ROOK,EMPTY);
    EXPECT_EQ(SQ_8B,move_from(m));
    EXPECT_EQ(SQ_1I,move_to(m));
    EXPECT_EQ(B_ROOK,move_piece(m));
    EXPECT_EQ(EMPTY,move_cap_piece(m));
    EXPECT_EQ(PMOTO,move_pmoto(m));
    
}