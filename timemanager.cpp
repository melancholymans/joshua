#include "types.h"
#include "common.h"

//Global object
extern USI::OptionsMap options;

const int move_horizon = 47;	//15分切れ負け用
const float max_ratio = 3.0;	//15分切れ負け用
const float steal_ratio = 0.33;	//?
//?
const int move_importance[512] = {
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
	7780, 7780, 7780, 7780, 7778, 7778, 7776, 7776, 7776, 7773, 7770, 7768, 7766, 7763, 7757, 7751,
	7743, 7735, 7724, 7713, 7696, 7689, 7670, 7656, 7627, 7605, 7571, 7549, 7522, 7493, 7462, 7425,
	7385, 7350, 7308, 7272, 7230, 7180, 7139, 7094, 7055, 7010, 6959, 6902, 6841, 6778, 6705, 6651,
	6569, 6508, 6435, 6378, 6323, 6253, 6152, 6085, 5995, 5931, 5859, 5794, 5717, 5646, 5544, 5462,
	5364, 5282, 5172, 5078, 4988, 4901, 4831, 4764, 4688, 4609, 4536, 4443, 4365, 4293, 4225, 4155,
	4085, 4005, 3927, 3844, 3765, 3693, 3634, 3560, 3479, 3404, 3331, 3268, 3207, 3146, 3077, 3011,
	2947, 2894, 2828, 2776, 2727, 2676, 2626, 2589, 2538, 2490, 2442, 2394, 2345, 2302, 2243, 2192,
	2156, 2115, 2078, 2043, 2004, 1967, 1922, 1893, 1845, 1809, 1772, 1736, 1702, 1674, 1640, 1605,
	1566, 1536, 1509, 1479, 1452, 1423, 1388, 1362, 1332, 1304, 1289, 1266, 1250, 1228, 1206, 1180,
	1160, 1134, 1118, 1100, 1080, 1068, 1051, 1034, 1012, 1001, 980, 960, 945, 934, 916, 900,
	888, 878, 865, 852, 828, 807, 787, 770, 753, 744, 731, 722, 706, 700, 683, 676,
	671, 664, 652, 641, 634, 627, 613, 604, 591, 582, 568, 560, 552, 540, 534, 529,
	519, 509, 495, 484, 474, 467, 460, 450, 438, 427, 419, 410, 406, 399, 394, 387,
	382, 377, 372, 366, 359, 353, 348, 343, 337, 333, 328, 321, 315, 309, 303, 298,
	293, 287, 284, 281, 277, 273, 265, 261, 255, 251, 247, 241, 240, 235, 229, 218 
};
int get_move_importance(const int ply)
{
	return move_importance[min(ply, 511)];
}
//?
enum TimeType{
	OptimumTime,
	MaxTime
};
//?
template<TimeType T>
int remaining(const int mytime, const int moves_to_go, const int current_ply, const int slow_mover)
{
	const float tmax_ratio = (T == OptimumTime ? 1 : max_ratio);
	const float tsteal_ratio = (T == OptimumTime ? 0 : steal_ratio);

	const float this_move_importance = get_move_importance(current_ply) * slow_mover / 100;
	float other_move_importance = 0;
	for (int i = 1; i < moves_to_go; i++){
		other_move_importance += get_move_importance(current_ply + 2 * i);
	}
	const float ratio1 =
		(tmax_ratio * this_move_importance) / static_cast<float>(tmax_ratio * this_move_importance + other_move_importance);
	const float ratio2 =
		(this_move_importance + tsteal_ratio * other_move_importance) / static_cast<float>(this_move_importance + other_move_importance);
	return static_cast<int>(mytime * min(ratio1, ratio2));
}
void TimeManager::pv_instability(const int curr_changes, const int prev_change)
{
	m_unstable_pv_extra_time =
		curr_changes * (m_optimum_search_time / 2) + prev_change * (m_optimum_search_time / 3);
}
//最終的にm_optimum_search_timeとm_maximum_search_timeを決めている。m_unstable_pv_extra_timeはprivate変数としてあるが
//init関数の中でゼロクリアされているだけでなんら寄与していない。
//このinit関数はsearcher::think関数からのみ呼び出され探索のたびごと設定される.
void TimeManager::init(LimitsType& limits, const int current_ply, const Color us, Searcher* sech)
{
	const int emergency_move_horizon = options["Emergency_Move_Horizon"];
	const int emergency_base_time = options["Emergency_Base_Time"];
	const int emergency_move_time = options["Emergency_Move_Time"];
	const int min_thinking_time = options["Minimum_Thinking_Time"];
	const int slow_mover = options["Slow_Mover"];
	m_unstable_pv_extra_time = 0;
	m_optimum_search_time = m_maximum_search_time = limits.time[us];
	for (int hyp_mtg = 1; hyp_mtg <= (limits.moves_to_go ? min(limits.moves_to_go, move_horizon) : move_horizon); hyp_mtg++){
		int hyp_my_time = limits.time[us] + limits.increment[us] * (hyp_mtg - 1) - emergency_base_time - emergency_move_time + min(hyp_mtg, emergency_move_horizon);
		hyp_my_time = max(hyp_my_time, 0);
		const int t1 = min_thinking_time + remaining<OptimumTime>(hyp_my_time, hyp_mtg, current_ply, slow_mover);
		const int t2 = min_thinking_time + remaining<MaxTime>(hyp_my_time, hyp_mtg, current_ply, slow_mover);
		m_optimum_search_time = min(m_optimum_search_time, t1);
		m_maximum_search_time = min(m_maximum_search_time, t2);
	}
	if (options["USI_Ponder"]){
		m_optimum_search_time += m_optimum_search_time / 4;
	}
	m_optimum_search_time = max(m_optimum_search_time, min_thinking_time);
	m_optimum_search_time = min(m_maximum_search_time, m_maximum_search_time);
	if (limits.move_time != 0){
		if (m_optimum_search_time < limits.move_time){
			m_optimum_search_time = min(limits.time[us], limits.move_time);
		}
		if (m_maximum_search_time < limits.move_time){
			m_maximum_search_time = min(limits.time[us], limits.move_time);
		}
		m_optimum_search_time += limits.move_time;
		m_maximum_search_time += limits.move_time;
		if (limits.time[us] != 0){
			limits.move_time = 0;
		}
	}
	sync_cout << "info string optimum_search_time = " << m_optimum_search_time << sync_endl;
	sync_cout << "info string maximum_search_time = " << m_maximum_search_time << sync_endl;
}

#ifdef _DEBUG
TEST(timemanager, init)
{
	
	std::vector<Move> moves;
	LimitsType limits;
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");
	
	Searcher *sech = new Searcher;
	
	sech->init();
	/*
	BitBoardns::init();
	Positionns::init();		//Positionns::init()はBitBoardの設定値を使用して初期化しているので順序変更禁止
	
	Position pos(ss, sech->threads.main_thread(), sech);
	limits.depth = 3;
	limits.infinite = false;
	limits.ponder = false;
	limits.time[Black] = 30000;
	limits.time[White] = 30000;
	limits.nodes = 0;	//デフォルト値がなかったので0とした
	limits.move_time = 0;	//デフォルト値がなかったので0とした
	limits.moves_to_go = 0;	//デフォルト値がなかったので0とした
	limits.increment[Black] = 0;	//デフォルト値がなかったので0とした,usiコマンドインターフェイスでも値を設定していない、なんのために存在する
	limits.increment[White] = 0;	//デフォルト値がなかったので0とした,usiコマンドインターフェイスでも値を設定していない、なんのために存在する
	TimeManager time_manager;
	time_manager.init(limits, 3, Black, sech);
	
	cout << "available_time: " << time_manager.available_time() << endl;
	cout << "maximum_time: " << time_manager.maximum_time() << endl;
	*/
	sech->signals.stop = true;
	sech->threads.main_thread()->thinking = true;
	sech->threads.main_thread()->notify_one();
	sech->threads.exit();

	delete sech;
}
#endif