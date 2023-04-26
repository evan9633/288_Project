/*
 * movement.h
 *
 *  Created on: Jan 31, 2023
 *      Author: Brock Dykhuis
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"

void movement_init(oi_t* s);
void move_forward(double distance_num);
void move_backward(double distance_num);
void turn_left(double degrees);
void turn_right(double degrees);
int check_Interrupts(void);

#endif /* MOVEMENT_H_ */
