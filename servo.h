/*
 *
 * pwm.h
 *
 */

#ifndef SERVO_H_
#define SERVO_H_

void servo_init(int bot);
void set_servo_angle(double angle);
void set_servo_offset_angle(double offset);
void set_servo_angle0(void);
void set_servo_angle_center(void);
void set_servo_angle180(void);
char get_current_angle(void);

#endif
