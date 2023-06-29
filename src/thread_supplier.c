#include <pthread.h>
#include <stdlib.h>
#include "hub.h"
#include "log.h"
#include "misc.h"
#include "ingredient.h"
#include "thread_supplier.h"


//==========SUPPLIER==========
void* supplier_thread_routine(void *data){
  enum Ingredient *ing = (enum Ingredient*)data;

  log_i("<SUPPLIER %d> Supplier thread started.\n", *ing);

  while(hub_cart_is_running()){
    long long sleepTimeMs = rand_ll(kSupRestockMsL, kSupRestockMsR);
    log_d("<SUPPLIER %d> Sleeping %lld ms.\n", *ing, sleepTimeMs);
    sleep_ms(sleepTimeMs);
    log_d("<SUPPLIER %d> Waking up.\n", *ing);

    if(!hub_cart_is_running()) break;
    
    if(hub_ingredient_add(*ing)){
      log_i("<SUPPLIER %d> Ingredient added. Total: %d\n", *ing, hub_ingredient_count(*ing));
    }
    else{
      log_i("<SUPPLIER %d> Ingredient not added. Total: %d\n", *ing, hub_ingredient_count(*ing));
    }
  }

  log_i("<SUPPLIER %d> Supplier thread terminated.\n", *ing);

  free(ing);

  return NULL;
}

void supplier_thread_create(pthread_t *client_thread, enum Ingredient ingredient){
  enum Ingredient *ing = malloc(sizeof(enum Ingredient));
  *ing = ingredient;
  pthread_create(client_thread, NULL, supplier_thread_routine, ing);
}