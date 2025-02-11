#include "Include/I2C_Driver.h"

void InitI2C1() {
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

void writeRegisterI2C1(uint8_t address, uint8_t regAddress, uint8_t value) {
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

uint8_t readRegisterI2C1(uint8_t address, uint8_t regAddress) {
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


