/*
 * gyroscope.h
 *
 *  Created on: 11 Feb 2025
 *      Author: Lenovo
 */

#ifndef INCLUDE_GYROSCOPE_H_
#define INCLUDE_GYROSCOPE_H_

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

#include "Include/I2C_Driver.h"
#include "Include/UART_Driver.h"
#include "Include/accelerometer.h"

#define BMX160 0x69 // CHIP_ID = D8 (address = 0x0)

typedef struct {
    float gyrX, gyrY, gyrZ;
} GyroscopeData;

void enableGyroscopeBMX160();

float configureGyroscopeBMX160(uint16_t outputDataRate, uint8_t bandwidthCoefficient, uint16_t measurementRange);

void disableGyroscopeBMX160();

void selfTestGyroscope();

int gyroisEnabledBMX160();

void readParseGyroSensor(uint8_t slaveAddress, GyroscopeData* data, float scaleFactor);

#endif /* INCLUDE_GYROSCOPE_H_ */
