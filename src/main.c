#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "analytics.h"
#include "hub.h"
#include "ingredient.h"
#include "thread_cart.h"
#include "thread_client_gen.h"
#include "thread_supplier.h"

void init(){
  srand(time(NULL));
  analytics_init();
  hub_client_init();
  for(int i=0;i<KINGREDIENT_QT;i++)
    hub_ingredient_init(i);
  hub_cart_init();
}

void run(){
  pthread_t thread_client, thread_supplier[KINGREDIENT_QT], thread_cart;

  client_thread_create(&thread_client);
  for(int i=0;i<KINGREDIENT_QT;i++)
    supplier_thread_create(&thread_supplier[i], i);
  cart_thread_create(&thread_cart);

  pthread_join(thread_client, NULL);
  for(int i=0;i<KINGREDIENT_QT;i++)
    pthread_join(thread_supplier[i], NULL);
  pthread_join(thread_cart, NULL);
}

void print_analytics(Analytics a){
  printf(
    "Analytics:\n"
    "  Client Generation:\n"
    "    Generated: %lld\n"
    "    Served: %lld\n"
    "    Lost: %lld\n"
    "  Client times:\n"
    "    Waiting:\n"
    "      Min: %lld ms\n"
    "      Max: %lld ms\n"
    "      Avg: %lf ms\n"
    "    Service:\n"
    "      Min: %lld ms\n"
    "      Max: %lld ms\n"
    "      Avg: %lf ms\n"
    "    Total:\n"
    "      Min: %lld ms\n"
    "      Max: %lld ms\n"
    "      Avg: %lf ms\n"
    "  Ingredients:\n",
    a.clientsServed + a.clientsLost,
    a.clientsServed,
    a.clientsLost,
    a.clientsWaitMin,
    a.clientsWaitMax,
    a.clientsWaitAvg,
    a.clientsServMin,
    a.clientsServMax,
    a.clientsServAvg,
    a.clientsTotalMin,
    a.clientsTotalMax,
    a.clientsTotalAvg);

  for(int ing=0;ing<KINGREDIENT_QT;ing++)
    printf(
      "    %s:\n"
      "      Produced: %lld\n"
      "      Used: %lld\n"
      "      Lost: %lld\n",
      ingredient_to_str(ing),
      a.ingredientsQt[ing][0],
      a.ingredientsQt[ing][1],
      a.ingredientsQt[ing][2]);

}

int main(){
  init();
  run();
  
  print_analytics(analytics_get());

  return 0;
}