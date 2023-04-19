/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "driverlib/interrupt.h"
#include "servo.h"

// Uncomment or add any include directives that are needed

/**
 * MTG18-SLIDES-GPTimerIC.pdf are super awesome and swag
 */

#define REPLACEME 0

int main(void) {

	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	ping_init();
	servo_init();

    timer_waitMillis(1000);
	servo_move(90);
	timer_waitMillis(1000);
	servo_move(30);
	timer_waitMillis(1000);
	servo_move(150);
	timer_waitMillis(1000);
	servo_move(90);


}
