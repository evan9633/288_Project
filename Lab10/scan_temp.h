/*
 * scan_temp.h
 *
 *  Created on: Feb 15, 2023
 *      Author: erdunn, czulk
 */

#ifndef SCAN_TEMP_H_
#define SCAN_TEMP_H_
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "open_interface.h"

typedef struct{
    float sound_dist;
    float IR_dist;
    int angle;
    int width;
    int x_co;
    int y_co;

}obj;

obj storage[10];
void drive();

void scan_cust(cyBOT_Scan_t getScan,oi_t *sensor_data,int num_scans, double high_deg, double low_deg);

void add_obj( int angle, float sound_dist, float IR_dist);
void initScan();
void print_data(int zo,cyBOT_Scan_t getScan, oi_t *sensor_data);
obj getNextObject();
int scan_precision(cyBOT_Scan_t getScan, oi_t *sensor_data);
int nearest_obj(cyBOT_Scan_t getScan, oi_t *sensor_data);
void calc_coor(cyBOT_Scan_t getScan, oi_t *sensor_data, int index);

#endif /* SCAN_TEMP_H_ */
