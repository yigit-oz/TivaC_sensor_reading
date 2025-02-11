#include "Include/UART_Driver.h"

void InitUART5() {

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

void sendMessage(char *msg) {
    uint8_t index = 0;
    UARTCharPut(UART5_BASE, sizeof(msg));

    while(1) {
        char curr = msg[index];
        if(curr == '\0') {
            return;
        }

        UARTCharPut(UART5_BASE, msg[index]);
        index++;
    }

}
