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
    //途中局面
    else if(cmd == "sfen"){
        string sfen;
        while(cmd != "moves" && !uip.at_end_of_line()){
            cmd = uip.get_next_token();
            sfen += cmd;
            sfen += ' ';
        }
        from_sfen(sfen);
    }
    //指し手再現
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token();
                //内部の指し手データ構造 move = cmd（文字列）を内部構造体に変換する関数　move_from_string(root_position,cmd)
                //局面の更新 do_move(move)
            }
        }
    }
}
TEST(usi,set_position)
{
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



