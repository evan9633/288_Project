/*
 * movement.c
 *
 *  Created on: Jan 31, 2023
 *      Author: akronau
 */

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"

    void move_forward(oi_t *sensor, double distance_num){
        double sum = 0;
        double tempsum = 0;
        oi_setWheels(50,50);
        while(sum < distance_num){
            oi_update(sensor);
            sum += sensor -> distance;
        }

        oi_setWheels(0,0);
    }


        void turn_right(oi_t *sensor, double degrees){
            double sum = 0;
            oi_setWheels(-100,100);
            while(sum > degrees){
                oi_update(sensor);
                sum += sensor -> angle;
            }
            oi_setWheels(0,0);
        }

        void turn_left(oi_t *sensor, double degrees){
        double sum = 0;
        oi_setWheels(100,-100);
        while(sum < degrees){
            oi_update(sensor);
            sum += sensor -> angle;
        }

        oi_setWheels(0,0);
        }

        void move_backward(oi_t *sensor,double distance_num){
            double sum = 0;
            oi_setWheels(-500,-500);
            while(sum > distance_num*-1){
                oi_update(sensor);
                sum += sensor -> distance;
            }

            oi_setWheels(0,0);

        }

