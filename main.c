#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/flash.h"
#include "driverlib/pin_map.h"
#include "driverlib/eeprom.h"

// Masaüstü

#define BUFFER_SIZE 100

void configureI2C() {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1) && !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)) {
    }

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
}

void configureUART() {

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


    configureUART();
    configureI2C();
    //blinkLED();

    loop();

    return 0;
}




















