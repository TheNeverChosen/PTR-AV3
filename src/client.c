#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include "central.h"
#include "client.h"
#include "log.h"
#include "misc.h"
#include "queue.h"

//==========Client Struct==========//
Client* client_new(time_t timeArrival){
  Client* client = (Client*) malloc(sizeof(Client));
  client->timeArrival = timeArrival;
  client->timeServiceStart = 0;
  client->timeServiceEnd = 0;
  return client;
}

void client_delete(Client* client){
  free(client);
}

time_t client_time_wait(Client* client){
  return client->timeServiceStart - client->timeArrival;
}

time_t client_time_service(Client* client){
  return client->timeServiceEnd - client->timeServiceStart;
}

time_t client_total_time(Client* client){
  return client->timeServiceEnd - client->timeArrival;
}

//====================CLIENTS====================
static pthread_mutex_t mutexClient; //init
static pthread_cond_t condClientAvailable; //init
static Queue *qClient = NULL;      //init

void client_init(){
  pthread_mutex_init(&mutexClient, NULL);
  pthread_cond_init(&condClientAvailable, NULL);
  qClient = queue_new();
}

int client_lock(){
  return pthread_mutex_lock(&mutexClient);
}

int client_unlock(){
  return pthread_mutex_unlock(&mutexClient);
}

int client_wait_available(){
  return pthread_cond_wait(&condClientAvailable, &mutexClient);
}

int client_signal_available(){
  return pthread_cond_signal(&condClientAvailable);
}

size_t client_get_count(){
  return queue_size(qClient);
}

bool client_add(){
  if(client_get_count()>=kClientMax) return false;
  queue_push(qClient, client_new(time(NULL)));
  if(client_get_count()==1) client_signal_available();
  return true;
}

bool client_remove(){
  if(client_get_count()<=0) return false;
  queue_pop(qClient, NULL);
  return true;
}

Client* client_front(){
  return queue_front(qClient);
}

//==========Client Thread==========//
void* client_thread_routine(void *arg){
  (void)arg; //unused
  log_i("<CLIENT> Client thread started.\n");

  do{
    long long sleepTimeMs = rand_ll(kClientGenMsL, kClientGenMsR);
    log_d("<CLIENT> Sleeping %lld ms.\n", sleepTimeMs);
    sleep_ms(sleepTimeMs);
    log_d("<CLIENT> Waking up.\n");
    
    if(!central_is_running()) break;
    client_lock();
    if(client_add())
      log_i("<CLIENT> Client added. Total: %zu\n", client_get_count());
    else
      log_i("<CLIENT> Client not added (MAX %zu). Total: %zu\n", kClientMax, client_get_count());
    client_unlock();
  }while(central_is_running());

  log_i("<CLIENT> Client thread terminated.\n");

  return NULL;
}

void client_thread_create(pthread_t *client_thread){
  pthread_create(client_thread, NULL, client_thread_routine, NULL);
}
