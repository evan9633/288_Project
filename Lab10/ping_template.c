/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"
#include "driverlib/interrupt.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile unsigned long OVERFLOW = 0;
volatile unsigned long CUR_OVERFLOW = 0;
volatile enum
{
    LOW, HIGH, DONE
} STATE = LOW; // State of ping echo pulse

void ping_init(void)
{

    SYSCTL_RCGCGPIO_R |= 0x02; //Port B
    SYSCTL_RCGCTIMER_R |= 0x08; //timer 3
    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02)
    {
    };
//    while((SYSCTL_RCGCGPIO_R & 0x02) != 0x02)
//    {
//    };
    GPIO_PORTB_PCTL_R |= 0x00007000;
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_DEN_R |= 0x08;

    // Configure and enable the timer
    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBMR_R &= ~0x10;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;

//set up NVIC for timer 3B IC interrupts
    NVIC_PRI9_R = 0x20;
    NVIC_EN1_R = 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();
}

void ping_trigger(void)
{
    STATE = LOW;
    CUR_OVERFLOW = 0;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    GPIO_PORTB_DIR_R |= 0x08;
//    GPIO_PORTB_AFSEL_R |= 0x08;

    GPIO_PORTB_DATA_R &= ~0x08;
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x08;

    GPIO_PORTB_DIR_R &= ~0x08;

//    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
//    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;

}

void TIMER3B_Handler(void)
{

    // YOUR CODE HERE
    // As needed, go back to review your interrupt handler code for the UART lab.
    // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
    // includes checking the source of the interrupt and clearing the interrupt status bit.
    // Checking the source: test the MIS bit in the MIS register (is the ISR executing
    // because the input capture event happened and interrupts were enabled for that event?
    // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
    // The rest of the code in the ISR depends on actions needed when the event happens.

    //check if handler called due to RX event


    if (TIMER3_MIS_R & 0x400)
    {
        //clear the trigger flag by putting 1 in the ICR
        TIMER3_ICR_R |= 0x400;

        if (STATE == LOW)
        {
            START_TIME = TIMER3_TBR_R;
//            lcd_printf("%lu\n",START_TIME);
//            timer_waitMillis(100);
            STATE = HIGH;
        }
        else if (STATE == HIGH)
        {
            END_TIME = TIMER3_TBR_R;
//            lcd_printf("%lu\n",END_TIME);
//            timer_waitMillis(100);
            STATE = DONE;
        }
    }

//    if (UART1_MIS_R & 0x10)
//    {
//        //byte was received in the UART data register
//        //clear the RX trigger flag (clear by writing 1 to ICR)
//        UART1_ICR_R |= 0b00010000;
//
//        //read the byte received from UART1_DR_R and echo it back to PuTTY
//        //ignore the error bits in UART1_DR_R
//        byte_received = UART1_DR_R;
//        uart_sendChar(byte_received);
//
//        //if byte received is a carriage return
//        if (byte_received == '\r')
//        {
//            //send a newline character back to PuTTY
//            uart_sendChar('\n');
//        }
//        else
//        {
//            //AS NEEDED
//            //code to handle any other special characters
//            //code to update global shared variables
//            //DO NOT PUT TIME-CONSUMING CODE IN AN ISR
//
//            if (byte_received == command_byte)
//            {
//                command_flag = 1;
//            }
//        }
//    }

}

float ping_getDistance(void)
{

    ping_trigger();
    // YOUR CODE HERE
    while(STATE != DONE){

    }

    float result;

    if(END_TIME > START_TIME){

        OVERFLOW += 1;
        CUR_OVERFLOW = 1;

    }

    unsigned long time = (START_TIME - END_TIME) + (CUR_OVERFLOW << 24);

    result = (0.0000343 * (time) * 62.5) / 2.0;

    return result;

}
