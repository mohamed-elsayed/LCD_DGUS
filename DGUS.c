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


void USART_hex( unsigned char data )
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void USART_hex_rs485( unsigned char data )
{
	PORTD |= (1<<PD2); // PIN DE
	while ( !( UCSRA & (1<<UDRE)) );
	_delay_us(10);
	UDR = data;
	while ( !( UCSRA & (1<<UDRE)) );
	PORTD &= ~(1<<PD2); // PIN DE
}

void USART_dec( int data )
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}

void USART_dec_rs485( int data )
{
	PORTD |= (1<<PD2); // PIN DE
	while ( !( UCSRA & (1<<UDRE)) );
	_delay_us(10);
	UDR = data;
	while ( !( UCSRA & (1<<UDRE)) );
}


void dec_to_bin(int liczba, int bin[])
{
	int i = 0;
	while(liczba)
	{
		bin[i++] = liczba%2;
		liczba = liczba/2;
	}
	for( ; i < 16; i++)
		bin[i] = 0;
}

void dec_to_8bin(int liczba, int bin[])
{
	int i = 0;
	while(liczba)
	{
		bin[i++] = liczba%2;
		liczba = liczba/2;
	}
	for( ; i < 8; i++)
		bin[i] = 0;
}

int bin_to_dec(int bin[])
{
	int i = 0;
	int int_ret = 0;

	for(i = 0; i < 8; i++)
	{
		if(bin[i] == 1 && i == 0)
			int_ret++;
		if(bin[i] == 1 && i > 0)
			int_ret = int_ret + (power(2,i));
	}
	return int_ret;
}

void dg_send_int(int adres,int liczba)
{
	int bin[20];
	int bin_1[10];
	int bin_2[10];
	int int_1;
	int int_2;
	uint8_t i = 0;
	uint8_t j = 0;

	dec_to_bin(liczba,bin);
	for(i = 0; i < 8; i++)
		bin_1[i] = bin[i];
	for(j = 0; i < 16; i++)
	{
		bin_2[j] = bin[i];
		j++;
	}

	int_1 = bin_to_dec(bin_1);
	int_2 = bin_to_dec(bin_2);

	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(5);
	USART_hex(0x82);
	USART_hex(0x00);
	USART_hex(adres);
	USART_dec(int_2);
	USART_dec(int_1);
}

void dg_send_int_rs485(int adres,int liczba)
{
	int bin[20];
	int bin_1[10];
	int bin_2[10];
	int int_1;
	int int_2;
	uint8_t i = 0;
	uint8_t j = 0;

	dec_to_bin(liczba,bin);
	for(i = 0; i < 8; i++)
		bin_1[i] = bin[i];
	for(j = 0; i < 16; i++)
	{
		bin_2[j] = bin[i];
		j++;
	}

	int_1 = bin_to_dec(bin_1);
	int_2 = bin_to_dec(bin_2);

	USART_hex_rs485(0x5A);
	USART_hex_rs485(0xA5);
	USART_hex_rs485(5);
	USART_hex_rs485(0x82);
	USART_hex_rs485(0x00);
	USART_hex_rs485(adres);
	USART_dec_rs485(int_2);
	USART_dec_rs485(int_1);
}

int power(int base,int n){
	int i, p;
	p = 1;
	for (i = 1; i<=n; ++i)
		p =p * base;
	return p;
}

int bin16_to_dec(int bin[])
{
	int i = 0;
	int int_ret = 0;

	for(i = 0; i < 16; i++)
	{
		if(bin[i] == 1 && i == 0)
			int_ret++;
		if(bin[i] == 1 && i > 0)
			int_ret = int_ret + (power(2,i));
	}
	return int_ret;
}

int dg_to_int(int liczba_1,int liczba_2)
{
	int bin[20];
	int bin_1[10];
	int bin_2[10];
	int8_t i;
	int8_t j;


	dec_to_8bin(liczba_1,bin_1);
	dec_to_8bin(liczba_2,bin_2);

	for(i = 0; i < 8 ;i++)
		bin[i] = bin_2[i];

	for(j = 0; j < 8; j++)
	{
		bin[i] = bin_1[j];
		i++;
	}

	return (bin16_to_dec(bin));
}

void change_display(int display){
	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(4);
	USART_hex(0x80);
	USART_hex(0x03);
	USART_hex(0x00);
	USART_dec(display);
}

void change_display_rs485(int display){
	USART_hex_rs485(0x5A);
	USART_hex_rs485(0xA5);
	USART_dec_rs485(4);
	USART_hex_rs485(0x80);
	USART_hex_rs485(0x03);
	USART_hex_rs485(0x00);
	USART_dec_rs485(display);
}

void read_number_of_display(void){
	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_dec(3);
	USART_hex(0x81);
	USART_hex(0x03);
	USART_hex(0x02);
}

void read_number_of_display_rs485(void){
	USART_hex_rs485(0x5A);
	USART_hex_rs485(0xA5);
	USART_dec_rs485(3);
	USART_hex_rs485(0x81);
	USART_hex_rs485(0x03);
	USART_hex_rs485(0x02);
	PORTD &= ~(1<<PD2); // PIN DE
}

void read_variable(unsigned char adress){
	USART_hex(0x5A);
	USART_hex(0xA5);
	USART_hex(0x04);
	USART_hex(0x83);
	USART_hex(0x00);
	USART_hex(adress);
	USART_hex(0x01);
}

void read_variable_rs485(unsigned char adress){
	USART_hex_rs485(0x5A);
	USART_hex_rs485(0xA5);
	USART_hex_rs485(0x04);
	USART_hex_rs485(0x83);
	USART_hex_rs485(0x00);
	USART_hex_rs485(adress);
	USART_hex_rs485(0x01);
	PORTD &= ~(1<<PD2); // PIN DE
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

void show_animation_rs485(int start, int finish){
	int i;
	for(i=start;i<=finish;i++){
		USART_hex_rs485(0x5A);
		USART_hex_rs485(0xA5);
		USART_hex_rs485(0x04);
		USART_hex_rs485(0x80);
		USART_hex_rs485(0x03);
		USART_hex_rs485(0x00);
		USART_dec_rs485(i);
		_delay_ms(40);
	}
}
