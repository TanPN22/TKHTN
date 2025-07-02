#ifndef MPU9250_H_INC
#define MPU9250_H_INC

#include "main.h"

#define MPU9250_ADDR         0x68 << 1
#define WHO_AM_I             0x75
#define PWR_MGMT_1           0x6B
#define ACCEL_XOUT_H         0x3B
#define GYRO_XOUT_H          0x43
#define ACCEL_CONFIG   0x1C
#define GYRO_CONFIG    0x1B

extern float ax;
extern float ay;
extern float az;

extern float gx;
extern float gy;
extern float gz;

void MPU9250_Getdata(void);

#endif
