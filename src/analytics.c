#include <stdio.h>
#include <pthread.h>
#include "analytics.h"
#include "client.h"
#include "ingredient.h"
#include "misc.h"

static Analytics a;
pthread_mutex_t mutexAnalytics;

void analytics_init(){
  a.clientsServed = 0;
  a.clientsLost = 0;

  a.clientsWaitMin = 0;
  a.clientsWaitMax = 0;
  a.clientsWaitAvg = 0;

  a.clientsServMin = 0;
  a.clientsServMax = 0;
  a.clientsServAvg = 0;

  a.clientsTotalMin = 0;
  a.clientsTotalMax = 0;
  a.clientsTotalAvg = 0;

  for(int ing=0;ing<KINGREDIENT_QT;ing++)
    for(int aI=0;aI<3;aI++)
      a.ingredientsQt[ing][aI] = 0;

  pthread_mutex_init(&mutexAnalytics, NULL);
}

void analytics_client_served(Client *client){
  pthread_mutex_lock(&mutexAnalytics);
  a.clientsServed++;
  
  a.clientsWaitMin = min_ll(a.clientsWaitMin, timespec_to_ms(client_time_wait(client)));
  a.clientsWaitMax = max_ll(a.clientsWaitMax, timespec_to_ms(client_time_wait(client)));
  a.clientsWaitAvg = (a.clientsWaitAvg * (a.clientsServed-1) + timespec_to_ms(client_time_wait(client))) / a.clientsServed;

  a.clientsServMin = min_ll(a.clientsServMin, timespec_to_ms(client_time_service(client)));
  a.clientsServMax = max_ll(a.clientsServMax, timespec_to_ms(client_time_service(client)));
  a.clientsServAvg = (a.clientsServAvg * (a.clientsServed-1) + timespec_to_ms(client_time_service(client))) / a.clientsServed;

  a.clientsTotalMin = min_ll(a.clientsTotalMin, timespec_to_ms(client_time_total(client)));
  a.clientsTotalMax = max_ll(a.clientsTotalMax, timespec_to_ms(client_time_total(client)));
  a.clientsTotalAvg = (a.clientsTotalAvg * (a.clientsServed-1) + timespec_to_ms(client_time_total(client))) / a.clientsServed;
  pthread_mutex_unlock(&mutexAnalytics);
}

void analytics_client_lost(){
  pthread_mutex_lock(&mutexAnalytics);
  a.clientsLost++;
  pthread_mutex_unlock(&mutexAnalytics);
}

void analytics_ingredient(enum Ingredient ing, enum AnalyticsIngredient aI){
  pthread_mutex_lock(&mutexAnalytics);
  a.ingredientsQt[ing][aI]++;
  pthread_mutex_unlock(&mutexAnalytics);
}

Analytics analytics_get(){
  return a;
}