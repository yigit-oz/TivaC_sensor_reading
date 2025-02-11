#include "Include/gyroscope.h"

#define BMX160 0x69 // CHIP_ID = D8 (address = 0x0)

void enableGyroscopeBMX160() {
    enableAccelerometerBMX160();
    writeRegisterI2C1(BMX160, 0x7E, 0x15); // Set gyroscope normal mode through cmd register 0x7E
    SysCtlDelay(SysCtlClockGet() / 6);
}

// outputDataRate legal arguments: 25hz, 100hz, 1600hz, 3200hz
// bandwidthCoefficient legal arguments: 2(normal mode), 1(OSR2 mode), 0(OSR4 mode)
// measurementRange legal arguments: ±125°/s, ±250°/s, ±500°/s, ±1000°/s, ±2000°/s
// returns the scale factor for parsing the raw data
float configureGyroscopeBMX160(uint16_t outputDataRate, uint8_t bandwidthCoefficient, uint16_t measurementRange) {
    if(gyroisEnabledBMX160() == 0) {
        return 0.0;
    }
    writeRegisterI2C1(BMX160, 0x42, 0x0);
    uint8_t gyrConf = 0x0;

    switch(outputDataRate) {
        case 25:
            gyrConf |= (1 << 1) | (1 << 2);
            break;
        case 100:
            gyrConf |= (1 << 3);
            break;
        case 1600:
            gyrConf |= (1 << 3) | (1 << 2);
            break;
        case 3200:
            gyrConf |= (1 << 3) | (1 << 2) | 1;
            break;
        default:
            sendMessage("configureGyroscope function error: Illegal outputDataRate argument. Read the top comment for legal arguments.");
            return 0.0;
    }

    switch(bandwidthCoefficient) {
            case 2:
                gyrConf |= (1 << 5);
                break;
            case 1:
                gyrConf |= (1 << 4);
                break;
            case 0:
                break;
            default:
                sendMessage("configureGyroscope function error: Illegal bandwidthCoefficient argument. Read the top comment for legal arguments.");
                return 0.0;
        }

    writeRegisterI2C1(BMX160, 0x43, 0x0);
    uint8_t gyrRange = 0x0;
    float scaleFactor = 0.0;

    switch(measurementRange) {
            case 125:
                gyrRange |= (1 << 2);
                scaleFactor = 1.0 / 262.4;
                break;
            case 250:
                gyrRange |= (1 << 1) | 1;
                scaleFactor = 1.0 / 131.2;
                break;
            case 500:
                gyrRange |= (1 << 1);
                scaleFactor = 1.0 / 65.6;
                break;
            case 1000:
                gyrRange |= 1;
                scaleFactor = 1.0 / 32.8;
                break;
            case 2000:
                scaleFactor = 1.0 / 16.4;
                break;
            default:
                sendMessage("configureGyroscope function error: Illegal measurementRange argument. Read the top comment for legal arguments.");
                return 0.0;
        }

    writeRegisterI2C1(BMX160, 0x42, gyrConf); // Set output data rate 1600 hz, bandwidth coefficient set to normal mode
    writeRegisterI2C1(BMX160, 0x43, gyrRange); // Measurement range +-1000 degrees/s
    return scaleFactor;
}

void disableGyroscopeBMX160() {

}

void selfTestGyroscope() {
    writeRegisterI2C1(BMX160, 0x6D, (1 << 4));
    SysCtlDelay(SysCtlClockGet() / 6);
    UARTCharPut(UART5_BASE, readRegisterI2C1(BMX160, 0x1B));
}

int gyroisEnabledBMX160() {
    uint8_t pmu = readRegisterI2C1(BMX160, 0x03);
    uint8_t mask = (1 << 4)|(1 << 2);
    if((pmu & mask) != mask) {
        char errorMsg[] = "Error: Gyroscope not enabled.";
        sendMessage(errorMsg);
        return 0;
    }
    else {
        return 1;
    }
}

void readParseGyroSensor(uint8_t slaveAddress, GyroscopeData* data, float scaleFactor) {
    uint8_t rawData[6];
    uint8_t j = 0;
    uint8_t i;

    for(i = 0x0C; i <= 0x11; i++) {
        rawData[j] = readRegisterI2C1(slaveAddress, i);
        j++;
    }

    data->gyrX = (float)((int16_t)((rawData[1] << 8) | rawData[0]) * scaleFactor);
    data->gyrY = (float)((int16_t)((rawData[3] << 8) | rawData[2]) * scaleFactor);
    data->gyrZ = (float)((int16_t)((rawData[5] << 8) | rawData[4]) * scaleFactor);

}

