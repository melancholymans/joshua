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
			移動先座標駒があったなら、成り駒ならPIECE_PROMOTED_REVERSE[]配列で元の駒種に戻してPiecesInHand[color][PieceType]をプラス１する
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
 */