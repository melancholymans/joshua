#include "types.h"
#include "position.h"
#include "movegen.h"
#include "thread.h"
#include "search.h"
#include "usi.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

//Global object
extern USI::OptionsMap options;
extern TranspositionTable TT;

//searcher struct static member
volatile SignalsType Searcher::signals;
LimitsType Searcher::limits;
vector<Move> Searcher::search_moves;
ptime_t Searcher::search_timer;
StateStackPtr Searcher::setup_states;
vector<RootMove> Searcher::root_moves;
size_t Searcher::pv_size;
size_t Searcher::pv_idx;
TimeManager Searcher::time_manager;
int Searcher::best_move_chages;
History Searcher::history;
Gains Searcher::gains;
//TranspositionTable Searcher::tt;
Position Searcher::root_position;
ThreadPool Searcher::threads;
//OptionsMap Searcher::options;
//Searcher* Searcher::thisptr;


void Searcher::init()
{
	USI::init(options);
	threads.init(this);
	TT.set_size(options["USI_Hash"]);
}

void Searcher::think()
{
    return;
}
void Searcher::check_time()
{
	return;
}
void Thread::idle_loop()
{
	return;
}
TEST(serach,search_root)
{
}
