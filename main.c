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

#define IRVALUE 400

// Uncomment or add any include directives that are needed

/**
 * MTG18-SLIDES-GPTimerIC.pdf are super awesome and swag
 */

#define REPLACEME 0

int main(void) {


    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();

    uart_interrupt_init();

    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    movement_init(sensor_data);

    ping_init();
    servo_init();
    adc_init();

    while (1) {
        char g = '/0';


//        uart_sendStr("Test");
        g = uart_receive();
        if (g == 'w')
        {
            move_forward(10);
        }
        else if (g == 'a')
        {
            turn_left(15);
        }
        else if (g == 's')
        {
            move_backward(10);
        }
        else if (g == 'd')
        {
            turn_right(15);
        }

        if (g == '8') {//0-180
            int i;
                int count;
                int object_counter = 0;
                uint16_t val;
                        char str[] = "";
                                    object objects[10];
                                    const int distance_threshold = 30;

                                    int same_distance = 0;
                                    int distance = 0;

                                    cyBOT_Scan_t info[181] = { };

                                    i = 0;
                                    servo_move(i);

                                    info[0].ir = adc_read();
                                    info[0].distance = ping_getDistance();
                                    info[0].angle = i;

                                    int previous_ir = info[0].ir;

                                    i += 1;

                                    while (i <= 180)
                                    {
                                        timer_waitMillis(200);
                                        servo_move(i);

                                        info[i].ir = adc_read();
                                        info[i].distance = ping_getDistance();
                                        info[i].angle = i;

                                        double period = START_TIME - END_TIME;
                                        count += 1;

                                        val = 195.52 - (27 * pow((double) val, 0.246));


                                        distance = info[i].ir;

                                        if (distance > IRVALUE)
                                        {
                                            if (distance <= previous_ir + distance_threshold
                                                    && distance >= previous_ir - distance_threshold)
                                            {
                                                same_distance += 1;

                                            }
                                            else
                                            {
                                                if (same_distance > 2)
                                                {
                                                    // sprintf(line, "%d %-10s\n\r", i - 1, "+ object");
                                                    // uart_sendStr(line);

                                                    // sprintf(line, "%-10d %-10s\n\r", same_distance, "degree [width]");
                                                    // uart_sendStr(line);

                                                    objects[object_counter].degree =
                                                            i - (same_distance / 2);
                                                    objects[object_counter].distance =
                                                            info[i - (same_distance / 2)].distance;
                                                    objects[object_counter].width = (same_distance);
                                                    object_counter++;

                                                }
                                                same_distance = 0;

                                            }

                                        }
                                        else
                                        {
                                            if (same_distance > 2)
                                            {
                                                // sprintf(line, "%d %-10s\n\r", i - (same_distance / 2), "[+] object");
                                                //uart_sendStr(line);

                                                // sprintf(line, "%-10d %-10s\n\r", same_distance, "[+] degree [width]");
                                                // uart_sendStr(line);

                                                objects[object_counter].degree = i
                                                        - (same_distance / 2);
                                                objects[object_counter].distance = info[i
                                                        - (same_distance / 2)].distance;
                                                objects[object_counter].width =
                                                        (objects[object_counter].distance
                                                                * tan(same_distance * 3.14 / 180)); //sin(0.0174533 * same_distance) * objects[object_counter].distance;

                                                object_counter++;

                                            }
                                            same_distance = 0;

                                        }
                                        previous_ir = distance;

                                        i += 1;
                                    }
                                    int z;
                                    for (z = 0; z < object_counter; z++)
                                    {
                                        if (objects[z].distance < 100 && objects[z].degree < 180 && objects[z].width < 90 && objects[z].distance > 0)
                                        {
                                            sprintf(str, "width:%d;angle:%d;dist:%d;",
                                                    objects[z].width, objects[z].degree,
                                                    objects[z].distance);
                                            uart_sendStr(str);
                                            printf(str);
                                            printf("\n");
                                        }
                                    }
        }
        if (g == '5') { //60-120
            int i;
                int count;
                int object_counter = 0;
                uint16_t val;
                        char str[] = "";
                                    object objects[10];
                                    const int distance_threshold = 30;

                                    int same_distance = 0;
                                    int distance = 0;

                                    cyBOT_Scan_t info[181] = { };

                                    i = 60;
                                    servo_move(i);

                                    info[0].ir = adc_read();
                                    info[0].distance = ping_getDistance();
                                    info[0].angle = i;

                                    int previous_ir = info[0].ir;

                                    i += 1;

                                    while (i <= 120)
                                    {
                                        timer_waitMillis(200);
                                        servo_move(i);

                                        info[i].ir = adc_read();
                                        info[i].distance = ping_getDistance();
                                        info[i].angle = i;

                                        double period = START_TIME - END_TIME;
                                        count += 1;

                                        val = 195.52 - (27 * pow((double) val, 0.246));


                                        distance = info[i].ir;

                                        if (distance > IRVALUE)
                                        {
                                            if (distance <= previous_ir + distance_threshold
                                                    && distance >= previous_ir - distance_threshold)
                                            {
                                                same_distance += 1;

                                            }
                                            else
                                            {
                                                if (same_distance > 2)
                                                {


                                                    objects[object_counter].degree =
                                                            i - (same_distance / 2);
                                                    objects[object_counter].distance =
                                                            info[i - (same_distance / 2)].distance;
                                                    objects[object_counter].width = (same_distance);
                                                    object_counter++;

                                                }
                                                same_distance = 0;

                                            }

                                        }
                                        else
                                        {
                                            if (same_distance > 2)
                                            {
                                                // sprintf(line, "%d %-10s\n\r", i - (same_distance / 2), "[+] object");
                                                //uart_sendStr(line);

                                                // sprintf(line, "%-10d %-10s\n\r", same_distance, "[+] degree [width]");
                                                // uart_sendStr(line);

                                                objects[object_counter].degree = i
                                                        - (same_distance / 2);
                                                objects[object_counter].distance = info[i
                                                        - (same_distance / 2)].distance;
                                                objects[object_counter].width =
                                                        (objects[object_counter].distance
                                                                * tan(same_distance * 3.14 / 180)); //sin(0.0174533 * same_distance) * objects[object_counter].distance;

                                                object_counter++;

                                            }
                                            same_distance = 0;

                                        }
                                        previous_ir = distance;

                                        i += 1;
                                    }
            int z;
                                    for (z = 0; z < object_counter; z++)
                                    {
                                        if (objects[z].distance < 100 && objects[z].degree < 180 && objects[z].width < 90 && objects[z].distance > 0)
                                        {
                                            sprintf(str, "width:%d;angle:%d;dist:%d;",
                                                    objects[z].width, objects[z].degree,
                                                    objects[z].distance);
                                            uart_sendStr(str);
                                            printf(str);
                                            printf("\n");
                                        }
                                    }
        }
//        if (g == '6') { //0-90
//         scan (0,90);
//        }
//        if (g == '4') { //90-180
//            scan(90,180);
//
//        }

    }

}
