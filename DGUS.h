/*
 * DGUS.h
 *
 *  Created on: 2014-10-10
 *  Authors: Bartosz Bilas & Arkadiusz Kierkicz
 *  E-mail: bartez942@gmail.com & arkonosze@gmail.com
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
void 	read_variable(unsigned char adress);
void 	show_animation(int start, int finish);
void 	change_display_rs485(int display);
void 	read_number_of_display_rs485(void);
void 	read_variable_rs485(unsigned char adress);
void 	show_animation_rs485(int start, int finish);
void 	USART_dec_rs485( int data );
void 	USART_hex_rs485( unsigned char data );
void 	dg_send_int_rs485(int adres,int liczba);
