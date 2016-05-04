#include "types.h"
#include "common.h"

//Global object
extern OptionsMap options;
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
//Searcher* Searcher::thisptr;


void Searcher::init()
{
	USI::init(options);
	threads.init(this);
	TT.set_size(options["USI_Hash"]);
}
template<NodeType NT>
int Searcher::search(Position& pos, SearchStack* ss, int alpha, int beta, const int depth, const bool cut_node)
{
	return 1;
}
void Searcher::think()
{
    return;
}
//定期的に探索ツリーの展開ノード数、消費された思考時間をを超えたら探索を強制停止させる。
void Searcher::check_time()
{
	return;
}
void Thread::idle_loop()
{
	SplitPoint* this_sp = split_point_size ? active_split_point : nullptr;
	while (true){
		while ((!searching && searcher->threads.sleep_while_idle) || exit){
			if (exit){
				return;
			}
			std::unique_lock<std::mutex> lock(sleep_lock);
			if (this_sp != nullptr && !this_sp->slaves_mask){
				break;
			}
			if (!searching && !exit){
				sleep_cond.wait(lock);
			}
		}
		if (searching){
			searcher->threads.mutex.lock();
			SplitPoint* sp = active_split_point;
			searcher->threads.mutex.unlock();
			SearchStack ss[MAX_PLY + 2];
			Position pos(*sp->pos, this);
			memcpy(ss, sp->ss - 1, 4 * sizeof(SearchStack));
			(ss + 1)->split_point = sp;
			sp->mutex.lock();
			active_position = &pos;
			switch (sp->node_type){
			case Root:
				searcher->search<SplitPointRoot>(pos, ss + 1, sp->alpha, sp->beta, sp->depth, sp->cut_node);
				break;
			case PV:
				searcher->search<SplitPointPV>(pos, ss + 1, sp->alpha, sp->beta, sp->depth, sp->cut_node);
				break;
			case NonPV:
				searcher->search<SplitPointNonPV>(pos, ss + 1, sp->alpha, sp->beta, sp->depth, sp->cut_node);
				break;
			default:
				_ASSERT(false);
			}
			searching = false;
			active_position = nullptr;
			sp->slaves_mask ^= uint64_t(1) << idx;
			sp->nodes += pos.nodes_searched();
			if (searcher->threads.sleep_while_idle && this != sp->masterThread && !sp->slaves_mask){
				sp->masterThread->notify_one();
			}
			sp->mutex.unlock();
		}
		if (this_sp != nullptr && !this_sp->slaves_mask){
			this_sp->mutex.lock();
			const bool finished = !this_sp->slaves_mask;
			this_sp->mutex.unlock();
			if (finished){
				return;
			}
		}
	}
}
TEST(serach,search_root)
{
}
