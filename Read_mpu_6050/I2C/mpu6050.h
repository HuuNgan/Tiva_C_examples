/*
 * mpu6050.h
 *
 *  Created on: 1 thg 4, 2021
 *      Author: huungan
 */

#ifndef I2C_MPU6050_H_
#define I2C_MPU6050_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

void initI2C(void);
void i2cWrite(uint8_t addr, uint8_t regAddr, uint8_t data);
void i2cWriteData(uint8_t addr, uint8_t regAddr, uint8_t *date, uint8_t length);
uint8_t i2cRead(uint8_t addr, uint8_t regAddr);
void i2cReadData(uint8_t addr, uint8_t regAddr, uint8_t *data, uint8_t length);
void initMPU6050(void);
void getMPU6050Data(void);
#ifdef __cplusplus
}
#endif


#endif /* I2C_MPU6050_H_ */
