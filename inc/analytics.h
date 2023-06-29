#ifndef __ANALYTICS_H__
#define __ANALYTICS_H__

#include "client.h"
#include "ingredient.h"

enum AnalyticsIngredient {
  kAnalyticsIngProduced=0,
  kAnalyticsIngUsed,
  kAnalyticsIngLost
};

typedef struct{
  long long clientsServed, clientsLost;
  
  long long clientsWaitMin, clientsWaitMax; //min max
  double clientsWaitAvg; //avg

  long long clientsServMin, clientsServMax; //min max
  double clientsServAvg; //avg

  long long clientsTotalMin, clientsTotalMax; //min max
  double clientsTotalAvg; //avg
  
  long long ingredientsQt[KINGREDIENT_QT][3]; //raw, processed, wasted
}Analytics;

//initialize analytics
void analytics_init();

//clients served, clients lost,
//wait, service and total times: min, avg and max
void analytics_client_served(Client *client);

//clients lost register
void analytics_client_lost();

//raw, processed, wasted ingredients register
void analytics_ingredient(enum Ingredient, enum AnalyticsIngredient);

Analytics analytics_get();

#endif