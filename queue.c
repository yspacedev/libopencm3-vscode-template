//implementation of circular queue datastructure
//ported from https://www.geeksforgeeks.org/dsa/introduction-to-circular-queue/
//and lightly modified

#include "queue.h"

uint8_t get_front(Queue *queue){
	if (queue->size==0){ //if empty
		return 0;
	}
	return queue->items[queue->front];
}
uint8_t get_back(Queue *queue){
	if (queue->size==0){ //if empty
		return 0;
	}
	uint16_t rear = (queue->front+queue->size-1)%queue->capacity;
	return queue->items[rear];
}
void init_queue_static(Queue *queue, uint16_t size, uint8_t *ptr){
	queue->items=ptr;
	queue->capacity=size;
	queue->size=0;
	queue->front=0;
}
void init_queue(Queue *queue, uint16_t size){
	queue->items=malloc(size);
	queue->capacity=size;
	queue->size=0;
	queue->front=0;
}
void enqueue(Queue *queue, uint8_t uint8_tacter){
	if (queue->size == queue->capacity){
		return;
	}
	uint16_t rear = (queue->front+queue->size)%queue->capacity;
	queue->items[rear]=uint8_tacter;
	queue->size++;
	return;
}
uint8_t dequeue(Queue *queue){
	if (queue->size==0){ //if empty
		return 0;
	}
	uint8_t result = queue->items[queue->front];
	queue->front=(queue->front+1)%queue->capacity;
	queue->size--;
	return result;
}

void free_queue(Queue *queue){
	free((void *)queue->items);
}

void queuetobuf(Queue *queue, uint8_t *buf, uint16_t size){
	for (uint16_t i=0; i<size; i++){
		buf[i]=dequeue(queue);
	}
}
