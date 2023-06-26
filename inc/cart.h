#ifndef __CART_H__
#define __CART_H__

#include <pthread.h>

//==========Cart Thread==========//

//interval between cart Service [kCartServMsMsL, kCartServMsR] ms
static const unsigned int kCartServMsL = 100, kCartServMsR=500; 

//will service the clients in client queue, if there is any
void cart_thread_create(pthread_t *cart_thread);

#endif