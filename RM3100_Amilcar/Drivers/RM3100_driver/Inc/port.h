/*
 * port.h
 *
 *  Created on: Jun 10, 2022
 *      Author: Amilcar Rincon
 */

#ifndef RM3100_DRIVER_INC_PORT_H_
#define RM3100_DRIVER_INC_PORT_H_

#include "main.h"
#include "rm3100.h"

SensorStatus mag_enable_interrupts();
SensorStatus mag_disable_interrupts();
SensorPowerMode mag_set_power_mode(SensorPowerMode mode);
SensorStatus mag_initialize_sensor();
SensorPowerMode mag_get_power_mode();
unsigned short mag_set_sample_rate(unsigned short sample_rate);
unsigned short mag_get_sample_rate();
void mag_get_sample_data(int * XYZ);

#endif /* RM3100_DRIVER_INC_PORT_H_ */
