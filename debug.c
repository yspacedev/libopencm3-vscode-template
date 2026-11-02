#include "debug.h"

//currently does not support LPUART without modification
#define DBG_USART_N     1        /* USART1 */
#define DBG_GPIO_LETTER A        /* GPIOA  */
#define DBG_TX_PIN      GPIO9
#define DBG_TX_AF       GPIO_AF7 /* depends on pin+USART, check the datasheet */

/* two-level macros so the arguments expand before pasting */
#define CAT2_(a, b)       a##b
#define CAT2(a, b)        CAT2_(a, b)
#define CAT3_(a, b, c)    a##b##c
#define CAT3(a, b, c)     CAT3_(a, b, c)

#define DBG_USART         CAT2(USART, DBG_USART_N)
#define DBG_USART_RCC     CAT2(RCC_USART, DBG_USART_N)
#define DBG_USART_NVIC    CAT3(NVIC_USART, DBG_USART_N, _IRQ)
#define DBG_USART_ISR     CAT3(usart, DBG_USART_N, _isr)

#define DBG_TX_PORT       CAT2(GPIO, DBG_GPIO_LETTER)
#define DBG_TX_PORT_RCC   CAT2(RCC_GPIO, DBG_GPIO_LETTER)

typedef struct {
	uint8_t *items;
	uint16_t size; //number of elements in queue
	uint16_t capacity; //maximum number of elements in Queue
	uint16_t front;
} Queue;

Queue debug_msg;

//prototypes

static void init_queue_static(Queue *queue, uint16_t size, uint8_t *ptr);
static void init_queue(Queue *queue, uint16_t size);
static void enqueue(Queue *queue, uint8_t uint8_tacter);
static uint8_t dequeue(Queue *queue);
static uint8_t get_front(Queue *queue);
static uint8_t get_back(Queue *queue);
static void print_queue(Queue *queue);
static void queuetobuf(Queue *queue, uint8_t *buf, uint16_t size);
static void free_queue(Queue *queue);

void enqueue_char(uint8_t in);

//used by tiny printf library
void _putchar(char character){
    while (debug_msg.size == debug_msg.capacity); //wait for queue to empty (this can lead to problems though)
    enqueue_char((uint8_t)character);
}

void DBG_USART_ISR(void){
    if (USART_ISR(DBG_USART) & USART_ISR_TXE){ //if TX empty
        if (debug_msg.size == 0){ //if last char in queue, disable irq
            USART_CR1(DBG_USART) &= ~USART_CR1_TXEIE;
        } else {
            usart_send(DBG_USART, dequeue(&debug_msg));
        }
    }
}

void init_hardware_debug_uart(uint32_t baud){
    rcc_periph_clock_enable(DBG_USART_RCC);
    rcc_periph_clock_enable(DBG_TX_PORT_RCC);

    init_queue(&debug_msg, 256);

    gpio_mode_setup(DBG_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, DBG_TX_PIN);
    gpio_set_output_options(DBG_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, DBG_TX_PIN);
    gpio_set_af(DBG_TX_PORT, DBG_TX_AF, DBG_TX_PIN);

    usart_set_baudrate(DBG_USART, baud);
    usart_set_databits(DBG_USART, 8);
    usart_set_stopbits(DBG_USART, USART_STOPBITS_1);
    usart_set_parity(DBG_USART, USART_PARITY_NONE);
    usart_set_flow_control(DBG_USART, USART_FLOWCONTROL_NONE);
    usart_set_mode(DBG_USART, USART_MODE_TX);

    nvic_enable_irq(DBG_USART_NVIC);
    usart_enable(DBG_USART);
}

void enqueue_char(uint8_t in){
    enqueue(&debug_msg, in);
    USART_CR1(DBG_USART) |= USART_CR1_TXEIE;
}

static uint8_t get_front(Queue *queue){
	if (queue->size==0){ //if empty
		return 0;
	}
	return queue->items[queue->front];
}
static uint8_t get_back(Queue *queue){
	if (queue->size==0){ //if empty
		return 0;
	}
	uint16_t rear = (queue->front+queue->size-1)%queue->capacity;
	return queue->items[rear];
}
static void init_queue_static(Queue *queue, uint16_t size, uint8_t *ptr){
	queue->items=ptr;
	queue->capacity=size;
	queue->size=0;
	queue->front=0;
}
static void init_queue(Queue *queue, uint16_t size){
	queue->items=malloc(size);
	queue->capacity=size;
	queue->size=0;
	queue->front=0;
}
static void enqueue(Queue *queue, uint8_t uint8_tacter){
	if (queue->size == queue->capacity){
		return;
	}
	uint16_t rear = (queue->front+queue->size)%queue->capacity;
	queue->items[rear]=uint8_tacter;
	queue->size++;
	return;
}
static uint8_t dequeue(Queue *queue){
	if (queue->size==0){ //if empty
		return 0;
	}
	uint8_t result = queue->items[queue->front];
	queue->front=(queue->front+1)%queue->capacity;
	queue->size--;
	return result;
}

static void free_queue(Queue *queue){
	free((void *)queue->items);
}

static void queuetobuf(Queue *queue, uint8_t *buf, uint16_t size){
	for (uint16_t i=0; i<size; i++){
		buf[i]=dequeue(queue);
	}
}

/*
//semihosting to be implemented later
//
//from https://bogdanthegeek.github.io/blog/insights/jlink-rtt-for-the-masses/
int SEMIHOST_SysCall(int reason, void *arg)
{
    int value;
    __asm volatile(
        "mov r0, %[rsn] \n" // place semihost operation code into R0
        "mov r1, %[arg] \n" // R1 points to the argument array
        "bkpt 0xAB      \n" // call debugger
        "mov %[val], r0 \n" // debugger has stored result code in R0

        : [val] "=r"(value)                 // outputs
        : [rsn] "r"(reason), [arg] "r"(arg) // inputs
        : "r0", "r1", "r2", "memory");
    return value; // return result code, stored in R0
}

int getchar(void)
{
    return SEMIHOST_SysCall(SYS_READC, NULL);
}

int _write(int fd, const char *buf, int size)
{
    if (fd == SEMIHOST_STDOUT || fd == SEMIHOST_STDERR)
    {
        int32_t args[3] = {fd, (int32_t)buf, size};
        return SEMIHOST_SysCall(SYS_WRITE, &args[0]);
    }
    return 0; // Unsupported file descriptor
}

*/