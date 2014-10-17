/*
 * DGUS.h
 *
 *  Created on: 2014-10-10
 *  Authors: Bartosz Bilas & Arkadiusz Kierkicz
 *  E-mail: bartez942@gmail.com & arkonosze@gmail.com
 */

#define 	UART_BAUD 115200	// baud rate
#define 	__UBRR F_CPU/16/UART_BAUD-1  // UBRR for U2X=0

#define UART_RX_BUF_SIZE 32 // buffer size of 32 bytes
#define 	UART_RX_BUF_MASK ( UART_RX_BUF_SIZE - 1) // mask for our buffer

#define 	UART_TX_BUF_SIZE 16 //  buffer size of 16 bytes
#define 	UART_TX_BUF_MASK ( UART_TX_BUF_SIZE - 1) // mask for our buffer

// Initialization

void 		USART_Init( uint16_t baud );

//Conversion
void		dec_to_bin(int32_t decimal, uint8_t binary[]);
int32_t 	bin_to_dec(uint8_t binary[]);
int16_t 	power(uint8_t base,uint8_t n);


//Transfer
void 		USART_hex( unsigned char data );
void 		USART_dec( int data );

//Communication
void		dg_send_int(char address[],int16_t decimal);
void		dg_send_lint(char address[],int32_t decimal);
void 		change_display(uint16_t display);
uint16_t 	read_display_number(void);
int16_t		read_int(char address[]);
void 		show_animation(int start, int finish);
