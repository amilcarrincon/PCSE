/*
 * rm3100.h
 *
 *  Created on: Jun 10, 2022
 *      Author: Amilcar Rincon
 */

#ifndef RM3100_DRIVER_INC_RM3100_H_
#define RM3100_DRIVER_INC_RM3100_H_

#include "main.h"


#define MAX_I2C_WRITE               32
#define RM3100_I2C_ADDRESS_7bit     0x20
#define RM3100_I2C_ADDRESS_8bit     0x20 << 1
#define RM3100_MAG_REG              0x00
#define RM3100_BEACON_REG           0x01
#define RM3100_TMRC_REG             0x0B
#define RM3100_REVID_REG            0x36
#define RM3100_TEST3_REG            0x72
#define RM3100_LROSCADJ_REG         0x63
#define RM3100_LROSCADJ_VALUE       0xA7
#define RM3100_SLPOSCADJ_VALUE      0x08
#define RM3100_ENABLED              0x79
#define RM3100_DISABLED             0x00
#define RM3100_QX2_REG      		0x24
#define RM3100_QX1_REG      		0x25
#define RM3100_QX0_REG      		0x26
#define RM3100_QY2_REG      		0x27
#define RM3100_QY1_REG      		0x28
#define RM3100_QY0_REG      		0x29
#define RM3100_QZ2_REG      		0x2A
#define RM3100_QZ1_REG      		0x2B
#define RM3100_QZ0_REG      		0x2C
#define RM3100_PNI_KEY1_REG     	0x2D
#define RM3100_PNI_KEY2_REG    		0x2E
#define RM3100_CCPX1_REG        	0x04
#define RM3100_CCPX0_REG        	0x05
#define RM3100_CCPY1_REG        	0x06
#define RM3100_CCPY0_REG        	0x07
#define RM3100_CCPZ1_REG        	0x08
#define RM3100_CCPZ0_REG        	0x09
#define RM3100_NOS_REG          	0x0A
#define CCP0    					0xC8    /* 200 Cycle Count */
#define CCP1    					0x00
#define NOS     					0x01    /* Number of Samples for averaging*/
#define TMRC    					0x04    /* Default rate 125 Hz */

/************************/
/*     Enumerations     */
/************************/
/**
* @enum SensorPowerMode
*
* @brief Possible sensor power modes supported.
*
*/
typedef enum {
    SensorPowerModePowerDown = 0,
    SensorPowerModeSuspend = 1,
    SensorPowerModeActive = 255,
} SensorPowerMode;

/**
* @enum SensorStatus
*
* @brief Sensor status results.
*/
typedef enum {
    /* Valid Responses */
	SensorOK,                       /**< @brief Sensor responded with expected data. */
	SensorInitialized,              /**< @brief Sensor has been initialized. */

    /* Error Responses */
	SensorUnknownError,             /**< @brief An unknown error has occurred. */
	SensorErrorNonExistant,         /**< @brief Unable to communicate with sensor, sensor did not ACK. */
	SensorErrorUnexpectedDevice,    /**< @brief A different sensor was detected at the address. */

	SensorStatusPending = 255,      /**< @brief Reserved for internal used */
} SensorStatus;

unsigned int rm3100_i2c_write(char registerAddress, char* buffer, short int length);
unsigned int rm3100_i2c_read(char registerAddress, char* buffer, short int length);


#endif /* RM3100_DRIVER_INC_RM3100_H_ */
