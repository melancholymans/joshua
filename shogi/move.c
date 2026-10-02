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
