/*
 * rm3100.c
 *
 *  Created on: Jun 10, 2022
 *      Author: Amilcar Rincon
 */
#include "main.h"
#include "rm3100.h"

extern I2C_HandleTypeDef hi2c1;

/*****************************/
/*        Static variables   */
/*****************************/
static unsigned short int mSampleRate;
static SensorPowerMode mSensorMode;
static char mSamples[9];

/*
 * CODIGO I2C
 * */
// MBED native READ/ WRITE functions
unsigned int rm3100_i2c_write(char registerAddress, char* buffer, short int length)
{
    char writeBuffer[MAX_I2C_WRITE + 1];
    writeBuffer[0] = registerAddress;
    memcpy(&writeBuffer[1], buffer, length);
    //i2c.write(RM3100_I2C_ADDRESS_8bit, const char *data, int length, 1);
    // returns 0 on success (ack), non-0 on failure (nack)
    //int status = i2c.write(RM3100_I2C_ADDRESS_8bit, writeBuffer, length+1, 0);
    int status = HAL_I2C_Master_Transmit(&hi2c1, RM3100_I2C_ADDRESS_8bit, writeBuffer, length+1, 100);
    return !status; // return True if successfull. mbed:0=Success
}

unsigned int rm3100_i2c_read(char registerAddress, char* buffer, short int length)
{
    char writeBuffer[1] = {registerAddress};
    HAL_I2C_Master_Transmit(&hi2c1, RM3100_I2C_ADDRESS_8bit, writeBuffer, 1, 100);
    //i2c.write(RM3100_I2C_ADDRESS_8bit, writeBuffer, 1, 1);
    //int status = i2c.read(RM3100_I2C_ADDRESS_8bit, buffer, length, 0);
    int status = HAL_I2C_Master_Receive(&hi2c1, RM3100_I2C_ADDRESS_8bit, buffer, length, 100);
    if (!status) //mbed:0=Success
        return length;
    else
        return 0;
}


/*****************************/
/*        Functions          */
/*****************************/
/**
 * @fn SensorStatus mag_enable_interrupts();
 *
 * @brief Enables the interrupt request from the sensor.
 *
 * @returns Status of the sensor. Not supported in AKM8975
 */
SensorStatus mag_enable_interrupts() {
	static char data[] = { RM3100_ENABLED };

	if (mSensorMode == SensorPowerModeActive) {
		rm3100_i2c_write(RM3100_BEACON_REG, data, sizeof(data) / sizeof(char));
	}
	return SensorOK;
}

/**
 * @fn SensorStatus mag_disable_interrupts();
 *
 * @brief Disables the interrupt request from the sensor.
 *
 * @returns Status of the sensor.
 */
SensorStatus mag_disable_interrupts() {
	static char data[] = { RM3100_DISABLED };
	rm3100_i2c_write(RM3100_BEACON_REG, data, sizeof(data) / sizeof(char));
	return SensorOK;
}

/**
 * @fn SensorMode mag_set_power_mode(SensorPowerMode mode);
 *
 * @brief If possible, sets the sensor to the requested power mode.
 *  *
 * @param mode  The requested sensor mode.
 * @returns     The actual state the sensor was set to.
 */
SensorPowerMode mag_set_power_mode(SensorPowerMode mode) {
	switch (mode) {
	default:
		return mSensorMode;

	case SensorPowerModePowerDown:
	case SensorPowerModeSuspend:
		mSensorMode = mode;
		mag_disable_interrupts();
		break;

	case SensorPowerModeActive:
		mSensorMode = SensorPowerModeActive;
		mag_enable_interrupts();
		break;
	}

	return mSensorMode;
}

/**
 * @fn SensorStatus mag_initialize_sensor();
 *
 * @brief Initializes the sensor into a known state.
 *
 * @retval SensorOK                         The sensor has already been initialized
 * @retval SensorErrorUnexpectedDevice      The sensor did not return expected results.
 * @retval SensorUnknownError               An unknown error has occured.
 */
SensorStatus mag_initialize_sensor() {
	char i2cbuffer[2];
	char settings[7];

	if (rm3100_i2c_read(RM3100_LROSCADJ_REG, i2cbuffer, 2) != SensorOK) {
		return SensorErrorNonExistant;
	}

	if ((i2cbuffer[0] != RM3100_LROSCADJ_VALUE)
			|| (i2cbuffer[1] != RM3100_SLPOSCADJ_VALUE)) {
		return SensorErrorUnexpectedDevice;
	}

	/* Zero buffer content */
	i2cbuffer[0] = 0;
	i2cbuffer[1] = 0;

	/* Clears MAG and BEACON register and any pending measurement */
	rm3100_i2c_write(RM3100_MAG_REG, i2cbuffer, 2);

	/* Initialize settings */
	settings[0] = CCP1; /* CCPX1 */
	settings[1] = CCP0; /* CCPX0 */
	settings[2] = CCP1; /* CCPY1 */
	settings[3] = CCP0; /* CCPY0 */
	settings[4] = CCP1; /* CCPZ1 */
	settings[5] = CCP0; /* CCPZ0 */
	settings[6] = NOS;
	/* settings[7]=TMRC;  */

	/*  Write register settings */
	rm3100_i2c_write(RM3100_CCPX1_REG, settings, 7);

	mag_set_power_mode(SensorPowerModePowerDown);

	return SensorOK;
}

/**
 * @fn SensorMode mag_get_power_mode();
 *
 * @brief Used to determine the current power mode of the sensor.
 *
 * @returns The current power mode of the sensor.
 */
SensorPowerMode mag_get_power_mode() {
	return mSensorMode;
}

/**
 * @fn unsigned short int mag_set_sample_rate(unsigned short int sample_rate);
 *
 * @brief Requests the hardware to perform sample conversions at the specified rate.
 *
 * @param sample_rate The requested sample rate of the sensor in Hz.
 *
 * @returns The actual sample rate of the sensor.
 */
unsigned short mag_set_sample_rate(unsigned short sample_rate) {
	int i;
	static char i2cbuffer[1];
	const unsigned short int supported_rates[][2] = {
	/* [Hz], register value */
	{ 2, 0x0A },   // up to 2Hz
			{ 4, 0x09 },   // up to 4Hz
			{ 8, 0x08 },   // up to 8Hz
			{ 16, 0x07 },   // up to 16Hz
			{ 31, 0x06 },   // up to 31Hz
			{ 62, 0x05 },   // up to 62Hz
			{ 125, 0x04 },  // up to 125Hz
			{ 220, 0x03 }   // up to 250Hz
	};

	for (i = 0;
			i < sizeof(supported_rates) / (sizeof(unsigned short int) * 2) - 1;
			i++) {
		if (sample_rate <= supported_rates[i][0])
			break;
	}

	if (mSensorMode == SensorPowerModeActive) {
		mag_disable_interrupts();
	}

	mSampleRate = supported_rates[i][0];
	i2cbuffer[0] = (char) supported_rates[i][1];

	rm3100_i2c_write(RM3100_TMRC_REG, i2cbuffer, 1);

	if (mSensorMode == SensorPowerModeActive) {
		mag_enable_interrupts();
	}

	return mSampleRate;

}

/**
 * @fn unsigned short int mag_get_sample_rate();
 *
 * @brief Retrieves the mset sample rate of the sensor.
 *
 * @returns The actual sample rate of the sensor.
 */
unsigned short mag_get_sample_rate() {
	return mSampleRate;
}

/**
 * @fn SensorStatus mag_get_sample_data(signed int *x, signed int *y, signed int *z);
 *
 * @brief Initiates an i2c read of the RM3100's sensor result registers.
 * @Each sensor reading consists of 3 bytes of data which are stored in 2’s
 * @complement format (range: -8388608 to 8388607) in the Results Registers
 *
 * @output: 3-axis Sensor data in Count
 *
 */
void mag_get_sample_data(int *XYZ) {
	// read out sensor data
	rm3100_i2c_read(RM3100_QX2_REG, (char*) &mSamples,
			sizeof(mSamples) / sizeof(char));

	XYZ[0] = ((signed char) mSamples[0]) * 256 * 256;
	XYZ[0] |= mSamples[1] * 256;
	XYZ[0] |= mSamples[2];

	XYZ[1] = ((signed char) mSamples[3]) * 256 * 256;
	XYZ[1] |= mSamples[4] * 256;
	XYZ[1] |= mSamples[5];

	XYZ[2] = ((signed char) mSamples[6]) * 256 * 256;
	XYZ[2] |= mSamples[7] * 256;
	XYZ[2] |= mSamples[8];
}



