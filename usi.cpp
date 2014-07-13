#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "usi.h"
#include "usioption.h"
#include "csa.h"
#include "search.h"
#include "movegen.h"
#include "move.h"
#include "evaluate.h"

void usi_main_loop(void)
{
    string command;

    do{
        if(!getline(cin,command)){
            //���s�̂ݏꍇ��quit����
            command = "quit";
        }
    }while(handle_command(command));
}

void game_init(const Position &pos)
{
    //�΋ǂ��Ə���������K�v������ϐ�
    next_move[0].last_move = mlist;
    next_modify[0].last_dirty = modifylist;
    sech.material = eval_material(pos);
}


//usi->isready->usinewgame�ő΋ǊJ�n
//position�Ŏv�l�ǖʂ̕t�^->go�Ŏv�l
//bestmove�Ŏv�l���ʂ�ԐM,gameover�ŏI��
bool handle_command(const string &command)
{
    USIInputParser uip(command);
    string cmd = uip.get_next_token();
    if(cmd == "quit"){
        //��n��
        return false;
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
        //�F�X�����ȂǏ���,���̃R�}���h�ɑ΂��ď������ɕԂ����̂͂Ȃ�
        from_sfen(start_position);
        game_init(root_position);
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
        go();
    }
    else if(cmd == "d"){
        print_board(root_position);
    }
    else if(cmd == "csa"){
        csa(uip);
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
    return true;    //�����I�ȏI���ȊO�͓���p��(���[�U�[����̏I���R�}���h(quit)�̂�)
}

void set_position(USIInputParser &uip)
{
    string cmd;
    short dummy[8];

    cmd = uip.get_next_token();
    //���菉���ǖ�
    if(cmd == "startpos"){
        from_sfen(start_position);
    }
    //��������ǖ�
    else if(cmd == "sfen"){
        string sfen;
        while(cmd != "moves" && !uip.at_end_of_line()){
            cmd = uip.get_next_token();
            sfen += cmd;
            sfen += ' ';
        }
        from_sfen(sfen);
    }
    //�w����Č�,�ǖʍX�V
    if(!uip.at_end_of_line()){
        if(cmd != "moves"){
            cmd = uip.get_next_token();
        }
        if(cmd == "moves"){
            while(!uip.at_end_of_line()){
                cmd = uip.get_next_token(); //cmd�ɂ͎w���育�ƕ�������ēn��
                Move m = move_from_string(root_position,cmd);
                DoMove(root_position.turn,root_position,m,dummy);
            }
        }
    }
}

void go(void)
{
    /*
    ������go�R�}���h�̌�ɂÂ��I�v�V������͂̃R�[�h�������\��
    */
    think(root_position);
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
    //������̃`�G�b�N
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

