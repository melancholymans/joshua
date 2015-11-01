#include "movegen.h"
#include "movepicker.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

using MoveGeneratens::generate_moves;

MovePicker::MovePicker(const Position& pos, const int depth) :m_pos(pos)
{
	lega_moves[0].score = INT_MAX;	//�ԕ�
	curr_move = last_move = get_first_move();
	//���Ƃ��낢��ݒ肵�Ă��邪�킩��Ȃ�
	if (pos.checker_bb().is_not_zero()){
		phase = EvasionSearch;
	}
	else{
		phase = MainSearch;
	}
	//killerMove�����邪�p�X
	//�킩��Ȃ�����
	//�u���\�̎�̏����炵��������A�p�X
}

//MovePiker�͑S���łR�R���X�g���N�^�����邪�p�X

void MovePicker::go_next_phase()
{
	curr_move = get_first_move();
	phase++;
	switch (phase){
	case PhTacticalMove0: case PhTacticalMove1:
		last_move = generate_moves(curr_move, m_pos);
		return;
	case PhKiller:
		//killer�̏������ł��Ă��Ȃ��̂�return���邾��
		return;
	case PhNonTactionMove0:
		last_move = generate_moves(curr_move, m_pos);
		curr_move = last_move;
		last_move = generate_moves(curr_move, m_pos);
		curr_move = get_first_move();
		//std::partion�A���S���Y�����������Ȃɂ�������
		insertion_sort(curr_move, last_move);
		return;
	case PhNonTactionMove1:
		curr_move = last_move;
		//�킩��Ȃ�����
		return;
	case PhBadCapture:
		curr_move = lega_moves + MAX_LEGAL_MOVE - 1;
		//�킩��Ȃ�����
		return;
	case PhEvasion:case PhQEvasion:
		last_move = generate_moves(curr_move, m_pos);
		//�킩��Ȃ�����
		return;
	case PhQCapture0:
		last_move = generate_moves(curr_move, m_pos);
		//�킩��Ȃ�����
		return;
	case PhQCapture1:
		last_move = generate_moves(curr_move, m_pos);
		//�킩��Ȃ�����
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