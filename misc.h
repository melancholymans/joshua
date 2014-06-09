#if !defined(MISC_H_INCLUDE)
#define MISC_H_INCLUDE

#include <time.h>

typedef clock_t ptime_t;

void ptime_init(ptime_t *iPT);
double ptime_now(const ptime_t *iPT);

#endif
