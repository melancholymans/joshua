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
    //ファイル名をパースする
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
    //１つファイルに複数の局面を登録しておく処理には対応しない
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
    "N+"に続き +側(先手、下手)の対局者名を記述する。"N-"に続き-側(後手、上手)の対局者名を記述する。それぞれ1行とする。
    省略可能とする。今はなにもしない。
    */
    return;
}

void csa_position(string token)
{
    //局面は一括表現のみ対応する
    int col = 1,row;
    char p;
    char *base_address;
    bool is_stand = false;

    for(int index = 0;index < token.size();index++){
        //盤上
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
        //駒台の対応
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
CSA標準棋譜ファイル形式

1991年 3月 8日 原案 柿木 義一
1991年 3月 9日 CSAで検討
第1版 1991年 5月11日 CSAで検討・決定
第3版 1997年 8月25日 CSA理事会で承認
%KACHI,%HIKIWAKE を追加

1. はじめに

異なる将棋ソフトの間で、棋譜や詰将棋・盤面のデータ交換を可能とするために、棋譜ファイルの標準形式を定める。したがってこの形式は、人にとってわかりやすい、ファイルサイズが小さくなる、等を主な目的としていない。

各ソフトの棋譜ファイル形式を本仕様にする必要はない。各ソフトの棋譜を本形式に変換するソフトを用意すれば、相互にデータの交換が可能となる。

本仕様は、既に定められている以下の規則を基本としている。

(1) 「コンピュータ将棋ファイル記述形式」(第1版)、CSA 資料集 Vol.1
(2) 「通信将棋規約の案」,CSA 資料集 Vol.2
(3) 「第1回コンピュータ将棋選手権, 通信仕様」, CSA 資料集 Vol.4

2. 棋譜ファイルの形式

2.1 概要

棋譜ファイルは処理を容易とするため、テキストファイルとする。MS-DOSファイルの場合は、コメントと対局者名にシフトJISコードの漢字を使用してもよい。

棋譜ファイルは。次のデータから成る。

(1) 対局者名
(2) 開始盤面(持駒、手番を含む)
(3) 指し手と消費時間
(4) コメント

コメント以外は、この順番でデータがなければならない。(1)(2)(3)は、省略できる。

セパレータ(/だけの行)をはさんで、これらデータを繰り返し、複数の棋譜や盤面を示すことができる。

2.2 駒と位置

駒名:歩から玉まで：FU,KY,KE,GI,KI,KA,HI,OU
上の成駒:TO,NY,NK,NG, UM,RY
位置:１一を"11"、５一を"51"、９九を"99"というふうに、2桁の数字で表す。
駒台は"00"。
先手(下手)は"+"、後手(上手)は"-"を付ける。

2.3 対局者名

"N+"に続き +側(先手、下手)の対局者名を記述する。"N-"に続き-側(後手、上手)の対局者名を記述する。それぞれ1行とする。
省略可能とする。

例:
N+NAKAHARA
N-YONENAGA

2.4 開始盤面

Pで始まる文字列(以前に決めたもの)。

(1) 平手初期配置と駒落ち

平手初期配置は、"PI"とする。駒落ちは、"PI"に続き、落とす駒の位置と種類を必要なだけ記述する。

例:二枚落ちPI82HI22KA

(2) 一括表現

1行の駒を以下のように示す。行番号に続き、先後の区別と駒の種類を記述する。先後の区別が"+""-"以外のとき、駒がないとする。
1升3文字で9升分記述しないといけない。

例:
P1-KY-KE-GI-KI-OU-KI-GI-KE-KY
P2 * -HI * * * * * -KA *

(3) 駒別単独表現

一つ一つの駒を示すときは、先後の区別に続き、位置と駒の種類を記述する。持駒に限り、駒の種類として"AL"が使用でき、残りの駒すべてを表す。駒台は"00"である。玉は、駒台へはいかない。

例:
P-22KA
P+99KY89KE
P+00KIOOFU
P-00AL

(4) 手番

"+"で+側(先手)を、"-"で-側(後手)の手番を示す。1行とする。
手番の指定は必要である。

(5) 捕足

初期状態はすべての駒が駒箱にあり、上記(2)(3)の指定は、駒を駒箱から盤上に移動する動作を表現する。したがって、以上の(1)から(3)の指定で位置が決まらないものは、駒箱にあるとする。また、盤面の指定が無いときは、盤上に何も無いとする。
上記(1)と(2)は同時に指定しない。
"P+00AL"、"P-00AL"は、最後に指定しなければならない。
手番は、盤面データの後に指定する。

2.5 指し手と消費時間

1手の指し手を1行とし、次の行にその指し手で消費した時間を示す。

(1) 通常の指し手

先後("+"、または"-")の後、移動前、移動後の位置、移動後の駒名、で表す。

(2) 特殊な指し手

%で始まる。

投了:%TORYO
待った:%MATTA
中断:%CHUDAN
千日手:%SENNICHITE
持将棋:%JISHOGI
詰み:%TSUMI
不詰み:%FUZUMI
エラー:%ERROR (指し手の文字列が正しくない。あるいはルール上指せない等)。
(入玉で)勝ちの宣言:%KACHI
(入玉で)引き分けの宣言:%HIKIWAKE

(注)%KACHI,%HIKIWAKE は、コンピュータ将棋選手権のルールに対応し、第3版で
追加。

(3) 消費時間

"T"に続き、その指し手で消費した時間を秒単位で示す。1秒未満は、切り捨て。省略可能とする。

例:
T10

2.6 コメント

"'"(アポストロフィー)で始まる行はコメントとする。文の途中からのコメントは、できない。

2.7 マルチステートメント

","(カンマ)を用いて、複数の行を1行にまとめることができる。

2.8 ファイル名の拡張子("."以降の名前)

"csa"とする。unixのように大文字と子文字の区別がある場合は子文字。

3. 棋譜ファイルの例

'----------棋譜ファイルの例"example.csa"-----------------
'対局者名
N+NAKAHARA
N-YONENAGA
'平手の盤面
P1-KY-KE-GI-KI-OU-KI-GI-KE-KY
P2 * -HI * * * * * -KA *
P3-FU-FU-FU-FU-FU-FU-FU-FU-FU
P4 * * * * * * * * *
P5 * * * * * * * * *
P6 * * * * * * * * *
P7+FU+FU+FU+FU+FU+FU+FU+FU+FU
P8 * +KA * * * * * +HI *
P9+KY+KE+GI+KI+OU+KI+GI+KE+KY
'先手番
+
'指し手と消費時間
+2726FU
T12
-3334FU
T6
%CHUDAN
*/