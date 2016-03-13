#if !defined(MOVEGEN_H_INCLUDE)
#define MOVEGEN_H_INCLUDE

#include "types.h"

using BitBoardns::IN_FRONT_MASK;
using BitBoardns::FILE_MASK;
using BitBoardns::alloff;
using BitBoardns::print;
using BitBoardns::make_between_bb;
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
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_silver_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_bishop_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_rook_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_gold_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_king_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_horse_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <MoveType MT, Color US, bool ALL>
	MoveStack* generate_dragon_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <Color US>
	MoveStack* generate_pawn_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <Color US>
	MoveStack* generate_lance_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <Color US>
	MoveStack* generate_night_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
	template <Color US>
	MoveStack* generate_silver_gold_bishop_rook_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq);
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

	const BitBoard tar1 =
		(MT == Capture) ? pos.color_of_bb(them) :
		(MT == NonCapture) ? pos.inver_bit_bb() : alloff;
	
	if (MT == Evasion){
		//自王に王手がかかっているなら王手回避手を生成して返す
		return  generate_evasions<US>(ml,pos);
	}
	else{
		if (MT != Drop){
			//王手回避手,打つ手以外(capture noncapture)
			ml = generate_pawn_moves<MT, US,ALL>(ml, pos, tar1, ksq);
			ml = generate_lance_moves<MT,US,ALL>(ml, pos, tar1, ksq);
			ml = generate_night_moves<MT, US, ALL>(ml, pos, tar1, ksq);
			ml = generate_silver_moves<MT,US,ALL>(ml, pos, tar1, ksq);
			ml = generate_bishop_moves<MT, US, ALL>(ml, pos, tar1, ksq);
			ml = generate_rook_moves<MT, US, ALL>(ml, pos, tar1, ksq);
			ml = generate_gold_moves<MT, US, ALL>(ml, pos, tar1, ksq);	//ProPawn,ProLance,ProNight,ProSilverもここで生成
			ml = generate_king_moves<MT, US, ALL>(ml, pos, tar1, ksq);
			ml = generate_horse_moves<MT, US, ALL>(ml, pos, tar1, ksq);
			ml = generate_dragon_moves<MT, US, ALL>(ml, pos, tar1, ksq);
		}
		//打つ手
		else{
			BitBoard bb(0x7FFFFFFFFFFFFFFF, 0x3FFFF);	//inver_bit_bb関数によってto_bb[1]の領域に余分な1を立てさせないため
			const BitBoard tar2 = pos.inver_bit_bb() & bb;	//空いている座標をonにしてtar2に入れておきDropのターゲットにする、同様にto_bb[0]の63bit目は使用していないのでここも0bitにしておく
			ml = generate_pawn_drop<US>(ml, pos, tar2, ksq);
			ml = generate_lance_drop<US>(ml, pos, tar2, ksq);
			ml = generate_night_drop<US>(ml, pos, tar2, ksq);
			ml = generate_silver_gold_bishop_rook_drop<US>(ml, pos, tar2, ksq);
		}
	}
	return ml;
}

template <Color US>
MoveStack* MoveGeneratens::generate_evasions(MoveStack* ml, const Position& pos)
{
	const Square ksq = pos.get_king_square(US);
	const Color them = over_turn(US);
	BitBoard checker_bb = pos.checker_bb();
	BitBoard bb = checker_bb;
	Square check_sq;
	Square to;
	int checker_num = 0;

	//checkerの数をカウントアップ
	do{
		check_sq = bb.first_one();
		checker_num++;
	} while (bb.is_not_zero());
	//kingが逃げて王手を回避する。手番側のKingの移動可能bit boardを生成,その座標に敵の利きが利いていなかったら着手リストに加える
	//TODO:attackers_to関数が重いようであれば別の方法を考える
	BitBoard possible_bb = pos.attackers_from_king(ksq) & (pos.inver_bit_bb() | pos.color_of_bb(them));	//inver_bit_bbは現在の局面で空いている座標のbit_boardを返す | 相手の駒のbitboardのbit or
	while (possible_bb.is_not_zero()){
		to = possible_bb.first_one();
		if (!pos.attackers_to(them, to, pos.all_bb()).is_not_zero()){
			(*ml++).move = make_move(ksq, Square(to), 0, King, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	//両王手ならばKingが回避するしかないので着手リスト生成はここで終了
	if (checker_num > 1){
		return ml;
	}

	BitBoard tar1 = make_between_bb(check_sq, ksq);
	BitBoard tar2 = checker_bb | tar1;
	ml = generate_pawn_drop<US>(ml, pos, tar1, ksq);
	ml = generate_lance_drop<US>(ml, pos, tar1, ksq);
	ml = generate_night_drop<US>(ml, pos, tar1, ksq);
	ml = generate_silver_gold_bishop_rook_drop<US>(ml, pos, tar1, ksq);
	ml = generate_pawn_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_lance_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_night_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_silver_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_bishop_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_rook_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_gold_moves<Evasion, US, false>(ml, pos, tar2, ksq);	//ProPawn,ProLance,ProNight,ProSilverもここで生成
	ml = generate_horse_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	ml = generate_dragon_moves<Evasion, US, false>(ml, pos, tar2, ksq);
	return ml;
}

template <MoveType MT, Color US,bool ALL>
MoveStack* MoveGeneratens::generate_pawn_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	const Rank rank6 = (US == Black) ? Rank6 : Rank4;
	const BitBoard rank789_bb = IN_FRONT_MASK[US][rank6];
	const int delta = (US == Black) ? DeltaS : DeltaN;
	int to;
	BitBoard to_bb = pos.attackers_from_pawns(US, pos.color_type_of_bb(US, Pawn)) & tar;
	//不成り（them陣地内の不成はここでは生成しない）
	{
		BitBoard on_123456_bb = to_bb;
		on_123456_bb.clear_bits(rank789_bb);
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
	BitBoard on_789_bb = to_bb & rank789_bb;
	if (!on_789_bb.is_not_zero()){
		return ml;
	}
	while (on_789_bb.p(0)){
		to = on_789_bb.first_one_right();
		const int from = to + delta;
		(*ml++).move = make_move(Square(from), Square(to), 1, type_of_piece(Piece(pos.get_board(from))), type_of_piece(Piece(pos.get_board(to))));
		if (ALL == true){	//ALL=trueのときは、通常歩は敵陣に入れば無条件でなるが、to座標が78ランクなら不成も生成する
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
	return ml;
}

template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_lance_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{	
	BitBoard from_bb = pos.color_type_of_bb(US, Lance);
	Square from;

	auto make_move_lance = [&pos,&from,&tar,&ml](const int part)
	{
		const Rank rank9 = (US == Black) ? Rank9 : Rank1;
		const Rank rank6 = (US == Black) ? Rank6 : Rank4;
		BitBoard to_bb;

		to_bb.set_p(part, pos.attackers_from_lance(US, from, pos.all_bb()).p(part) & tar.p(part));
		while (to_bb.p(part)){
			const Square to = (0 == part) ? to_bb.first_one_right() : to_bb.first_one_left();
			const Rank to_rank = make_rank(to);
			//移動座標のランクがthem陣地内(7段目含む）のときの成り、不成の場合分け
			if (is_infront_rank(US, rank6, to_rank)){
				(*ml++).move = make_move(from, to, 1, Lance, type_of_piece(Piece(pos.get_board(to))));
				if (to_rank != rank9){
					(*ml++).move = make_move(from, to, 0, Lance, type_of_piece(Piece(pos.get_board(to))));
				}
			}
			//them陣地以外では全て不成を生成
			else{	
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
	const Rank rank8 = (US == Black) ? Rank8 : Rank2;
	const Rank rank9 = (US == Black) ? Rank9 : Rank1;
	const Rank rank6 = (US == Black) ? Rank6 : Rank4;
	BitBoard from_bb = pos.color_type_of_bb(US, Night);

	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_night(US, from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			const Rank to_rank = make_rank(to);
			//移動座標のランクがthem陣地内(7段目は含む）のときの成り、不成の場合分け
			if (is_infront_rank(US, rank6, to_rank)){
				(*ml++).move = make_move(from, to, 1, Night, type_of_piece(Piece(pos.get_board(to))));
			}
			if (to_rank != rank8 && to_rank != rank9){
				(*ml++).move = make_move(from, to, 0, Night, type_of_piece(Piece(pos.get_board(to))));
			}
		}
	}
	return ml;
}
//silver move 
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_silver_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	BitBoard from_bb = pos.color_type_of_bb(US, Silver);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		const bool from_can_promoto = is_promoto(US, make_rank(from));
		BitBoard to_bb = pos.attackers_from_silver(US, from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			const bool to_can_promoto = is_promoto(US, make_rank(to));
			if ((from_can_promoto || to_can_promoto)){
				(*ml++).move = make_move(from, to, 1, Silver, type_of_piece(Piece(pos.get_board(to))));
			}
			(*ml++).move = make_move(from, to, 0, Silver, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
//bishop move
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_bishop_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	const Rank rank6 = (US == Black) ? Rank6 : Rank4;
	BitBoard rank789_bb = IN_FRONT_MASK[US][rank6];
	BitBoard from_bb = pos.color_type_of_bb(US, Bishop);
	//Rank789（Rank321）にいる駒専用で、移動先には制限はない。ALL==trueなら不成も生成する
	BitBoard from_on_rank789 = rank789_bb & from_bb;
	while (from_on_rank789.is_not_zero()){
		const Square from = from_on_rank789.first_one();
		BitBoard to_bb = pos.attackers_from_bishop(from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			(*ml++).move = make_move(from, to, 1, Bishop, type_of_piece(Piece(pos.get_board(to))));
			if (ALL == true){
				(*ml++).move = make_move(from, to, 0, Bishop, type_of_piece(Piece(pos.get_board(to))));
			}
		}
	}
	//Rank789(Ran321)以外にいる駒専用
	from_bb.clear_bits(rank789_bb);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_bishop(from) & tar;
		//成り生成、ALL==trueなら不成も生成する
		BitBoard to_on_rank789 = rank789_bb & to_bb;
		while (to_on_rank789.is_not_zero()){
			const Square to = to_on_rank789.first_one();
			(*ml++).move = make_move(from, to, 1, Bishop, type_of_piece(Piece(pos.get_board(to))));
			if (ALL == true){
				(*ml++).move = make_move(from, to, 0, Bishop, type_of_piece(Piece(pos.get_board(to))));
			}
		}
		BitBoard to_on_rank654321 = to_bb.clear_bits(rank789_bb);
		while (to_on_rank654321.is_not_zero()){
			const Square to = to_on_rank654321.first_one();
			(*ml++).move = make_move(from, to, 0, Bishop, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
//rook move
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_rook_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	const Rank rank6 = (US == Black) ? Rank6 : Rank4;
	const BitBoard rank789_bb = IN_FRONT_MASK[US][rank6];
	BitBoard from_bb = pos.color_type_of_bb(US, Rook);
	//Rank789（Rank321）にいる駒専用で、移動先には制限はない。ALL==trueなら不成も生成する
	BitBoard from_on_rank789 = rank789_bb & from_bb;
	while (from_on_rank789.is_not_zero()){
		const Square from = from_on_rank789.first_one();
		BitBoard to_bb = pos.attackers_from_rook(from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			(*ml++).move = make_move(from, to, 1, Rook, type_of_piece(Piece(pos.get_board(to))));
			if (ALL == true){
				(*ml++).move = make_move(from, to, 0, Rook, type_of_piece(Piece(pos.get_board(to))));
			}
		}
	}
	//Rank789(Ran321)以外にいる駒専用
	from_bb.clear_bits(rank789_bb);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_rook(from) & tar;
		//成り生成、ALL==trueなら不成も生成する
		BitBoard to_on_rank789 = rank789_bb & to_bb;
		while (to_on_rank789.is_not_zero()){
			const Square to = to_on_rank789.first_one();
			(*ml++).move = make_move(from, to, 1, Rook, type_of_piece(Piece(pos.get_board(to))));
			if (ALL == true){
				(*ml++).move = make_move(from, to, 0, Rook, type_of_piece(Piece(pos.get_board(to))));
			}
		}
		BitBoard to_on_rank654321 = to_bb.clear_bits(rank789_bb);
		while (to_on_rank654321.is_not_zero()){
			const Square to = to_on_rank654321.first_one();
			(*ml++).move = make_move(from, to, 0, Rook, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
//gold move
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_gold_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	BitBoard from_bb = pos.color_of_bb(US) & (pos.piece_type_of_bb(Gold) | pos.piece_type_of_bb(ProPawn) | pos.piece_type_of_bb(ProLance) | pos.piece_type_of_bb(ProNight) | pos.piece_type_of_bb(ProSilver));
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		const PieceType pt = type_of_piece(Piece(pos.get_board(from)));
		BitBoard to_bb = pos.attackers_from_gold(US, from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			(*ml++).move = make_move(from, to, 0, pt, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
//king move
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_king_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	BitBoard from_bb = pos.color_type_of_bb(US, King);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_king(from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			(*ml++).move = make_move(from, to, 0, King, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_horse_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	BitBoard from_bb = pos.color_type_of_bb(US, Horse);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_horse(from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			(*ml++).move = make_move(from, to, 0, Horse, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
template <MoveType MT, Color US, bool ALL>
MoveStack* MoveGeneratens::generate_dragon_moves(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	BitBoard from_bb = pos.color_type_of_bb(US, Dragon);
	while (from_bb.is_not_zero()){
		const Square from = from_bb.first_one();
		BitBoard to_bb = pos.attackers_from_dragon(from) & tar;
		while (to_bb.is_not_zero()){
			const Square to = to_bb.first_one();
			(*ml++).move = make_move(from, to, 0, Dragon, type_of_piece(Piece(pos.get_board(to))));
		}
	}
	return ml;
}
template <Color US>
MoveStack* MoveGeneratens::generate_pawn_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	if (!pos.is_hand(US,Pawn)){
		return ml;
	}
	BitBoard to_bb = tar;
	const Rank rank8 = (US == Black) ? Rank8 : Rank2;
	const BitBoard rank9_bb = IN_FRONT_MASK[US][rank8];
	//１段目には打てない
	to_bb.clear_bits(rank9_bb);
	while (to_bb.is_not_zero()){
		const Square to = to_bb.first_one();
		//２歩の回避
		File f = make_file(to);
		if ((FILE_MASK[f] & pos.color_type_of_bb(US, Pawn)).is_not_zero()){
			to_bb.clear_bits(FILE_MASK[f]);	//pawnがいる列を全てクリアして２重にチエックさせない
			continue;
		}
		//打歩詰の回避
		if (pos.is_pawn_drop_checkmate(US, to,ksq)){
			continue;
		}
		//from 駒打ちのときはPieceType + SquareNum - 1 (81->87)なのでわざわざPieceTypeは登録しなくてよい
		(*ml++).move = make_move(Square(Pawn + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
	}
	return ml;
}
template <Color US>
MoveStack* MoveGeneratens::generate_lance_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	//TODO:香車は離して打ての格言があるが、障害物（敵にしろ味方にしろ）がない場合は一番後ろに置き中間位置には置かない手の生成方法が考えられるが、現地点ではパス
	if (!pos.is_hand(US, Lance)){
		return ml;
	}
	BitBoard to_bb = tar;
	const Rank rank8 = (US == Black) ? Rank8 : Rank2;
	const BitBoard rank9_bb = IN_FRONT_MASK[US][rank8];
	//１段目には打てない
	to_bb.clear_bits(rank9_bb);
	while (to_bb.is_not_zero()){
		const Square to = to_bb.first_one();
		//from 駒打ちのときはPieceType + SquareNum - 1 (81->87)なのでわざわざPieceTypeは登録しなくてよい
		(*ml++).move = make_move(Square(Lance + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
	}
	return ml;
}
template <Color US>
MoveStack* MoveGeneratens::generate_night_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	if (!pos.is_hand(US, Night)){
		return ml;
	}
	BitBoard to_bb = tar;
	const Rank rank7 = (US == Black) ? Rank7 : Rank3;
	const BitBoard rank89_bb = IN_FRONT_MASK[US][rank7];
	//１,2段目には打てない
	to_bb.clear_bits(rank89_bb);
	while (to_bb.is_not_zero()){
		const Square to = to_bb.first_one();
		//from 駒打ちのときはPieceType + SquareNum - 1 (81->87)なのでわざわざPieceTypeは登録しなくてよい
		(*ml++).move = make_move(Square(Night + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
	}
	return ml;
}
template <Color US>
MoveStack* MoveGeneratens::generate_silver_gold_bishop_rook_drop(MoveStack* ml, const Position& pos, const BitBoard& tar, Square ksq)
{
	//TODO:現在は座標をループでまわしているがこのループのなかで逐一is_hand関数で駒の有無を確認している。駒の有無の確認は１度だけしてループ中ではしないようにはできないか？
	BitBoard to_bb = tar;
	while (to_bb.is_not_zero()){
		const Square to = to_bb.first_one();
		//from 駒打ちのときはPieceType + SquareNum - 1 (81->87)なのでわざわざPieceTypeは登録しなくてよい
		if (pos.is_hand(US, Silver)){
			(*ml++).move = make_move(Square(Silver + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
		}
		if (pos.is_hand(US, Gold)){
			(*ml++).move = make_move(Square(Gold + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
		}
		if (pos.is_hand(US, Bishop)){
			(*ml++).move = make_move(Square(Bishop + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
		}
		if (pos.is_hand(US, Rook)){
			(*ml++).move = make_move(Square(Rook + SquareNum - 1), to, 0, PieceType(0), EmptyPiece);
		}
	}
	return ml;
}
#endif



