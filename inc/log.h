#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#ifdef WARNING
  #define ERROR
#endif

#ifdef INFO
  #define ERROR
  #define WARNING
#endif

#ifdef DEBUG
  #define ERROR
  #define WARNING
  #define INFO
#endif

#ifdef VERBOSE
  #define ERROR
  #define WARNING
  #define INFO
  #define DEBUG
#endif

#ifdef ERROR
  #define log_e(...) printf("ERROR: " __VA_ARGS__)
#else
  #define log_e(...)
#endif

#ifdef WARNING
  #define log_w(...) printf("WARNING: " __VA_ARGS__)
#else
  #define log_w(...)
#endif

#ifdef INFO
  #define log_i(...) printf("INFO: " __VA_ARGS__)
#else
  #define log_i(...)
#endif

#ifdef DEBUG
  #define log_d(...) printf("DEBUG: " __VA_ARGS__)
#else
  #define log_d(...)
#endif

#ifdef VERBOSE
  #define log_v(...) printf("VERBOSE: " __VA_ARGS__)
#else
  #define log_v(...)
#endif

#endif