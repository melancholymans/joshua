
#include "position.h"
#include "move.h"
/*
 xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx 32bit
 xxxxxxxx xxxxxxxx xxxxxxxx x1111111 to: 移動元座標
 xxxxxxxx xxxxxxxx xx111111 1xxxxxxx from: 移動先座標 駒打ちの時はpieceType + squareNumber - 1 <-この駒打ちの時のフォーマットは謎(TODO:)
 xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx pmoto: 成:1 不成:0
 xxxxxxxx xxxx1111 xxxxxxxx xxxxxxxx 移動する駒のpieceType,駒打ちの時は使用しない
 xxxxxxxx 1111xxxx xxxxxxxx xxxxxxxx 取った駒の(captured piece)
 promotoFlag = 1 << 14 不要
 to = pd & 0x7f
 from = (pd >> 7) & 0x7f
 ispromoto = (pd >> 14) & 0x01
 piece = (pd >> 16) & 0x0f	//移動する駒の種類を返す（駒種になる、駒番ではない、駒番がほしければ (pd >> 16) & 0x1f）
 cap = (pd >> 20) & 0x0f	//取った駒の種類を返す（駒種になる、駒番ではない、駒番がほしければ (pd >> 20) & 0x1f）

 このファイルには合法手の生成関数も置く
*/
/*
 新しいフォーマット
 xxxxxxxx xxxxxxxx xxxxxxxx x1111111 to: 移動元座標
 xxxxxxxx xxxxxxxx xx111111 1xxxxxxx from: 移動先座標 駒打ちの時はpiece(駒番)
 xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx pmoto: 成:1 不成:0,駒打ちの時は0で決め打ち
 xxxxxxxx xxxxxxxx 1xxxxxxx xxxxxxxx morh: main board(0) or hand board(1)
 xxxxxxxx xxx11111 xxxxxxxx xxxxxxxx 移動する駒のpiece(駒番),駒打ちの時は使用しない
 xxx11111 xxxxxxxx xxxxxxxx xxxxxxxx 取った駒の(captured piece)駒番,そのままの駒番を保存colorの切替pmotoの削除は登録の時はしない,駒打ちの時は使用しない
 to = pd & 0x7f
 from = (pd >> 7) & 0x7f
 ispromoto = (pd >> 14) & 0x01
 morh(main or hand) = pd >> 15 & 0x01
 piece = pd >> 16 & 0x1f
 cap piece pd >> 24 & 0x1f

*/
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
		移動手はAttacksFrom関数から探索する
		打つ駒は盤上の空き座標を探索して、そこに駒台にある駒を当てはめていく
		配下にある関数はPseudoLegalMoves関数から呼ばれている
			AttacksFrom()
				attacksBGold()
				attacksBishop()
				attacksRook()
				attacksPromBishop()
				attacksPromRook()
				attacksWGold()
				fileIndex()
				rankIndex()
			isDoublePawn()
		指し手を生成する関数群がかなりのボリュームがある


	gshogiのアルゴリズムをみると打つ手なのか移動する手なのかを判定するのに、いろいろ考えて判定しているが手を生成するとき
	どっちなのかフラグを立てておけば、判定する必要はないのではないかと思う。Move構造体にフラグを追加すればよいのではないか。
	 xxxxxxxx xxxxxxxx 1xxxxxxx xxxxxxxx morh: main board or hand board
	 このアイデアいかがでしょうか(TODO:)
 */

int usisq_to_movesq(char* usi_sq) {
	return (usi_sq[0] - '1') * 9 + (usi_sq[1] - 'a');
}

initial[8] = { ' ', 'P', 'L', 'N', 'S', 'B', 'R', 'G'};
int initial_to_piecetype(char p) {
	for (int pt = 1; pt <= 7; pt++) {
		if (initial[pt] == p) {
			return pt;
		}
	}
	fprintf(stderr, "[ERROR] %s:%d: Array out of bounds\n", __FILE__, __LINE__);
	exit(0);
}

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
			move = (bd->mb[to]/*^0x10*/) << 24 | bd->mb[from] << 16 | from << 7 | to;	//cap:<<24,piece:<<16,from:<<7,to
		}
	}
	else {	//成る手
		int from = usisq_to_movesq(usi_move);
		int to = usisq_to_movesq(usi_move+2);
		move = (bd->mb[to]/*^0x10*/) << 24 | bd->mb[from] << 16 | 1 << 14 | from << 7 | to;	//cap:<<24,piece:<<16,pmoto:1<<14,from:<<7,to
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