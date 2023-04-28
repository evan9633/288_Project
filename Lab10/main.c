/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "adc.h"
#include "driverlib/interrupt.h"
#include "open_interface.h"
#include "movement.h"
#include <stdint.h>
#include "uart-interrupt.h"

// Uncomment or add any include directives that are needed

/**
 * MTG18-SLIDES-GPTimerIC.pdf are super awesome and swag
 */

#warning "Possible unimplemented functions"
#define REPLACEME 0

int main(void) {
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();

    ping_init();
    adc_init();
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()

    while(1){
        oi_update(sensor_data);
        char temp = '}';




//        while (temp == '}'){
//        temp = uart_receive();
//        }
        if (command_flag == 1)
                {

        if(command_byte == 'q'){
            move_forward(sensor_data, 2);
        }
        if(command_byte == 'w'){
            move_forward(sensor_data, 5);
        }
        if(command_byte == 'e'){
            move_forward(sensor_data, 7);
        }
        if(command_byte == 'r'){
            move_forward(sensor_data, 10);
        }
        if(command_byte == 't'){
            move_forward(sensor_data, 15);
        }
        if(command_byte == 'y'){
            move_forward(sensor_data, 20);
        }
        if(command_byte == 'u'){
            move_forward(sensor_data, 25);
        }
        if(command_byte == 'i'){
            move_forward(sensor_data, 30);
        }
        if(command_byte == 'o'){
            move_forward(sensor_data, 35);
        }
        if(command_byte == 'p'){
            //scan
        }
        // move backwards
        if(command_byte == 'a'){
            move_backward(sensor_data, 2);
               }
               if(command_byte == 's'){
                   move_backward(sensor_data, 5);
               }
               if(command_byte == 'd'){
                   move_backward(sensor_data, 7);
               }
               if(command_byte == 'f'){
                   move_backward(sensor_data, 10);
               }
               if(command_byte == 'g'){
                   move_backward(sensor_data, 15);
               }
               if(command_byte == 'h'){
                   move_backward(sensor_data, 20);
               }
               if(command_byte == 'j'){
                   move_backward(sensor_data, 25);
               }
               if(command_byte == 'k'){
                   move_backward(sensor_data, 30);
               }
               if(command_byte == 'l'){
                   move_backward(sensor_data, 35);
               }
        // turn right
        if(command_byte == 'z'){
            turn_right(sensor_data, 15);
        }
        if(command_byte == 'x'){
            turn_right(sensor_data, 45);
        }
        if(command_byte == 'c'){
            turn_right(sensor_data, 60);
        }
        if(command_byte == 'v'){
            turn_right(sensor_data, 90);
        }
        //turn left
        if(command_byte == 'b'){
            turn_left(sensor_data, 15);
        }
        if(command_byte == 'n'){
            turn_left(sensor_data, 45);
        }
        if(command_byte == 'm'){
            turn_left(sensor_data, 60);
        }
        if(command_byte == ','){
            turn_left(sensor_data, 90);
        }





        oi_update(sensor_data);
        //does the api/ui need the tape sensor data or does the bot just need to know that
        char x[20];
        //sensor data    bumpleft bumpright tapefrontleft tapefrontright tapeleft taperight clifffrontleft clifffrontright cliffleft cliffright

        //send obj  can't find where objects stored
        //uart_sendStr("dist: %lf angle: %d width: %d;",  );
//        int i = 0;
//        char send_obj[50];
//        for(i i <= obj[].length; i++){
//            send_obj += obj[i].distance;
//            send_obj += obj[i].width;
//            send_obj += obj[i].angle;
//        }
//        uart_sendStr(send_obj);
        temp = '}';
         }
    }
}
