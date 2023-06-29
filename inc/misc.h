#ifndef __MISC_H__
#define __MISC_H__

#include <time.h>

long long min_ll(long long a, long long b);
long long max_ll(long long a, long long b);
long long rand_ll(long long l, long long r); //random [l,min(r,l+RAND_MAX)]
void sleep_ms(long long milliseconds);
struct timespec get_time();
struct timespec timespec_plus_ms(struct timespec t, long long ms);
long long timespec_to_ms(struct timespec t);

#endif