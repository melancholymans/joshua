#if !defined(MOVEPICKER_H_INCLUDE)
#define MOVEPICKER_H_INCLUDE

//éwÇµéËåƒÇ—èoÇµ
enum GenerateMovePhase{
	MainSearch,
	EvasionSearch
};

class MovePicker{
public:
	MovePicker(const Position& pos, int depth);
private:
	void go_next_phase();
	MoveStack* get_first_move()
	{
		return lega_moves[1];
	}
	MoveStack* get_curr_move()
	{
		return curr_move;
	}
	MoveStack* get_last_move()
	{
		return last_move;
	}
	GenerateMovePhase phase;
	MoveStack* curr_move;
	MoveStack* last_move;

	MoveStack lega_moves[MAX_LEGAL_MOVE];
};
#endif