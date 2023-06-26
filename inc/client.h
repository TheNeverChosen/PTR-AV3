#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <pthread.h>
#include <time.h>

//==========Client Struct==========//
typedef struct{
  time_t timeArrival, timeServiceStart, timeServiceEnd;
}Client;

Client* client_new(time_t timeArrival);
void client_delete(Client* client);
time_t client_time_wait(Client* client);
time_t client_time_service(Client* client);
time_t client_total_time(Client* client);

//====================CLIENTS====================
static const size_t kClientMax = 100;   //max number of clients

void client_init();             //initialize the client  
int client_lock();              //lock the client mutex
int client_unlock();            //unlock the client mutex
int client_wait_available();    //wait available clients
int client_signal_available();  //signal available clients
size_t client_get_count();      //get the number of clients
bool client_add();              //add one client
bool client_remove();           //remove one client
Client* client_front();         //get the first client

//==========Client Thread==========//

//interval between client generation [kClientGenMsL, kClientGenMsR] ms
static const unsigned int kClientGenMsL = 100, kClientGenMsR=500; 

void client_thread_create(pthread_t *client_thread);

#endif