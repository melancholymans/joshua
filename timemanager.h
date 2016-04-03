#if !defined(TIME_MANAGER_H_INCLUDE)
#define TIME_MANAGER_H_INCLUDE

#include "types.h"

struct LimitsType;

//ƒNƒ‰ƒX‚Ì”ç‚¾‚¯’è‹`
class TimeManager{
public:
	void init(LimitsType& limits, const int current_ply, const Color us, Searcher* sech);
	void pv_instability(const int curr_chages, const int prev_changes);
	int available_time() const
	{
		return m_optimum_search_time + m_unstable_pv_extra_time;
	}
	int maximum_time() const
	{
		return m_maximum_search_time;
	}
private:
	int m_optimum_search_time;
	int m_unstable_pv_extra_time;
	int m_maximum_search_time;
};
#endif