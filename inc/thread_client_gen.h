#ifndef __THREAD_CLIENT_GEN_H__
#define __THREAD_CLIENT_GEN_H__

#include <pthread.h>

//interval between client generation [kClientGenMsL, kClientGenMsR] ms
static const unsigned int kClientGenMsL = 50, kClientGenMsR=500; 

void client_thread_create(pthread_t *client_thread);

#endif