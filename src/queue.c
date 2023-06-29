#include <pthread.h>
#include "queue.h"


//==================LinkedNode==================
LinkedNode* linked_node_new(void *data, LinkedNode *next){
  LinkedNode *node = (LinkedNode*)malloc(sizeof(LinkedNode));
  node->data = data;
  node->next = next;
  return node;
}

void linked_node_delete(LinkedNode *node, void (*free_data)(void *)){
  if(free_data) free_data(node->data);
  free(node);
}

void linked_node_delete_all(LinkedNode *node, void (*free_data)(void *)){
  if(node==NULL) return;
  linked_node_delete_all(node->next, free_data); //delete next node
  linked_node_delete(node, free_data);           //delete current node
}


//==================Queue==================
Queue* queue_new(){
  Queue *queue = (Queue*)malloc(sizeof(Queue));
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  pthread_mutex_init(&queue->mutex, NULL);
  return queue;
}

size_t queue_size(Queue *queue){
  if(queue==NULL) return 0;
  pthread_mutex_lock(&queue->mutex);
  size_t size = queue->size;
  pthread_mutex_unlock(&queue->mutex);
  return size;
}

bool queue_is_empty(Queue *queue){
  return queue_size(queue)==0;
}

void queue_delete(Queue *queue, void (*free_data)(void*)){
  linked_node_delete_all(queue->head, free_data);
  pthread_mutex_destroy(&queue->mutex);
  free(queue);
}

void queue_push(Queue *queue, void *data){
  if(queue==NULL) return;
  pthread_mutex_lock(&queue->mutex);

  LinkedNode *node = linked_node_new(data, NULL);
  if(queue->size<=0) queue->head = node;
  else queue->tail->next = node;
  queue->tail = node;
  queue->size++;
  pthread_mutex_unlock(&queue->mutex);
}

void queue_pop(Queue *queue, void (*free_data)(void*)){
  if(queue==NULL) return;
  pthread_mutex_lock(&queue->mutex);
  if(queue->size<=0){
    pthread_mutex_unlock(&queue->mutex);
    return;
  }
  LinkedNode *node = queue->head;
  queue->head = node->next;
  queue->size--;
  pthread_mutex_unlock(&queue->mutex);
  linked_node_delete(node, free_data);
}

void* queue_front(Queue *queue){
  if(queue==NULL) return NULL;
  pthread_mutex_lock(&queue->mutex);
  void *data = queue->size ? queue->head->data : NULL;
  pthread_mutex_unlock(&queue->mutex);
  return data;
}