#include "thread.h"
#include "search.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

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
//TimerThread�N���X�̃}���`�X���b�h�Ώۊ֐�
void TimerThread::idle_loop()
{
	while (!exit){
		{
			std::unique_lock<std::mutex> lock(sleep_lock);  //sleep_lock��Thread�N���X��mutex�ϐ�
			if (!exit){
				sleep_cond.wait_for(lock, std::chrono::milliseconds(msec ? msec : INT_MAX));
			}
		}
		if (msec){
			searcher->check_time();
		}
	}
}
//MainThread�N���X�̃}���`�X���b�h�Ώۊ֐�
//idle_loop�֐���think�֐����Ăяo���P����O�̊֐���
void MainThread::idle_loop()
{
	while (true){
		std::unique_lock<std::mutex> lock(sleep_lock);  //sleep_lock��Thread�N���X��mutex�ϐ�
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
void ThreadPool::start_thinking(const Position& pos, const LimitsType& limits, const vector<Move>& search_moves)
{
	wait_for_think_finised();
    /*
    apery��position�N���X��searcher�|�C���^��ݒu���Ă���
    ����joshua�͂���ɑΉ����Ă��Ȃ��̂ł����ł͂���ȏ�ł��Ȃ�
    */
    /*
	for (MoveList)
    */
	main_thread()->thinking = true;
	main_thread()->notify_one();
}
TEST(thread, start_thinking)
{

}

