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
	i2c(i2c, i2c_addr, Struct::msb_first),
	mag_x(&(this->i2c), reg_mag_x_addr),
	mag_y(&(this->i2c), reg_mag_y_addr),
	mag_z(&(this->i2c), reg_mag_z_addr)
{
	this->x_cal = 0.0f;
	this->y_cal = 0.0f;
	this->z_cal = 0.0f;
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

	// Set range to 130uT (default)
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
 * - range_088uT = +/-088uT
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
		case range_088uT: reg_val = reg_config_B_088uT; uT_per_lsb = 0.073f; break;
		case range_130uT: reg_val = reg_config_B_130uT; uT_per_lsb = 0.092f; break;
		case range_190uT: reg_val = reg_config_B_190uT; uT_per_lsb = 0.122f; break;
		case range_250uT: reg_val = reg_config_B_250uT; uT_per_lsb = 0.152f; break;
		case range_400uT: reg_val = reg_config_B_400uT; uT_per_lsb = 0.227f; break;
		case range_470uT: reg_val = reg_config_B_470uT; uT_per_lsb = 0.256f; break;
		case range_560uT: reg_val = reg_config_B_560uT; uT_per_lsb = 0.303f; break;
		case range_810uT: reg_val = reg_config_B_810uT; uT_per_lsb = 0.435f; break;
	}
	i2c.set(reg_config_B_addr, reg_val);
}

/**
 * @brief Gets new readings from magnetometer
 */
void HMC5883L::update()
{
	i2c.get_seq(reg_mag_x_addr, 6);
	mag_x.update();
	mag_z.update();
	mag_y.update();
}

/**
 * @brief Returns magnetic field X [uT]
 */
float HMC5883L::get_x()
{
	return mag_x * uT_per_lsb - x_cal;
}

/**
 * @brief Returns magnetic field Y [uT]
 */
float HMC5883L::get_y()
{
	return mag_y * uT_per_lsb - y_cal;
}

/**
 * @brief Returns magnetic field Z [uT]
 */
float HMC5883L::get_z()
{
	return mag_z * uT_per_lsb - z_cal;
}
