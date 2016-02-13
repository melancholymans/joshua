#include "movegen.h"
#include "movepicker.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using MoveGeneratens::generate_moves;

MovePicker::MovePicker(const Position& pos, const int depth) :m_pos(pos)
{	
	m_current_move = m_last_move = m_legal_moves;
	//���Ƃ��낢��ݒ肵�Ă��邪�킩��Ȃ�
	if (pos.checker_bb().is_not_zero()){
		m_phase = EvasionSearch;
	}
	else{
		m_phase = MainSearch;
	}
	//killerMove�����邪�p�X
	//�킩��Ȃ�����
	//�u���\�̎�̏����炵��������A�p�X
	m_ttmove = MoveNone;
}

//MovePiker�͑S���łR�R���X�g���N�^�����邪�Ƃ肠�����͂P�������

void MovePicker::go_next_phase()
{
	m_current_move = m_legal_moves;
	m_phase++;
	switch (m_phase){
	case PhTacticalMove0: case PhTacticalMove1:
		m_last_move = generate_moves<Capture>(m_current_move, m_pos);
		//������scroeCaptures�֐������邪�܂������ł��Ă��Ȃ��̂�PASS
		return;
	case PhKiller:
		//killer�̏������ł��Ă��Ȃ��̂�return���邾��
		return;
	case PhNonTactionMove0:
		m_last_move = generate_moves<NonCapture>(m_current_move, m_pos);
		//scro�֌W�̊֐������邪�������Ă��Ȃ��̂�PASS
		m_current_move = m_last_move;
		m_last_move = generate_moves<Drop>(m_current_move, m_pos);
		//scro�֌W�̊֐������邪�������Ă��Ȃ��̂�PASS
		m_current_move = m_first_move;
		////std::partion�A���S���Y�����������Ȃɂ�������
		//insertion_sort(curr_move, last_move);
		return;
	case PhNonTactionMove1:
		//�킩��Ȃ�����
		return;
	case PhBadCapture:
		//�킩��Ȃ�����
		return;
	case PhEvasion:case PhQEvasion:
		m_last_move = generate_moves<Evasion>(m_current_move, m_pos);
		//�킩��Ȃ�����
		return;
	case PhQCapture0:
		//�킩��Ȃ�����
		return;
	case PhQCapture1:
		//�킩��Ȃ�����
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
			//���낢��Ə��������邪�����ł��Ă��Ȃ��̂Ńp�X
			return ms->move;
			break;
		case PhKiller:
			//�����ł��Ă��Ȃ��̂Ńp�X
			break;
		case PhNonTactionMove0: case PhNonTactionMove1:
			move = (m_current_move++)->move;
			//���낢����������邪�p�X
			return move;
			break;
		case PhBadCapture:
			//���낢�날�邪���܂̂Ƃ���͂���
			return MoveNone;
		case PhEvasion: case PhQEvasion: case PhQCapture0:
			move = pick_best(m_current_move++, m_last_move)->move;
			return move;
			break;
		case PhTacticalMove1:
			ms = pick_best(m_current_move++, m_last_move);
			//���낢�돈�����Ă��邪���̓p�X
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