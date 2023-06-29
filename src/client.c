#include <stdlib.h>
#include <time.h>
#include "client.h"

//==========Client Struct==========//
Client* client_new(struct timespec timeArrival){
  Client* client = (Client*) calloc(1, sizeof(Client));
  client->timeArrival = timeArrival;
  return client;
}

void client_delete(Client* client){
  free(client);
}

struct timespec client_time_wait(Client* client){
  struct timespec t;
  t.tv_sec = client->timeServiceStart.tv_sec - client->timeArrival.tv_sec;
  t.tv_nsec = client->timeServiceStart.tv_nsec - client->timeArrival.tv_nsec;
  return t;
}

struct timespec client_time_service(Client* client){
  struct timespec t;
  t.tv_sec = client->timeServiceEnd.tv_sec - client->timeServiceStart.tv_sec;
  t.tv_nsec = client->timeServiceEnd.tv_nsec - client->timeServiceStart.tv_nsec;
  return t;
}

struct timespec client_time_total(Client* client){
  struct timespec t;
  t.tv_sec = client->timeServiceEnd.tv_sec - client->timeArrival.tv_sec;
  t.tv_nsec = client->timeServiceEnd.tv_nsec - client->timeArrival.tv_nsec;
  return t;
}