
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "driverlib/interrupt.h"
#include "servo.h"
#include "uart-interrupt.h"
#include "open_interface.h"
#include "adc.h"
#include "movement.h"

#include "structs.h"

// Uncomment or add any include directives that are needed

/**
 * MTG18-SLIDES-GPTimerIC.pdf are super awesome and swag
 */

#define REPLACEME 0

int main(void) {
    oi_t *sensor_data = oi_alloc(); //free memory
        oi_init(sensor_data);
        movement_init(sensor_data);

    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();

    uart_interrupt_init();


//    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
//    oi_init(sensor_data); // do this only once at start of main()
//    movement_init(sensor_data);

    ping_init();
    servo_init();
    adc_init();

    while (1) {
        int j= 0;
        int i = 0;
        char g = '/0';
        float time = 0;
        int count = 0;
        uint16_t val;
        uint16_t reading;
        char str[] = "";


        uart_sendStr("Test");
        g = uart_receive();
        uart_sendChar(g);
        if(g == 'w') {
            move_forward(10);
        } else if (g == 'a') {
            turn_left(90);
        } else if (g == 's') {
            move_backward(10);
        } else if (g == 'd') {
            turn_right(90);
        }

        if(g == 'm'){
            int object_counter = 0;
            object objects[50];
            const int distance_threshold = 50;

            int same_distance = 0;
            int distance = 0;



            cyBOT_Scan_t info[181] = { };
            char line[48];//print format
            int z = 0;

            for(z = 0; z < 48; z++) line[z] = 0;
            sprintf(line, "%-10s %-15s\n\r", "Degrees", "Distance");
            uart_sendStr(line);


            info[0].ir = adc_read();
            info[0].distance = ping_getDistance();
            info[0].angle = i;

            int previous_ir = info[0].ir;

            i++;

            while (i <= 180) {
                timer_waitMillis(200);
                servo_move(i);

                info[i].ir = adc_read();
                info[i].distance = ping_getDistance();
                info[i].angle = i;

                double period = START_TIME - END_TIME;
                count += 1;

                val = 195.52 - (27 *pow((double)val, 0.246));
                sprintf(str, "%u", reading);

//                sprintf(str, "\n\r%d\t%f\t%d\n\r", info[i].angle, info[i].distance, info[i].ir);
//                uart_sendStr(str);

                distance = info[i].ir;

                if(distance > 800) {
                                    if(distance <= previous_ir + distance_threshold && distance >= previous_ir - distance_threshold) {
                                        same_distance += 1;


                                    }
                                    else {
                                        if(same_distance > 2) {
                                            sprintf(line, "%d %-10s\n\r", i - 1, "+ object");
                                            uart_sendStr(line);

                                            sprintf(line, "%-10d %-10s\n\r", same_distance, "degree [width]");
                                            uart_sendStr(line);

                                            objects[object_counter].degree = i - (same_distance / 2);
                                            objects[object_counter].distance = info[i - (same_distance / 2)].ir;
                                            objects[object_counter].width = (objects[object_counter].distance * tan(same_distance * 3.14/180));//sin(0.0174533 * same_distance) * objects[object_counter].distance;
                                            object_counter++;

                                        }
                                        same_distance = 0;

                                    }

                                }
                               else if(distance <= 800) {
                                   if(same_distance > 2) {
                                       sprintf(line, "%d %-10s\n\r", i - (same_distance / 2), "[+] object");
                                       uart_sendStr(line);

                                       sprintf(line, "%-10d %-10s\n\r", same_distance, "[+] degree [width]");
                                       uart_sendStr(line);

                                       objects[object_counter].degree = i - (same_distance / 2);
                                       objects[object_counter].distance = info[i - (same_distance / 2)].ir;
                                       objects[object_counter].width = (objects[object_counter].distance * tan(same_distance * 3.14/180));//sin(0.0174533 * same_distance) * objects[object_counter].distance;
                                       object_counter++;

                                   }
                                   same_distance = 0;

                               }
                                previous_ir = distance;

                i+=1;
            }

        }
    }

}



