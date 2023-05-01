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
//        double cyBot_x = 0;
//        double cyBot_y = 0;
//        double cyBot_angle = 0;
    }

    void move_forward(double distance_num){
           int bot_distance = 0;
           oi_setWheels(50,50);

           while( bot_distance < distance_num) {//STILL NEED TO CALIBRATE
               oi_update(sensor);
               bot_distance += sensor -> distance;
               check_Interrupts();
           }

           oi_setWheels(0,0);
       }


           void turn_right(double degrees){
               double bot_angle = 0;
               oi_setWheels(-50,50);

               while(bot_angle > degrees * -0.93) {//STILL NEED TO CALIBRATE
                   oi_update(sensor);
                   bot_angle += sensor -> angle;
                   check_Interrupts();
               }
               oi_setWheels(0,0);
           }

           void turn_left(double degrees){
           double bot_angle = 0;
           oi_setWheels(50,-50);
           while(bot_angle > degrees * 0.93) {//STILL NEED TO CALIBRATE
               oi_update(sensor);
               bot_angle += sensor -> angle;
               check_Interrupts();
           }

           oi_setWheels(0,0);
           }

           void move_backward(double distance_num){
               int bot_distance = 0;
               oi_setWheels(-50,-50);
               int dnum = (int) distance_num;
               dnum = dnum * -1;
               while(bot_distance > dnum) {//STILL NEED TO CALIBRATE
                   oi_update(sensor);
                   bot_distance += sensor -> distance;
                   check_Interrupts();
               }

               oi_setWheels(0,0);

           }


           int check_Interrupts() {

               if (sensor -> wheelDropLeft || sensor -> cliffFrontLeft || sensor -> cliffLeft ) {
//                   uart_sendStr("drop;");
                   //move_backward(sensor,5);
                   oi_setWheels(0,0);
                   return 1;
               } else if (sensor -> wheelDropRight || sensor -> cliffFrontRight || sensor -> cliffRight) {
//                   uart_sendStr("drop;");
                //   move_backward(sensor, 5);
                   oi_setWheels(0,0);
                   return 2;
               } else if (sensor -> cliffLeftSignal > TAPEVALUE) {
//                   uart_sendStr("tape: L;");
                   oi_setWheels(0,0);
                   return 3;
               } else if (sensor -> cliffFrontLeftSignal > TAPEVALUE) {
//                   uart_sendStr("tape: FL;");
                   oi_setWheels(0,0);
                   return 4;
               } else if (sensor -> cliffFrontRightSignal > TAPEVALUE) {
//                   uart_sendStr("tape: FR;");
                   oi_setWheels(0,0);
                   return 5;
               } else if (sensor -> cliffRightSignal > TAPEVALUE) {
//                   uart_sendStr("tape: R;");
                   oi_setWheels(0,0);
                   return 6;
               } else if(sensor -> bumpLeft) {
//                   uart_sendStr("bumpl;");
                   move_backward( 5);
                   return 7;
               } else if(sensor -> bumpRight){
//                   uart_sendStr("bumpr;");
                   move_backward(5);
                   return 8;
               }

               return 0;
           }
