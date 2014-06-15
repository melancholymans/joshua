#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;

#include "gtest\gtest.h"
#include "position.h"
#include "usioption.h"
#include "csa.h"

void csa(USIInputParser &uip)
{
    //�t�@�C�������p�[�X����
    string token;
    fstream file;

    for(int index = 0;index < LIMIT;index++){
        root_position.board[index] = EMPTY;
    }
    token = uip.get_next_token();
    if(token == ""){
        assert(false);
    }
    file.open(token,ios::in);
    if(!file.is_open()){
        assert(false);
        return;
    }
    do{
        getline(file,token);
        csa_parser(token);
        //cout << token << endl;    debug
    }while(!file.eof());
}

void csa_parser(string token)
{
    //�P�t�@�C���ɕ����̋ǖʂ�o�^���Ă��������ɂ͑Ή����Ȃ�
    if(token.empty()){
        return;
    }
    if(token[0] == 'N'){
        csa_header(token);
    }
    if(token[0] == 'P'){
        csa_position(token);
    }
    if(token[0] == '+' || token[0] == '-'){
        if(token[0] == '+'){
            turn = BLACK;
        }
        else if(token[0] == '-'){
            turn = WHITE;
        }
    }
}

void csa_header(string token)
{
    /*
    "N+"�ɑ��� +��(���A����)�̑΋ǎҖ����L�q����B"N-"�ɑ���-��(���A���)�̑΋ǎҖ����L�q����B���ꂼ��1�s�Ƃ���B
    �ȗ��\�Ƃ���B���͂Ȃɂ����Ȃ��B
    */
    return;
}

void csa_position(string token)
{
    //�ǖʂ͈ꊇ�\���̂ݑΉ�����
    int col = 1,row;
    char p;
    char *base_address;
    bool is_stand = false;

    for(int index = 0;index < token.size();index++){
        //�Տ�
        if(token[index] == 'P' && isdigit(token[index+1])){
            row = atoi(token.substr(index+1,1).c_str());
            index +=1;
        }
        else if(token[index] == ' ' || token[index] == '+' || token[index] == '-'){
            p = csa_letters[token.substr(index+1,2)];
            if(token[index] == '-'){
                p = p | 0xF0;
            }
            int sq = make_square(col,row);
            root_position.board[sq] = p;
            col++;
            index += 2;
        }
        //���̑Ή�
        else if(token[index] == 'P' && (token[index+1] == '+' || token[index+1] == '-')){
            is_stand = true;
            base_address = (token[index+1] == '-') ? &root_position.board[215] : &root_position.board[208];
            index += 1;
        }
        else if(is_stand){
            p = csa_letters[token.substr(index+2,2)];
            *(base_address + (p - 9)) += 1;
            index += 3;
        }
    }
}

TEST(csa,csa)
{
    string sfen;
    string expect;

    string cmd("Q1.csa");
    USIInputParser uip1(cmd);
    csa(uip1);
    sfen = to_sfen(root_position);
    expect ="lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b P5psr 1";
    EXPECT_STREQ(expect.c_str(),sfen.c_str()); 

    cmd = "Q2.csa";
    USIInputParser uip2(cmd);
    csa(uip2);
    sfen = to_sfen(root_position);
    expect ="5S2l/1rP2s1k1/p2+B1gnp1/5np2/3G3n1/5S2p/P1+p1PpPP1/1P1PG2KP/L2+rLPGNL b B3ps 1";
    EXPECT_STREQ(expect.c_str(),sfen.c_str()); 

    cmd = "Q3.csa";
    USIInputParser uip3(cmd);
    csa(uip3);
    sfen = to_sfen(root_position);
    expect ="lR6l/1s1g5/1k1s1+P2p/1+bpp1+Bs2/1n1n2Pp1/2P6/S2R4P/K1GG5/9 b P2Ng9p2l 1";
    EXPECT_STREQ(expect.c_str(),sfen.c_str()); 
}
/*
CSA�W�������t�@�C���`��

1991�N 3�� 8�� ���� �`�� �`��
1991�N 3�� 9�� CSA�Ō���
��1�� 1991�N 5��11�� CSA�Ō����E����
��3�� 1997�N 8��25�� CSA������ŏ��F
%KACHI,%HIKIWAKE ��ǉ�

1. �͂��߂�

�قȂ鏫���\�t�g�̊ԂŁA������l�����E�Ֆʂ̃f�[�^�������\�Ƃ��邽�߂ɁA�����t�@�C���̕W���`�����߂�B���������Ă��̌`���́A�l�ɂƂ��Ă킩��₷���A�t�@�C���T�C�Y���������Ȃ�A������ȖړI�Ƃ��Ă��Ȃ��B

�e�\�t�g�̊����t�@�C���`����{�d�l�ɂ���K�v�͂Ȃ��B�e�\�t�g�̊�����{�`���ɕϊ�����\�t�g��p�ӂ���΁A���݂Ƀf�[�^�̌������\�ƂȂ�B

�{�d�l�́A���ɒ�߂��Ă���ȉ��̋K������{�Ƃ��Ă���B

(1) �u�R���s���[�^�����t�@�C���L�q�`���v(��1��)�ACSA �����W Vol.1
(2) �u�ʐM�����K��̈āv,CSA �����W Vol.2
(3) �u��1��R���s���[�^�����I�茠, �ʐM�d�l�v, CSA �����W Vol.4

2. �����t�@�C���̌`��

2.1 �T�v

�����t�@�C���͏�����e�ՂƂ��邽�߁A�e�L�X�g�t�@�C���Ƃ���BMS-DOS�t�@�C���̏ꍇ�́A�R�����g�Ƒ΋ǎҖ��ɃV�t�gJIS�R�[�h�̊������g�p���Ă��悢�B

�����t�@�C���́B���̃f�[�^���琬��B

(1) �΋ǎҖ�
(2) �J�n�Ֆ�(����A��Ԃ��܂�)
(3) �w����Ə����
(4) �R�����g

�R�����g�ȊO�́A���̏��ԂŃf�[�^���Ȃ���΂Ȃ�Ȃ��B(1)(2)(3)�́A�ȗ��ł���B

�Z�p���[�^(/�����̍s)���͂���ŁA�����f�[�^���J��Ԃ��A�����̊�����Ֆʂ��������Ƃ��ł���B

2.2 ��ƈʒu

�:������ʂ܂ŁFFU,KY,KE,GI,KI,KA,HI,OU
��̐���:TO,NY,NK,NG, UM,RY
�ʒu:�P���"11"�A�T���"51"�A�X���"99"�Ƃ����ӂ��ɁA2���̐����ŕ\���B
����"00"�B
���(����)��"+"�A���(���)��"-"��t����B

2.3 �΋ǎҖ�

"N+"�ɑ��� +��(���A����)�̑΋ǎҖ����L�q����B"N-"�ɑ���-��(���A���)�̑΋ǎҖ����L�q����B���ꂼ��1�s�Ƃ���B
�ȗ��\�Ƃ���B

��:
N+NAKAHARA
N-YONENAGA

2.4 �J�n�Ֆ�

P�Ŏn�܂镶����(�ȑO�Ɍ��߂�����)�B

(1) ���菉���z�u�Ƌ��

���菉���z�u�́A"PI"�Ƃ���B����́A"PI"�ɑ����A���Ƃ���̈ʒu�Ǝ�ނ�K�v�Ȃ����L�q����B

��:�񖇗���PI82HI22KA

(2) �ꊇ�\��

1�s�̋���ȉ��̂悤�Ɏ����B�s�ԍ��ɑ����A���̋�ʂƋ�̎�ނ��L�q����B���̋�ʂ�"+""-"�ȊO�̂Ƃ��A��Ȃ��Ƃ���B
1��3������9�����L�q���Ȃ��Ƃ����Ȃ��B

��:
P1-KY-KE-GI-KI-OU-KI-GI-KE-KY
P2 * -HI * * * * * -KA *

(3) ��ʒP�ƕ\��

���̋�������Ƃ��́A���̋�ʂɑ����A�ʒu�Ƌ�̎�ނ��L�q����B����Ɍ���A��̎�ނƂ���"AL"���g�p�ł��A�c��̋�ׂĂ�\���B����"00"�ł���B�ʂ́A���ւ͂����Ȃ��B

��:
P-22KA
P+99KY89KE
P+00KIOOFU
P-00AL

(4) ���

"+"��+��(���)���A"-"��-��(���)�̎�Ԃ������B1�s�Ƃ���B
��Ԃ̎w��͕K�v�ł���B

(5) �ߑ�

������Ԃ͂��ׂĂ̋��ɂ���A��L(2)(3)�̎w��́A��������Տ�Ɉړ����铮���\������B���������āA�ȏ��(1)����(3)�̎w��ňʒu�����܂�Ȃ����̂́A��ɂ���Ƃ���B�܂��A�Ֆʂ̎w�肪�����Ƃ��́A�Տ�ɉ��������Ƃ���B
��L(1)��(2)�͓����Ɏw�肵�Ȃ��B
"P+00AL"�A"P-00AL"�́A�Ō�Ɏw�肵�Ȃ���΂Ȃ�Ȃ��B
��Ԃ́A�Ֆʃf�[�^�̌�Ɏw�肷��B

2.5 �w����Ə����

1��̎w�����1�s�Ƃ��A���̍s�ɂ��̎w����ŏ�������Ԃ������B

(1) �ʏ�̎w����

���("+"�A�܂���"-")�̌�A�ړ��O�A�ړ���̈ʒu�A�ړ���̋�A�ŕ\���B

(2) ����Ȏw����

%�Ŏn�܂�B

����:%TORYO
�҂���:%MATTA
���f:%CHUDAN
�����:%SENNICHITE
������:%JISHOGI
�l��:%TSUMI
�s�l��:%FUZUMI
�G���[:%ERROR (�w����̕����񂪐������Ȃ��B���邢�̓��[����w���Ȃ���)�B
(���ʂ�)�����̐錾:%KACHI
(���ʂ�)���������̐錾:%HIKIWAKE

(��)%KACHI,%HIKIWAKE �́A�R���s���[�^�����I�茠�̃��[���ɑΉ����A��3�ł�
�ǉ��B

(3) �����

"T"�ɑ����A���̎w����ŏ�������Ԃ�b�P�ʂŎ����B1�b�����́A�؂�̂āB�ȗ��\�Ƃ���B

��:
T10

2.6 �R�����g

"'"(�A�|�X�g���t�B�[)�Ŏn�܂�s�̓R�����g�Ƃ���B���̓r������̃R�����g�́A�ł��Ȃ��B

2.7 �}���`�X�e�[�g�����g

","(�J���})��p���āA�����̍s��1�s�ɂ܂Ƃ߂邱�Ƃ��ł���B

2.8 �t�@�C�����̊g���q("."�ȍ~�̖��O)

"csa"�Ƃ���Bunix�̂悤�ɑ啶���Ǝq�����̋�ʂ�����ꍇ�͎q�����B

3. �����t�@�C���̗�

'----------�����t�@�C���̗�"example.csa"-----------------
'�΋ǎҖ�
N+NAKAHARA
N-YONENAGA
'����̔Ֆ�
P1-KY-KE-GI-KI-OU-KI-GI-KE-KY
P2 * -HI * * * * * -KA *
P3-FU-FU-FU-FU-FU-FU-FU-FU-FU
P4 * * * * * * * * *
P5 * * * * * * * * *
P6 * * * * * * * * *
P7+FU+FU+FU+FU+FU+FU+FU+FU+FU
P8 * +KA * * * * * +HI *
P9+KY+KE+GI+KI+OU+KI+GI+KE+KY
'����
+
'�w����Ə����
+2726FU
T12
-3334FU
T6
%CHUDAN
*/