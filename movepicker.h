#if !defined(MOVEPICKER_H_INCLUDE)
#define MOVEPICKER_H_INCLUDE

#include "move.h"
#include "position.h"

//指し手呼び出し
enum GenerateMovePhase{
	MainSearch,
	PhTacticalMove0,
	PhKiller,
	PhNonTactionMove0,
	PhNonTactionMove1,
	PhBadCapture,
	EvasionSearch,
	PhEvasion,
	QSearch,
	PhQCapture0,
	QEvasionSearch,
	PhQEvasion,
	ProCut,
	PhTacticalMove1,
	QRecapture,
	PhQCapture1,
	PhStop
};


class MovePicker{
public:
	MovePicker(const Position& pos, const int depth);
	Move next_move();
	//インサート降順ソート(本当にMovestackのソートができるのか不安)
	template<typename T>
	void insert_sort(T first, T last)
	{
		if (first == last){
			return;
		}
		for (T curr = first + 1; curr != last; curr++){
			if (*(curr - 1) < *curr){
				T tmp = std::move(*curr);
				do{
					*curr = *(curr - 1);
					curr--;
				} while (curr != first && *(curr - 1) < tmp);
				*curr = std::move(tmp);
			}
		}
		return;
	}
private:
	void go_next_phase();
	MoveStack* get_first_move()
	{
		return &lega_moves[1];
	}
	MoveStack* get_curr_move()
	{
		return curr_move;
	}
	MoveStack* get_last_move()
	{
		return last_move;
	}
	int phase;
	MoveStack* curr_move;
	MoveStack* last_move;
	MoveStack lega_moves[MAX_LEGAL_MOVE];
	const Position& m_pos;
};
#endif