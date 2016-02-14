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
	64bitの変数を16進数で表示させるためのprintf関数の表記
		printf("occ.p(0) = %llx,occ.p(1) = %llx\n", occ.p(0), occ.p(1));
		ストリームで表示させる方法もある。
		cout << hex;
		cout << vv << "," << uu << "," << "test" << "," << "words" << endl;
	むやみと名前空間を導入しない、使用するときは限定的に導入する
	× using namespace std;
	○ using std::cout;
*/
#include <stdio.h>
#include <cstdlib>
#include <iostream>

#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

#include "cpuid.h"
#include "types.h"
#include "misc.h"
#include "bitboard.h"
#include "position.h"
#include "usi.h"
#include "mt64bit.h"

using std::cout;
using std::endl;
/*
#include "usioption.h"
#include "evaluate.h"
#include "search.h"
void profile(void);
*/

int main_test(int argc,char *argv[]);

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
	Positionns::init();		//Positionns::init()はBitBoardの設定値を使用して初期化しているので順序変更禁止
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
TEST(main, is_promoto)
{
	EXPECT_TRUE(is_promoto(Black, Rank9));
	EXPECT_TRUE(is_promoto(Black, Rank8));
	EXPECT_TRUE(is_promoto(Black, Rank7));
	EXPECT_FALSE(is_promoto(Black, Rank6));
	EXPECT_FALSE(is_promoto(Black, Rank5));
	EXPECT_FALSE(is_promoto(Black, Rank4));
	EXPECT_FALSE(is_promoto(Black, Rank3));
	EXPECT_FALSE(is_promoto(Black, Rank2));
	EXPECT_FALSE(is_promoto(Black, Rank1));

	EXPECT_TRUE(is_promoto(White, Rank1));
	EXPECT_TRUE(is_promoto(White, Rank2));
	EXPECT_TRUE(is_promoto(White, Rank3));
	EXPECT_FALSE(is_promoto(White, Rank4));
	EXPECT_FALSE(is_promoto(White, Rank5));
	EXPECT_FALSE(is_promoto(White, Rank6));
	EXPECT_FALSE(is_promoto(White, Rank7));
	EXPECT_FALSE(is_promoto(White, Rank8));
	EXPECT_FALSE(is_promoto(White, Rank9));
}
TEST(mt64bit, mt64bit)
{
	//mt64bitはヘッダだけなのでmain.cppでテスト(動作確認)
	//http://elephnote.com/blog/archives/768 このライブラリの使い方
	//cout << "seed固定でいつも同じ乱数を返してくれる" << endl;
	
	MT64bit r;
	for (int i = 0; i < 10; i++){
		//cout << r.random() << endl;
	}
	//cppref.githubのサンプルコード、random_deviceは予測不可能な乱数を生成してくれるクラス、擬似乱数生成エンジンのシードとして使用される
	//cout << "seed変動でいつも違う乱数を返してくれる" << endl;
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	for (int i = 0; i < 10; i++){
		uint64_t result = engine();
		//cout << result << endl;
	}
}

TEST(main, is_infront_rank)
{
	Color c = Black;
	EXPECT_TRUE(is_infront_rank(c, Rank7, Rank9));
	EXPECT_TRUE(is_infront_rank(c, Rank7, Rank8));
	EXPECT_FALSE(is_infront_rank(c, Rank7, Rank7));
	EXPECT_FALSE(is_infront_rank(c, Rank7, Rank6));
	c = White;
	EXPECT_TRUE(is_infront_rank(c, Rank3, Rank1));
	EXPECT_TRUE(is_infront_rank(c, Rank3, Rank2));
	EXPECT_FALSE(is_infront_rank(c, Rank3, Rank3));
	EXPECT_FALSE(is_infront_rank(c, Rank3, Rank4));
}
TEST(main, over_turn)
{
	Color c;
	c = Black;
	c = over_turn(c);
	EXPECT_EQ(White, c);
	c = over_turn(c);
	EXPECT_EQ(Black, c);
}
TEST(main, is_rank)
{
	for (int r = Rank9; r < RankNum; r++){
		EXPECT_TRUE(is_rank(Rank(r)));
	}
	EXPECT_FALSE(is_rank(Rank(-1)));
	EXPECT_FALSE(is_rank(Rank(9)));
}
TEST(main, is_file)
{
	for (int f = FileI; f < FileNum; f++){
		EXPECT_TRUE(is_file(File(f)));
	}
	EXPECT_FALSE(is_file(File(-1)));
	EXPECT_FALSE(is_file(File(9)));
}
TEST(main, make_file)
{
	EXPECT_EQ(FileI, make_file(I9));
	EXPECT_EQ(FileH, make_file(H8));
	EXPECT_EQ(FileG, make_file(G7));
	EXPECT_EQ(FileF, make_file(F6));
	EXPECT_EQ(FileE, make_file(E5));
	EXPECT_EQ(FileD, make_file(D4));
	EXPECT_EQ(FileC, make_file(C3));
	EXPECT_EQ(FileB, make_file(B2));
	EXPECT_EQ(FileA, make_file(A1));
}
TEST(main, make_rank)
{
	EXPECT_EQ(Rank9, make_rank(I9));
	EXPECT_EQ(Rank8, make_rank(H8));
	EXPECT_EQ(Rank7, make_rank(G7));
	EXPECT_EQ(Rank6, make_rank(F6));
	EXPECT_EQ(Rank5, make_rank(E5));
	EXPECT_EQ(Rank4, make_rank(D4));
	EXPECT_EQ(Rank3, make_rank(C3));
	EXPECT_EQ(Rank2, make_rank(B2));
	EXPECT_EQ(Rank1, make_rank(A1));
}
TEST(main, is_square)
{
	for (int sq = I9; sq < SquareNum; sq++){
		EXPECT_EQ(true, is_square(Square(sq)));
	}
	EXPECT_EQ(false, is_square(Square(-1)));
	EXPECT_EQ(false, is_square(Square(81)));
}
TEST(main, color_of_piece)
{
	EXPECT_EQ(Black, color_of_piece(BPawn));
	EXPECT_EQ(Black, color_of_piece(BLance));
	EXPECT_EQ(Black, color_of_piece(BNight));
	EXPECT_EQ(Black, color_of_piece(BSilver));
	EXPECT_EQ(Black, color_of_piece(BBishop));
	EXPECT_EQ(Black, color_of_piece(BRook));
	EXPECT_EQ(Black, color_of_piece(BGold));
	EXPECT_EQ(Black, color_of_piece(BKing));
	EXPECT_EQ(Black, color_of_piece(BProPawn));
	EXPECT_EQ(Black, color_of_piece(BProLance));
	EXPECT_EQ(Black, color_of_piece(BProNight));
	EXPECT_EQ(Black, color_of_piece(BProSilver));
	EXPECT_EQ(Black, color_of_piece(BHorse));
	EXPECT_EQ(Black, color_of_piece(BDragon));

	EXPECT_EQ(White, color_of_piece(WPawn));
	EXPECT_EQ(White, color_of_piece(WLance));
	EXPECT_EQ(White, color_of_piece(WNight));
	EXPECT_EQ(White, color_of_piece(WSilver));
	EXPECT_EQ(White, color_of_piece(WBishop));
	EXPECT_EQ(White, color_of_piece(WRook));
	EXPECT_EQ(White, color_of_piece(WGold));
	EXPECT_EQ(White, color_of_piece(WKing));
	EXPECT_EQ(White, color_of_piece(WProPawn));
	EXPECT_EQ(White, color_of_piece(WProLance));
	EXPECT_EQ(White, color_of_piece(WProNight));
	EXPECT_EQ(White, color_of_piece(WProSilver));
	EXPECT_EQ(White, color_of_piece(WHorse));
	EXPECT_EQ(White, color_of_piece(WDragon));
}
TEST(main, type_of_piece)
{
	EXPECT_EQ(Pawn, type_of_piece(BPawn));
	EXPECT_EQ(Lance, type_of_piece(BLance));
	EXPECT_EQ(Night, type_of_piece(BNight));
	EXPECT_EQ(Silver, type_of_piece(BSilver));
	EXPECT_EQ(Bishop, type_of_piece(BBishop));
	EXPECT_EQ(Rook, type_of_piece(BRook));
	EXPECT_EQ(Gold, type_of_piece(BGold));
	EXPECT_EQ(King, type_of_piece(BKing));
	EXPECT_EQ(ProPawn, type_of_piece(BProPawn));
	EXPECT_EQ(ProLance, type_of_piece(BProLance));
	EXPECT_EQ(ProNight, type_of_piece(BProNight));
	EXPECT_EQ(ProSilver, type_of_piece(BProSilver));
	EXPECT_EQ(Horse, type_of_piece(BHorse));
	EXPECT_EQ(Dragon, type_of_piece(BDragon));

	EXPECT_EQ(Pawn, type_of_piece(WPawn));
	EXPECT_EQ(Lance, type_of_piece(WLance));
	EXPECT_EQ(Night, type_of_piece(WNight));
	EXPECT_EQ(Silver, type_of_piece(WSilver));
	EXPECT_EQ(Bishop, type_of_piece(WBishop));
	EXPECT_EQ(Rook, type_of_piece(WRook));
	EXPECT_EQ(Gold, type_of_piece(WGold));
	EXPECT_EQ(King, type_of_piece(WKing));
	EXPECT_EQ(ProPawn, type_of_piece(WProPawn));
	EXPECT_EQ(ProLance, type_of_piece(WProLance));
	EXPECT_EQ(ProNight, type_of_piece(WProNight));
	EXPECT_EQ(ProSilver, type_of_piece(WProSilver));
	EXPECT_EQ(Horse, type_of_piece(WHorse));
	EXPECT_EQ(Dragon, type_of_piece(WDragon));
}
TEST(main, make_square)
{
	//make_square(file,rank)で指定
	EXPECT_EQ(E2, make_square(FileE, Rank2));
	EXPECT_EQ(H4, make_square(FileH, Rank4));
	EXPECT_EQ(D2, make_square(FileD, Rank2));
	EXPECT_EQ(A7, make_square(FileA, Rank7));
	EXPECT_EQ(B1, make_square(FileB, Rank1));
	EXPECT_EQ(C3, make_square(FileC, Rank3));
	EXPECT_EQ(F5, make_square(FileF, Rank5));
}
#endif
