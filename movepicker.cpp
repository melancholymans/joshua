#include "movegen.h"
#include "movepicker.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using MoveGeneratens::generate_moves;

MovePicker::MovePicker(const Position& pos, const int depth) :m_pos(pos)
{
	lega_moves[0].score = INT_MAX;	//番兵
	curr_move = last_move = get_first_move();
	//あといろいろ設定してあるがわからない
	if (pos.checker_bb().is_not_zero()){
		phase = EvasionSearch;
	}
	else{
		phase = MainSearch;
	}
	//killerMoveがあるがパス
	//わからない処理
	//置換表の手の処理らしきもあり、パス
}

//MovePikerは全部で３つコンストラクタがあるがパス

void MovePicker::go_next_phase()
{
	curr_move = get_first_move();
	phase++;
	switch (phase){
	case PhTacticalMove0: case PhTacticalMove1:
		last_move = generate_moves(curr_move, m_pos);
		return;
	case PhKiller:
		//killerの準備ができていないのでreturnするだけ
		return;
	case PhNonTactionMove0:
		last_move = generate_moves(curr_move, m_pos);
		curr_move = last_move;
		last_move = generate_moves(curr_move, m_pos);
		curr_move = get_first_move();
		//std::partionアルゴリズムをつかったなにかがある
		insertion_sort(curr_move, last_move);
		return;
	case PhNonTactionMove1:
		curr_move = last_move;
		//わからない操作
		return;
	case PhBadCapture:
		curr_move = lega_moves + MAX_LEGAL_MOVE - 1;
		//わからない操作
		return;
	case PhEvasion:case PhQEvasion:
		last_move = generate_moves(curr_move, m_pos);
		//わからない操作
		return;
	case PhQCapture0:
		last_move = generate_moves(curr_move, m_pos);
		//わからない操作
		return;
	case PhQCapture1:
		last_move = generate_moves(curr_move, m_pos);
		//わからない操作
		return;
	case EvasionSearch:case QSearch:case QEvasionSearch:case QRecapture:case ProCut:
		phase = PhStop;
	case PhStop:
		last_move = curr_move + 1;
		return;
	default:
		_ASSERT(false);
	}
}

Move MovePicker::next_move()
{
	do{
		while (curr_move == last_move){
			go_next_phase();
		}
		switch (phase){
		case MainSearch: case EvasionSearch:
			curr_move++;
		default:
			_ASSERT(false);
		}
	} while (true);
}