#include <stdlib.h>
#include <time.h>
#include "misc.h"

long long min_ll(long long a, long long b){
  return a<b?a:b;
}
long long max_ll(long long a, long long b){
  return a>b?a:b;
}

long long rand_ll(long long l, long long r){
  r=min_ll(r, l+RAND_MAX);
  if(l>r) return 0;
  return (((long long)rand()) % (r-l+1)) + l;
}

void sleep_ms(long long milliseconds) {
  struct timespec req;
  req.tv_sec = milliseconds / 1000; // seconds
  req.tv_nsec = (milliseconds % 1000) * 1000000; // nanoseconds
  nanosleep(&req, NULL);
}

struct timespec timespec_plus_ms(struct timespec t, long long ms){
  struct timespec res;
  res.tv_sec = t.tv_sec + ms/1000;
  res.tv_nsec = t.tv_nsec + (ms%1000)*1000000;
  if(res.tv_nsec >= 1000000000LL){
    res.tv_sec += res.tv_nsec/1000000000LL;
    res.tv_nsec %= 1000000000LL;
  }
  return res;
}

struct timespec get_time(){
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return t;
}

long long timespec_to_ms(struct timespec t){
  return t.tv_sec*1000 + t.tv_nsec/1000000;
}