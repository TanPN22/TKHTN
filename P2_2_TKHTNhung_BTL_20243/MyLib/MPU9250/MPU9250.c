#include "MPU9250.h"
#include "HCSR04.h"

float ax;
float ay;
float az;

float gx;
float gy;
float gz;

int16_t accel[3], gyro[3];

//Hàm đọc thanh ghi
uint8_t MPU_Read_Register(uint8_t reg) {
	uint8_t value;
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, reg, 1, &value, 1, 100);
	return value;
}

//Hàm ghi giá trị vào thanh ghi
void MPU_Write_Register(uint8_t reg, uint8_t value) {
	HAL_I2C_Mem_Write(&hi2c1, MPU9250_ADDR, reg, 1, &value, 1, 100);
}

//Khởi tạo MPU
void MPU9250_Init(void) {
	// Đánh thức MPU
	MPU_Write_Register(PWR_MGMT_1, 0x00);
	Delay_ms(100);

	// Cấu hình phạm vi đo ±2g cho ACC và ±250dps cho GYRO
	MPU_Write_Register(ACCEL_CONFIG, 0x00);
	MPU_Write_Register(GYRO_CONFIG,  0x00);
}

//Đọc dữ liệu
void MPU9250_Read_Accel_Gyro(int16_t *accel, int16_t *gyro) {
	uint8_t data[14];
	HAL_I2C_Mem_Read(&hi2c1, MPU9250_ADDR, ACCEL_XOUT_H, 1, data, 14, 100);

	// Gia tốc
	accel[0] = (int16_t) (data[0] << 8 | data[1]);
	accel[1] = (int16_t) (data[2] << 8 | data[3]);
	accel[2] = (int16_t) (data[4] << 8 | data[5]);

	// Con quay
	gyro[0] = (int16_t) (data[8] << 8 | data[9]);
	gyro[1] = (int16_t) (data[10] << 8 | data[11]);
	gyro[2] = (int16_t) (data[12] << 8 | data[13]);
}

void Send_Sensor_Data(int16_t *accel, int16_t *gyro) {
	    // Chuyển về đơn vị vật lý
	    ax = accel[0] / 16384.0f;
	    ay = accel[1] / 16384.0f;
	    az = accel[2] / 16384.0f;

	    gx = gyro[0] / 131.0f;
	    gy = gyro[1] / 131.0f;
	    gz = gyro[2] / 131.0f;
}

void MPU9250_Getdata(void)
{
	MPU9250_Read_Accel_Gyro(accel, gyro);
	Send_Sensor_Data(accel, gyro);
}

