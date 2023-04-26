#include "servo.h"

#define BYTE 8

double ang;
int clockwise;

void servo_init(void) {
//INIT PORT B: GPIO PB5, turn on clk, alt. function, output, enable
    SYSCTL_RCGCGPIO_R |= 0x2;
    ang = 0;
    clockwise = 0;

    SYSCTL_RCGCGPIO_R |= 0x02;                      //Enables port B GPIO
    SYSCTL_RCGCTIMER_R |= 0x02;                     //Enables Timer 1B

    GPIO_PORTB_DIR_R |= 0x20;                       //Sets direction to output for pin 5
    GPIO_PORTB_DEN_R |= 0x20;                       //Sets digital enable for pin 5

    GPIO_PORTB_AFSEL_R |= 0x20;                     //Sets alternate function for pin 5
    GPIO_PORTB_PCTL_R |= 0x00700000;                //Sets alternate function to Timer 1B for pin 5

    TIMER1_CTL_R &= 0xFEFF;                         //Disables Timer 1B
    TIMER1_CFG_R = 0x4;                             //Selects Timer 1B 16 bit

    TIMER1_TBMR_R |= 0x00A;                         //PWM Mode Enabled, Edge Count, Periodic Timer
    TIMER1_TBMR_R &= 0xFFA;

    //TIMER1_CTL_R &= 0xBFFF;                       //Set Timer to a non inverted state
    TIMER1_TBPR_R = 0x0004;                         //Set an 8 bit pre-scale

    TIMER1_TBILR_R = 0xE200;                       //Sets start value for timer

    TIMER1_TBPMR_R = 0x0004;

    int MS = 6942;

    TIMER1_TBMATCHR_R = (TIMER1_TBPR_R << 4 | TIMER1_TBILR_R) - MS;    //Sets match value;

    TIMER1_CTL_R |= 0x0100;


}

void servo_move(uint16_t degrees) {

    TIMER1_CTL_R &= 0xFEFF;
    int MS = 6942;
    int x = (int) (((4.13) * MS * (degrees/180.0)) + MS);
    TIMER1_TBMATCHR_R = TIMER1_TBILR_R - x;
    TIMER1_CTL_R |= 0x0100;

}
