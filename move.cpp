#include <string>

#include "gtest\gtest.h"
#include "position.h"
#include "move.h"
#include "usi.h"

//名前空間宣言
using std::string;
using std::cout;
using std::endl;
using namespace Movens;
//局所関数宣言
static string string_from_square(Square sq);

//将棋所の説明　(http://www.geocities.jp/shogidokoro/usi.html)
//次に、指し手の表記について解説します。筋(縦の方向)に関しては１から９までの数字で表記され、段（横の方向）に関してはaからiまでのアルファベット（１段目がa、２段目がb、・・・、９段目がi）というように表記されます。位置の表記は、この２つを組み合わせます。５一なら5a、１九なら1iとなります。
//そして、指し手に関しては、駒の移動元の位置と移動先の位置を並べて書きます。(筋＋段と表記)７七の駒が７六に移動したのであれば、7g7fと表記します。（駒の種類を表記する必要はありません。）
//駒が成るときは、最後に + を追加します。８八の駒が２二に移動して成るなら8h2b + と表記します。
//持ち駒を打つときは、最初に駒の種類を大文字で書き、それに*を追加し、さらに打った場所を追加します。金を５二に打つ場合はG*5bとなります
//  987654321 <-将棋所の座標系
//a lnsgkgsnl
//b *r*****b*
//c ppppppppp
//d *********
//e *********
//f *********
//g PPPPPPPPP
//h *B*****R*
//i LNSGKGSNL
//参考にしているaperyの座標系とは異なることに注意

//Moveを将棋所の指し手文字列に変換
string Movens::string_from_move(const Move m)
{
	string piece_string(" PLNSBRG");
    Square from = move_from(m);
    Square to = move_to(m);
    PieceType piece = move_piece(m);
    PieceType cap_piece = move_cap_piece(m);
    bool pmoto = is_pmoto(m);
    string result = "";

    if(from > (SquareNum -1)){
        //打つ手
		result = piece_string[drop_piece(m)];
        result += "*" + string_from_square(to);
    }
    else{
        //盤上
        result = string_from_square(from) + string_from_square(to);
        if(pmoto){
            result += "+";
        }
    }
    return result;
}
//将棋所指し手文字列からMoveに変換
Move Movens::move_from_string(const Position &pos, const string &cmd)
{
	//渡される文字列は sample 7g7f 又は G*5b
	string piece_string(" PLNSBRG");
	string square_string = "123456789";
	Square from, to;
	bool pmoto;
	PieceType pt, cap_pt;

	if (cmd.size() < 4){
		cout << "Error move string format" << endl;
		exit(0);
	}
	if (cmd[1] == '*'){
		from = Square(piece_string.find(cmd[0]) + SquareNum - 1);   //打手の場合fromに駒種を入れておく
		to = square_from_string(cmd.substr(2, 2));
		pmoto = false;
		pt = PieceType(0);	//打ち手の時は使わない
		cap_pt = PieceType(0);
	}
	else if (square_string.find(cmd[0]) != string::npos){
		//盤上の移動
		from = square_from_string(cmd.substr(0, 2));
		to = square_from_string(cmd.substr(2, 2));
		pmoto = false;
		if (cmd.size() == 5 && cmd[4] == '+'){
			pmoto = true;
		}
		pt = PieceType(pos.get_board(from) & 0x0F);     //colorは外す
		cap_pt = PieceType(pos.get_board(to) & 0x0F);   //colorは外す
	}
	else{
		cout << "Error move string format" << endl;
		exit(0);
	}
	return make_move(from, to, pmoto, pt, cap_pt);
}

//将棋所表記の座標文字列をapery座標に変換
Square Movens::square_from_string(const string sq)
{
	//座標文字列を渡される(sample 7g)
	File f;
	Rank r;

	f = File(sq[0] - '1');
	r = Rank(sq[1] - 'a');
	return make_square(f, r);
}
//apery座標を将棋所表記の座標文字列に変換
static string string_from_square(Square sq)
{
    int row,col;
    string col_string = "123456789";
    string row_string = "abcdefghi";

	col = sq / 9;
	row = sq - col * 9;
    return col_string.substr(col,1) + row_string.substr(row,1);
}


#ifdef _DEBUG
TEST(move, string_from_move)
{
	Move m;

	m = make_move(A3, A4, 0, Pawn, EmptyPiece);
	EXPECT_STREQ("9g9f",string_from_move(m).c_str());
	m = make_move(C4, D6, 0, Night, EmptyPiece);
	EXPECT_STREQ("7f6d",string_from_move(m).c_str());
	m = make_move(B8, G8, 0, Rook, EmptyPiece);
	EXPECT_STREQ("8b3b",string_from_move(m).c_str());
	m = make_move(drop_piece_from(Rook), E5, 0, EmptyPiece, EmptyPiece);
	EXPECT_STREQ("R*5e",string_from_move(m).c_str());
	m = make_move(B8, B2, 1, Rook, EmptyPiece);
	EXPECT_STREQ("8b8h+",string_from_move(m).c_str());
}

TEST(move,move_from_string)
{
    //平手初期局面を与え、駒を動かして、正しいMoveが返ってくるか調べる
	Position pos(USI::start_sfen);
	using Movens::square_from_string;

    string cmd;
    Move m,am;
	Square from, to;
	PieceType pt;

	cmd = "7g7f";   //第１手で77歩を76歩へ移動
    m = move_from_string(pos,cmd);
	from = square_from_string(cmd.substr(0, 2));
	to = square_from_string(cmd.substr(2, 2));
	pt = PieceType(pos.get_board(from) & 0x0F);
	am = make_move(from, to, 0, pt, EmptyPiece);
    EXPECT_EQ(am,m);
	cmd = "5a6b";   //51王を62王へ移動
    m = move_from_string(pos,cmd);
	from = square_from_string(cmd.substr(0, 2));
	to = square_from_string(cmd.substr(2, 2));
	pt = PieceType(pos.get_board(from) & 0x0F);
	am = make_move(from, to, 0, pt, EmptyPiece);
	EXPECT_EQ(am, m);
    cmd = "9g9c+"; //取る手,成るテスト
	m = move_from_string(pos, cmd);
	from = square_from_string(cmd.substr(0, 2));
	to = square_from_string(cmd.substr(2, 2));
	pt = PieceType(pos.get_board(from) & 0x0F);
	am = make_move(from, to, 1, pt, PieceType(pos.get_board(to) & 0x0F));
	//打つテスト
    cmd = "P*9d";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Pawn), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
	cmd = "L*8e";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Lance), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
	cmd = "N*7f";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Night), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
	cmd = "S*6d";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Silver), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
	cmd = "B*5e";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Bishop), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
	cmd = "R*4f";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Rook), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
	cmd = "G*3d";
	m = move_from_string(pos, cmd);
	to = square_from_string(cmd.substr(2, 2));
	am = make_move(drop_piece_from(Gold), to, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(am, m);
}
TEST(move,square_from_string)
{
	using Movens::square_from_string;

    EXPECT_EQ(A9,square_from_string(string("9a")));
    EXPECT_EQ(B8,square_from_string(string("8b")));
    EXPECT_EQ(C7,square_from_string(string("7c")));
    EXPECT_EQ(D6,square_from_string(string("6d")));
    EXPECT_EQ(E5,square_from_string(string("5e")));
    EXPECT_EQ(F4,square_from_string(string("4f")));
    EXPECT_EQ(G3,square_from_string(string("3g")));
    EXPECT_EQ(H2,square_from_string(string("2h")));
    EXPECT_EQ(I1,square_from_string(string("1i")));
}

TEST(move,string_from_square)
{
    EXPECT_STREQ("1i",string_from_square(I1).c_str());
    EXPECT_STREQ("2h",string_from_square(H2).c_str());
    EXPECT_STREQ("3g",string_from_square(G3).c_str());
    EXPECT_STREQ("4f",string_from_square(F4).c_str());
    EXPECT_STREQ("5e",string_from_square(E5).c_str());
    EXPECT_STREQ("6d",string_from_square(D6).c_str());
    EXPECT_STREQ("7c",string_from_square(C7).c_str());
    EXPECT_STREQ("8b",string_from_square(B8).c_str());
    EXPECT_STREQ("9a",string_from_square(A9).c_str());
}
// xxxxxxxxxxxxxxxxxxxxxxxxx1111111 移動先(7bit=127) to
// xxxxxxxxxxxxxxxxxx1111111xxxxxxx 移動元(7bit=127) from 駒打ちのときはPieceType + SquareNum - 1 (81->87)
// xxxxxxxxxxxxxxxxx1xxxxxxxxxxxxxx １なら成 pmoto
// xxxxxxxxxxxx1111xxxxxxxxxxxxxxxx 移動する駒の駒種(4bit=15) piece
// xxxxxxxx1111xxxxxxxxxxxxxxxxxxxx 獲られた駒の駒種(4bit=15) cap_piece
TEST(move, make_move)
{
	Move m = make_move(B1, C3, 0, Night, EmptyPiece);
	EXPECT_EQ(B1, move_from(m));
	EXPECT_EQ(C3, move_to(m));
	EXPECT_EQ(Night, move_piece(m));
	EXPECT_EQ(EmptyPiece, move_cap_piece(m));
	EXPECT_FALSE(is_pmoto(m));
	EXPECT_FALSE(is_drop(m));
	EXPECT_FALSE(is_capture(m));

	m = make_move(A9, A5, 0, Lance, EmptyPiece);
	EXPECT_EQ(A9, move_from(m));
	EXPECT_EQ(A5, move_to(m));
	EXPECT_EQ(Lance, move_piece(m)); 
	EXPECT_EQ(EmptyPiece, move_cap_piece(m));
	EXPECT_FALSE(is_pmoto(m));
	EXPECT_FALSE(is_drop(m));
	EXPECT_FALSE(is_capture(m));

	m = make_move(drop_piece_from(Pawn), E5, 0, EmptyPiece, EmptyPiece);
	EXPECT_EQ(Pawn, drop_piece(m));
	EXPECT_EQ(E5, move_to(m));
	EXPECT_EQ(EmptyPiece, move_piece(m));	//この行に意味はない、確認だけ
	EXPECT_EQ(EmptyPiece, move_cap_piece(m));	//この行に意味はない、確認だけ
	EXPECT_FALSE(is_pmoto(m));
	EXPECT_TRUE(is_drop(m));
	EXPECT_FALSE(is_capture(m));

	m = make_move(F4, E3, 1, Silver, Pawn);
	EXPECT_EQ(F4, move_from(m));
	EXPECT_EQ(E3, move_to(m));
	EXPECT_EQ(Silver, move_piece(m));
	EXPECT_EQ(Pawn, move_cap_piece(m));
	EXPECT_TRUE(is_pmoto(m));
	EXPECT_FALSE(is_drop(m));
	EXPECT_TRUE(is_capture(m));

	m = make_move(B8, B1, 1, Rook, EmptyPiece);
	EXPECT_EQ(B8, move_from(m));
	EXPECT_EQ(B1, move_to(m));
	EXPECT_EQ(Rook, move_piece(m));
	EXPECT_EQ(EmptyPiece, move_cap_piece(m));
	EXPECT_TRUE(is_pmoto(m));
	EXPECT_FALSE(is_drop(m));
	EXPECT_FALSE(is_capture(m));
}
TEST(move, is_pmoto)
{
	Color us = Black;
	Move m = make_move(B8, B1, 1, Rook, EmptyPiece);
	EXPECT_EQ(1,is_pmoto(m));
	PieceType pt_from = move_piece(m);
	EXPECT_EQ(BDragon,(us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Bishop, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(BHorse, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Silver, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(BProSilver, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Night, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(BProNight, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Lance, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(BProLance, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Pawn, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(BProPawn, (us << 4) | pt_from | (is_pmoto(m) << 3));

	us = White;
	m = make_move(B8, B1, 1, Rook, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(WDragon, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Bishop, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(WHorse, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Silver, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(WProSilver, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Night, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(WProNight, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Lance, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(WProLance, (us << 4) | pt_from | (is_pmoto(m) << 3));

	m = make_move(B8, B1, 1, Pawn, EmptyPiece);
	EXPECT_EQ(1, is_pmoto(m));
	pt_from = move_piece(m);
	EXPECT_EQ(WProPawn, (us << 4) | pt_from | (is_pmoto(m) << 3));
}
TEST(move, move_pmoto)
{
	Move m = B2 | G2 << 7 | 1 << 14 | Rook << 16 | Pawn << 20;
	EXPECT_TRUE(is_pmoto(m));
	Move m1 = B8 | E5 << 7 | Bishop << 16;
	EXPECT_FALSE(is_pmoto(m1));
}
TEST(move, move_cap_piece)
{
	Move m = B2 | G2 << 7 | 1 << 14 | Rook << 16 | Pawn << 20;
	EXPECT_EQ(Pawn, move_cap_piece(m));
}
TEST(move, move_piece)
{
	Move m = B8 | E5 << 7 | Bishop << 16;
	EXPECT_EQ(Bishop, move_piece(m));
}
TEST(move, move_to)
{
	Move m = B8 | E5 << 7 | Bishop << 16;
	EXPECT_EQ(B8, move_to(m));
}
TEST(move,move_from)
{
    Move m = B8 | E5 << 7 | Bishop << 16;
    EXPECT_EQ(E5,move_from(m));
}
#endif