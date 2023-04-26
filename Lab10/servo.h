#ifndef SERVO_H_
 #define SERVO_H_


 #include <inc/tm4c123gh6pm.h>
 #include <stdint.h>
 #include <stdbool.h>
 #include "Timer.h"

 void servo_init(void);
 void servo_move(uint16_t degrees);

#endif
