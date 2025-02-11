#include "Include/accelerometer.h"

#define BMX160 0x69 // CHIP_ID = D8 (address = 0x0)

void enableAccelerometerBMX160() {
    writeRegisterI2C1(BMX160, 0x7E, 0x11); // Set accelerometer normal mode through cmd register 0x7E
    SysCtlDelay(SysCtlClockGet() / 6);
}

void configureAccelerometerBMX160() {

}
