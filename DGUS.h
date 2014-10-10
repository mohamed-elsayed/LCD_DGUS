/*
 * DGUS.h
 *
 *  Created on: 2014-10-10
 *  Autor: Bartosz Bilas & Arkadiusz Kierkicz
 *  E-mail: bartez942@gmail.com & arkonosze@tlen.pl
 */

void 	USART_hex( unsigned char data );
void 	USART_dec( int data );
void	dec_to_bin(int liczba,int bin[]);
int		bin_to_dec(int bin[]);
void 	dg_send_int(int adres,int liczba);
int 	power(int base,int n);
int 	dg_to_int(int liczba_1,int liczba_2);
int 	bin16_to_dec(int bin[]);
void 	change_display(int display);
void 	read_number_of_display(void);
