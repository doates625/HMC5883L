/**
 * @file HMC5883L.cpp
 * @author Dan Oates (WPI Class of 2020)
 */
#include "HMC5883L.h"

/**
 * I2C Buffer Size Macro
 */
#if I2CDEVICE_BUFFER_SIZE < 6
	#error HMC5883L requires I2CDEVICE_BUFFER_SIZE >= 6
#endif

/**
 * @brief Constructs new magnetometer with given I2C interface
 */
HMC5883L::HMC5883L(I2CDevice::i2c_t* i2c) :
	i2c(i2c, i2c_addr, Struct::msb_first)
{
	this->mag_x = 0.0f; this->read_mag_x = false;
	this->mag_y = 0.0f; this->read_mag_y = false;
	this->mag_z = 0.0f; this->read_mag_z = false;
}

/**
 * @brief Initializes magnetometer in continuous mode
 * @return True if I2C communication was successful
 */
bool HMC5883L::init()
{
	// Attempt ID register read
	i2c.get_seq(reg_id_A_addr, 3);
	uint8_t reg_id_A_test = (uint8_t)i2c;
	uint8_t reg_id_B_test = (uint8_t)i2c;
	uint8_t reg_id_C_test = (uint8_t)i2c;

	// Compare reads to reference values
	if (reg_id_A_test != reg_id_A_ref) return false;
	if (reg_id_B_test != reg_id_B_ref) return false;
	if (reg_id_C_test != reg_id_C_ref) return false;

	// Set range to 1300mG (default)
	set_range(range_130uT);

	// Set device to continuous sampling
	i2c.set(reg_mode_addr, reg_mode_cont);
	
	// Everything succeeded
	return true;
}

/**
 * @brief Sets full-scale range of magnetometer
 * @param range Full-scale range [enum]
 * 
 * Range options:
 * - range_88uT = +/-88uT
 * - range_130uT = +/-130uT
 * - range_190uT = +/-190uT
 * - range_250uT = +/-250uT
 * - range_400uT = +/-400uT
 * - range_470uT = +/-470uT
 * - range_560uT = +/-560uT
 * - range_810uT = +/-810uT
 */
void HMC5883L::set_range(range_t range)
{
	uint8_t reg_val = 0x00;
	switch (range)
	{
		case range_88uT:
			reg_val = reg_config_B_88uT;
			uT_per_lsb = 0.073f;
			break;
		case range_130uT:
			reg_val = reg_config_B_130uT;
			uT_per_lsb = 0.092f;
			break;
		case range_190uT:
			reg_val = reg_config_B_190uT;
			uT_per_lsb = 0.122f;
			break;
		case range_250uT:
			reg_val = reg_config_B_250uT;
			uT_per_lsb = 0.152f;
			break;
		case range_400uT:
			reg_val = reg_config_B_400uT;
			uT_per_lsb = 0.227f;
			break;
		case range_470uT:
			reg_val = reg_config_B_470uT;
			uT_per_lsb = 0.256f;
			break;
		case range_560uT:
			reg_val = reg_config_B_560uT;
			uT_per_lsb = 0.303f;
			break;
		case range_810uT:
			reg_val = reg_config_B_810uT;
			uT_per_lsb = 0.435f;
			break;
	}
	i2c.set(reg_config_B_addr, reg_val);
}

/**
 * @brief Gets new readings from magnetometer
 */
void HMC5883L::update()
{
	i2c.get_seq(reg_mag_x_addr, 6);
	mag_x = (int16_t)i2c * uT_per_lsb; read_mag_x = true;
	mag_z = (int16_t)i2c * uT_per_lsb; read_mag_y = true;
	mag_y = (int16_t)i2c * uT_per_lsb; read_mag_z = true;
}

/**
 * @brief Returns magnetic field X [uT]
 */
float HMC5883L::get_mag_x()
{
	if (read_mag_x)
	{
		read_mag_x = false;
		return mag_x;
	}
	return (int16_t)i2c.get_seq(reg_mag_x_addr, 2) * uT_per_lsb;
}

/**
 * @brief Returns magnetic field Y [uT]
 */
float HMC5883L::get_mag_y()
{
	if (read_mag_y)
	{
		read_mag_y = false;
		return mag_y;
	}
	return (int16_t)i2c.get_seq(reg_mag_y_addr, 2) * uT_per_lsb;
}

/**
 * @brief Returns magnetic field Z [uT]
 */
float HMC5883L::get_mag_z()
{
	if (read_mag_z)
	{
		read_mag_z = false;
		return mag_z;
	}
	return (int16_t)i2c.get_seq(reg_mag_z_addr, 2) * uT_per_lsb;
}