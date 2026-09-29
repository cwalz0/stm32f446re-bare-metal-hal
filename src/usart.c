#include "usart.h"
#include "rcc.h"
#include "gpio.h"


#include <stdint.h>

void USART2_Init(){

	// power gpio port a
	RCC_EnableClock(AHB1ENR_OFFSET, 0);

	// power usart2
	RCC_EnableClock(APB1ENR_OFFSET, 17); 


	GPIO_InitTypeDef USART2_INIT = {0};

	USART2_INIT.Mode = GPIO_MODE_ALT;
	USART2_INIT.Speed = 3;
	USART2_INIT.Alternate = 7;

	USART2_INIT.Pin = 2; // Port A Pin 2 (PA 2) : TX

	GPIO_Init(GPIOA, &USART2_INIT);

	USART2_INIT.Pin = 3; // PA3 : RX
	GPIO_Init(GPIOA, &USART2_INIT);


	*(volatile uint32_t *)(USART2_BASE + USART_BRR_OFFSET) = (24U << 4) | (7U << 0); 

	*(volatile uint32_t *)(USART2_BASE + USART_CR1_OFFSET) |= (1U << 2) | (1U << 3) | (1U << 13); // enable Recieve, Transmit, USART

	return;
}

int USART2_ReadChar(void){
	while ((*(volatile uint32_t *)(USART2_BASE + USART_SR_OFFSET) & (1U << 5)) == 0){ // wait for data ready to be read
		continue;
	}
	return *(volatile uint32_t *)(USART2_BASE + USART_DR_OFFSET) & 0xFF;
	
}

void USART2_WriteChar(int ch){
	while ((*(volatile uint32_t *)(USART2_BASE + USART_SR_OFFSET) & (1U << 7)) == 0){ // wait for data is not being transferred to the shift register
		continue;
	}

	*(volatile uint32_t *)(USART2_BASE + USART_DR_OFFSET) = (ch & 0xFF); // send data (ASCII & 1111 1111 TO LEAVE GAPS IN 0s TO PREVENT GARBAGE)

}

void USART2_WriteInt(int value){
	char str[10];
	int i = 0;
	if (value == 0) {
		USART2_WriteChar('0');
	} else if (value < 0) {
		USART2_WriteChar('-');
		value = -value;
	}
	while (value != 0) {
		str[i++] = (value % 10) + '0';
		value /= 10;
	}
	while (i > 0) {
		USART2_WriteChar(str[--i]);
	}
}

void USART2_WriteFloat(float value){
	if (value < 0.0f){
		USART2_WriteChar('-');
		value = -value;
	}
	int whole = (int)value;

	// extract decimal (2 decimal places)
	float remainder = (value - (float)whole) + 0.005f; // .005 keeps it from truncating
	int frac = (int)(remainder * 100.0f);

	if (frac >= 100) {
		whole += 1;
		frac = 0;
	}

	// print whole num
	USART2_WriteInt(whole);
	USART2_WriteString(".");
	if (frac < 10) {
		USART2_WriteChar('0');
	}
	USART2_WriteInt(frac);
}


void USART2_WriteString(char *str){
	while (*str != '\0'){
		USART2_WriteChar(*str); // write the current character in the string
		str++; // iterate through the string
	}
	return;
}

void USART2_WriteHex(uint8_t value){
	char hex_chars[] = "0123456789ABCDEF";
	char buffer[2];
	
	buffer[0] = hex_chars[(value >> 4) & 0x0F];
	buffer[1] = hex_chars[value & 0x0F];

	USART2_WriteChar(buffer[0]);
	USART2_WriteChar(buffer[1]);
	USART2_WriteString("\r\n");
}

