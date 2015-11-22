#if !defined(MOVEGEN_H_INCLUDE)
#define MOVEGEN_H_INCLUDE

#include "types.h"
#include "bitboard.h"
#include "position.h"
#include "move.h"

using BitBoardns::IN_FRONT_MASK;
using BitBoardns::alloff;
using Movens::make_move;

namespace MoveGeneratens
{
	//王手回避手を生成
	template <Color US>
	MoveStack* generate_evasions(MoveStack* ml, const Position& pos);
	//本体呼び出し用のカラー切り替え関数
	template <MoveType MT>
	MoveStack* generate_moves(MoveStack* ml, const Position& pos);
	//ここから全ての指し手生成関数を呼び出してMoveStackを返す。
	template <MoveType MT, Color US,bool ALL>
	MoveStack* generate_moves(MoveStack* ml, const Position& pos);
	template <MoveType MT, Color US,bool ALL>
	MoveStack* generate_pawn_moves(MoveStack* ml, const Position &pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_lance_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_night_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
}
//最初にこれを呼び出して本体のgenerate_moves関数を呼び出すのはテンプレート引数に変数が使えないためである。必ず定数で呼び出す必要がある
template<MoveType MT>
MoveStack* MoveGeneratens::generate_moves(MoveStack* ml, const Position& pos)
{
	return pos.get_color_turn() == Black ?
		generate_moves<MT, Black>(ml, pos) : generate_moves<MT, White>(ml, pos);
}
template <MoveType MT,Color US,bool ALL = false>
MoveStack* MoveGeneratens::generate_moves(MoveStack* ml, const Position& pos)
{
	const Color them = over_turn(US);
	const Square ksq = pos.get_king_square(them);
	const Rank rank6 = (US == Black) ? Rank6 : Rank4;	//them陣のひとつ手前のランク
	const Rank rank7 = (US == Black) ? Rank7 : Rank3;	//them陣の最上層ランク
	const Rank rank8 = (US == Black) ? Rank8 : Rank2;	//them陣の中間層ランク
	const BitBoard rank_789_bb = IN_FRONT_MASK[US][rank6];			//them陣全て
	const BitBoard rank_123456_bb = IN_FRONT_MASK[them][rank7];		//them陣以外
	const BitBoard rank_1234567_bb = IN_FRONT_MASK[them][rank8];	//us陣+中間+them陣最上層
	//tar1,2,3でCapture,NonCaptureは同じターゲット（Captureはthem側駒がある座標,NonCaptureは駒がいない座標）
	//CapturePlusPro（駒取＋駒を取らず成る手）ではtar1はCaptureとまったく同じで成る手の追加なし、tar2,3は駒取+駒のいない成れる座標
	//NonCaptureMinusPro（駒取りしない-成る手）ではtar2はそのまま駒を取らない手と成る手を排除している。tar3では駒取しない＋７段目までは許容している

	//promotoed king silver
	const BitBoard tar1 =
	(MT == Capture) ? pos.color_of_bb(them) :
	(MT == NonCapture) ? pos.inver_bit_bb() :
	(MT == CapturePlusPro) ? pos.color_of_bb(them):
	(MT == NonCaptureMinusPro) ? pos.inver_bit_bb():alloff;
	//pawn lance night bishop rook 
	const BitBoard tar2 =
	(MT == Capture) ? tar1 :
	(MT == NonCapture) ? tar1 : 
	(MT == CapturePlusPro) ? pos.color_of_bb(them) | (pos.inver_bit_bb() & rank_789_bb):
	(MT == NonCaptureMinusPro) ? pos.inver_bit_bb() & rank_123456_bb :alloff;
	const BitBoard tar3 =
	(MT == Capture) ? tar2 :
	(MT == NonCapture) ? tar2 : 
	(MT == CapturePlusPro) ? tar2:
	(MT == NonCaptureMinusPro) ? pos.inver_bit_bb() & rank_1234567_bb:alloff;

	if (MT == Evasion){
		//自王に王手がかかっているなら王手回避手を生成して返す
		generate_evasions<US>(ml,pos);
		return ml;
	}
	else{
		if (MT != Drop){
			//王手回避手,打つ手以外(capture promoto noncapture recapture)
			ml = generate_pawn_moves<MT, US,ALL>(ml, pos, tar2, ksq);
			ml = generate_lance_moves<MT,US,ALL>(ml, pos, (ALL ? tar3:tar2), ksq);
			ml = generate_night_moves<MT, US, ALL>(ml, pos, tar3, ksq);
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
			printf("dummy\n");
			//ml = generate_gold_drop<us>(ml,pos);
			//ml = generate_pawn_drop<us>(ml, pos);
			//ml = generate_lance_drop<us>(ml, pos);
			//ml = generate_knight_drop<us>(ml, pos);
			//ml = generate_silver_drop<us>(ml, pos);
			//ml = generate_bishop_drop<us>(ml, pos);
			//ml = generate_rook_drop<us>(ml, pos);
		}
	}
	return ml;
}

template <Color US>
MoveStack* MoveGeneratens::generate_evasions(MoveStack* ml, const Position& pos)
{
	MoveStack ss;
	return &ss;
}

template <MoveType MT, Color US,bool ALL>
MoveStack* MoveGeneratens::generate_pawn_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	const Rank rank6 = (US == Black) ? Rank6 : Rank4;
	const BitBoard rank789_bb = IN_FRONT_MASK[US][rank6];
	const int delta = (US == Black) ? DeltaS : DeltaN;
	int to;
	//BitBoard bb = tar;
	//pos.print_bb(bb,"generate_pawn_moves");

	BitBoard to_bb = pos.attackers_from_pawns(US, pos.color_type_of_bb(US, Pawn)) & tar;
	//不成り（them陣地内の不成はここでは生成しない）
	{
		BitBoard on_123456_bb = to_bb & tar;
		while (on_123456_bb.p(0)){
			to = on_123456_bb.first_one_right();
			const int from = to + delta;
			(*ml++).move = make_move(Square(from), Square(to), 0, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
		}
		while (on_123456_bb.p(1)){
			to = on_123456_bb.first_one_left();
			const int from = to + delta;
			(*ml++).move = make_move(Square(from), Square(to), 0, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
		}
	}
	//成り(them陣地内の不成はここで生成)
	if (MT != NonCaptureMinusPro){
		BitBoard on_789_bb = to_bb & rank789_bb;
		if (!on_789_bb.is_not_zero()){
			return ml;
		}
		while (on_789_bb.p(0)){
			to = on_789_bb.first_one_right();
			const int from = to + delta;
			(*ml++).move = make_move(Square(from), Square(to), 1, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
			if (MT == NonEvasion || ALL){	//王手がかかっていない　または　ALL=trueのときは、通常歩は敵陣に入れば無条件でなるが、to座標が78ランクなら不成も生成する(TODO:pawnのときはon_78_bbでよいのでは）
				const Rank rank9 = (US == Black) ? Rank9 : Rank1;
				if (make_rank(Square(to)) != rank9){
					(*ml++).move = make_move(Square(from), Square(to), 0, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
				}
			}
		}
		while (on_789_bb.p(1)){
			to = on_789_bb.first_one_left();
			const int from = to + delta;
			(*ml++).move = make_move(Square(from), Square(to), 1, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
			const Rank rank9 = (US == Black) ? Rank9 : Rank1;
			//通常歩は敵陣に入れば無条件でなるが、to座標が78ランクなら不成も生成する
			if (make_rank(Square(to)) != rank9){
				(*ml++).move = make_move(Square(from), Square(to), 0, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
			}
		}
	}
	return ml;
}

template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_lance_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{	
	BitBoard from_bb = pos.color_type_of_bb(US, Lance);
	Square from;

	auto make_move_lance = [&pos,&from,&tar,&ml](const int part)
	{
		BitBoard to_bb;
		to_bb.set_p(part, pos.attackers_from_lance(US, from, pos.all_bb()).p(part) & tar.p(part));
		while (to_bb.p(part)){
			const Square to = to_bb.first_one_right();
			if (MT == Capture || MT == NonCapture || MT == NonEvasion || MT == CapturePlusPro){
				const Rank to_rank = make_rank(to);
				//移動座標のランクがthem陣地内(7段目(3段目)は含まない）のときの成り、不成の場合分け
				if (is_infront_rank(US, Rank7, to_rank)){
					(*ml++).move = make_move(from, to, 1, Lance, type_of_piece(Piece(pos.get_board(to))));
					if (MT == NonEvasion || ALL){
						if (is_infront_rank(US, Rank8, to_rank)){
							(*ml++).move = make_move(from, to, 0, Lance, type_of_piece(Piece(pos.get_board(to))));
						}
					}
				}
				//移動座標のランクがthem陣地内(7段目(3段目)含む）のときの成り、不成の場合分け
				else if (is_infront_rank(US, Rank6, to_rank)){
					(*ml++).move = make_move(from, to, 1, Lance, type_of_piece(Piece(pos.get_board(to))));	//成りはどの座標でも生成する
					//MT == CapturePlusProなら駒取りがあれば不成を生成
					if (MT == CapturePlusPro && pos.get_board(to) != EmptyPiece){
						(*ml++).move = make_move(from, to, 0, Lance, type_of_piece(Piece(pos.get_board(to))));
					}
					else{	//CapturePlusPro以外のMoveTypeでは全ての不成を生成
						(*ml++).move = make_move(from, to, 0, Lance, type_of_piece(Piece(pos.get_board(to))));
					}
				}
				else{	//them陣地以外では全て不成を生成
					(*ml++).move = make_move(from, to, 0, Lance, type_of_piece(Piece(pos.get_board(to))));
				}
			}
			else if (MT == NonCaptureMinusPro){
				(*ml++).move = make_move(from, to, 0, Lance, type_of_piece(Piece(pos.get_board(to))));
			}
		}
		return ml;
	};	//make_move_lanceラムダ式終了

	while (from_bb.p(0)){
		from = from_bb.first_one_right();
		ml = make_move_lance(0);
	}
	while (from_bb.p(1)){
		from = from_bb.first_one_left();
		ml = make_move_lance(1);
	}
	return ml;
}
//night move
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_night_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	BitBoard from_bb = pos.color_type_of_bb(US, Night);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_night(US, from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			if (MT == Capture || MT == NonCapture || MT == CapturePlusPro || MT == NonEvasion || MT == Evasion){
				const Rank to_rank = make_rank(to);
				//移動座標のランクがthem陣地内(7段目(3段目)は含まない）のときの成り、不成の場合分け
				if (is_infront_rank(US, Rank7, to_rank)){
					(*ml++).move = make_move(from, to, 1, Night, type_of_piece(Piece(pos.get_board(to))));
				}
				else if (is_infront_rank(US, Rank6, to_rank)){
					(*ml++).move = make_move(from, to, 1, Night, type_of_piece(Piece(pos.get_board(to))));
					//MT == CapturePlusProなら駒取りがあれば不成を生成
					if (MT == CapturePlusPro && pos.get_board(to) != EmptyPiece){
						(*ml++).move = make_move(from, to, 0, Night, type_of_piece(Piece(pos.get_board(to))));
					}
					else{
						(*ml++).move = make_move(from, to, 0, Night, type_of_piece(Piece(pos.get_board(to))));
					}
				}
				else{
					(*ml++).move = make_move(from, to, 0, Night, type_of_piece(Piece(pos.get_board(to))));
				}
			}
			else if (MT == NonCaptureMinusPro){
				(*ml++).move = make_move(from, to, 0, Night, type_of_piece(Piece(pos.get_board(to))));
			}
		}
	}
	return ml;
}
#endif
