#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <time.h>
typedef struct{
  struct timespec timeArrival, timeServiceStart, timeServiceEnd;
}Client;

Client* client_new(struct timespec timeArrival);
void client_delete(Client* client);
struct timespec client_time_wait(Client* client);
struct timespec client_time_service(Client* client);
struct timespec client_time_total(Client* client);

#endif