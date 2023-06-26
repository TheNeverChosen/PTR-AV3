#include <pthread.h>
#include <stdio.h>
#include "central.h"
#include "log.h"
#include "stop.h"

void* stop_thread_routine(void *arg){
  (void)arg; //unused

  log_i("<STOP> Stop thread started.\n");
  log_i("<STOP> Press enter to stop.\n");

  getchar(); //wait for enter
  central_stop();
  
  log_i("<STOP> Central stopped.\n");
  log_i("<STOP> Stop thread terminated.\n");
  return NULL;
}

void stop_thread_create(pthread_t *stop_thread){
  pthread_create(stop_thread, NULL, stop_thread_routine, NULL);
}
