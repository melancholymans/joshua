#if !defined(THREAD_H_INCLUDE)
#define THREAD_H_INCLUDE
#include <string.h>
#include <thread>
#include <mutex>
#include "types.h"
#include "position.h"

//定数宣言
const int MAX_THREADS = 64;
const int MAX_SPLIT_POINT_PER_THREAD = 8;
//先行宣言
struct Thread;
struct SearchStack;
class MovePicker;
//探索ルーチンの型（ルート局面なのか、主探索スレッドなのか、分割探索スレッドなのか）
enum NodeType{
	Root, PV, NoPV, SplitPointRoot, SplitPointPV, SplitPointNonPV
};
//時間や探索深さの制限のためのフラグなどの構造体
struct LimitsType{
	LimitsType()
	{
		memset(this, 0, sizeof(LimitsType));
	}
	//不明
	bool use_time_management() const
	{
		return !(depth | nodes | move_time | static_cast<int>(infinite));
	}
	int time[ColorNum];
	int increment[ColorNum];
	int depth;
	uint32_t nodes;
	bool infinite;
	bool ponder;
};

struct SplitPoint{
	const Position* pos;
	const SearchStack* ss;
	Thread* masterThread;
	int depth;
	int beta;
	NodeType node_type;
	Move threatMove;
	bool cut_node;
	MovePicker* move_picker;
	SplitPoint* parent_split_point;
	std::mutex mutex;
	volatile uint64_t slaves_mask;
	volatile int64_t nodes;
	volatile int alpha;
	volatile int best_score;
	volatile Move best_move;
	volatile int move_count;
	volatile bool cut_off;
};
struct Thread{
	explicit Thread(Searcher* s);
	virtual ~Thread();
	virtual void idle_loop();
	void notify_one();
	bool cutoff_occurred() const;
	bool is_available_to(Thread* master) const;
	void wait_for(volatile const bool& b);

	template<bool Fake >
	void split(Position& pos, SearchStack* ss, const int alpha, int beta, int& best_score, 
		Move& bestMove, const int depth, const Move threat_move, const int move_count, 
		MovePicker& mp, const MoveType node_type, const bool cut_node);
	SplitPoint split_points[MAX_SPLIT_POINT_PER_THREAD];
	Position *active_position;
	int idx;
	int max_ply;
	std::mutex sleep_lock;
	std::condition_variable sleep_cond;
	std::thread handle;
	SplitPoint* volatile active_split_point;
	volatile int split_point_size;
	volatile bool searching;
	volatile bool exit;
	Searcher* searcher;
};
struct MainThread :public Thread{
	explicit MainThread(seracher* s) :Thread(s), thinking(true){}
	virtual void idle_loop();
	virtual bool thinking;
};
struct TimerThread :public Thread{
	explicit TimerThread(seracher* s) :Thread(s), msec(0){}
	virtual void idle_loop();
	int msec;
};
class ThreadPool :public std::vector<Thread *>
{
public:
	void init(Searcher *s);
	~ThreadPool();
	MainThread* main_thread()
	{
		return static_cast<MainThread*>((*this)[0]);
	}
	int min_split_depth() const
	{
		return m_minimum_split_depth;
	}
	TimerThread* timer_thread()
	{
		return m_timer;
	}
	void wake_up(seracher* s);
	void sleep();
	void read_usi_options(Searcher* s);
	Thread* available_slave(Thread* master) const;
	void set_timer(const int msec);
	void wait_for_thking_finised()();
	void start_thinking(const Postion& pos, const LimitsType& limits, const std::vector<Move>& serachMovers);
	bool sleep_while_idle;
	size_t max_thread_per_split_point;
	std::mutex mutex;
	std::condition_variable sleep_condd;
private:
	TimerThread* timer;
	int m_minimum_split_depth;
}
#endif