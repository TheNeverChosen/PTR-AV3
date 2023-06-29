#ifndef __THREAD_SUPPLIER_H__
#define __THREAD_SUPPLIER_H__

//==========SUPPLIER==========
//interval between supllier restock [kSupRestockMsL, kSupRestockMsR] ms
static const unsigned int kSupRestockMsL = 50, kSupRestockMsR=500;

void supplier_thread_create(pthread_t *client_thread, enum Ingredient ingredient);

#endif