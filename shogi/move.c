
#include <stdlib.h>
#include <stdbool.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "position.h"
#include "move.h"

/*
gshogiのmove
	指し手の表記
	駒の移動元の位置と移動先の位置を並べて書きます。７七の駒が７六に移動したのであれば、7g7fと表記します。
	駒が成るときは、最後に+を追加します。８八の駒が２二に移動して成るなら8h2b+と表記します。
	持ち駒を打つときは、最初に駒の種類を大文字で書き、それに*を追加し、さらに打った場所を追加します。金を５二に打つ場合はG*5bとなります

	//usi.cのhandlePositionからのみ呼ばれている。shen文字列から局面を更新するために必要。Move構造体を返しているが使われていない
	PushUSI(u string) 
		m = NewMoveFromUSI(u)	usiでの指手文字列をMove構造体に変換する（盤上移動手、駒打ち手）
		b.Push(m)	Move構造体を受け取って局面を更新する
		return m
	Push(m Move) 
		MoveNumbaerをインクリメントする
		MoveStackにMove構造体を追加する
		指し手がないという手がある、この場合はターンを切り替えて終了する（将棋にはパスという概念はないはず）
		移動先座標に敵の駒がいればCapturedPieceStackにPieceTypeを追加する
		指し手文字列により移動手なのか打ち手なのかを判定する
			打ち手ならRemovePieceFromHand()を呼んでPiecesInHand[color][PieceType]を減らす
			その駒を指定された座標に置く
		
			移動手なら移動元座標のPieceTypeを取り出す。
			移動先座標に駒があったなら、成り駒ならPIECE_PROMOTED_REVERSE[]配列で元の駒種に戻してPiecesInHand[color][PieceType]をプラス１する
			移動元座標にnilを設定
		ここからは移動手、駒打ち手共通の処理
			移動先座標に駒を置く
		ターンを切り替える

	Pop()
		引数はない。*Moveを返す関数である。この関数はIsSuicideOrCheckByDroppingPawn関数からのみ呼び出されている
		MoveStackから最後の要素を取り出す(Pushで追加されたもの)
		もしMoveStackが空ならnilを返す
		CapturedPieceStack[]配列からcapturedPieceTypeを取り出す。Pushで追加されたもの
		MoveNumbaerをデクリメントする
		もし指し手がnilならreturn
		移動先座標にあるPieceTypeを取り出す。Moveの成のフラグが立っているならPieceTypeを元の駒種に戻す
		Move構造体のFromSquareがnilであれば、それは打ち手であることを意味する。AddPieceIntoHand関数でPiecesInHand[color][pieceType]をインクリメントする
		打つ手でなければそれは移動する手であるのでMove構造体から移動元座標を取り出して、駒を置く
		ここからは打つ手、移動する手共通の処理
		capturedPieceTypeがなしであれば（駒取りではなかった）移動先座標はnilを設定する
		駒取り手であれば、PiecesInHand[color][PieceType]をデクリメントする
		移動先座標に取られた駒を置く
		ターンを切り替える
		Move構造体のポインタを返す

	IsSuicideOrCheckByDroppingPawn()	なにか駒の手の中で、歩を打つ手が自殺手か王手になるかを判定する関数
		LegalMoves関数（これは合法手を生成する関数）から呼ばれている。
		PseudoLegalMoves関数が生成したPseudoLegalMoves[]配列の中で、歩を打つ手が自殺手か王手になるかを判定する関数

	LegalMoves関数 engine.cのNext関数から呼ばれている。

	PseudoLegalMoves関数 LegalMoves関数から呼ばれている。PseudoLegalMoves[]配列を生成する関数である。
		PseudoLegalMoves[]配列の中には自殺手も含まれる。？？？
		boardをスキャンして、座標に駒があり、その駒が自分の駒であればAttacksFrom関数を呼び出す
		移動手はAttacksFrom関数から探索する
		AttacksFrom関数ではカラーごと、駒種ごとに手を生成している、細かなチエックはしていないので
		完全な合法手ではない。
		チエックは、自駒は取らない、
		CanMoveWithoutpromototion(s2,pt,truen)は死駒ではないことを確認（下段のpawn,lance,knight）
		CanPromotoは成れる駒かどうかをチエックしている。成れる駒はリストに追加する
		打つ駒は盤上の空き座標を探索して、そこに駒台にある駒を当てはめていく
		打駒のチエックは２歩ではないこと、死駒ではないことをチエックしている
		配下にある関数はPseudoLegalMoves関数から呼ばれている
			AttacksFrom()
			AttacksForm関数はカラーで分けられていて大きな関数、280行ある
			次は駒種で分けられていてswitch文で切分けて行く
				attacksBGold()
				attacksBishop()
				attacksRook()
				attacksPromBishop()
				attacksPromRook()
				attacksWGold()
				fileIndex()
				rankIndex()
			CanMovewithoutpromototion()
			CanPromoto()
			isDoublePawn()
		指し手を生成する関数群がかなりのボリュームがある


	gshogiのアルゴリズムをみると打つ手なのか移動する手なのかを判定するのに、いろいろ考えて判定しているが手を生成するとき
	どっちなのかフラグを立てておけば、判定する必要はないのではないかと思う。Move構造体にフラグを追加すればよいのではないか。
	 xxxxxxxx xxxxxxxx 1xxxxxxx xxxxxxxx morh: main board or hand board
	 このアイデアいかがでしょうか(TODO:)

	 YaneuraOU-9.00のメモ
	 F:\yaneuraOU/yaneuraOU-9.00/yaneuraOU.sln
								source/ *.cpp,*.hpp
								build/
									Deep-ORT-CPU/
										YaneuraOU.exe
										book/
											最初ここにはuserbook1.dbがあったがyaneuraOU9.00を動かすとエラーがでる（standard_book.dbがないというエラー）
											そこでuserbook1.dbをリネームしたところプログラムからエラーが出なくなった
										eval/
											最初ここにはnn.binがあったがyaneuraOU9.00を動かすとエラーがでる（model.onnxファイルがない）
											ファイルを探したら出てきたので、eval/においた
	リリース版はbuild/Deep-ORT-CPUフォルダにできる

	リリースモデルはこれで動くがDebugモデルではbook,evalファイルがないとエラーがでる。book,evalファイルをいろいろ動かしても改善されないので
	bookはno-book、evalファイルもパスする

	book
	book.cppのread_book(filename,on_the_fly)のなかにファイルを読み込む処理がある。
	pure_file変数が定跡なしでプログラムを動かすかどうかを決める変数(253行) ->コンパイルOK

	eval
	FukauraEngin.cppのinit_gpu()121行にevalファイルを読み込む処理がある。これをパスすると後工程でエラーがでたので
	model_pathに直Pathを書いて実行させた
	auto model_path = "F:\\yaneuraOu\\YaneuraOu-9.00\\build\\Deep-ORT-CPU\\eval\\model.onnx";  //Path::Combine(eval_dir, model_name);
 
	usi
	isready
 */

initial[8] = { ' ', 'P', 'L', 'N', 'S', 'B', 'R', 'G' };

//GUIから思考開始に対応する
int go(usi_handler_t* hd) {
	int best_move = 0;
	hd->ml = legel_move(hd,hd->ml);
	srand((unsigned)time(NULL));
	int len = arrlen(hd->ml);
	best_move = hd->ml[rand() % len];
	arrfree(hd->ml);	
	return best_move;
}

//合法手（簡略版）生成
int* legel_move(usi_handler_t* hd,int* ml) {
	int* pml = NULL;
	pml = possible_moves(hd,pml);
	/*for (int i = 0; i < arrlen(pml); i++) {
		if (Is_suicide_or_check_by_dropping_pawn(hd,pml[i])) {
			arrput(ml, pml[i]);
		}
	}*/
	ml = pml;	//一時的に合法手のチエックをオミットしてpmlをそのままmlとする。将来変更する
	return ml;
}

//ポーンを前進させての自殺手をチエックする
//gshogiのこのへんのコードはTODOになっているので完成形ではないと思われるのでオミットする
/*_Bool Is_suicide_or_check_by_dropping_pawn(usi_handler_t* hd,int m) {
	do_move(hd, m);
	//_Bool s = was_suicide();
	_Bool d = was_check_by_dropping_pawn(m);
	undo_move(hd, m);
	return  d;
}*/

//自殺手である?
/*_Bool was_suicide(void) {
	return is_attacked_by(black, wking);
}*/

//gshogiのコードが理解できないのでこの関数はオミット
/*_Bool is_attacked_by(usi_handler_t* hd,int c, int sq) {
	board_t* bd = hd->bd;
	int* sqml;
	int s = c ? 0 : 16;	//blackならpcはwhite側の駒を選択する
	for (int p = pawn + s; p <= dragon + s; p++) {
		sqml = attacks_from(hd, p, sq, hd->bd->turn ^ 1);
		for (int to = 0; to < arrlen(sqml); to++) {
			int pc = bd->mb[to];
			if (pc != empty && pc == p && 1) {
				return true;
			}
		}
	}
	return false;
}*/

//pawnを打つ手であればtrueを返す？
/*int was_check_by_dropping_pawn(int m) {

}*/

//盤上、駒台をスキャンして着手可能手をリストアップする。ここでは合法手であるかのチエックは少ない。move_list関数で絞り込む
int* possible_moves(usi_handler_t* hd, int* ml) {
	board_t* bd = hd->bd;
	int* sqml = NULL;
	int* empty_square = NULL;
	int move = 0;
	for (int from = sq1a; from <= sq9i; from++) {
		int pc = bd->mb[from];
		if (pc != 0 && ((pc & 0x10) >> 4) == bd->turn) {
			sqml = attacks_from(hd, pc, from, bd->turn);
			for (int i = 0; i < arrlen(sqml);i++ ) {
				int to = sqml[i];
				if (bd->mb[to] != 0 && ((bd->mb[to] & 0x10) >> 4) == bd->turn){	//toで移動先の駒が自陣側の駒だったパス
					continue;
				}
				if (canmove_without_promotion(to,pc,bd->turn)) {	//不成の移動
					move = bd->mb[to] << 24 | bd->mb[from] << 16 | from << 7 | to;
					arrput(ml, move);
				}
				if (can_promote(from,pc)) {	//成る移動
					move = bd->mb[to] << 24 | bd->mb[from] << 16 | 1 << 14 | from << 7 | to;
					arrput(ml, move);
				}
			}
		}
		else if(pc == 0) {
			arrput(empty_square, from);
		}
	}
	for (int pt = bpawn; pt <= bgold; pt++) {	//white側の駒台はパス
		if (hd->bd->hb[pt] == 0) {
			continue;
		}
		for (int i = 0; i < arrlen(empty_square); i++) {
			int to = empty_square[i];
			if (canmove_without_promotion(to, pt, black) && !is_double_pawn(hd, to, pt)) {
				move = (1 << 15) | (pt << 7) | to;
				arrput(ml, move);
			}
		}
	}
	return ml;
}

//2歩であればtrueを返す
_Bool is_double_pawn(usi_handler_t* hd,int sq, int pc) {
	if (pc != bpawn && pc != wpawn) {
		return false;
	}
	board_t* bt = hd->bd;
	int f = set_file(sq);
	for (int r = set_rank(sq + 1); r <= rank9; r += 1) {
		int s = set_square(f, r);
		int p = bt->mb[s];
		if (p != 0 && p == pc) {
			return true;
		}
	}
	for (int r = set_rank(sq - 1); r >= rank1; r -= 1) {
		int s = set_square(f, r);
		int p = bt->mb[s];
		if (p != 0 && p == pc) {
			return true;
		}
	}
	return false;
}

//pawn,lance.knightが死駒にならないならtrueを返す 
_Bool canmove_without_promotion(int to, int pt, int c) {
	int r = set_rank(to);
	if (c == black) {
		return
			(pt != bpawn && pt != blance && pt != bknight) ||
			(pt == bpawn && r > rank1) ||
			(pt == blance && r > rank1) ||
			(pt == bknight && r > rank2);
	}
	else {
		return
			(pt != wpawn && pt != wlance && pt != wknight) ||
			(pt == wpawn && r < rank9) ||
			(pt == wlance && r < rank9) ||
			(pt == wknight && r < rank8);
	}
}

//成駒になる可能性のある着手ならtrue この関数はsqがto座標のみで成判定をしているので間違いであるがとりあえずはこれでOK(TODO:)
_Bool can_promote(int sq,int pt) {
	int r = set_rank(sq);
	if (pt >= bpawn && pt <= brook) {
		return r <= rank3;
	}
	else if(pt>=wpawn && pt<=wrook){
		return r >= rank7;
	}
	return false;
}

//盤上での着手可能手をリストアップ
int* attacks_from(usi_handler_t* hd, int pc, int sq, int turn) {
	board_t* bd = hd->bd;
	int* m = NULL;
	int to = 0;
	int f, r;
	switch (pc) {
	case bpawn:
		to = sq - 1;
		arrput(m, to);
		return m;
	case blance:
		f = set_file(sq);
		r = set_rank(sq) - 1;
		for (; r >= rank1; r -= 1) {
			int to = set_square(f, r);
			int topc = bd->mb[to];
			if (topc == 0) {
				arrput(m, to);
				continue;
			}
			if (((topc & 0x10) >> 4) != turn) {
				arrput(m, to);
			}
			break;
		}
		return m;
	case bknight:
		/*knightのサーチ
		1   2  +7   -11

		  s        s
		*/
		f = set_file(sq);
		r = set_rank(sq);
		switch (f) {
		case file1:
			to = sq + 7;
			arrput(m, to);
			return m;
		case file9:
			to = sq - 11;
			arrput(m, to);
			return m;
		default:
			to = sq + 7;
			arrput(m, to);
			to = sq - 11;
			arrput(m, to);
			return m;
		}
	case bsilver:
		return attacks_bsilver(sq, m);
	case bbishop:
		return attacks_bbishop(hd,sq, m);
	case brook:
		return attacks_brook(hd, sq, m);
	case bgold:
		return attacks_bgold(sq, m);
	case bking:
		return attaks_bking(sq, m);
	case bpropawn:
		return attacks_bgold(sq, m);
	case bprolance:
		return attacks_bgold(sq, m);
	case bproknight:
		return attacks_bgold(sq, m);
	case bprosilver:
		return attacks_bgold(sq, m);
	case bhorse:
		return attacks_bhorse(hd, sq, m);
	case bdragon:
		return attacks_bdragon(hd, sq, m);
	//white ,blackのbold,propawn,prolance,proknight,prosilver,bishop,rook,house,dragon,kingは今はパス
	}
}

int* attacks_bsilver(int sq,int* m) {
	/*silverのサーチ sはsqの略
	1 2 3    +8 -1 -10
	  s          s  
	4   5    +10    -8
	*/ 
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		return m;
	}
}

//black bishop
int* attacks_bbishop(usi_handler_t* hd, int sq, int* m) {
	/*bishopのサーチ sはsqの略
	  1 2  +8    -10
	   s       s
	  3 4  +10   -8
	*/
	//upper left
	for (int to = sq + 8;; to += 8) {
		if (set_file(to - 8) == file9 || set_rank(to - 8) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//upper right
	for (int to = sq - 10;; to -= 10) {
		if (set_file(to + 10) == file1 || set_rank(to + 10)==rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down left
	for (int to = sq + 10;; to += 10) {
		if (set_file(to - 10) == file9 || set_rank(to - 10)==rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down right
	for (int to = sq - 8;; to -= 8) {
		if (set_file(to + 8) == file1 || set_rank(to + 8) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	return m;
}

//black rook
int* attacks_brook(usi_handler_t* hd, int sq, int* m) {
	/*bishopのサーチ sはsqの略
	    1         -1    
	 2  s  3   +9  s  -9
	    4         +1   
	*/
	//upper
	for (int to = sq - 1;; to -= 1) {
		if (set_rank(to + 1) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//left
	for (int to = sq + 9;; to += 9) {
		if (set_file(to - 9) == file9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//right
	for (int to = sq - 9;; to -= 9) {
		if (set_file(to + 9) == file1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down
	for (int to = sq + 1;; to += 1) {
		if (set_rank(to - 1) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	return m;
}

int* attacks_bgold(int sq, int* m) {
	/*goldのサーチ sはsqの略
	1 2 3     +8 -1 -10 
	4 s 5     +9  s  -9
	  6          +1
	*/
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
		}
		return m;
	}
}

//black king
int* attaks_bking(int sq, int* m) {
	/*kingのサーチ sはsqの略
	1 2 3     +8 -1 -10
	4 s 5     +9  s  -9
	6 7 8     +10 +1 -8
	*/
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 1;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		return m;
	}
}

//black horse
int* attacks_bhorse(usi_handler_t* hd, int sq, int* m) {
	/*horseのサーチ sはsqの略
	1   2     +8    -10
	  s           s    
	3   4     +10    -8

	+

  	  5          -1
	6 s 7     +9  s  -9
	  8          +1
	*/
	//upper left
	for (int to = sq + 8;; to += 8) {
		if (set_file(to - 8) == file9 || set_rank(to - 8) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//upper right
	for (int to = sq - 10;; to -= 10) {
		if (set_file(to + 10) == file1 || set_rank(to + 10) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down left
	for (int to = sq + 10;; to += 10) {
		if (set_file(to - 10) == file9 || set_rank(to - 10) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down right
	for (int to = sq - 8;; to -= 8) {
		if (set_file(to + 8) == file1 || set_rank(to + 8) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
		}
		else {
			to = sq - 1;
			arrput(m, to);
			to = sq + 9;
			arrput(m, to);
			to = sq - 9;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
		}
		return m;
	}
}

//dragon black
int* attacks_bdragon(usi_handler_t* hd,int sq, int* m) {
	/*dragonのサーチ sはsqの略
	  1          -1    
	2 s 3     +9  s  -9
	  4          +1

	+

	5   6    +8     -10
	  s          s  
	7   8    +10    -8
	*/
	//upper
	for (int to = sq - 1;; to -= 1) {
		if (set_rank(to + 1) == rank1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//left
	for (int to = sq + 9;; to += 9) {
		if (set_file(to - 9) == file9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//right
	for (int to = sq - 9;; to -= 9) {
		if (set_file(to + 9) == file1) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	//down
	for (int to = sq + 1;; to += 1) {
		if (set_rank(to - 1) == rank9) {
			break;
		}
		int pc = hd->bd->mb[to];
		if (pc == empty) {
			arrput(m, to);
			continue;
		}
		if (((pc & 0x10) >> 4) != hd->bd->turn) {
			arrput(m, to);
		}
		break;
	}
	int to = 0;
	int f, r;
	f = set_file(sq);
	r = set_rank(sq);
	switch (f) {
	case file1:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			return m;
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq + 1;
			arrput(m, to);
			return m;
		}
		break;
	case file9:
		if (r == rank1) {
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		else if (r == rank9) {
			to = sq - 10;
			arrput(m, to);
			return m;
		}
		else {
			to = sq - 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
			return m;
		}
		break;
	default:
		if (r == rank1) {
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		else if (r == rank9) {
			to = sq + 8;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
		}
		else {
			to = sq + 8;
			arrput(m, to);
			to = sq - 10;
			arrput(m, to);
			to = sq + 10;
			arrput(m, to);
			to = sq - 8;
			arrput(m, to);
		}
		return m;
	}
}

//usimove形式の打手の駒種を意味する大文字１文字を駒種に変換する
int initial_to_piecetype(char p) {
	for (int pt = 1; pt <= 7; pt++) {
		if (initial[pt] == p) {
			return pt;
		}
	}
	fprintf(stderr, "[ERROR] %s:%d: Array out of bounds\n", __FILE__, __LINE__);
	exit(0);
}

//usimove形式の文字列を受け取って局面を更新する。ここでは文字列をmve形式に変換して実際に局面を更新するのはdo_move関数
void do_usi_move(usi_handler_t* hd, char* usi_move) {
	board_t* bd = hd->bd;
	int move = 0;
	if (strlen(usi_move) == 4) {
		if (usi_move[1] == '*') {	//打つ手
			if (bd->turn) {
				move = (1 << 15) | ((initial_to_piecetype(usi_move[0]) + 16) << 7) | usisq_to_movesq(usi_move+2);	//morh:<<15,from(piece):<<7,to 
			}
			else {
				move = (1 << 15) | ((initial_to_piecetype(usi_move[0])) << 7) | usisq_to_movesq(usi_move+2);	//morh:<<15,from(piece)<<7,to
			}
		}
		else {	//移動手（不成）
			int from = usisq_to_movesq(usi_move);
			int to = usisq_to_movesq(usi_move+2);
			move = (bd->mb[to]) << 24 | bd->mb[from] << 16 | from << 7 | to;	//cap:<<24,piece:<<16,from:<<7,to
		}
	}
	else {	//成る手
		int from = usisq_to_movesq(usi_move);
		int to = usisq_to_movesq(usi_move+2);
		move = (bd->mb[to]) << 24 | bd->mb[from] << 16 | 1 << 14 | from << 7 | to;	//cap:<<24,piece:<<16,pmoto:1<<14,from:<<7,to
	}
	do_move(hd, move);
}

void do_move(usi_handler_t* hd, const int move) {
	board_t* bd = hd->bd;
	bd->move_number++;
	int us = bd->turn;
	int to, from, pmoto, morh, piece, cap;
	get_move(move, &to, &from, &pmoto, &morh, &piece, &cap);
	if (morh) {
		bd->hb[from]--;		//打つ手の時は駒番はfromに入る
		bd->mb[to] = from;
		//ここに駒打ちの時、詰めがかかるかのフラグ(moveIsCheck)による処理が入る(TODO:)

	}
	else {
		if (cap != empty) {
			cap = strip_pmoto(cap) ^ 0x10;	//取った駒のカラー切替え、不成への切替
			bd->hb[cap]++;
		}
		//ここにkingSquare配列に関する処理が入るが今はパス(TODO:)
		//ここに詰めが掛かってくる場合の処理がはいるがいまはパス(TODO:)
		if (pmoto) {
			piece = piece + 8;
		}
		bd->mb[from] = empty;
		bd->mb[to] = piece;
	}
	bd->turn = opposite_color(us);
	return;
}

void undo_move(usi_handler_t* hd,const int move) {
	board_t* bd = hd->bd;
	int us = bd->turn;
	int to, from, pmoto, morh, piece, cap;
	get_move(move, &to, &from, &pmoto, &morh, &piece, &cap);
	if (morh) {	//打ち手
		//bitboard関係の操作が入るが今はパス(TODO:)
		bd->mb[to] = empty;
		bd->hb[from]++;			//打つ手の時は駒番はfromに入る
	}
	else {
		//king専用の配列の操作がある、今はパス(TODO:)
		if (cap) {
			//bitboard関係の操作が入るが今はパス(TODO:)
			bd->mb[to] = cap;	//capには取られた駒の情報がそのまま入っている（color,pmoto情報がそのまま）
			cap = strip_pmoto(cap) ^ 0x10;	//ここで成情報を除去、カラーを敵側に合わせて駒台に載せる
			bd->hb[cap]--;
		}
		else {
			bd->mb[to] = empty;
		}
		bd->mb[from] = piece;
	}
	//ここでbitboardの操作かある、今はパス(TODO:)
	bd->turn = opposite_color(us);
	bd->move_number--;
}

void get_move(const int move,int* to,int* from,int* ispmoto,int* morh,int* piece,int* cap) {
	*to = move & 0x7f;
	*from = (move >> 7) & 0x7f;
	*ispmoto = (move >> 14) & 0x01;
	*morh = (move >> 15) & 0x01;
	*piece = (move >> 16) & 0x1f;
	*cap = (move >> 24) & 0x1f;
}

int strip_pmoto(int pc) {
	if (pc >= 9 && pc <= 14) {
		return pc - 8;
	}
	else if (pc >= 25 && pc <= 30) {
		return pc - 8;
	}
	else {
		return pc;
	}
}

//moveをsfen形式に変換する
void move_to_usi(int mv, char* usi_move) {
	int from, to, ispmoto, morh, piece, cap;
	get_move(mv, &to, &from, &ispmoto, &morh, &piece, &cap);
	if (morh){	//打手
		usi_move[0] = initial[from & 0xf];
		usi_move[1] = '*';
		movesq_to_usisq(to, usi_move + 2);
	}
	else {
		movesq_to_usisq(from, usi_move);
		movesq_to_usisq(to, usi_move + 2);
		if (ispmoto) {
			usi_move[4] = '+';
			usi_move[5] = '\0';
		}
	}
}
