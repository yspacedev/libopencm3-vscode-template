#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <stdint.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct {
	uint8_t *items;
	uint16_t size; //number of elements in queue
	uint16_t capacity; //maximum number of elements in Queue
	uint16_t front;
} Queue;

void init_queue_static(Queue *queue, uint16_t size, uint8_t *ptr);
void init_queue(Queue *queue, uint16_t size);
void enqueue(Queue *queue, uint8_t uint8_tacter);
uint8_t dequeue(Queue *queue);
uint8_t get_front(Queue *queue);
uint8_t get_back(Queue *queue);
void print_queue(Queue *queue);
void queuetobuf(Queue *queue, uint8_t *buf, uint16_t size);
void free_queue(Queue *queue);

#endif /* INC_QUEUE_H_ */
