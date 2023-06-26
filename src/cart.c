#include <pthread.h>
#include "cart.h"
#include "central.h"
#include "misc.h"
#include "log.h"

void* cart_thread_routine(void *arg){
  (void)arg; //unused

  log_i("<CART> Cart thread started.\n");

  do{
    client_lock();
    Client *client;
    
    while(central_is_running() && (client = client_front())==NULL){
      log_i("<CART> No client to service. Total: %zu\n", client_get_count());
      log_i("<CART> Waiting for client arrival...\n");
      client_wait_available(); //wait for client
      log_i("<CART> Waking up\n");
    }
    if(!central_is_running()){
      client_unlock(); break;
    }

    client_remove(); //picking client from queue to serve

    client_unlock(); //releasing client interface


    long long sleepTimeMs = rand_ll(kCartServMsL, kCartServMsR);
    log_i("<CART> Client being serviced. ServTime: %lld ms\n", sleepTimeMs);
    client->timeServiceStart = time(NULL);
    sleep_ms(sleepTimeMs);
    client->timeServiceEnd = client->timeServiceStart + sleepTimeMs;
    log_i("<CART> Client serviced: [Arrival: %lld, ServStart: %lld, ServEnd: %lld]\n",
      (long long) client->timeArrival,
      (long long) client->timeServiceStart,
      (long long) client->timeServiceEnd);
  }while(central_is_running());

  log_i("<CART> Cart thread terminated.\n");

  return NULL;
}

void cart_thread_create(pthread_t *cart_thread){
  pthread_create(cart_thread, NULL, cart_thread_routine, NULL);
}
