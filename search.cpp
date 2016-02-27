#include "types.h"
#include "position.h"
#include "movegen.h"
#include "thread.h"
#include "search.h"
#ifdef _DEBUG
	#include <gtest\gtest.h>
#endif

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

//ThreadPool Searcher::threads;
//OptionsMap Searcher::options;
Searcher* Searcher::thisptr;

/*
void Searcher::init()
{
	options.init(thisptr);
}
*/
/*
bool think(Position &pos)
{
    ptime_t aPT;
    int ply = 0;
    
    //nps計測
    stats.search_node = 0;
    ptime_init(&aPT);
    Move m = search_root(pos,ply);
    if(m == 0){
        //合法手がなければ投了
        print_board(pos);
        cout << "bestmove resign" << endl;
        return false;
    }
    else{
        //指し手を実行して、その手を将棋所に送信
        short dummy[8];
        DoMove(pos.turn,pos,m,dummy);
        print_board(pos);
        cout << "bestmove " << string_from_move(m) << endl;
    }
    //局面データを表示
    double eapsed_time = ptime_now(&aPT);  //sec
    printf("eapsed_time=%.2f(sec) \n",eapsed_time);
    printf("search node=%dk \n",stats.search_node/1000);
    printf("nps=%.2fk \n",double(stats.search_node/(eapsed_time*1000)));
    print_board(pos);
    return true;
}

Move search_root(Position &pos,int ply)
{
    Move m;
    backup_info_t bfo;

    short *mf = next_modify[ply].next_dirty = next_modify[ply].last_dirty;
    //全ての手を生成
    Move *ml = next_move[ply].next_move = next_move[ply].last_move;
    next_move[ply+1].last_move = next_move[ply].last_move = generate_moves(pos,ml);
    int max_score = -2147483648;
    //Move構造体をDoMove関数に渡して局面更新、もし合法手が0なら（王手を避ける手がないなど）このforループをパスする
    for(;ml != next_move[ply].last_move;ml++){
        bfo.material = sech.material;
        next_modify[ply+1].last_dirty=next_modify[ply].last_dirty = DoMove(pos.turn,pos,*ml,mf);
        int score = search_min(pos,ply+1);
        undo_move(pos,ply);
        sech.material = bfo.material;
        if(max_score < score){
            max_score = score;
            m = *ml;
        }
    }
    return m;
}

TEST(serach,search_root)
{
    //Q1の局面でテスト
    string expect ="lR1B3nl/2gp5/ngk1+BspPp/1s2p2p1/p4S3/1Pp6/P5P1P/LGG6/KN5NL b P5psr 1";
    from_sfen(expect);
    game_init(root_position);
    think(root_position);
}
*/