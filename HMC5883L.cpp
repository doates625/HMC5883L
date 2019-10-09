/**
 * @file HMC5883L.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include "HMC5883L.h"

/**
 * @brief Constructs new magnetometer with given I2C interface
 */
HMC5883L::HMC5883L(I2CDEVICE_I2C_CLASS* i2c)
{
	this->i2c = I2CDevice(i2c, i2c_addr, I2CDevice::msb_first);
	this->mag_x = 0.0f;
	this->mag_y = 0.0f;
	this->mag_z = 0.0f;
}

/**
 * @brief Initializes magnetometer in continuous mode
 * @return True if I2C communication was successful
 */
bool HMC5883L::init()
{
	// Attempt ID register read
	i2c.read_sequence(reg_id_A_addr, 3);
	uint8_t reg_id_A_test = i2c.read_uint8();
	uint8_t reg_id_B_test = i2c.read_uint8();
	uint8_t reg_id_C_test = i2c.read_uint8();

	// Compare reads to reference values
	if (reg_id_A_test != reg_id_A_ref) return false;
	if (reg_id_B_test != reg_id_B_ref) return false;
	if (reg_id_C_test != reg_id_C_ref) return false;

	// Set device to continuous sampling
	i2c.write_uint8(reg_mode_addr, reg_mode_continuous);
	
	// Everything succeeded
	return true;
}

/**
 * @brief Gets new readings from magnetometer
 */
void HMC5883L::update()
{
	i2c.read_sequence(reg_data_addr, 6);
	mag_x = i2c.read_int16() * gauss_per_lsb;
	mag_y = i2c.read_int16() * gauss_per_lsb;
	mag_z = i2c.read_int16() * gauss_per_lsb;
}

/**
 * @return Magnetic field X reading [Gauss]
 * 
 * Call after successful call to update().
 */
float HMC5883L::get_mag_x()
{
	return mag_x;
}

/**
 * @return Magnetic field Y reading [Gauss]
 * 
 * Call after successful call to update().
 */
float HMC5883L::get_mag_y()
{
	return mag_y;
}

/**
 * @return Magnetic field Z reading [Gauss]
 * 
 * Call after successful call to update().
 */
float HMC5883L::get_mag_z()
{
	return mag_z;
}
