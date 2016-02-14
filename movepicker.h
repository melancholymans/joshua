#if !defined(MOVEPICKER_H_INCLUDE)
#define MOVEPICKER_H_INCLUDE
#include <array>
#include <algorithm>
#include <utility>
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
	MoveStack* m_first_move;
	MoveStack* m_last_move;
	MoveStack* m_current_move;

	int m_phase;
	MoveStack m_legal_moves[MAX_LEGAL_MOVE];
	const Position& m_pos;
	Move m_ttmove;	//置換表からの指し手
};
/*
pick_best関数で必要
*/
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.score < s.score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.score > s.score; }
inline MoveStack* pick_best(MoveStack* current_move, MoveStack* last_move)
{
	std::swap(*current_move, *std::max_element(current_move, last_move));
	return current_move;
}
#endif