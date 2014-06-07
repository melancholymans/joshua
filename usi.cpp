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
        //���s�̂ݏꍇ��quit����
        command = "quit";
    }
    handle_command(command);
}

//usi->isready->usinewgame�ő΋ǊJ�n
//position�Ŏv�l�ǖʂ̕t�^->go�Ŏv�l
//bestmove�Ŏv�l���ʂ�ԐM,gameover�ŏI��
void handle_command(const string &command)
{
    USIInputParser uip(command);
    string cmd = uip.get_next_token();
    if(cmd == "quit"){
        //��n��
        exit(0);
    }
    else if(cmd == "usi"){
        cout << "id name " << "joshua" << endl;
        cout << "id author takemori masami" << endl;
        //option���������炱���ő��M
        cout << "option name BookFile value public.bin" << endl;
        cout << "option name UseBook type default true" << endl;
        cout << "usiok" << endl;
    }
    else if(cmd == "isready"){
        cout << "readyok" << endl;
    }
    else if(cmd == "usinewgame"){
        //�F�X�����ȂǏ���
    }
    else if(cmd == "position"){
        //�G���W���Ɏv�l������ǖʂ𒀈ꑗ���Ă���̂ŉ��߂��������̃f�[�^�\���ɕϊ�����
        set_position(uip);
    }
    else if(cmd == "setoption"){
        //�G���W���ɑ΂���l��ݒ肷��Ƃ����M
        set_option(uip);
    }
    else if(cmd == "go"){
        //�v�l�J�n
    }
    else if(cmd == "gameover"){
        //GUI������̏I�ǂ̒ʒm
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
    //���菉���ǖ�
    if(cmd == "startpos"){
        from_sfen(start_position);
    }
    //�r���ǖ�
    else if(cmd == "sfen"){
        string sfen;
        while(cmd != "moves" && !uip.at_end_of_line()){
            cmd = uip.get_next_token();
            sfen += cmd;
            sfen += ' ';
        }
        from_sfen(sfen);
    }
    //�w����Č�
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token();
                //�����̎w����f�[�^�\�� move = cmd�i������j������\���̂ɕϊ�����֐��@move_from_string(root_position,cmd)
                //�ǖʂ̍X�V do_move(move)
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



