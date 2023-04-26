

/**
 * main.c
 *
 * Bots for room 2141
 * 1, 8, 7, 11, 25
 *
 * Bots for room 1318
 * 18
 *
 */

#include "lcd.h"
#include "Timer.h"
#include "button.h"
#include <string.h>

#define PERIOD 320000
//#define BOT 8 // choose bot being used here

// public variables
double data[3];
double servo_curr_time;
double servo_curr_angle;
int counter;
int direction;

// calibration struct for different bots
struct Offset {
    double b011[2];
    double b071[2];
    double b081[2];
    double b111[2];
    double b182[2]; // downstairs
    double b251[2];
};

struct Offset offset; // create offset struct

void calibrate(int bot) {

    // PWM offset struct
    // [offset, multiplier]
    offset.b011[0] = -1000.0;
    offset.b011[1] = 175.0;

    offset.b071[0] = -500.0; //wip
    offset.b071[1] = 172.0;

    offset.b081[0] = -500.0;
    offset.b081[1] = 180.0;

    offset.b111[0] = 2400.0; // wip
    offset.b111[1] = 175.0;

    offset.b182[0] = -800.0;
    offset.b182[1] = 175.0;

    offset.b251[0] = -850.0;
    offset.b251[1] = 180.0;

    // select data
    // [offset, multiplier]
    int i;
    switch (bot) {
        case 1:
            for (i = 0; i < 2; i++) {
                data[i] = offset.b011[i];
            }
            break;
        case 7:
            for (i = 0; i < 2; i++) {
                data[i] = offset.b071[i];
            }
            break;
        case 8:
            for (i = 0; i < 2; i++) {
                data[i] = offset.b081[i];
            }
            break;
        case 11:
            for (i = 0; i < 2; i++) {
                data[i] = offset.b111[i];
            }
            break;
        case 18:
            for (i = 0; i < 2; i++) {
                data[i] = offset.b182[i];
            }
            break;
        case 25:
            for (i = 0; i < 2; i++) {
                data[i] = offset.b251[i];
            }
            break;
        default:
            for (i = 0; i < 2; i++) {
                data[i] = 0;
            }
            break;
    }

    data[2] = 100.0 / 18.0; // set 1 degree delta
}


void servo_init(int bot) {
    calibrate(bot);
    // GPIO
    SYSCTL_RCGCGPIO_R |= 0x02; // port b clock
    GPIO_PORTB_AFSEL_R |= BIT5; // alternate function for bit 5
    GPIO_PORTB_PCTL_R |= 0x700000; // t1ccp1
    GPIO_PORTB_DIR_R |= BIT5; // make bit 5 output
    GPIO_PORTB_DEN_R |= BIT5; // digital enable bit 5

    // PWM Timer
    SYSCTL_RCGCTIMER_R |= BIT1; // timer 1 clock
    TIMER1_CTL_R &= ~0x0100; // temp disable timer1b

    TIMER1_CFG_R |= 0x04; // 16 bit timer
    TIMER1_TBMR_R |= 0x0a; // periodic mode
    TIMER1_TBILR_R = PERIOD & 0xffff; // max out load for timer
    TIMER1_TBPR_R = PERIOD >> 16; // shift to get 24 bit timer
    set_servo_angle_center();
    TIMER1_CTL_R |= 0x0100; // re-enable timer
}

void set_servo_angle(double angle) {
    servo_curr_angle = angle;
    double time = data[2] * angle + 1000;
    set_pulse(time);
}

void set_servo_offset_angle(double angle) {
    servo_curr_angle += angle;
    set_servo_angle(servo_curr_angle);
}

void set_servo_angle0(void) {
    direction = 0;
    set_pulse(1000.0);
}

void set_servo_angle_center(void) {
    set_pulse(1500.0);
}

void set_servo_angle180(void) {
    direction = 1;
    set_pulse(2000.0);
}

void set_pulse(double microseconds) {

    double OFFSET = data[0];

    int offset2 = (PERIOD - (16 * 1500) + OFFSET); // 90 degrees
    int match = (int) (PERIOD - (16 * (microseconds) + OFFSET));
    match = (((match - offset2) * ((int) data[1])) / 100) + offset2;
    TIMER1_TBMATCHR_R = match & 0xffff;
    TIMER1_TBPMR_R = match >> 16;

    servo_curr_angle = (microseconds * 0.18) - 180;
    counter = match;
    servo_curr_time = microseconds;
//    print_data();
}

double get_current_angle(void) {
    return servo_curr_angle;
}

//void print_data(void) {
//    if (direction == 1) {
//        lcd_printf("Angle: %.1f\nTime: %.1f\nCycles: %d\nIncrementing", servo_curr_angle, servo_curr_time, counter);
//    } else if (direction == -1) {
//        lcd_printf("Angle: %.1f\nTime: %.1f\nCycles: %d\nDecrementing", servo_curr_angle, servo_curr_time, counter);
//    } else {
//        lcd_printf("Angle: %.1f\nTime: %.1f\nCycles: %d\nError", servo_curr_angle, servo_curr_time, counter);
//    }
//}

//int main(void) {
//
//    // init public vars
//    servo_curr_time = 0;
//    servo_servo_curr_angle = 0;
//    counter = 0;
//    direction = 0;
//
//    // run init functions
//    lcd_init();
//    calibrate();
////    timer_waitMillis(2000);
//    pwm_init();
//    button_init();
//
//    int button = 0;
//
//    // use for calibration
//    timer_waitMillis(2000);
//    set_servo_angle0();
//
//    timer_waitMillis(2000);
//    set_servo_angle180();
//
//    timer_waitMillis(2000);
//    set_servo_angle_center();
//
////    timer_waitMillis(2000);
////    set_servo_angle(2.5);
//
////    while(1);
//
//	while(1) {
//	    button = button_getButton();
//
//	    if (button == 4) {
//	        direction *= -1;
//	        print_data();
//	        timer_waitMillis(250);
//	    }
//
//	    int comp_angle = (int) servo_curr_angle;
//
//	    if ((comp_angle < 180 && comp_angle > 0) || (comp_angle <= 0 && direction > 0) || (comp_angle >= 180 && direction < 0)) {
//	        switch (button) {
//	            case 1:
//	                set_servo_offset_angle(1.0 * direction);
//	                timer_waitMillis(250);
//	                break;
//	            case 2:
//	                set_servo_offset_angle(2.5 * direction);
//	                timer_waitMillis(250);
//	                break;
//	            case 3:
//	                set_servo_offset_angle(5.0 * direction);
//	                timer_waitMillis(250);
//	                break;
//	            default:
//	                break;
//	        }
//	    }
//	}
//}
