/**
  ******************************************************************************
  * @file    tmp102_i2c.h
  * @author  Ngonidzashe Gwata
  * @version V1.1.1
  * @date    15-January-2021
  * @brief   This file contains all the functions prototypes for the 
  *          tmp102_i2c firmware driver.
  ******************************************************************************
  *
  *
  ******************************************************************************
  */  
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMP102_H
#define __TMP102_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h" 
#include "config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/**
  * @brief  Block Size
  */
#define NAN 0xffff

#define TEMPERATURE_REGISTER 0x00
#define CONFIG_REGISTER 0x01
#define T_LOW_REGISTER 0x02
#define T_HIGH_REGISTER 0x03
#define I2C_TIMEOUT         (uint32_t)0x3FFFF /*!< I2C Time out */
#define TMP102_ADDR           0x90 /*!< Address of Temperature sensor (0x48,0x49,0x4A,0x4B) << 1*/
#define TMP102_I2C_SPEED      100000 /*!< I2C Speed */

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
	ErrorStatus TMP102_GetStatus(void); // Checks the TMP102 status
	void TMP102_reset(void);	//reset registers
	void TMP102_WriteReg(uint8_t RegName, uint16_t RegValue);
	int16_t readTempC(void);	// Returns the temperature in degrees C
	float readTempF(void);	// Converts readTempC result to degrees F
	void tmp102_sleep(void);	// Switch sensor to low power mode
	void tmp102_wakeup(void);	// Wakeup and start running in normal power mode
	bool alert(void);	// Returns state of Alert register
	void setLowTempC(float temperature);  // Sets T_LOW (degrees C) alert threshold
	void setHighTempC(float temperature); // Sets T_HIGH (degrees C) alert threshold
	void setLowTempF(float temperature);  // Sets T_LOW (degrees F) alert threshold
	void setHighTempF(float temperature); // Sets T_HIGH (degrees F) alert threshold
	float readLowTempC(void);	// Reads T_LOW register in C
	float readHighTempC(void);	// Reads T_HIGH register in C
	float readLowTempF(void);	// Reads T_LOW register in F
	float readHighTempF(void);	// Reads T_HIGH register in F		
	
	// Set the conversion rate (0-3)
	// 0 - 0.25 Hz
	// 1 - 1 Hz
	// 2 - 4 Hz (default)
	// 3 - 8 Hz
	void setConversionRate(uint8_t rate);
	
	// Enable or disable extended mode
	// 0 - disabled (-55C to +128C)
	// 1 - enabled  (-55C to +150C)
	void setExtendedMode(bool mode);
	
	// Set the polarity of Alert
	// 0 - Active LOW
	// 1 - Active HIGH
	void setAlertPolarity(bool polarity); 
	
	// Set the number of consecutive faults
	// 0 - 1 fault
	// 1 - 2 faults
	// 2 - 4 faults
	// 3 - 6 faults
	void setFault(uint8_t faultSetting);
	
	// Set Alert type
	// 0 - Comparator Mode: Active from temp > T_HIGH until temp < T_LOW
	// 1 - Thermostat Mode: Active when temp > T_HIGH until any read operation occurs
	void setAlertMode(bool mode);

	void openPointerRegister(uint8_t RegName); // Changes the pointer register
	uint8_t readRegister(bool registerNumber);	// reads 1 byte of from register
	uint8_t oneShot(bool setOneShot);	//
 

#endif /* __STM8_EVAL_I2C_TSENSOR_H */
