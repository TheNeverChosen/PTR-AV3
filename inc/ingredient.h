#ifndef __INGREDIENT_H__
#define __INGREDIENT_H__

#include <stdbool.h>
#include "stdlib.h"

enum Ingredient{
  kIngredientBread,
  kIngredientSauce,
  kIngredientSausage
};
#define KINGREDIENT_QT 3 //number of ingredients

bool ingredient_is_valid(enum Ingredient ing);    //check if the ingredient is valid

char* ingredient_to_str(enum Ingredient ing);     //convert the ingredient to string

#endif