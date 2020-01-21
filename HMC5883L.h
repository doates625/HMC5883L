/**
 * @file HMC5883L.h
 * @brief Class for interfacing with the HMC5883L 3-axis I2C magnetometer
 * @author Dan Oates (WPI Class of 2020)
 */
#pragma once
#include <I2CDevice.h>
#include <I2CReading.h>

class HMC5883L
{
public:

	// Full-scale range
	typedef enum
	{
		range_088uT,	// +/-088uT
		range_130uT,	// +/-130uT
		range_190uT,	// +/-190uT
		range_250uT,	// +/-250uT
		range_400uT,	// +/-400uT
		range_470uT,	// +/-470uT
		range_560uT,	// +/-560uT
		range_810uT,	// +/-810uT
	}
	range_t;

	// Calibrations
	float x_cal;	// X offset [uT]
	float y_cal;	// Y offset [uT]
	float z_cal;	// Z offset [uT]

	// Initialization
	HMC5883L(I2CDevice::i2c_t* i2c);
	bool init();
	void set_range(range_t range);

	// Readings
	void update();
	float get_x();
	float get_y();
	float get_z();

protected:

	// I2C Communication
	static const int i2c_addr = 0x1E;
	I2CDevice i2c;

	// I2C Interface
	static const uint8_t reg_id_A_addr = 0x0A;
	static const uint8_t reg_id_A_ref = 0x48;
	static const uint8_t reg_id_B_addr = 0x0B;
	static const uint8_t reg_id_B_ref = 0x34;
	static const uint8_t reg_id_C_addr = 0x0C;
	static const uint8_t reg_id_C_ref = 0x33;
	static const uint8_t reg_mode_addr = 0x02;
	static const uint8_t reg_mode_cont = 0x00;
	static const uint8_t reg_config_B_addr = 0x01;
	static const uint8_t reg_config_B_088uT = 0x0 << 5;
	static const uint8_t reg_config_B_130uT = 0x1 << 5;
	static const uint8_t reg_config_B_190uT = 0x2 << 5;
	static const uint8_t reg_config_B_250uT = 0x3 << 5;
	static const uint8_t reg_config_B_400uT = 0x4 << 5;
	static const uint8_t reg_config_B_470uT = 0x5 << 5;
	static const uint8_t reg_config_B_560uT = 0x6 << 5;
	static const uint8_t reg_config_B_810uT = 0x7 << 5;
	static const uint8_t reg_mag_x_addr = 0x03;
	static const uint8_t reg_mag_z_addr = 0x05;
	static const uint8_t reg_mag_y_addr = 0x07;
	
	// Readings
	float uT_per_lsb;
	I2CReading<int16_t> mag_x, mag_y, mag_z;
};
