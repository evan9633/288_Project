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

#define TAPEVALUE 470 //STILL NEED TAPE VALUE
    oi_t* sensor;

    void movement_init(oi_t* s) {
        sensor = s;
    }

    void move_forward(double distance_num){
        int bot_distance = 0;
        oi_setWheels(50,50);

        while(check_Interrupts() == 0 && bot_distance < distance_num) {//STILL NEED TO CALIBRATE
            oi_update(sensor);
            bot_distance += sensor -> distance;
        }

        oi_setWheels(0,0);
    }


        void turn_right(double degrees){
            double bot_angle = 0;
            oi_setWheels(-50,50);

            while(check_Interrupts() == 0 && bot_angle > degrees * -1) {//STILL NEED TO CALIBRATE
                oi_update(sensor);
                bot_angle += sensor -> angle;
            }
            oi_setWheels(0,0);
        }

        void turn_left(double degrees){
        double bot_angle = 0;
        oi_setWheels(50,-50);
        while(check_Interrupts() == 0 && bot_angle > degrees * 1) {//STILL NEED TO CALIBRATE
            oi_update(sensor);
            bot_angle += sensor -> angle;
        }

        oi_setWheels(0,0);
        }

        void move_backward(double distance_num){
            int bot_distance = 0;
            oi_setWheels(-50,-50);
            while(check_Interrupts() == 0 && bot_distance > distance_num * -1) {//STILL NEED TO CALIBRATE
                oi_update(sensor);
                bot_distance += sensor -> distance;
            }

            oi_setWheels(0,0);

        }


        int check_Interrupts() {

            if (sensor -> wheelDropLeft || sensor -> cliffFrontLeft || sensor -> cliffLeft || sensor -> bumpLeft) {

                move_backward(10);
                return 1;
            } else if (sensor -> wheelDropRight || sensor -> cliffFrontRight || sensor -> cliffRight || sensor -> bumpRight) {

                move_backward(10);
                return 2;
            } else if (sensor -> cliffLeftSignal > TAPEVALUE) {

                move_backward(10);
                return 3;
            } else if (sensor -> cliffFrontLeftSignal > TAPEVALUE) {

                move_backward(10);
                return 4;
            } else if (sensor -> cliffFrontRightSignal > TAPEVALUE) {

                move_backward(10);
                return 5;
            } else if (sensor -> cliffRightSignal > TAPEVALUE) {

                move_backward(10);
                return 6;
            }

            return 0;
        }
