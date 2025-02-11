/*
 * Accelerometer.h
 *
 *  Created on: 11 Feb 2025
 *      Author: Lenovo
 */

#ifndef INCLUDE_ACCELEROMETER_H_
#define INCLUDE_ACCELEROMETER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/flash.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

#include "Include/gyroscope.h"
#include "Include/I2C_Driver.h"
#include "Include/UART_Driver.h"

#define BMX160 0x69 // CHIP_ID = D8 (address = 0x0)

typedef struct {
    float accX, accY, accZ;
} AccelerometerData;

void enableAccelerometerBMX160();

void configureAccelerometerBMX160();

#endif /* INCLUDE_ACCELEROMETER_H_ */
