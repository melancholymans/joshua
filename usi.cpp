#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "gtest\gtest.h"
#include "usi.h"
#include "usioption.h"
#include "position.h"


void usi_main_loop(void)
{
    while(true){
        wait_for_command();
    }
}

void wait_for_command(void)
{
    string command;

    if(!getline(cin,command)){
        //改行のみ場合はquit扱い
        command = "quit";
    }
    handle_command(command);
}

//usi->isready->usinewgameで対局開始
//positionで思考局面の付与->goで思考
//bestmoveで思考結果を返信,gameoverで終局
void handle_command(const string &command)
{
    USIInputParser uip(command);
    string cmd = uip.get_next_token();
    if(cmd == "quit"){
        //後始末
        exit(0);
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
        //色々初期など準備
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
                //局面の更新 do_move(move)/////////////////////////////////////////////////////////////////////////////
            }
        }
    }
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
    /////////////////////////////////////////////////////////sai テスト
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

int square_from_string(const string sq)
{
    //座標文字列を渡される(sample 7g)
    int col,row;

    col = 9 - (sq[0] - '1');
    row = (sq[1] - 'a') + 1;
    return make_square(col,row);
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

    EXPECT_EQ(W_LANCE,root_position.board[SQ_A9]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_B9]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C9]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D9]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E9]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F9]);
    EXPECT_EQ(BP_ROOK,root_position.board[SQ_G9]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_H9]);
    EXPECT_EQ(BP_SILVER,root_position.board[SQ_I9]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G8]);
    EXPECT_EQ(BP_PAWN,root_position.board[SQ_H8]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I8]);

    EXPECT_EQ(W_PAWN,root_position.board[SQ_A7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_B7]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C7]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D7]);
    EXPECT_EQ(W_KING,root_position.board[SQ_E7]);
    EXPECT_EQ(W_SILVER,root_position.board[SQ_F7]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G7]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H7]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_I7]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B6]);
    EXPECT_EQ(B_GOLD,root_position.board[SQ_C6]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_D6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E6]);
    EXPECT_EQ(W_GOLD,root_position.board[SQ_F6]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_G6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H6]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I6]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_C5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D5]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_E5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G5]);
    EXPECT_EQ(W_PAWN,root_position.board[SQ_H5]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I5]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_B4]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_C4]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_D4]);
    EXPECT_EQ(W_BISHOP,root_position.board[SQ_E4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F4]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_G4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H4]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I4]);

    EXPECT_EQ(B_PAWN,root_position.board[SQ_A3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_B3]);
    EXPECT_EQ(B_SILVER,root_position.board[SQ_C3]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D3]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E3]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F3]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G3]);
    EXPECT_EQ(W_KNIGHT,root_position.board[SQ_H3]);
    EXPECT_EQ(B_PAWN,root_position.board[SQ_I3]);

    EXPECT_EQ(EMPTY,root_position.board[SQ_A2]);
    EXPECT_EQ(B_BISHOP,root_position.board[SQ_B2]);
    EXPECT_EQ(WP_SILVER,root_position.board[SQ_C2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D2]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_E2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F2]);
    EXPECT_EQ(WP_ROOK,root_position.board[SQ_G2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H2]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_I2]);

    EXPECT_EQ(B_LANCE,root_position.board[SQ_A1]);
    EXPECT_EQ(B_KNIGHT,root_position.board[SQ_B1]);
    EXPECT_EQ(B_KING,root_position.board[SQ_C1]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_D1]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_E1]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_F1]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_G1]);
    EXPECT_EQ(EMPTY,root_position.board[SQ_H1]);
    EXPECT_EQ(B_LANCE,root_position.board[SQ_I1]);
}



