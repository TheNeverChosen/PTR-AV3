#ifndef __MISC_H__
#define __MISC_H__

long long min_ll(long long a, long long b);
long long max_ll(long long a, long long b);
long long rand_ll(long long l, long long r); //random [l,min(r,l+RAND_MAX)]
void sleep_ms(long long milliseconds);

#endif