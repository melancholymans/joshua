#include "types.h"
#include "common.h"

//Global object
extern USI::OptionsMap options;

Thread::Thread(Searcher* sech)
{
	searcher = sech;
	exit = false;
	searching = false;
	split_point_size = 0;
	max_ply = 0;
	active_split_point = nullptr;
	active_position = nullptr;
	idx = sech->threads.size();
	handle = std::thread(&Thread::idle_loop, this);
}

Thread::~Thread()
{
	exit = true;
	notify_one();
	handle.join();
}
//TimerThreadクラスのマルチスレッド対象関数
void TimerThread::idle_loop()
{
	while (!exit){
		{
			std::unique_lock<std::mutex> lock(sleep_lock);  //sleep_lockはThreadクラスのmutex変数
			if (!exit){
				sleep_cond.wait_for(lock, std::chrono::milliseconds(msec ? msec : INT_MAX));
			}
		}
		if (msec){
			searcher->check_time();
		}
	}
}
//MainThreadクラスのマルチスレッド対象関数
//idle_loop関数はthink関数を呼び出す１歩手前の関数で
void MainThread::idle_loop()
{
	while (true){
		std::unique_lock<std::mutex> lock(sleep_lock);  //sleep_lockはThreadクラスのmutex変数
		thinking = false;
		while (!thinking && !exit){
			searcher->threads.sleep_cond.notify_one();
			sleep_cond.wait(lock);
		}
	}
	if (exit){
		return;
	}
	searching = true;
	searcher->think();
	_ASSERT(searching);
	searching = false;
}

void Thread::notify_one()
{
	std::unique_lock<std::mutex > lock(sleep_lock);
	sleep_cond.notify_one();
}
bool Thread::cutoff_occurred() const
{
	for (SplitPoint* sp = active_split_point; sp != nullptr; sp = sp->parent_split_point){
		if (sp->cut_node){
			return true;
		}
	}
	return false;
}
bool Thread::is_available_to(Thread* master) const
{
	if (searching){
		return false;
	}
	const int sp_count = split_point_size;
	return !sp_count || (split_points[sp_count - 1].slaves_mask & (uint64_t(1) << master->idx));
}
void Thread::wait_for(volatile const bool& b)
{
	std::unique_lock<std::mutex> lock(sleep_lock);
	sleep_cond.wait(lock, [&]{return b; });
}
void ThreadPool::init(Searcher* sech)
{
	sleep_while_idle = true;
	m_timer = new TimerThread(sech);
	push_back(new MainThread(sech));
	read_usi_options(sech);
}
ThreadPool::~ThreadPool()
{
	delete m_timer;
	for (auto elem : *this){
		delete elem;
	}
}
void ThreadPool::read_usi_options(Searcher* sech)
{
	max_thread_per_split_point = options["Max_Threads_per_Split_point"];
	m_minimum_split_depth = options["Min_Split_Depth"];
	const size_t requested = options["Threads"];
	_ASSERT(requested > 0);
	while (size() < requested){
		push_back(new Thread(sech));
	}
	while (requested < size()){
		delete back();
		pop_back();
	}
}
Thread* ThreadPool::available_slave(Thread* master) const
{
	for (auto elem : *this){
		if (elem->is_available_to(master)){
			return elem;
		}
	}
	return nullptr;
}
void ThreadPool::set_timer(const int msec)
{
	m_timer->max_ply = msec;
	m_timer->notify_one();
}
void ThreadPool::wait_for_think_finised()
{
	MainThread* t = main_thread();
	std::unique_lock<std::mutex> lock(t->sleep_lock);
	sleep_cond.wait(lock, [&]{return !(t->thinking); });
}
//go関数からのみ呼ばれている
//pos.get_searcher()->threads.start_thinking(pos, limits, moves)のように呼ばれる
void ThreadPool::start_thinking(const Position& pos, const LimitsType& limits, const vector<Move>& search_moves)
{
	wait_for_think_finised();
	ptime_init(&pos.get_searcher()->search_timer);
	pos.get_searcher()->signals.stop_on_ponder_hit = pos.get_searcher()->signals.first_root_move =
		pos.get_searcher()->signals.stop = pos.get_searcher()->signals.failed_low_at_root = false;
	pos.get_searcher()->root_position = pos;
	pos.get_searcher()->limits = limits;
	pos.get_searcher()->root_moves.clear();
	MoveType MT = Legal;
	Color c = pos.get_color_turn();
	Color them = over_turn(c);
	Square ksq = pos.get_king_square(c);
	//王手をかけている駒があればMTはEvasion
	if (!pos.attackers_to(them, ksq, pos.all_bb()).is_not_zero()){
		MT = Evasion;
	}
	for (MoveList<Legal> ml(pos); !ml.end(); ++ml){
		pos.get_searcher()->root_moves.push_back(RootMove(ml.move()));
	}
	main_thread()->thinking = true;
	main_thread()->notify_one();
}

#ifdef _DEBUG
TEST(thread, start_thinking)
{
	std::vector<Move> moves;
	LimitsType limits;
	//問題図は将棋世界６月付録新手ポカ妙手選No6より
	string ss("ln1g1p1+R1/3kb1+S2/2p1p1n1p/p2s1g3/1nL3p2/PKP1S4/1P1pP1P1P/4G4/L1S3b+pL b R2Pgn2p 1");

	Searcher *sech = new Searcher;
	sech->init();
	BitBoardns::init();
	Positionns::init();		//Positionns::init()はBitBoardの設定値を使用して初期化しているので順序変更禁止

	Position pos(ss,sech->threads.main_thread(),sech);
	limits.depth = 3;
	limits.infinite = false;
	limits.ponder = false;
	limits.time[Black] = 30000;
	limits.time[White] = 30000;
	pos.get_searcher()->threads.start_thinking(pos, limits, moves);
}
#endif

