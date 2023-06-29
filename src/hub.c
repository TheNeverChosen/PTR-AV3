#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include "analytics.h"
#include "client.h"
#include "hub.h"
#include "ingredient.h"
#include "log.h"
#include "misc.h"
#include "queue.h"

//==========CLIENTS==========
static pthread_mutex_t mutexClient;
static pthread_cond_t condClient;
static Queue *qClient = NULL;

static bool genRunning;
static size_t clientGenerated;


void hub_client_init(){
  pthread_mutex_init(&mutexClient, NULL);
  pthread_cond_init(&condClient, NULL);
  qClient = queue_new();

  genRunning = true;
  clientGenerated = 0;
}

bool hub_client_is_generating(){
  pthread_mutex_lock(&mutexClient);
  bool val = genRunning;
  pthread_mutex_unlock(&mutexClient);
  return val;
}

size_t hub_client_count(){
  pthread_mutex_lock(&mutexClient);
  size_t val = queue_size(qClient);
  pthread_mutex_unlock(&mutexClient);
  return val;
}

bool hub_client_add(){
  pthread_mutex_lock(&mutexClient);

  genRunning = (++clientGenerated) < KCLIENT_GENERATED_MAX;
  
  if(queue_size(qClient)>=KCLIENT_MAX || clientGenerated>KCLIENT_GENERATED_MAX){
    pthread_mutex_unlock(&mutexClient);
    analytics_client_lost(); //Analytics: lost client
    return false;
  }

  queue_push(qClient, client_new(get_time()));

  pthread_cond_signal(&condClient);

  pthread_mutex_unlock(&mutexClient);

  return true;
}

bool hub_client_wait(){
  pthread_mutex_lock(&mutexClient);
  while(queue_size(qClient)<=0 && genRunning && hub_cart_is_running()){
    log_i("<CART> No client to service. Total: %zu\n", queue_size(qClient));
    log_i("<CART> Waiting for client arrival...\n");
    pthread_cond_wait(&condClient, &mutexClient);
    log_i("<CART> Waking up: client arrived\n");
  }
  pthread_mutex_unlock(&mutexClient);
  return queue_size(qClient)>0 && hub_cart_is_running();
}

Client* hub_client_remove(){
  pthread_mutex_lock(&mutexClient);
  Client *c = queue_front(qClient);
  queue_pop(qClient, NULL);
  pthread_mutex_unlock(&mutexClient);
  return c;
}

//==========INGREDIENTS==========
static int qtIngredient[KINGREDIENT_QT]; //init
static pthread_mutex_t mutexIngredient[KINGREDIENT_QT]; //init
static pthread_cond_t condIngAvailable[KINGREDIENT_QT]; //init

void hub_ingredient_init(enum Ingredient ing){
  if(!ingredient_is_valid(ing)) return; //invalid
  qtIngredient[ing] = 0;
  pthread_mutex_init(&mutexIngredient[ing], NULL);
  pthread_cond_init(&condIngAvailable[ing], NULL);
}

int hub_ingredient_count(enum Ingredient ing){
  if(!ingredient_is_valid(ing)) return -1; //invalid

  pthread_mutex_lock(&mutexIngredient[ing]);
  int val = qtIngredient[ing];
  pthread_mutex_unlock(&mutexIngredient[ing]);

  return val;
}

bool hub_ingredient_add(enum Ingredient ing){
  if(!ingredient_is_valid(ing)) return false; //invalid

  pthread_mutex_lock(&mutexIngredient[ing]);

  if(qtIngredient[ing]>=KINGREDIENT_MAX){ //full
    pthread_mutex_unlock(&mutexIngredient[ing]);
    analytics_ingredient(ing, kAnalyticsIngLost); //Analytics: wasted
    return false;
  }

  qtIngredient[ing]++;
  analytics_ingredient(ing, kAnalyticsIngProduced); //Analytics: raw
  
  pthread_cond_signal(&condIngAvailable[ing]);

  pthread_mutex_unlock(&mutexIngredient[ing]);

  return true;
}

bool hub_ingredient_wait(enum Ingredient ing){
  if(!ingredient_is_valid(ing)) return false; //invalid

  pthread_mutex_lock(&mutexIngredient[ing]);
  while(qtIngredient[ing]<=0 && hub_cart_is_running()){
    log_i("<CART> Missing ingredient %d. Waiting for supplier...\n", ing);
    pthread_cond_wait(&condIngAvailable[ing], &mutexIngredient[ing]);
    log_i("<CART> Waking up: ingredient %d supplied\n", ing);
  }
  pthread_mutex_unlock(&mutexIngredient[ing]);

  return hub_cart_is_running();
}

bool hub_ingredient_remove(enum Ingredient ing){
  if(!ingredient_is_valid(ing)) return false; //invalid
  
  pthread_mutex_lock(&mutexIngredient[ing]);
  
  if(qtIngredient[ing]<=0){
    pthread_mutex_unlock(&mutexIngredient[ing]);
    return false;
  }
  qtIngredient[ing]--;

  pthread_mutex_unlock(&mutexIngredient[ing]);

  analytics_ingredient(ing, kAnalyticsIngUsed); //Analytics: processed

  return true;
}

//==========CART==========
static pthread_mutex_t mutexCart;
bool cartRunning;

void hub_cart_init(){
  pthread_mutex_init(&mutexCart, NULL);
  cartRunning=true;
}

void hub_cart_stop(){
  pthread_mutex_lock(&mutexCart);
  cartRunning=false;
  pthread_mutex_unlock(&mutexCart);
}

bool hub_cart_is_running(){
  pthread_mutex_lock(&mutexCart);
  bool val = cartRunning;
  pthread_mutex_unlock(&mutexCart);
  return val;
}

