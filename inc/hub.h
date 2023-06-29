#ifndef __HUB_H__
#define __HUB_H__

#include <stdlib.h>
#include <stdbool.h>
#include "client.h"
#include "ingredient.h"

//==========CLIENTS==========
#define KCLIENT_MAX 5   //max number of clients in queue at same time
#define KCLIENT_GENERATED_MAX 50 //max number of clients to be generated

void hub_client_init();             //initialize the client structures
bool hub_client_is_generating();    //get the generating status (off/on - 0/1)
size_t hub_client_count();          //get the number of clients in queue
bool hub_client_add();              //add one client
bool hub_client_wait();             //wait for available clients
Client* hub_client_remove();        //remove one client from queue

//==========INGREDIENTS==========
#define KINGREDIENT_MAX 3 //max number of ingredients in supply

void hub_ingredient_init(enum Ingredient);   //initialize the ingredient supply
int hub_ingredient_count(enum Ingredient);   //get the ingredient supply count
bool hub_ingredient_add(enum Ingredient);    //add one ingredient to cart
bool hub_ingredient_wait(enum Ingredient);        //wait available clients
bool hub_ingredient_remove(enum Ingredient); //remove one ingredient from cart

//==========CART==========
void hub_cart_init();               //initialize the cart
void hub_cart_stop();               //stop the cart
bool hub_cart_is_running();         //get the running status (off/on - 0/1)

#endif