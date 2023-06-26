#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include "cart.h"
#include "central.h"
#include "client.h"
#include "stop.h"

//====================GENERAL====================
static bool running; //init
static pthread_mutex_t mutexRunning; //init

static void central_set_running(bool val){
  pthread_mutex_lock(&mutexRunning);
  running = val;
  pthread_mutex_unlock(&mutexRunning);
}

bool central_is_running(){
  pthread_mutex_lock(&mutexRunning);
  bool val = running;
  pthread_mutex_unlock(&mutexRunning);
  return val;
}

void central_init(){
  srand(time(NULL));
  client_init();
  pthread_mutex_init(&mutexRunning, NULL);
  running = false;
}

void central_start(){
  pthread_t thread_stop, thread_client, thread_cart;
  central_set_running(true);

  stop_thread_create(&thread_stop);
  cart_thread_create(&thread_cart);
  client_thread_create(&thread_client);

  pthread_join(thread_stop, NULL);
  pthread_join(thread_client, NULL);
  pthread_join(thread_cart, NULL);
}

void central_stop(){
  central_set_running(false);
}
