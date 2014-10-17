/*
 * DGUS.h
 *
 *  Created on: 2010-09-04
 *  Authors: Bartosz Bilas & Arkadiusz Kierkicz
 *  E-mail: bartez942@gmail.com & arkonosze@gmail.com
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "DGUS.h"
#include <util/delay.h>

volatile uint8_t check_int = 0,check_answer = 0;
volatile int16_t bufor[20];

//---------------------------------------------------------------------------------
//								Initialization
//---------------------------------------------------------------------------------


void USART_Init( uint16_t baud ) {
	/* Set baud rate */
	UBRRH = (uint8_t)(baud>>8);
	UBRRL = (uint8_t)baud;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	UCSRB |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
}

//---------------------------------------------------------------------------------
//									Transfer
//---------------------------------------------------------------------------------

void	USART_hex( unsigned char data )
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void	USART_dec( int data )
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

//---------------------------------------------------------------------------------
//									Conversion
//---------------------------------------------------------------------------------

void	dec_to_bin(int32_t decimal, uint8_t binary[])
{
	uint8_t i = 0;


	if(decimal >= 0)
	{
		while(decimal)
		{
			binary[i++] = decimal%2;
			decimal = decimal/2;
		}
		for( ; i < 32; i++)
			binary[i] = 0;
	}
	else
	{
		decimal++;
		while(decimal)
		{
			binary[i++] = !(decimal%2);
			decimal = decimal/2;
		}
		for( ; i < 32; i++)
			binary[i] = 1;
	}
}

int32_t bin_to_dec(uint8_t binary[])
{
	uint8_t i = 0;
	int32_t decimal = 0;

	if(binary[31] == 0)
	{
		for(i = 0; i < 31; i++)
		{
			if(binary[i] == 1 && i == 0)
				decimal++;
			if(binary[i] == 1 && i > 0)
				decimal = decimal + (power(2,i));
		}
	}

	if(binary[31] == 1)
	{
		for(i = 0; i < 31; i++)
		{
			if(binary[i] == 0 && i == 0)
				decimal++;
			if(binary[i] == 0 && i > 0)
				decimal = decimal + (power(2,i));
		}

		decimal = decimal  * (-1);
	}
	return decimal;
}

int16_t power(uint8_t base,uint8_t n)
{
	uint16_t i, p;
	p = 1;
	for (i = 1; i<=n; ++i)
		p = p * base;
	return p;
}
void	hex_to_bin(char hexadecimal[],uint8_t binary[])
{

	int8_t i;
	uint8_t j = 0;
	uint8_t length = 0;

	length = strlen(hexadecimal);

	for(i = (length - 1); i >= 0; i--)
	{
		switch(hexadecimal[i])
		{
		case '0':
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 0;
			break;
		case '1':
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 0;
			break;
		case '2':
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 0;
			break;
		case '3':
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 0;
			break;
		case '4':
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 0;
			break;
		case '5':
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 0;
			break;
		case '6':
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 0;
			break;
		case '7':
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 0;
			break;
		case '8':
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 1;
			break;
		case '9':
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 1;
			break;
		case 'A':
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 1;
			break;
		case 'B':
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 1;
			break;
		case 'C':
			binary[j++] = 0;
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 1;
			break;
		case 'D':
			binary[j++] = 1;
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 1;
			break;
		case 'E':
			binary[j++] = 0;
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 1;
			break;
		case 'F':
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 1;
			binary[j++] = 1;
			break;
		}
	}
	for( ; j < 32; j++)
		binary[j] = 0;

}

//---------------------------------------------------------------------------------
//								Communication
//---------------------------------------------------------------------------------

void dg_send_int(char address[],int16_t decimal)
{
	uint8_t binary[32]; //binary representation of our number
	uint8_t binary_support[32]; //part of our binary string
	uint8_t i,j; //control variables
	uint8_t dec = 0;

	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(5);
	USART_hex(0x82);
	//ADRES ZMIENNEJ
	hex_to_bin(address,binary);
	for(i = 8, j = 0; i < 16; i++)
		binary_support[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_support[i] = 0;
	dec = bin_to_dec(binary_support);
	USART_dec(dec);
	for(i = 0; i < 8; i++)
		binary_support[i] = binary[i];
	for(i = 8; i < 32; i++)
		binary_support[i] = 0;
	dec = bin_to_dec(binary_support);
	USART_dec(dec);
	//WARTOSC ZMIENNEJ
	dec_to_bin(decimal,binary);
	j = 0;
	for(i = 8; i < 16; i++)
		binary_support[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_support[i] = 0;
	dec = bin_to_dec(binary_support);
	USART_dec(dec);
	for(i = 0; i < 8; i++)
		binary_support[i] = binary[i];
	for(i = 8; i < 32; i++)
		binary_support[i] = 0;
	dec = bin_to_dec(binary_support);
	USART_dec(dec);
}

void dg_send_lint(char address[],int32_t decimal)
{
	uint8_t binary[32]; //binary representation of our number
	uint8_t binary_short[32]; //part of our binary string
	uint8_t i,j; //control variables
	int16_t dec = 0;

	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(7);
	USART_hex(0x82);
	//ADRES ZMIENNEJ
	hex_to_bin(address,binary);
	j = 0;
	for(i = 8; i < 16; i++)
		binary_short[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
	for(i = 0; i < 8; i++)
		binary_short[i] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
	//WARTOSC ZMIENNEJ
	dec_to_bin(decimal,binary);
	j = 0;
	for(i = 24; i < 32; i++)
		binary_short[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
	j = 0;
	for(i = 16; i < 24; i++)
		binary_short[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
	j = 0;
	for(i = 8; i < 16; i++)
		binary_short[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
	for(i = 0; i < 8; i++)
		binary_short[i] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);

}

void change_display(uint16_t display)
{
	uint8_t binary[32]; //binary representation of our number
	uint8_t binary_short[32]; //part of our binary string
	uint8_t i,j; //control variables
	uint8_t dec = 0;

	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(4);
	USART_hex(0x80);
	USART_hex(0x03);
	//WARTOSC ZMIENNEJ
	dec_to_bin(display,binary);
	j = 0;
	for(i = 8; i < 16; i++)
		binary_short[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
	for(i = 0; i < 8; i++)
		binary_short[i] = binary[i];
	for(i = 8; i < 32; i++)
		binary_short[i] = 0;
	dec = bin_to_dec(binary_short);
	USART_dec(dec);
}

uint16_t read_display_number(void)
{
	uint8_t binary[32];
	uint8_t binary_support[32];
	uint8_t i,j;

	//Ask for display number
	check_answer = 8;
	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(3);
	USART_hex(0x81);
	USART_hex(0x03);
	USART_hex(0x02);

	//Wait for response
	while(check_answer != 1)
		;

	//Analise response from display
	dec_to_bin(bufor[7],binary_support);
	for(i = 0; i < 8; i++)
		binary[i] = binary_support[i];
	dec_to_bin(bufor[6],binary_support);
	for(i = 8, j = 0; i < 16; i++, j++)
		binary[i] = binary_support[j];
	for(i = 16; i < 32; i++)
		binary[i] = 0;

	return bin_to_dec(binary);
}

int16_t read_int(char address[])
{
	uint8_t binary[32];
	uint8_t binary_support[32];
	int i,j;
	uint8_t dec = 0;

	//Ask to send us int variable
	check_answer = 9;
	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_hex(0x04);
	USART_hex(0x83);
	//Variable address
	hex_to_bin(address,binary);
	for(i = 8, j = 0; i < 16; i++)
		binary_support[j++] = binary[i];
	for(i = 8; i < 32; i++)
		binary_support[i] = 0;
	dec = bin_to_dec(binary_support);
	USART_dec(dec);
	for(i = 0; i < 8; i++)
		binary_support[i] = binary[i];
	for(i = 8; i < 32; i++)
		binary_support[i] = 0;
	dec = bin_to_dec(binary_support);
	USART_dec(dec);
	USART_hex(0x01);

	//Wait for response
	while(check_answer != 1)
		;

	//Analise response from display

	dec_to_bin(bufor[8],binary_support);
	for(i = 0; i < 8; i++)
		binary[i] = binary_support[i];
	dec_to_bin(bufor[7],binary_support);
	for(i = 8, j = 0; i < 16; i++, j++)
		binary[i] = binary_support[j];

	for(i = 16; i < 32; i++)
		binary[i] = 0;


	return bin_to_dec(binary);
}

void show_animation(int start, int finish){
	int i;
	for(i=start;i<=finish;i++){
		USART_hex(0x5A);
		USART_hex(0xA5);
		USART_hex(0x04);
		USART_hex(0x80);
		USART_hex(0x03);
		USART_hex(0x00);
		USART_dec(i);
		_delay_ms(90);
	}
}

ISR (USART_RXC_vect)
{
	static uint8_t index = 0;

	bufor[index++]=UDR;

	if(index == check_answer && check_answer == 8)
	{
		check_answer=1;
		index = 0;
	}

	if(index == check_answer && check_answer == 9)
	{
		check_answer=1;
		index = 0;
	}
}
