#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "gtest\gtest.h"
#include "usi.h"

typedef struct position{
    void set_posiition();
    unsigned char board[16*13];
}position_t;


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
    else if(cmd == "usinewgame"){
        //色々初期など準備
    }
    else if(cmd == "isready"){
        cout << "readyok" << endl;
    }
    else if(cmd == "position"){
        //エンジンに思考させる局面を逐一送ってくるので解釈させ内部のデータ構造に変換する
        set_position();
    }
    else if(cmd == "setoption"){
        //エンジンに対する値を設定するとき送信
        set_option(uip);
    }
    else if(cmd == "go"){
        //思考開始
    }
    else{
        cout << "Unknow command" << endl;
        while(!uip.at_end_of_line()){
            cout << uip.get_next_token() << endl;
        }
    }
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

void position_t::set_posiition(void)
{
    this->board[0] = 128;
}

unsigned char set_position(void)
{
    position_t pos;
    pos.set_posiition();
    return pos.board[0];
}

TEST(TestCaseName2,TestCase2)
{
    EXPECT_EQ(128,set_position());
}

