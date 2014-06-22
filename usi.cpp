#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "gtest\gtest.h"
#include "usi.h"
#include "usioption.h"
#include "csa.h"
#include "position.h"
#include "search.h"
#include "movegen.h"

void usi_main_loop(void)
{
    string command;

    do{
        if(!getline(cin,command)){
            //改行のみ場合はquit扱い
            command = "quit";
        }
    }while(handle_command(command));
}

void game_init(void)
{
    //対局ごと初期化する必要がある変数
}


//usi->isready->usinewgameで対局開始
//positionで思考局面の付与->goで思考
//bestmoveで思考結果を返信,gameoverで終局
bool handle_command(const string &command)
{
    USIInputParser uip(command);
    string cmd = uip.get_next_token();
    if(cmd == "quit"){
        //後始末
        return false;
    }
    else if(cmd == "usi"){
        cout << "id name " << "joshua" << endl;
        cout << "id author takemori masami" << endl;
        //optionがあったらここで送信
        cout << "option name BookFile value public.bin" << endl;
        cout << "option name UseBook type default true" << endl;
        cout << "usiok" << endl;
    }
    else if(cmd == "isready"){
        cout << "readyok" << endl;
    }
    else if(cmd == "usinewgame"){
        //色々初期など準備,このコマンドに対して将棋所に返すものはない
        game_init();
        from_sfen(start_position);
    }
    else if(cmd == "position"){
        //エンジンに思考させる局面を逐一送ってくるので解釈させ内部のデータ構造に変換する
        set_position(uip);
    }
    else if(cmd == "setoption"){
        //エンジンに対する値を設定するとき送信
        set_option(uip);
    }
    else if(cmd == "go"){
        //思考開始
        go();
    }
    else if(cmd == "d"){
        print_board(root_position);
    }
    else if(cmd == "csa"){
        csa(uip);
    }
    else if(cmd == "gameover"){
        //GUI側からの終局の通知

    }
    else{
        cout << "Unknow command" << endl;
        while(!uip.at_end_of_line()){
            cout << uip.get_next_token() << endl;
        }
    }
    return true;    //明示的な終了以外は動作継続(ユーザーからの終了コマンド(quit)のみ)
}

void set_position(USIInputParser &uip)
{
    string cmd;

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
    //指し手再現,局面更新
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token(); //cmdには指し手ごと分割されて渡す
                Move m = move_from_string(root_position,cmd);
                do_move(root_position,m);
            }
        }
    }
}

void go(void)
{
    next_move[0].last_move = mlist;
    think(root_position,mlist);
}

string string_from_move(const Move m)
{
    //Move構造体を座標文字に変換
    int from,to;
    char piece,cap_piece;
    int pmoto;
    string result = "";
    string piece_string(" GPLNSBR");

    from = (m >> 8) & 0xFF;
    to = m & 0xFF;
    piece = (m >> 17) & 0x0F;   //先手、後手とも強制的に駒種へ
    cap_piece = (m >> 21) & 0x0F;
    pmoto = m >> 16 & 0x01;
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

    row = sq/16;
    col = sq - row*16;
    row = row - 1;
    return col_string.substr(col,1) + row_string.substr(row,1);
}

Move move_from_string(const Position &pos, const string &cmd)
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

USIInputParser::USIInputParser(const string &line) : input_line(line)
{
    this->current_index = 0;
    this->length = line.length();
}

void USIInputParser::skip_whitespace(void)
{
    while(isspace((int)(unsigned char)this->input_line[this->current_index])){
        this->current_index++;
    }
}

string USIInputParser::get_next_token(void)
{
    int i,j;

    this->skip_whitespace();
    for(i = j = this->current_index;j < this->length && !isspace(this->input_line[j]);j++){
        ;
    }
    this->current_index = j;
    this->skip_whitespace();
    string str = this->input_line.substr(i,j - i);
    return str;
}

string USIInputParser::get_rest_of_line(void)
{
    this->skip_whitespace();
    return this->input_line.substr(this->current_index,this->length);
}

bool USIInputParser::at_end_of_line(void)
{
    return this->current_index == this->length;
}

TEST(usi,set_position)
{
    string command("startpos moves 7g7f 3c3d 2g2f 4c4d 3i4h 3a4b 5g5f 5c5d 2f2e 2b3c 5i6h 4b4c 6h7h 7a6b 7i6h 6b5c 4i5h 7c7d 6g6f 4a3b 6h7g 4d4e 5h6g 5a4b 3g3f 5d5e 2i3g 5c5d 5f5e 3c5e 2e2d 2c2d 2h2d 4b3a P*2c 4e4f 4g4f P*4g 4h4g 6a5b 3g4e 5d4e 4f4e 5e3g+ S*2b 3a4b 2b1a+ 3b3c 2d2e P*2d P*5c 2d2e 5c5b+ 8b5b G*3h 3g3h 4g3h R*4h L*5h 4h3h+ 2c2b+ N*2g B*3a 4b4a P*5c 5b3b 4e4d 3c4d P*4b 3b4b 3a4b+ 4a4b R*7a P*5g 7a3a+ 4b5c 6g5g P*5f 5g5f P*5e 5f6e 6c6d 6e7d B*5f 7h6h S*6g 6h7i P*7h 6i7h 6g7h+");
    USIInputParser uip(command);
    set_position(uip);

    EXPECT_EQ(W_LANCE,root_position.board[SQ_9A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_8A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5A]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4A]);
    EXPECT_EQ(BP_ROOK,root_position.board[SQ_3A]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_2A]);
    EXPECT_EQ(BP_SILVER,root_position.board[SQ_1A]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3B]);
    EXPECT_EQ(BP_PAWN,root_position.board[SQ_2B]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1B]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_9C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_8C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6C]);
    EXPECT_EQ(W_KING,root_position.board[SQ_5C]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_4C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3C]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2C]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_1C]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8D]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_7D]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_6D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5D]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_4D]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_3D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2D]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1D]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_7E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6E]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_5E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3E]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_2E]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1E]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_8F]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_7F]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_6F]);
    EXPECT_EQ(W_BISHOP,root_position.board[SQ_5F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4F]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_3F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2F]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1F]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_9G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_8G]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_7G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4G]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3G]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_2G]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_1G]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_9H]);
    EXPECT_EQ(B_BISHOP,root_position.board[SQ_8H]);
    EXPECT_EQ(WP_SILVER,root_position.board[SQ_7H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6H]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_5H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4H]);
    EXPECT_EQ(WP_ROOK,root_position.board[SQ_3H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2H]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_1H]);

    EXPECT_EQ(B_LANCE,root_position.board[SQ_9I]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_8I]);
    EXPECT_EQ(B_KING,root_position.board[SQ_7I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_6I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_5I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_4I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_3I]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_2I]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_1I]);
    //持ち駒のチエック
    EXPECT_EQ(0,root_position.board[208]);  //black gold
    EXPECT_EQ(4,root_position.board[209]);  //black pawn
    EXPECT_EQ(0,root_position.board[210]);  //black lance
    EXPECT_EQ(0,root_position.board[211]);  //black knight
    EXPECT_EQ(0,root_position.board[212]);  //black silver
    EXPECT_EQ(0,root_position.board[213]);  //black bishop
    EXPECT_EQ(0,root_position.board[214]);  //black rook

    EXPECT_EQ(2,root_position.board[215]);  //white gold
    EXPECT_EQ(0,root_position.board[216]);  //white pawn
    EXPECT_EQ(0,root_position.board[217]);  //white lance
    EXPECT_EQ(0,root_position.board[218]);  //white knight
    EXPECT_EQ(0,root_position.board[219]);  //white silver
    EXPECT_EQ(0,root_position.board[220]);  //white bishop
    EXPECT_EQ(0,root_position.board[221]);  //white rook
}

TEST(usi,move_from_string)
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

TEST(usi,square_from_string)
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

TEST(usi,string_from_square)
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

TEST(usi,string_from_move)
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
