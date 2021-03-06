#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cbuff.h"


uint8_t Full(cb_t *cb){
	return (cb->num_items == cb->length);
}

/*This function check if the buffer is empty if empty it returns 1*/
uint8_t Empty(cb_t *cb){
	return (cb->num_items == 0);
}

/*This function is used to add an element in the buffer*/
state add_item(cb_t *cb, uint8_t item){
	if(Full(cb))
		return FULL;

	*(cb->head) = item;
	cb->head++;
	cb->num_items++;						//Increaments the number of items after addition to buffer

	if((cb->head) == cb->buffer + cb->length) //This statement prevents buffer overflow by bringing head back
		cb -> head = cb->buffer;			  //back to the first elemnt

	return SUCCESS;
}
/*This function remove an item from the buffer*/
state remove_item(cb_t *cb, volatile uint8_t * dest){
	if (Empty(cb)){
		return EMPTY ;
	}

	*dest = *(cb->tail);
	cb->tail++;
	cb->num_items--;							//Decrements the number after removal

	if((cb->tail) == cb->buffer + cb->length) //Brings the tail back to the first element
		cb -> tail = cb->buffer;

	return SUCCESS;
}

/*This function initialises the circular buffer of a perticular length*/
void cbuff_init(cb_t *buff, uint32_t length){
	buff->buffer = malloc(length);
	buff->head = buff->buffer;
	buff->tail = buff->buffer;
	buff->length = length;
	buff->num_items = 0;
}

void destroy(cb_t *buff){
	free(buff->buffer);
}

void empty_buff(cb_t *buff){
	uint8_t i,len,value;

	len = buff->num_items;

	for(i=0; i<len; i++){
		remove_item(buff, &value);
	}
}

void fill_buff(cb_t *buff){
	uint8_t i, len;
	len = buff->length;
	empty_buff(buff);

	for(i=0;i<len;i++){
		add_item(buff,i);
	}
}
