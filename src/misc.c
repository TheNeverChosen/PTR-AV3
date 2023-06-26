#include <stdlib.h>
#include "misc.h"

#ifdef _WIN32 // Windows
    #include <windows.h>
#else         // Linux
    #include <time.h>
#endif

long long min_ll(long long a, long long b){
  return a<b?a:b;
}
long long max_ll(long long a, long long b){
  return a>b?a:b;
}

long long rand_ll(long long l, long long r){
  r=min_ll(r, l+RAND_MAX);
  if(l>r) return 0;
  return ((long long)rand()) % (r-l+1)+l;
}

void sleep_ms(long long milliseconds) {
  #ifdef _WIN32
      Sleep(milliseconds);
  #else
    struct timespec req;
    req.tv_sec = milliseconds / 1000; // seconds
    req.tv_nsec = milliseconds % 1000 * 1000000; // nanoseconds
    nanosleep(&req, NULL);
  #endif
}