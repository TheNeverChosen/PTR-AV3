#include <stdbool.h>
#include "ingredient.h"

bool ingredient_is_valid(enum Ingredient ing){
  return ing>=0 && ing<KINGREDIENT_QT;
}

char* ingredient_to_str(enum Ingredient ing){
  switch(ing){
    case kIngredientBread: return "Bread";
    case kIngredientSauce: return "Sauce";
    case kIngredientSausage: return "Sausage";
    default: return "Invalid";
  }
}