﻿#if !defined(SEARCH_H_INCLUDE)
#define SEARCH_H_INCLUDE

#include "types.h"
//先行宣言
class Position;
struct SplitPoint;
class ThreadPool;
class TimeManager;

struct SearchStack{
	SplitPoint* split_point;
	int ply;
	Move current_move;
	Move exclude_move;
	Move killers[2];
	int reduction;
	int static_eval;
	bool skip_null_move;
	int static_eval_raw;
};

struct SignalsType{
	bool stop_on_ponder_hit;
	bool first_root_move;
	bool stop;
	bool failed_low_at_root;
};
//稲庭対策用フラグ
enum InaniwaFlag{
	NotInaniwa,
	InaniwaIsNlack,
	InaniwaIsWhite,
	InaniwaFlagNum
};
class RootMove{
public:
	RootMove(){}
	explicit RootMove(const Move m) :m_score(-ScoreInfinite), m_prev_score(-ScoreInfinite)
	{
		m_pv.push_back(m);
		m_pv.push_back(MoveNone);
	}
	bool operator < (const RootMove& rm) const
	{
		return m_score < rm.m_score;
	}
	bool operator == (const Move& m) const
	{
		return m_pv[0] == m;
	}
	void extract_pv_from_tt(Position& pos);
	void insert_pv_in_tt(Position& pos, SearchStack* ss);
private:
	int m_score;
	int m_prev_score;
	vector<Move> m_pv;
};

template<bool Gain>
class Stats{
public:
	static const int max_score = 2000;
	void clear()
	{
		memset(m_table, 0, sizeof(m_table));
	}
	int value(const bool is_drop, const Piece piece, const Square to)
	{
		_ASSERT(0 < piece && piece < PieceNum);
		_ASSERT(0 <= to && to < SquareNum);
		return m_table[is_drop][piece][to];
	}
	void update(const bool is_drop, const Piece piece, const Square to, const int score)
	{
		if (Gain){
			m_table[is_drop][piece][to] = std::max(score,value(is_drop,piece,to)-1);
		}
		else if (abs(value(is_drop, piece, to) + score) < max_score){
			m_table[is_drop][piece][to] += score;
		}
	}
private:
	int m_table[2][PieceNum][SquareNum];
};

typedef Stats<false> History;
typedef Stats<true> Gains;

//class TranspositionTable;

struct Searcher{
	//静的メンバーのときはthisが呼べないのでthisptrで代用する。thisptrはinit関数内で設定
	static Searcher* thisptr;
	static volatile SignalsType signals;	//thread.cpp->start_thinkingで初期化
	static LimitsType limits;		//thread.cpp->start_thinkingで初期化
	static vector<Move> search_moves;
	static ptime_t search_timer;	//thread.cpp->start_thinkingで初期化
	static StateStackPtr setup_states;
	static vector<RootMove> root_moves;	//thread.cpp->start_thinkingで初期化
	static size_t pv_size;
	static size_t pv_idx;
	static TimeManager time_manager;
	static int best_move_chages;
	static History history;
	static Gains gains;
//	static TranspositionTable tt;
	static Position root_position;	//thread.cpp->start_thinkingで初期化
	static ThreadPool threads;
//	static OptionsMap options;
	void init();
	static void id_loop(Position& pos);
	static string pv_info_to_usi(Position& pos, const int depth, const int alpha, const int beta);
	template<NodeType NT, bool IN_CHECK>
	static int qsearch(Position& pos, SearchStack* ss, int alpha, int beta, const int depth);
	template<NodeType NT>
	static int search(Position& pos, SearchStack* ss, int alpha, int beta, const int depth, const bool cut_node);
	static void think();
	static void check_time();
	static void do_usi_command_loop(int argc, char* argv[]);
	static void set_option(std::istringstream& uip);
};

//void init_search_table();
#endif
