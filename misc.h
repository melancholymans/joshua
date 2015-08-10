#if !defined(MISC_H_INCLUDE)
#define MISC_H_INCLUDE
#include <fstream>
#include <time.h>

typedef clock_t ptime_t;

void ptime_init(ptime_t *iPT);
double ptime_now(const ptime_t *iPT);

/*
sync_cout,sync_endlで出力したい文字列を挟んで使う
*/
enum SyncCout { io_lock, io_unlock };
std::ostream& operator<<(std::ostream&, SyncCout);
#define sync_cout std::cout << io_lock
#define sync_endl std::endl << io_unlock

#endif
