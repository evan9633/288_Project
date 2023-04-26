/*
 * movement.c
 *
 *  Created on: Jan 31, 2023
 *      Author: Brock Dykhuis
 */

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"

#define TAPEVALUE 2000 //STILL NEED TAPE VALUE
    oi_t* sensor;

    void movement_init(oi_t* s) {
        sensor = s;
    }

    void move_forward(oi_t *sensor,double distance_num){
        int bot_distance = 0;
        oi_setWheels(50,50);

        while( bot_distance < distance_num) {//STILL NEED TO CALIBRATE
            oi_update(sensor);
            bot_distance += sensor -> distance;
            check_Interrupts(sensor);
        }

        oi_setWheels(0,0);
    }


        void turn_right(oi_t *sensor,double degrees){
            double bot_angle = 0;
            oi_setWheels(-50,50);

            while(bot_angle > degrees * -1) {//STILL NEED TO CALIBRATE
                oi_update(sensor);
                bot_angle += sensor -> angle;
                check_Interrupts(sensor);
            }
            oi_setWheels(0,0);
        }

        void turn_left(oi_t *sensor,double degrees){
        double bot_angle = 0;
        oi_setWheels(50,-50);
        while(bot_angle > degrees * 1) {//STILL NEED TO CALIBRATE
            oi_update(sensor);
            bot_angle += sensor -> angle;
            check_Interrupts(sensor);
        }

        oi_setWheels(0,0);
        }

        void move_backward(oi_t *sensor,double distance_num){
            int bot_distance = 0;
            oi_setWheels(-50,-50);
            while(bot_distance > distance_num * -1) {//STILL NEED TO CALIBRATE
                oi_update(sensor);
                bot_distance += sensor -> distance;
                check_Interrupts(sensor);
            }

            oi_setWheels(0,0);

        }


        int check_Interrupts(oi_t *sensor) {

            if (sensor -> wheelDropLeft || sensor -> cliffFrontLeft || sensor -> cliffLeft ) {
                uart_sendStr("drop;");
                move_backward(sensor,5);
                return 1;
            } else if (sensor -> wheelDropRight || sensor -> cliffFrontRight || sensor -> cliffRight) {
                uart_sendStr("drop;");
                move_backward(sensor, 5);
                return 2;
            } else if (sensor -> cliffLeftSignal > TAPEVALUE) {
                uart_sendStr("tape: L;");
                oi_setWheels(0,0);
                return 3;
            } else if (sensor -> cliffFrontLeftSignal > TAPEVALUE) {
                uart_sendStr("tape: FL;");
                oi_setWheels(0,0);
                return 4;
            } else if (sensor -> cliffFrontRightSignal > TAPEVALUE) {
                uart_sendStr("tape: FR;");
                oi_setWheels(0,0);
                return 5;
            } else if (sensor -> cliffRightSignal > TAPEVALUE) {
                uart_sendStr("tape: R;");
                oi_setWheels(0,0);
                return 6;
            } else if(sensor -> bumpLeft) {
                uart_sendStr("bumpl;");
                move_backward(sensor, 5);
                return 7;
            } else if(sensor -> bumpRight){
                uart_sendStr("bumpr;");
                move_backward(sensor, 5);
                return 8;
            }

            return 0;
        }
