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
	m_ttmove = MoveNone;
}

//MovePikerは全部で３つコンストラクタがあるがとりあえずは１つだけ作る

void MovePicker::go_next_phase()
{
	m_current_move = m_legal_moves;
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
		//わからない操作
		return;
	case PhBadCapture:
		//わからない操作
		return;
	case PhEvasion:case PhQEvasion:
		m_last_move = generate_moves<Evasion>(m_current_move, m_pos);
		//わからない操作
		return;
	case PhQCapture0:
		//わからない操作
		return;
	case PhQCapture1:
		//わからない操作
		return;
	case EvasionSearch:case QSearch:case QEvasionSearch:case QRecapture:case ProCut:
		m_phase = PhStop;
	case PhStop:
		m_last_move = m_current_move + 1;
		return;
	default:
		_ASSERT(false);
	}
}

Move MovePicker::next_move()
{
	MoveStack* ms;
	Move move;

	do{
		while (m_current_move == m_last_move){
			go_next_phase();
		}
		switch (m_phase){
		case MainSearch: case EvasionSearch: case QSearch: case QEvasionSearch: case ProCut:
			m_current_move++;
			return m_ttmove;
		case PhTacticalMove0:
			ms = pick_best(m_current_move++, m_last_move);
			//いろいろと処理があるが実装できていないのでパス
			return ms->move;
			break;
		case PhKiller:
			//実装できていないのでパス
			break;
		case PhNonTactionMove0: case PhNonTactionMove1:
			move = (m_current_move++)->move;
			//いろいろ条件があるがパス
			return move;
			break;
		case PhBadCapture:
			//いろいろあるがいまのところはこれ
			return MoveNone;
		case PhEvasion: case PhQEvasion: case PhQCapture0:
			move = pick_best(m_current_move++, m_last_move)->move;
			return move;
			break;
		case PhTacticalMove1:
			ms = pick_best(m_current_move++, m_last_move);
			//いろいろ処理しているが今はパス
			return ms->move;
			break;
		case PhQCapture1:
			move = pick_best(m_current_move++, m_last_move)->move;
			return move;
		case PhStop:
			return MoveNone;
		default:
			_ASSERT(false);
		}
	} while (true);	
}

TEST(movepicker, movepicker)
{
}