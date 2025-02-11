/*
 * I2C_Drvier.h
 *
 *  Created on: 11 Feb 2025
 *      Author: Lenovo
 */

#ifndef INCLUDE_I2C_DRIVER_H_
#define INCLUDE_I2C_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"

void InitI2C1();

bool addressExists(uint8_t address);

void scanI2CAddress();

void writeRegisterI2C1(uint8_t address, uint8_t regAddress, uint8_t value);

uint8_t readRegisterI2C1(uint8_t address, uint8_t regAddress);

#endif /* INCLUDE_I2C_DRIVER_H_ */
