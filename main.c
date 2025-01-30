#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/flash.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"

#define BUFFER_SIZE 100
#define BMX160 0x69 // CHIP_ID = D8 (address = 0x0)
#define BME680 0x77 // CHIP_ID = 61 (for I2C, address = 0xD0)

void InitI2C() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {
    }

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);

    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
}

bool addressExists(uint8_t address) {
    I2CMasterSlaveAddrSet(I2C1_BASE, address, false);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(I2CMasterBusy(I2C1_BASE)) {
    }

    if(I2CMasterErr(I2C1_BASE) == I2C_MASTER_ERR_NONE) {
        return true;
    }

    return false;
}

void scanI2CAddress() {

    while(1) {
        uint8_t currAddress;
        for(currAddress = 0x0; currAddress <= 0x7f; currAddress++) {
            if(addressExists(currAddress)) {
                UARTCharPut(UART5_BASE, currAddress);
                UARTCharPut(UART5_BASE, '\n');
            }
        }
        SysCtlDelay(SysCtlClockGet() / 3);
    }

}

void writeRegisterI2C(uint8_t address, uint8_t regAddress, uint8_t value) {
    I2CMasterSlaveAddrSet(I2C1_BASE, address, false);
    I2CMasterDataPut(I2C1_BASE, regAddress);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    while (I2CMasterBusy(I2C1_BASE)) {
    }

    I2CMasterDataPut(I2C1_BASE, value);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    while (I2CMasterBusy(I2C1_BASE)) {
    }
}

uint8_t readRegisterI2C(uint8_t address, uint8_t regAddress) {
    I2CMasterSlaveAddrSet(I2C1_BASE, address, false);
    I2CMasterDataPut(I2C1_BASE, regAddress);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while (I2CMasterBusy(I2C1_BASE)) {
    }

    I2CMasterSlaveAddrSet(I2C1_BASE, address, true);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    while (I2CMasterBusy(I2C1_BASE)) {
    }

    return I2CMasterDataGet(I2C1_BASE);

}

void InitUART() {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {
    }

    GPIOPinConfigure(GPIO_PE4_U5RX);
    GPIOPinConfigure(GPIO_PE5_U5TX);
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    UARTConfigSetExpClk(UART5_BASE, SysCtlClockGet(), 9600,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    UART_CONFIG_PAR_NONE));

    /*int32_t pc_to_tivaC = UARTCharGet(UART5_BASE);
    UARTCharPut(UART5_BASE, pc_to_tivaC);
    UARTCharPut(UART5_BASE, '\n');*/

    //UARTDisable(UART5_BASE);
}

void getCommand(char *buffer, uint32_t size) {
    uint32_t index = 0;

    while(index < (size - 1)) {
        if(UARTCharsAvail(UART5_BASE)) {
            char c = UARTCharGet(UART5_BASE);

            if(c == '\n') {
                break;
            }

            buffer[index++] = c;
        }
    }

    buffer[index] = '\0';
}

void loop() {
    char uartBuffer[BUFFER_SIZE];
    char currChar;

    while(1) {
        getCommand(uartBuffer, BUFFER_SIZE);
        uint8_t i = 0;

        while(1) {
            currChar = uartBuffer[i];
            if(currChar == '\0') {
                break;
            }

            UARTCharPut(UART5_BASE, currChar);
            i++;
        }
        UARTCharPut(UART5_BASE, '\n');
        //SysCtlDelay(SysCtlClockGet() / 3);
    }

}

void blinkLED() {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {
    }

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    while(1) {

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

        SysCtlDelay(1000000);

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

        SysCtlDelay(1000000);
    }

}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    InitUART();
    InitI2C();
    //scanI2CAddress();

    SysCtlDelay(SysCtlClockGet());
    writeRegisterI2C(BMX160, 0x42, 0x28);
    UARTCharPut(UART5_BASE, readRegisterI2C(BMX160, 0x0));
    UARTCharPut(UART5_BASE, readRegisterI2C(BMX160, 0x42));

    //loop();

    return 0;
}



















