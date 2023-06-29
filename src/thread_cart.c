#include <pthread.h>
#include "analytics.h"
#include "thread_cart.h"
#include "hub.h"
#include "ingredient.h"
#include "misc.h"
#include "log.h"

bool ingredient_wait_all(){
  for(int i=0;i<KINGREDIENT_QT;i++)
    if(!hub_ingredient_wait(i)) return false;
  return true;
}
bool ingredient_remove_all(){
  for(int i=0;i<KINGREDIENT_QT;i++)
    if(!hub_ingredient_remove(i)) return false;
  return true;
}

void* cart_thread_routine(void *arg){
  (void)arg; //unused

  log_i("<CART> Cart thread started.\n");

  while(hub_client_wait() && ingredient_wait_all()){
    Client *client = hub_client_remove(); //picking client from queue to serve
    if(client == NULL){
      log_e("<CART> Client is NULL. This should not happen.\n");
      continue;
    }
    if(!ingredient_remove_all()){
      log_e("<CART> Ingredient is NULL. This should not happen.\n");
      continue;
    }

    //serving client
    long long sleepTimeMs = rand_ll(kCartServMsL, kCartServMsR);
    log_i("<CART> Client being serviced. ServTime: %lld ms\n", sleepTimeMs);
    client->timeServiceStart = get_time();
    sleep_ms(sleepTimeMs);
    client->timeServiceEnd = timespec_plus_ms(client->timeServiceStart, sleepTimeMs);
    log_i("<CART> Client serviced: [Arrival: %lld, ServStart: %lld, ServEnd: %lld]\n",
      timespec_to_ms(client->timeArrival),
      timespec_to_ms(client->timeServiceStart),
      timespec_to_ms(client->timeServiceEnd));
    
    //analytics and freeing client
    analytics_client_served(client);
    free(client);
  };

  log_i("<CART> Cart thread terminated.\n");

  hub_cart_stop();

  return NULL;
}

void cart_thread_create(pthread_t *cart_thread){
  pthread_create(cart_thread, NULL, cart_thread_routine, NULL);
}
