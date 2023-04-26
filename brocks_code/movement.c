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
    int cyBot_x;
    int cyBot_y;
    int cyBot_angle;

    void movement_init(oi_t* s) {
        sensor = s;
        int cyBot_x = 0;
        int cyBot_y = 0;
        int cyBot_angle = 0;
    }

    void move_forward(double distance_num){
        oi_setWheels(75,75);
            double sum = 0;
           while (sum <  (distance_num * 10.65 - 2.5)) {//STILL NEED TO CALIBRATE
//               if (check_Interrupts() != 0) {
//                   break;
//               }

           oi_update(sensor);

           sum += sensor -> distance; // use -> notation since pointer
          }

           cyBot_x -= (sum * 0.938 + 2.38) * sin(cyBot_angle * 3.14 / 180);
           cyBot_y += (sum * 0.938 + 2.38) * cos(cyBot_angle * 3.14 / 180);

        oi_setWheels(0,0);
    }


        void turn_right(double degrees){
            oi_setWheels(-100, 100);

                   double angle_t = 0;
                   while (angle_t >= degrees * 0.93) {//STILL NEED TO CALIBRATE

                       oi_update(sensor);
                       angle_t+=sensor->angle;
                   }
            oi_setWheels(0,0);
        }

        void turn_left(double degrees){
            oi_setWheels(100,-100);

                   double angle_t = 0;
                   while (angle_t <= degrees * 0.93) {//STILL NEED TO CALIBRATE


                       oi_update(sensor);

                       angle_t+=sensor->angle;
                   }
        oi_setWheels(0,0);
        }

        void move_backward(double distance_num){
            oi_setWheels(-75,-75);
               double sum = 0;
              while (sum > distance_num * -10.65 + 2.5) {//STILL NEED TO CALIBRATE


              oi_update(sensor);

              sum += sensor -> distance; // use -> notation since pointer
             }

            cyBot_x -= (sum * 0.938 + 2.38) * cos(cyBot_angle * 3.14 / 180);
            cyBot_y += (sum * 0.938 + 2.38) * sin(cyBot_angle * 3.14 / 180);
            oi_setWheels(0,0);

        }


        int check_Interrupts() {

            if (sensor->bumpLeft > 0) {

                return 1;
            } else if (sensor->bumpRight > 0) {

                return 2;
            } else if (sensor->cliffLeftSignal > TAPEVALUE) {

                return 3;
            } else if (sensor->cliffFrontLeftSignal > TAPEVALUE) {

                return 4;
            } else if (sensor->cliffFrontRightSignal > TAPEVALUE) {

                return 5;
            } else if (sensor->cliffRightSignal > TAPEVALUE) {

                return 6;
            }

            return 0;
        }

