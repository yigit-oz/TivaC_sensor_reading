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
#include "Include/accelerometer.h"

#define BME680 0x77 // CHIP_ID = 61 (for I2C, address = 0xD0)

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    InitUART5();
    InitI2C1();

    writeRegisterI2C1(BMX160, 0x7E, 0xB6); // Reset
    SysCtlDelay(SysCtlClockGet() / 6);

    enableGyroscopeBMX160();
    float scaleFactor = configureGyroscopeBMX160(3200, 2, 125);

    UARTCharPut(UART5_BASE, readRegisterI2C1(BMX160, 0x0)); // Chip Id
    UARTCharPut(UART5_BASE, readRegisterI2C1(BMX160, 0x03)); // PMU register
    UARTCharPut(UART5_BASE, readRegisterI2C1(BMX160, 0x42)); // GYR_CONF
    UARTCharPut(UART5_BASE, readRegisterI2C1(BMX160, 0x43)); // GYR_RANGE
    selfTestGyroscope();

    GyroscopeData gyroData;

    while(1) {
        readParseGyroSensor(BMX160, &gyroData, scaleFactor);
        UARTCharPut(UART5_BASE, gyroData.gyrX);
        UARTCharPut(UART5_BASE, gyroData.gyrY);
        UARTCharPut(UART5_BASE, gyroData.gyrZ);

        SysCtlDelay(SysCtlClockGet() / 6);
    }

    return 1;
}



















