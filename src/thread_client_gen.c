#include <pthread.h>
#include "hub.h"
#include "log.h"
#include "misc.h"
#include "thread_client_gen.h"

void* client_thread_routine(void *arg){
  (void)arg; //unused
  log_i("<CLIENT> Client thread started.\n");

  while(hub_client_is_generating()){
    long long sleepTimeMs = rand_ll(kClientGenMsL, kClientGenMsR);
    log_d("<CLIENT> Sleeping %lld ms.\n", sleepTimeMs);
    sleep_ms(sleepTimeMs);
    log_d("<CLIENT> Waking up.\n");
    
    if(hub_client_add()){
      log_i("<CLIENT> Client added. Total: %zu\n", hub_client_count());
    }
    else{
      log_i("<CLIENT> Client not added (MAX %d). Total: %zu\n", KCLIENT_MAX, hub_client_count());
    }
  };

  log_i("<CLIENT> Client thread terminated.\n");

  return NULL;
}

void client_thread_create(pthread_t *client_thread){
  pthread_create(client_thread, NULL, client_thread_routine, NULL);
}
