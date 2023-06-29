#ifndef __QUEUE_H__
#define __QUEUE_H__

#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>

//==================LinkedNode==================
typedef struct LinkedNode{
  void *data;
  struct LinkedNode *next;
}LinkedNode;

LinkedNode* linked_node_new(void *data, LinkedNode *next);
void linked_node_delete(LinkedNode *node, void (*free_data)(void *));
void linked_node_delete_all(LinkedNode *node, void (*free_data)(void *));

//==================Queue==================
//Thread-Safe Queue (mutex lock)
typedef struct{
  LinkedNode *head, *tail;
  size_t size;
  pthread_mutex_t mutex;
}Queue;

Queue* queue_new();
size_t queue_size(Queue *queue);
bool queue_is_empty(Queue *queue);
void queue_delete(Queue *queue, void (*free_data)(void*));
void queue_push(Queue *queue, void *data);
void queue_pop(Queue *queue, void (*free_data)(void*));
void* queue_front(Queue *queue);



#endif