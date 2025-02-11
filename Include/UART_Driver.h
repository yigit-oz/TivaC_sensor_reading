/*
 * UART_Driver.h
 *
 *  Created on: 11 Feb 2025
 *      Author: Lenovo
 */

#ifndef INCLUDE_UART_DRIVER_H_
#define INCLUDE_UART_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"

void InitUART5();

void sendMessage(char *msg);

#endif /* INCLUDE_UART_DRIVER_H_ */
