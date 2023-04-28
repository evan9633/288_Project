/*
 * scan_temp.c
 *
 *  Created on: Feb 15, 2023
 *      Author: erdunn, czulk
 */
#include "scan_temp.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "Timer.h"
#include "scan_temp.h"
#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include <math.h>
#define MIN_WIDTH 40
#define MAX_DIST 500
#define MAX_OBJ 50
#define NUM_SCANS 4
#define PI 3.14159276
int small = 0;
int index = 0;
//cyBOT_SERVO_cal();

void drive(cyBOT_Scan_t getScan, oi_t *sensor_data)
{
    //gets the index of the smallest obj. storage struct(s) should be populated
    int small = scan_precision(getScan, sensor_data);

    //Rotatte to face the smallest obj.
    if (storage[small].angle > 90)
    {
        turn_right(sensor_data, (180 - storage[small].angle));
    }
    else
    {
        turn_left(sensor_data, (0 + storage[small].angle));
    }

    while (1)
    {
        //Move towards smallest obj incrementally.
        move_Forward(sensor_data, 25);
        scan_cust(getScan, sensor_data, NUM_SCANS, 110, 85); //scan reduced arc in front
        oi_update(sensor_data); //update data presumably
        int near = nearest_obj(getScan, sensor_data); //nearest obj index
        small = scan_precision(getScan, sensor_data); //smallest obj index
        if (near != small) //There is another object closer than the smallest
        {
            if (storage[near].IR_dist < 50)
            {
                scan_cust(getScan, sensor_data, NUM_SCANS, 110, 60);
                calc_coor(getScan, sensor_data, near);
                if (storage[near].x_co < 140 && storage[near].x_co > -140)
                {
                    turn_left(sensor_data, 90);
                    move_Forward(sensor_data, 140);
                    turn_right(sensor_data, 90);
                }
            }

        }
        else if (near == small)
        {
            break;
        }

        //    turn_right(sensor_data, 180);
        //    oi_update(sensor_data);
        //    turn_left(sensor_data, 180);
        if (sensor_data->bumpLeft && sensor_data->bumpRight)
        {
            //        dist-=40;
            move_Backwards(sensor_data, -75);
            turn_right(sensor_data, 90);
            move_Forward(sensor_data, 75);
            turn_left(sensor_data, 90);
            move_Forward(sensor_data, 75);

        }
        else if (sensor_data->bumpLeft)
        {

            move_Backwards(sensor_data, -75);
            turn_right(sensor_data, 90);
            move_Forward(sensor_data, 75);
            turn_left(sensor_data, 90);
            move_Forward(sensor_data, 75);

        }
        else if (sensor_data->bumpRight)
        {

            move_Backwards(sensor_data, -75);
            turn_left(sensor_data, 90);
            move_Forward(sensor_data, 75);
            turn_right(sensor_data, 90);
            move_Forward(sensor_data, 75);
        }

    }

    if (storage[small].angle > 90)
    {
        turn_right(sensor_data, (180 - storage[small].angle));
    }
    else
    {
        turn_left(sensor_data, (0 + storage[small].angle));
    }

    move_Forward(sensor_data, storage[small].sound_dist + 5);

}
void add_obj(int angle, float sound_dist, float IR_dist)
{

    if ((sound_dist > MAX_DIST) || (IR_dist > MAX_DIST))
    {
        return;
    }

    obj n;
    n.angle = angle;
    n.sound_dist = sound_dist;
    n.width = 0;
    n.IR_dist = IR_dist;

    storage[index++] = n;

}
void calc_coor(cyBOT_Scan_t getScan, oi_t *sensor_data, int index)
{
    double cdeg = (double) index * 2 * PI / 180.00;
    double cangle = cos(cdeg);
    double x_coord = cangle * storage[index].sound_dist;
    storage[index].x_co = x_coord;
    double csin = sin(cdeg);
    double y_coord = csin * storage[index].sound_dist;
    storage[index].y_co = y_coord;

}

void scan_cust(cyBOT_Scan_t getScan, oi_t *sensor_data, int num_scans,
               double high_deg, double low_deg)
{

    cyBOT_Scan(0, &getScan);
    stop_m(sensor_data);
    int i, j;
    char c[20];
    int IR = 0;
    int sound = 0;

    float temp = 0;
    float temp2 = 0;

    for (i = low_deg; i < high_deg; i++)
    {
        cyBOT_Scan(i, &getScan);

        if (index == 0)
        {
            for (j = 0; j < num_scans; j++)
            {
                temp += getScan.sound_dist;
                temp2 += getScan.IR_raw_val;
            }
            temp /= num_scans;
            temp2 /= num_scans;
            add_obj(i, temp, temp2);
        }
        else
        {
            for (j = 0; j < num_scans; j++)
            {
                temp += getScan.sound_dist;
                temp2 += getScan.IR_raw_val;
            }
            temp /= num_scans;
            temp2 /= num_scans;
            if (temp < 20)
            {
                if (temp > storage[index].sound_dist)
                {
                    if (temp - storage[index].sound_dist > MIN_WIDTH)
                    {
                        add_obj(i, temp, temp2);
                    }
                    else
                    {
                        storage[index].width++;
                        storage[index].angle = i;

                    }

                }
                else
                {
                    if (storage[index].sound_dist - temp > MIN_WIDTH)
                    {
                        add_obj(i, temp, temp2);

                    }
                    else
                    {
                        storage[index].width++;
                        storage[index].angle = i;

                    }
                }

            }
            else
            {
                if (temp2 > storage[index].IR_dist)
                {
                    if (temp2 - storage[index].IR_dist > MIN_WIDTH)
                    {
                        add_obj(i, temp, temp2);
                    }
                    else
                    {
                        storage[index].width++;
                        storage[index].angle = i;

                    }

                }
                else
                {
                    if (storage[index].IR_dist - temp2 > MIN_WIDTH)
                    {
                        add_obj(i, temp, temp2);

                    }
                    else
                    {
                        storage[index].width++;
                        storage[index].angle = i;
                    }
                }
            }
        }
        int xz = i;
        char c[15];
        sprintf(c, "Angle: %d   \0", xz);

        char *p = c;
        uart_sendStr(p);

        char z[15];

        sprintf(z, " Width: %d   \0", storage[index].width);

        p = z;
        uart_sendStr(p);

        char x[15];

        sprintf(x, " Distance: %f\n\r\0", getScan.sound_dist);

        p = x;
        uart_sendStr(p);
        //print_data(i, getScan, sensor_data);
       // int t = 1337; Fuck this ridiculous code/compiler
    }

}
//Prints data over the TCP socket.
void print_data(int zo, cyBOT_Scan_t getScan, oi_t *sensor_data)
{
    int i = zo;
    char c[15];
    sprintf(c, "Angle: %d   \0", i);

    char *p = c;
    uart_sendStr(p);

    char z[15];

    sprintf(z, " Width: %d   \0", storage[index].width);

    p = z;
    uart_sendStr(p);

    char x[15];

    sprintf(x, " Distance: %f\n\r\0", getScan.sound_dist);

    p = x;
    uart_sendStr(p);

}

//Misnomer - finds the 'smallest' object by width and returns the index
int scan_precision(cyBOT_Scan_t getScan, oi_t *sensor_data)
{
    int j, i, sm;
    double wi;
    i = 1;
    while ((i < sizeof(storage) / sizeof(obj)) - 1)
    {
        sm = i;
        if (storage[i - 1].width < sm)
        {
            sm = i - 1;

        }
        i++;
    }
    if (sm > 1000)
    {
        sm = i;
    }

    return sm;
}

//Returns the Nearest Object detected via IR distance - returns index
int nearest_obj(cyBOT_Scan_t getScan, oi_t *sensor_data)
{
    int i = 0;

    int near = 0;
    for (i = 1; i < (sizeof(storage) / sizeof(obj)) - 1; i++)
    {
        if (storage[i].IR_dist < storage[near].IR_dist)
        {
            near = i;
        }

    }
    return near;
}

