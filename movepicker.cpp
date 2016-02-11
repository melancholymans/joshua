#include "movegen.h"
#include "movepicker.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using MoveGeneratens::generate_moves;

MovePicker::MovePicker(const Position& pos, const int depth) :m_pos(pos)
{	
	m_current_move = m_last_move = m_legal_moves;
	//あといろいろ設定してあるがわからない
	if (pos.checker_bb().is_not_zero()){
		m_phase = EvasionSearch;
	}
	else{
		m_phase = MainSearch;
	}
	//killerMoveがあるがパス
	//わからない処理
	//置換表の手の処理らしきもあり、パス
}

//MovePikerは全部で３つコンストラクタがあるがパス

void MovePicker::go_next_phase()
{
	m_current_move = m_legal_moves;
	Color us = m_pos.get_color_turn();
	m_phase++;
	switch (m_phase){
	case PhTacticalMove0: case PhTacticalMove1:
		m_last_move = generate_moves<Capture>(m_current_move, m_pos);
		//ここにscroeCaptures関数があるがまだ実装できていないのでPASS
		return;
	case PhKiller:
		//killerの準備ができていないのでreturnするだけ
		return;
	case PhNonTactionMove0:
		m_last_move = generate_moves<NonCapture>(m_current_move, m_pos);
		//scro関係の関数があるが実装していないのでPASS
		m_current_move = m_last_move;
		m_last_move = generate_moves<Drop>(m_current_move, m_pos);
		//scro関係の関数があるが実装していないのでPASS
		m_current_move = m_first_move;
		////std::partionアルゴリズムをつかったなにかがある
		//insertion_sort(curr_move, last_move);
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
		//last_move = generate_moves(curr_move, m_pos);
		//わからない操作
		return;
	case PhQCapture0:
		//last_move = generate_moves(curr_move, m_pos);
		//わからない操作
		return;
	case PhQCapture1:
		//last_move = generate_moves(curr_move, m_pos);
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
	/**/
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
	
	return static_cast<Move>(0);
}
