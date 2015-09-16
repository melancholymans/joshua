/*
Shogidokoro shogi playing engin
命名規則 2015/8/9
	定数
		UPPER_CASE_UNDER
	パブリック変数、プライベート変数
		lower_case_with_under
	関数、メソッド
		lower_case_with_under
	namespace,クラス、列挙型の型
		CamelCase
	グローバル変数の宣言
		元ソースファイルの冒頭に宣言、そのヘッダファイルにextern宣言、グローバル変数を使用したいソースファイル内でインクルード
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

#include "cpuid.h"
#include "misc.h"
#include "bitboard.h"

using namespace std;

/*
#include "position.h"
#include "types.h"
*/
#include "usi.h"
/*
#include "usioption.h"
#include "evaluate.h"
#include "misc.h"
#include "search.h"
*/
/*
void profile(void);
*/

int main_test(int argc,char *argv[]);
/*
void init(void);
*/
int main(int argc,char *argv[])
{
#ifdef _DEBUG
	//CPUの性能を表示させる
	//cpuid();
	//implementation_avx();
	//profile();
	main_test(argc, argv);
#endif
	BitBoardns::init();
    //init();
    //init_usi_options();
    USI::usi_main_loop();
    return 0;
}

//アプリケーション全体の初期化
/*
void init(void)
{
    //駒割配列評価値設定
    memset(piece_value,0,sizeof(piece_value));
    piece_value[PAWN] = DPawn;
    piece_value[LANCE] = DLance;
    piece_value[KNIGHT] = DKnight;
    piece_value[SILVER] = DSilver;
    piece_value[GOLD] = DGold;
    piece_value[BISHOP] = DBishop;
    piece_value[ROOK] = DRook;
    piece_value[KING] = DKing;
    piece_value[PPAWN] = DPPawn;
    piece_value[PLANCE] = DPLance;
    piece_value[PKNIGHT] = DPKnight;
    piece_value[PSILVER] = DPSilver;
    piece_value[PBISHOP] = DPBishop;
    piece_value[PROOK] = DPRook;
    //駒を取った場合の評価値（敵の駒をとると全体の評価値(material)は取られた側の駒の価値+取った側の駒の価値となる）
    memset(cap_piece_value,0,sizeof(cap_piece_value));
    cap_piece_value[PAWN] = DPawn + DPawn;
    cap_piece_value[LANCE] = DLance + DLance;
    cap_piece_value[KNIGHT] = DKnight + DKnight;
    cap_piece_value[SILVER] = DSilver + DSilver;
    cap_piece_value[GOLD] = DGold + DGold;
    cap_piece_value[BISHOP] = DBishop + DBishop;
    cap_piece_value[ROOK] = DRook + DRook;
    cap_piece_value[KING] = DKing + DKing;
    cap_piece_value[PPAWN] = DPPawn + DPawn;
    cap_piece_value[PLANCE] = DPLance + DLance;
    cap_piece_value[PKNIGHT] = DPKnight + DKnight;
    cap_piece_value[PSILVER] = DPSilver + DSilver;
    cap_piece_value[PBISHOP] = DPBishop + DBishop;
    cap_piece_value[PROOK] = DPRook + DRook;
    //駒が成った時の評価値(成った場合は現在の評価値から成る前の駒の価値を引いて、改めて成った後の駒の価値を足し合わせる)
    memset(pmoto_piece_value,0,sizeof(pmoto_piece_value));
    pmoto_piece_value[PPAWN] = DPPawn - DPawn;
    pmoto_piece_value[PLANCE] = DPLance - DLance;
    pmoto_piece_value[PKNIGHT] = DPKnight - DKnight;
    pmoto_piece_value[PSILVER] = DPSilver - DSilver;
    pmoto_piece_value[PBISHOP] = DPBishop - DBishop;
    pmoto_piece_value[PROOK] = DPRook - DRook;
}
*/
/*
void profile(void)
{
    int ply = 0;
    ptime_t aPT;

    ptime_init(&aPT);
    from_sfen(start_position);
    game_init(root_position);

    Move m = search_root(root_position,ply);
    double eapsed_time = ptime_now(&aPT);  //sec
    printf("eapsed_time=%.2f(sec) \n",eapsed_time);
    printf("search node=%dk \n",stats.search_node/1000);
    printf("nps=%.2fk \n",double(stats.search_node/(eapsed_time*1000)));
}
*/
/*
成績（局面の更新のみ）
2014/7
    debug           release
PLY nps             nps             seach node      
1   891k            -               -
2   1,188k          12,474k         2k
3   2,431k                          724k   
4   2,555k          25,119k         19,794k
5   2,561k                          552,915k
6   -時間掛りすぎ    4,320k(10min)   15,496,002k      1局面 38 nsec
7
30乗で増えていくかんじ
30^1 = 30
30^2 = 0.9k         PLY=1
30^3 = 27k          PLY=2   ?
30^4 = 810k         PLY=3
30^5 = 24,300k      PLY=4
30^6 = 729,000k     PLY=5
30^7 = 21,870,000k  PLY=6
*/

#ifdef _DEBUG
//テスト起動
int main_test(int argc,char *argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    int result = RUN_ALL_TESTS();
    return result;
}

TEST(main_case, main)
{
}
#endif
