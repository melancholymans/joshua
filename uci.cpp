#include <iostream>
#include <string>

using namespace std;

#include "uci.h"

void uci_main_loop(void)
{
    while(true){
        wait_for_command();
    }
}

void wait_for_command(void)
{
    string command;

    if(!getline(cin,command)){
        //���s�̂ݏꍇ��quit����
        command = "quit";
    }
    handle_command(command);
}

void handle_command(const string &command)
{
    UCIInputParser uip(command);
    string cmd = uip.get_next_token();
    //cout << cmd << endl;
    if(cmd == "quit"){
        //��n��
        exit(0);
    }
    else if(cmd == "usi"){
        cout << "id name " << "joshua" << endl;
        cout << "id author takemori masami" << endl;
        //option���������炱���ő��M
        cout << "usiok" << endl;
    }
    else if(cmd == "usinewgame"){
        //�F�X�����ȂǏ���
    }
    else if(cmd == "isready"){
        cout << "readyok" << endl;
    }
    else if(cmd == "position"){
        //�G���W���Ɏv�l������ǖʂ𒀈ꑗ���Ă���̂ŉ��߂��������̃f�[�^�\���ɕϊ�����
    }
    else if(cmd == "setoption"){
        //�G���W���ɑ΂���l��ݒ肷��Ƃ����M
    }
    else if(cmd == "go"){
        //�v�l�J�n
    }
    else{
        cout << "Unknow command" << endl;
        while(!uip.at_end_of_line()){
            cout << uip.get_next_token() << endl;
        }
    }
}

UCIInputParser::UCIInputParser(const string &line) : input_line(line)
{
    this->current_index = 0;
    this->length = line.length();
}

void UCIInputParser::skip_whitespace(void)
{
    while(isspace((int)(unsigned char)this->input_line[this->current_index])){
        this->current_index++;
    }
}

string UCIInputParser::get_next_token(void)
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

string UCIInputParser::get_rest_of_line(void)
{
    this->skip_whitespace();
    return this->input_line.substr(this->current_index,this->length);
}

bool UCIInputParser::at_end_of_line(void)
{
    return this->current_index == this->length;
}
