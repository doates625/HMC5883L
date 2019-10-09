/**
 * @file HMC5883L.h
 * @brief Class for interfacing with HMC5883L 3-axis magnetometer
 * @author Dan Oates (WPI Class of 2020)
 * 
 * The HMC5883L is a 3-axis I2C magnetometer. This class acts as an I2C
 * interface with the device with support for both the Arduino and Mbed
 * platforms.
 * 
 * Dependencies:
 * - I2CDevice: https://github.com/doates625/I2CDevice.git
 * - Platform: https://github.com/doates625/Platform.git
 * - Unions: https://github.com/doates625/Unions.git
 * 
 * References:
 * - Datasheet: https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
 */
#pragma once
#include <I2CDevice.h>

class HMC5883L
{
public:

	// Initialization
	HMC5883L(I2CDEVICE_I2C_CLASS* i2c);
	bool init();

	// Readings
	void update();
	float get_mag_x();
	float get_mag_y();
	float get_mag_z();

protected:

	// I2C Interface
	static const int i2c_addr = 0x3D;
	static const char reg_id_A_addr = 0x0A;
	static const char reg_id_A_ref = 0x48;
	static const char reg_id_B_addr = 0x0B;
	static const char reg_id_B_ref = 0x34;
	static const char reg_id_C_addr = 0x0C;
	static const char reg_id_C_ref = 0x33;
	static const char reg_mode_addr = 0x02;
	static const char reg_mode_continuous = 0x00;
	static const char reg_data_addr = 0x03;
	static const float gauss_per_lsb = 0.00092f;

	// Communication
	I2CDevice i2c;

	// Readings
	float mag_x, mag_y, mag_z;
};