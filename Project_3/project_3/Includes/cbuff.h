#ifndef CBUFF
#define CBUFF

/*This structure contains the variables for the buffer*/
typedef struct {
	uint8_t* buffer;
	uint8_t* head;
	uint8_t* tail;
	uint32_t length;
	uint32_t num_items;
}cb_t;


/*Enumeration for the state of the buffer*/
typedef enum {
	FULL,
	EMPTY,
	SUCCESS,
	FAIL
}state;


uint8_t Full(cb_t *cb);
uint8_t Empty(cb_t *cb);
state add_item(cb_t *cb, uint8_t item);
state remove_item(cb_t *cb, volatile uint8_t *dest);
void cbuff_init(cb_t *buff, uint32_t length);
void empty_buff(cb_t *buff);
void fill_buff(cb_t *buff);

#endif
