/*
*
*   uart-interrupt.c
*
*
*
*   @author
*   @date
*/

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart-interrupt.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
//#include "Timer.h"



// These variables are declared as examples for your use in the interrupt handler.
volatile char command_byte = 0; // byte value for special character used as a command
volatile int command_flag = 0; // flag to tell the main program a special command was received

void uart_interrupt_init(void){
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0x2;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0x2;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};
  while ((SYSCTL_PRUART_R & 0x2) == 0) {};

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x3;

  //enable alternate functions on port B pins
  GPIO_PORTB_DIR_R = (GPIO_PORTB_DIR_R | 0b00000001) & 0xFD;
  GPIO_PORTB_AFSEL_R |= 0x3;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R |= 0x00000011;

  //calculate baud rate
  uint16_t iBRD = 0x8; //use equations
  uint16_t fBRD = (int)(0.6805 * 64 + 0.5); //use equations   16000000 % (16 * 115200)
  //turn off UART1 while setting it up
  UART1_CTL_R &= 0xFFFFFFFE;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
  UART1_IBRD_R = iBRD;
  UART1_FBRD_R = fBRD;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R = 0b01100000;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //////Enable interrupts

  //first clear RX interrupt flag (clear by writing 1 to ICR)
  UART1_ICR_R |= 0b00010000;

  //enable RX raw interrupts in interrupt mask register
  UART1_IM_R |= 0x10;

  //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
  NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

  //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
  NVIC_EN0_R |= 0b001000000;

  //tell CPU to use ISR handler for UART1 (see interrupt.h file)
  //from system header file: #define INT_UART1 22
  IntRegister(INT_UART1, UART1_Handler);

  //globally allow CPU to service interrupts (see interrupt.h file)
  IntMasterEnable();

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R |= 0x301;

}

void uart_sendChar(char data){

    int j = UART1_FR_R;
    while((UART1_FR_R & 0b00100000)){};
        //UART1_DR_R = 0x0;
        UART1_DR_R = data;   //data

}

char uart_receive(void){
	//DO NOT USE this busy-wait function if using RX interrupt
    char recieve = '0';
        while((UART1_FR_R & 0b00010000) == 0){};

           int i=0;
           i = (UART1_DR_R|=0xFFFFFF);
           recieve = (char)i;

        return recieve;
}
char uart_receive_nonblocking(void){
    //DO NOT USE this busy-wait function if using RX interrupt
    char recieve = 'v';
        if((UART1_FR_R & 0b00010000) == 0){

           //int i=0;
           int i = (UART1_DR_R);
           recieve = i;
        }
        return recieve;
}

void uart_sendStr(const char *data){
	//TODO for reference see lcd_puts from lcd.c file
    while(*data != '\0') {
        uart_sendChar(*data);
        //printf("%c", *data);
        data++;

    }
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char byte_received;
    //check if handler called due to RX event
    if (UART1_MIS_R & 0b10000)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0b10000;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        byte_received = uart_receive_nonblocking();
        uart_sendChar(byte_received);

        //if byte received is a carriage return
        if (byte_received == '\r')
        {
            //send a newline character back to PuTTY
            uart_sendChar('\n');
        }
        else
        {
            command_byte = byte_received;
            //AS NEEDED
            //code to handle any other special characters
            //code to update global shared variables
            //DO NOT PUT TIME-CONSUMING CODE IN AN ISR
            command_flag = 1;

        }
    }
}
