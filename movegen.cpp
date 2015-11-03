#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "move.h"
#include "movepicker.h"
#include "movegen.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using BitBoardns::alloff;
using BitBoardns::IN_FRONT_MASK;

/*
Move *generate_king_moves_w(const Position &pos,Move *ml,int from)
{
    int to;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 8;i++){
        to = from + DIRECT_WHITE[KING][i];
        cp = pos.board[to];
        //KINGなので敵の利きのあるところにはいけないがまだそこまでのデータが揃ってないのでPASS
        if(cp > 1 || cp == 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_gold_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 6;i++){
        to = from + DIRECT_WHITE[GOLD][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}


Move *generate_lance_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 1;i++){
        //loopを回す必要がないが他のこまのためにこうしている
        do{
            to = to + DIRECT_WHITE[LANCE][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){ 
                pmoto = is_pmoto_w(to);
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_knight_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 2;i++){
        to = from + DIRECT_WHITE[KNIGHT][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない,さらに桂馬は成る必須のラインがあることを忘れないように
            pmoto = is_pmoto_w(to);
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_silver_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 5;i++){
        to = from + DIRECT_WHITE[SILVER][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない
            pmoto = is_pmoto_w(to);
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_bishop_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[BISHOP][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){
                pmoto = is_pmoto_w(to);
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_rook_moves_w(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[ROOK][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){
                pmoto = is_pmoto_w(to);
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_pbishop_moves_w(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[BISHOP][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(i = 0;i < 4;i++){
        to = from + DIRECT_WHITE[ROOK][i];
        cp = pos.board[to];
        if(cp == 0 || cp > 1){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_prook_moves_w(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_WHITE[ROOK][i];
            cp = pos.board[to];
            if(cp == 0 || cp > 1){
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(i = 0;i < 4;i++){
        to = from + DIRECT_WHITE[BISHOP][i];
        cp = pos.board[to];
        if(cp > 1 || cp == 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_gold_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_GOLD,0);
            }
        }
    }
    return ml;
}

Move *generate_pawn_drop_w(const Position &pos,Move *ml)
{
    //colごとに歩がいるかチエックしている、本当は常時局面を
    //点検しているのいいのかもしれないがとても大変そうなので
    //必要な時（drop_pawnの時）だけチエックする方法にする
    int dpc[10] = {0,0,0,0,0,0,0,0,0,0};

    for(int col = 1;col < 10;col++){
        for(int row = 1;row < 9;row++){
            int sq = make_square(col,row);
            if(pos.board[sq] == W_PAWN){
                dpc[col] = 1;
                break;
            }
        }
    }
    //筋に歩がすでにいるか判断する処理と、手を生成する
    //処理は同時にはできない
    //２歩判定ができていない、
    for(int col = 1;col < 10;col++){ 
        for(int row = 1;row < 9;row++){    //死駒対策
            int sq = make_square(col,row);
            if(dpc[col]){
                break;
            }
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_PAWN,0);
            }
        }
    }
    return ml;
}

Move *generate_lance_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 9;row++){ //死駒対策
        for(int col = 1;col < 10;col++){ 
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_LANCE,0);
            }
        }
    }
    return ml;
}

Move *generate_knight_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 8;row++){ //死駒対策
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_KNIGHT,0);
            }
        }
    }
    return ml;
}

Move *generate_silver_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_SILVER,0);
            }
        }
    }
    return ml;
}

Move *generate_bishop_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_BISHOP,0);
            }
        }
    }
    return ml;
}

Move *generate_rook_drop_w(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,W_ROOK,0);
            }
        }
    }
    return ml;
}

Move *generate_king_moves_b(const Position &pos,Move *ml,int from)
{
    int to;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 8;i++){
        to = from + DIRECT_BLACK[KING][i];
        cp = pos.board[to];
        //KINGなので敵の利きのあるところにはいけないがまだそこまでのデータが揃ってないのでPASS
        if(cp <= 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_gold_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto = 0;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 6;i++){
        to = from + DIRECT_BLACK[GOLD][i];
        cp = pos.board[to];
        if(cp <= 0){
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_lance_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 1;i++){
        do{
            to = to + DIRECT_BLACK[LANCE][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                pmoto = is_pmoto_b(to);
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_knight_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 2;i++){
        to = from + DIRECT_BLACK[KNIGHT][i];
        cp = pos.board[to];
        if(cp <= 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない,さらに桂馬は成る必須のラインがあることを忘れないように
            pmoto = is_pmoto_b(to);
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}

Move *generate_silver_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto;
    char p,cp;

    p = pos.board[from];
    for(int i = 0;i < 5;i++){
        to = from + DIRECT_BLACK[SILVER][i];
        cp = pos.board[to];
        if(cp <= 0){
            //本来は成るで１手、成らないで１手なので、この決め打ちはよくない
            pmoto = is_pmoto_b(to);
            *(ml++) = make_move(from,to,pmoto,p,cp);
        }
    }
    return ml;
}


Move *generate_bishop_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[BISHOP][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                pmoto = is_pmoto_b(to);
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}


Move *generate_rook_moves_b(const Position &pos,Move *ml,int from)
{
    int to,pmoto,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[ROOK][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                pmoto = is_pmoto_b(to);
                *(ml++) = make_move(from,to,pmoto,p,cp);
            }
        }while(cp == EMPTY);
    }
    return ml;
}

Move *generate_pbishop_moves_b(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[BISHOP][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(i = 0;i < 4;i++){
        to = from + DIRECT_BLACK[ROOK][i];
        cp = pos.board[to];
        if(cp <= 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_prook_moves_b(const Position &pos,Move *ml,int from)
{
    int to,i;
    char p,cp;

    p = pos.board[from];
    for(to = from,i = 0;i < 4;i++,to = from){
        do{
            to = to + DIRECT_BLACK[ROOK][i];
            cp = pos.board[to];
            if(cp <= 0){ 
                *(ml++) = make_move(from,to,0,p,cp);
            }
        }while(cp == EMPTY);
    }
    for(int i = 0;i < 4;i++){
        to = from + DIRECT_BLACK[BISHOP][i];
        cp = pos.board[to];
        if(cp <= 0){
            *(ml++) = make_move(from,to,0,p,cp);
        }
    }
    return ml;
}

Move *generate_gold_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_GOLD,0);
            }
        }
    }
    return ml;
}

Move *generate_pawn_drop_b(const Position &pos,Move *ml)
{
    //colごとに歩がいるかチエックしている、本当は常時局面を
    //点検しているのいいのかもしれないがとても大変そうなので
    //必要な時（drop_pawnの時）だけチエックする方法にする
    int dpc[10] = {0,0,0,0,0,0,0,0,0,0};

    for(int col = 1;col < 10;col++){
        for(int row = 2;row < 10;row++){
            int sq = make_square(col,row);
            if(pos.board[sq] == B_PAWN){
                dpc[col] = 1;
                break;
            }
        }
    }
    //筋に歩がすでにいるか判断する処理と、手を生成する
    //処理は同時にはできない
    for(int col = 1;col < 10;col++){ 
        for(int row = 2;row < 10;row++){    //死駒対策
            int sq = make_square(col,row);
            if(dpc[col]){
                break;
            }
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_PAWN,0);
            }
        }
    }
    return ml;
}

Move *generate_lance_drop_b(const Position &pos,Move *ml)
{
    for(int row = 2;row < 10;row++){ //死駒対策
        for(int col = 1;col < 10;col++){ 
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_LANCE,0);
            }
        }
    }
    return ml;
}

Move *generate_knight_drop_b(const Position &pos,Move *ml)
{
    for(int row = 3;row < 10;row++){ //死駒対策
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_KNIGHT,0);
            }
        }
    }
    return ml;
}

Move *generate_silver_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_SILVER,0);
            }
        }
    }
    return ml;
}

Move *generate_bishop_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_BISHOP,0);
            }
        }
    }
    return ml;
}

Move *generate_rook_drop_b(const Position &pos,Move *ml)
{
    for(int row = 1;row < 10;row++){
        for(int col = 1;col < 10;col++){
            int sq = make_square(col,row);
            if(pos.board[sq] == EMPTY){
                *(ml++) = make_move(0,sq,0,B_ROOK,0);
            }
        }
    }
    return ml;
}



//Move *generate_captures(const Position &pos,Move *ml)
//{
//    Move *m = NULL;
//    return m;
//}
//
//Move *generate_checks(const Position &pos,Move *ml)
//{
//    Move *m = NULL;
//    return m;
//}
//
//Move *generate_drop(const Position &pos,Move *ml)
//{
//    Move *m = NULL;
//    return m;
//}


bool array_check(Move anser,Move *m,int n);
*/
template <MoveType MT,Color US>
MoveStack* MoveGeneratens::generate_pawn_moves(MoveStack* ml, const Position &pos, BitBoard& tar, Square ksq)
{
	const Rank rank6 = US == Black ? Rank6 : Rank4;
	const BitBoard rank789_bb = IN_FRONT_MASK[US][rank6];
	const SquareDelta delta = US == Black ? DeltaS : DeltaN;
	Square to;

	BitBoard to_bb = pos.attackers_from_pawns(US, pos.color_type_of_bb(US, Pawn) & tar;
	//不成り
	{
		BitBoard on_123456_bb = to_bb & tar;
		while (on_123456_bb.p(0)){
			to = on_123456_bb.first_one_right();
			const from = to + delta;
			(*ml++).move = make_move(from, to, 0, type_of_piece(pos.get_board(from)), type_of_piece(pos.get_board(to)));
		}
		while (on_123456_bb.p(1)){
			to = on_123456_bb.first_one_left();
			const from = to + delta;
			(*ml++).move = make_move(from, to, 0, type_of_piece(pos.get_board(from)), type_of_piece(pos.get_board(to)));
		}
	}
	//成り
	if (MT == Promoto){
		BitBoard on_789_bb = to_bb & rank789_bb;
		if (!on_789_bb.is_not_zero()){
			return ml;
		}
		while (on_789_bb.p(0)){
			to = on_789_bb.first_one_right();
			const Square from = to + delta;
			(*ml++).move = make_move(from, to, 1, type_of_piece(pos.get_board(from)), type_of_piece(pos.get_board(to)));
			const Rank rank9 = US == Black Rank9 : Rank1;
			//通常歩は敵陣に入れば無条件でなるが、to座標が78ランクなら歩なりも生成する
			if (make_rank(to) != rank9){
				(*ml++).move = make_move(from, to, 0, type_of_piece(pos.get_board(from)), type_of_piece(pos.get_board(to)));
			}
		}
		while (on_789_bb.p(1)){
			to = on_789_bb.first_one_left();
			const Square from = to + delta;
			(*ml++).move = make_move(from, to, 1, type_of_piece(pos.get_board(from)), type_of_piece(pos.get_board(to)));
			const Rank rank9 = US == Black Rank9 : Rank1;
			//通常歩は敵陣に入れば無条件でなるが、to座標が78ランクなら歩なりも生成する
			if (make_rank(to) != rank9){
				(*ml++).move = make_move(from, to, 0, type_of_piece(pos.get_board(from)), type_of_piece(pos.get_board(to)));
			}
		}
	}
	return ml;
}
/*
template <Color US>
MoveStack* MoveGeneratens::generate_evasions(MoveStack* ml, const Position &pos)
{
	//今はなにもしない
	for (int i = 0; i < 8; i++){
	}
	return ml;
}
*/
/**/
template <MoveType MT,PieceType PT ,Color US>
MoveStack* MoveGeneratens::generate_moves(MoveStack* ml, const Position &pos)
{
	const Color us = US;
	const Color them = over_turn(US);
	const Square ksq = pos.get_king_square(them);
	const Rank rank6 = (us == Black) ? Rank6 : Rank4;	//them陣のひとつ手前のランク
	const Rank rank7 = (us == Black) ? Rank7 : Rank3;	//them陣の最上層ランク
	const Rank rank8 = (us == Black) ? Rank8 : Rank2;	//them陣の中間層ランク
	const BitBoard rank_789_bb = IN_FRONT_MASK[us][rank6];			//them陣全て
	const BitBoard rank_123456_bb = IN_FRONT_MASK[them][rank7];		//them陣以外
	const BitBoard rank_1234567_bb = IN_FRONT_MASK[them][rank8];	//us陣+中間+them陣最上層
	const BitBoard tar1 =
		(MT == Capture) ? pos.color_of_bb(them) :
		(MT == NonCapture) ? pos.inver_bit_bb() : allof;
	const BitBoard tar2 =
		(MT == Capture) ? tar1 :
		(MT == NonCapture) ? tar1 : alloff;
	const BitBoard tar3 =
		(MT == Capture) ? tar2 :
		(MT == NonCapture) ? tar2 : alloff;

	if (MT == Evasion){
		//自王に王手がかかっているなら王手回避手を生成して返す
		//generate_evasions<us>(pos, ml);
		return ml;
	}
	else if(MT != DROP){
		//王手回避手,打つ手以外(capture promoto noncapture recapture)
		ml = generate_pawn_moves<MT,us>(ml,pos, tar2,ksq);
		//ml = generate_lance_moves<us>(ml,pos, tar3,ksq);
		//ml = generate_knight_moves<us>(ml,pos, tar3,ksq);
		//ml = generate_silver_moves<us>(ml,pos, tar1,ksq);
		//ml = generate_bishop_moves<us>(ml,pos, tar2,ksq);
		//ml = generate_rook_moves<us>(ml,pos, tar2,ksq);
		//ml = generate_gold_moves<us>(ml,pos, tar1,ksq);
		//ml = generate_king_moves<us>(ml.pos,  tar1,ksq);
		//ml = generate_horse_moves<us>(ml,pos, tar1,ksq);
		//ml = generate_dragon_moves<us>(ml,pos, tar1,ksq);
	}
	//打つ手
	else{
		ml = generate_gold_drop<us>(ml,pos);
		ml = generate_pawn_drop<us>(ml, pos);
		ml = generate_lance_drop<us>(ml, pos);
		ml = generate_knight_drop<us>(ml, pos);
		ml = generate_silver_drop<us>(ml, pos);
		ml = generate_bishop_drop<us>(ml, pos);
		ml = generate_rook_drop<us>(ml, pos);
	}
	return ml;
}
/**/
/*
TEST(movegen,generate_moves)
{
    //指し手のテストは生成される手の数と生成された手そのものを比較する
    from_sfen(start_position);
    next_move[0].last_move = mlist;
    Move *m = generate_moves(root_position,mlist);
    int n = m - next_move[0].last_move;
    EXPECT_EQ(30,n);
    m = mlist;
    //pawn
    Move anser = make_move(SQ_9G,SQ_9F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8G,SQ_8F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7G,SQ_7F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6G,SQ_6F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5G,SQ_5F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4G,SQ_4F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3G,SQ_3F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2G,SQ_2F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1G,SQ_1F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance
    anser = make_move(SQ_9I,SQ_9H,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1I,SQ_1H,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver
    anser = make_move(SQ_7I,SQ_7H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7I,SQ_6H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3I,SQ_4H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3I,SQ_3H,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold
    anser = make_move(SQ_6I,SQ_7H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6I,SQ_6H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6I,SQ_5H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4I,SQ_5H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4I,SQ_4H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4I,SQ_3H,0,B_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //king
    anser = make_move(SQ_5I,SQ_6H,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5I,SQ_5H,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5I,SQ_4H,0,B_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //rook
    anser = make_move(SQ_2H,SQ_1H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_3H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_4H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_5H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_6H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2H,SQ_7H,0,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));

    //Q1の局面でテスト,BLACK側
    string expect ="lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b P5psr 1";
    from_sfen(expect);
    next_move[0].last_move = mlist;
    m = generate_moves(root_position,mlist);
    n = m - next_move[0].last_move;
    EXPECT_EQ(61,n);
    m = mlist;
    //pawn  5
    anser = make_move(SQ_9G,SQ_9F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8F,SQ_8E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3G,SQ_3F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1G,SQ_1F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2C,SQ_2B,1,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance 1
    anser = make_move(SQ_1I,SQ_1H,0,B_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //knight    1
    anser = make_move(SQ_8I,SQ_7G,0,B_KNIGHT,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver    5
    anser = make_move(SQ_4E,SQ_5D,0,B_SILVER,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_4D,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_3D,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_5F,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4E,SQ_3F,0,B_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold  7
    anser = make_move(SQ_8H,SQ_8G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8H,SQ_7G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_8G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_7G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_6G,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_6H,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7H,SQ_7I,0,B_GOLD,0);
    EXPECT_TRUE(array_check(anser,m,n));
    //bishop
    anser = make_move(SQ_6A,SQ_7B,1,B_BISHOP,W_GOLD);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6A,SQ_5B,1,B_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6A,SQ_4C,1,B_BISHOP,W_SILVER);
    EXPECT_TRUE(array_check(anser,m,n));
    //pbishop
    anser = make_move(SQ_5C,SQ_6B,0,BP_BISHOP,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_4B,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_3A,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_6D,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_7E,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_4D,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_3E,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_2F,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_5B,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_6C,0,BP_BISHOP,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_4C,0,BP_BISHOP,W_SILVER);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5C,SQ_5D,0,BP_BISHOP,W_PAWN);
    EXPECT_TRUE(array_check(anser,m,n));
    //rook  4
    anser = make_move(SQ_8A,SQ_7A,1,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8A,SQ_8B,1,B_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8A,SQ_8C,1,B_ROOK,W_GOLD);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8A,SQ_9A,1,B_ROOK,W_LANCE);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop pawn
    anser = make_move(0,SQ_7D,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7I,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6C,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6D,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6H,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6I,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5B,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5E,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5H,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5I,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,B_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4I,0,B_PAWN,EMPTY);
    ASSERT_TRUE(array_check(anser,m,n));

    //Q1の局面でテスト,WHITE側
    from_sfen(expect);
    root_position.turn = WHITE;
    next_move[0].last_move = mlist;
    m = generate_moves(root_position,mlist);
    n = m - next_move[0].last_move;
    EXPECT_EQ(133,n);
    m = mlist;
    //pawn  7
    anser = make_move(SQ_9E,SQ_9F,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7F,SQ_7G,1,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_6B,SQ_6C,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_5D,SQ_5E,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_3C,SQ_3D,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_2D,SQ_2E,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1C,SQ_1D,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //lance 2
    anser = make_move(SQ_9A,SQ_9B,0,W_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_1A,SQ_1B,0,W_LANCE,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //knight
    anser = make_move(SQ_9C,SQ_8E,0,W_KNIGHT,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //silver    6
    anser = make_move(SQ_8D,SQ_8E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8D,SQ_7E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_4D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_3D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_5B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_4C,SQ_3B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //gold
    anser = make_move(SQ_8C,SQ_9D,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8C,SQ_7D,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_8C,SQ_8B,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7B,SQ_6C,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7B,SQ_8B,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7B,SQ_7A,0,W_GOLD,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //king
    anser = make_move(SQ_7C,SQ_7D,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7C,SQ_6D,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7C,SQ_6C,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(SQ_7C,SQ_8B,0,W_KING,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop pawn 9
    anser = make_move(0,SQ_8B,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8E,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8G,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4A,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,W_PAWN,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop silver
    anser = make_move(0,SQ_9B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6C,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3A,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3I,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2G,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1B,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1D,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1E,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1F,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1H,0,W_SILVER,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    //drop rook
    anser = make_move(0,SQ_9B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_9F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_8G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_7I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6C,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_6I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_5I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_4I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3A,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_3I,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2G,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_2H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1B,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1D,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1E,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1F,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
    anser = make_move(0,SQ_1H,0,W_ROOK,EMPTY);
    EXPECT_TRUE(array_check(anser,m,n));
}

bool array_check(Move anser,Move *m,int n)
{
    bool flag = false;

    for(int i = 0;i < n;i++){
        if(anser == *(m++)){
            flag = true;
        }
    }
    return flag;
}
*/

