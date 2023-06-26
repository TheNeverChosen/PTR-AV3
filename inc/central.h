#ifndef __CENTRAL_H__
#define __CENTRAL_H__

#include <stdlib.h>
#include <stdbool.h>
#include "client.h"


// //=====INGREDIENTS=====
// const size_t kIngredientMax = 10; //max number of ingredients in cart
// const size_t kIngredientQt = 3;   //number of ingredients

// enum Ingredient{
//   kIngredientBread = 0,
//   kIngredientSauce,
//   kIngredientSausage
// };

// bool ingredient_is_full(enum Ingredient);  //check if the cart is full of ingredient
// bool ingredient_add(enum Ingredient);      //add one ingredient to cart
// bool ingredient_remove(enum Ingredient);   //remove one ingredient from cart


//====================GENERAL====================
bool central_is_running();  //get the running status
void central_init();        //initialize the central
void central_start();       //start the central
void central_stop();        //stop the central

#endif