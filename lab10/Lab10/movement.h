/*
 * movement.h
 *
 *  Created on: Jan 31, 2023
 *      Author: akronau
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"

void move_forward(oi_t *sensor, double distance_num);
void move_backward(oi_t *sensor,double distance_num);
void turn_left(oi_t *sensor,double degrees);
void turn_right(oi_t *sensor,double degrees);

#endif /* MOVEMENT_H_ */
