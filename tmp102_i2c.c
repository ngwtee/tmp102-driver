/**
  ******************************************************************************
  * @file    tmp102_i2c.h
  * @author  Ngonidzashe Gwata
  * @version V1.1.1
  * @date    15-January-2021
  * @brief   This file provides a set of functions needed to manage the I2C TMP102
  *          temperature sensor mounted on stm8l uc.
  *          It implements a high level communication layer for read and write
  *          from/to this sensor. The needed STM8 hardware resources (I2C and
  *          GPIO) are defined in config.h file, and the initialization is
  *          performed in i2c config() function declared in config.c
  *          file.

  ******************************************************************************
 */ 
 
 /**
  * @brief  Set the TMP102 pointer register to a specific register address.
  * @param  pointerReg: specifies the TMP102 register to be pointed to.
  *              This member can be one of the following values:
  *                  - TEMPERATURE_REGISTER: temperature register  0x00
  *					 - CONFIG_REGISTER: configuration register
  *                  - T_LOW_REGISTER: Over-limit temperature register
  *                  - T_HIGH_REGISTER: Hysteresis temperature register
  * @retval None
  */
	
#include "tmp102_i2c.h"

/**
  * @brief  Checks the TMP102 status.
  * @param  None
  * @retval ErrorStatus: TMP102 Status (ERROR or SUCCESS).
  */
ErrorStatus TMP102_GetStatus(void)
{
  uint32_t I2C_TimeOut = I2C_TIMEOUT;

  /* Clear the TMP102_I2C AF flag */
  I2C_ClearFlag(TMP102_I2C, I2C_FLAG_AF);

  /* Enable TMP102_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(TMP102_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /* Send TMP102_I2C START condition */
  I2C_GenerateSTART(TMP102_I2C, ENABLE);

  /* Test on TMP102_I2C EV5 and clear it */
  while ((!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_TimeOut)  /* EV5 */
  {
    I2C_TimeOut--;
  }
  if (I2C_TimeOut == 0)
  {
    return ERROR;
  }
  I2C_TimeOut = I2C_TIMEOUT;

  /*!< Send STTMP102 slave address for write */
  I2C_Send7bitAddress(TMP102_I2C, TMP102_ADDR, I2C_Direction_Transmitter);

  while ((!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_TimeOut)/* EV6 */
  {
    I2C_TimeOut--;
  }

  if ((I2C_GetFlagStatus(TMP102_I2C, I2C_FLAG_AF) != 0x00) || (I2C_TimeOut == 0))
  {
    return ERROR;
  }
  else
  {
		/* Send TMP102_I2C STOP Condition */
		I2C_GenerateSTOP(TMP102_I2C, ENABLE); // Close communication with TMP102
    return SUCCESS;
  }
}

/**
  * @brief  reset registers to power up values. by sendi 0000000, general call address, followed by 0000 0110
  * @param  None
  * @retval none.
  */
void TMP102_reset(void)
{

  /* Clear the TMP102_I2C AF flag */
  I2C_ClearFlag(TMP102_I2C, I2C_FLAG_AF);

  /* Enable TMP102_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(TMP102_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /* Send TMP102_I2C START condition */
  I2C_GenerateSTART(TMP102_I2C, ENABLE);

  /* Test on TMP102_I2C EV5 and clear it */
  while ((!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_MODE_SELECT)) )  /* EV5 */
  {
  }

  /*!< Send STTMP102 slave address for write */
  I2C_Send7bitAddress(TMP102_I2C, 0x00, I2C_Direction_Transmitter);

  while ((!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)))/* EV6 */
  {
  }
	 /* Send the reset cmd */
  I2C_SendData(TMP102_I2C, 6);
	/* Test on TMP102_I2C EV8 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }
  /* Send TMP102_I2C STOP Condition */
  I2C_GenerateSTOP(TMP102_I2C, ENABLE); // Close communication with TMP102
}


void openPointerRegister(uint8_t RegName)
{ 
	/* Enable TMP102_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(TMP102_I2C, ENABLE);

  /*--------------------------- Transmission Phase ----------------------------*/
  /* Send TMP102_I2C START condition */
  I2C_GenerateSTART(TMP102_I2C, ENABLE);

  /* Test on TMP102_I2C EV5 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STTMP102 slave address for write */
  I2C_Send7bitAddress(TMP102_I2C, TMP102_ADDR, I2C_Direction_Transmitter);

  /* Test on TMP102_I2C EV6 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(TMP102_I2C, RegName);

  /* Test on TMP102_I2C EV8 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }
  /* Send TMP102_I2C STOP Condition */
  I2C_GenerateSTOP(TMP102_I2C, ENABLE); // Close communication with TMP102
}

/**
  * @brief  Write to the specified register of the TMP102.
  * @param  RegName: specifies the TMP102 register to be written.
  *              This member can be one of the following values:
  *                  - CONFIG_REGISTER: configuration register
  *                  - T_LOW_REGISTER: Over-limit temperature register
  *                  - T_HIGH_REGISTER: Hysteresis temperature register
  * @param  RegValue: value to be written to TMP102 register.
  * @retval None
  * @Note 	after the write operation, the pointer register is set to temperature register
  */
void TMP102_WriteReg(uint8_t RegName, uint16_t RegValue)
{
	/* Enable TMP102_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(TMP102_I2C, ENABLE);

  /*-------------------------------- Transmission Phase -----------------------*/
  //#if 0
  /* Send TMP102_I2C START condition */
  I2C_GenerateSTART(TMP102_I2C, ENABLE);

  /* Test on TMP102_I2C EV5 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STTMP102 slave address for write */
  I2C_Send7bitAddress(TMP102_I2C, TMP102_ADDR, I2C_Direction_Transmitter);

  /* Test on TMP102_I2C EV6 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(TMP102_I2C, RegName);

  /* Test on TMP102_I2C EV8 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send TMP102_I2C data */
  I2C_SendData(TMP102_I2C, (uint8_t)(RegValue >> 8));

  /* Test on TMP102_I2C EV8 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send TMP102_I2C data */
  I2C_SendData(TMP102_I2C, (uint8_t)RegValue);

  /* Test on TMP102_I2C EV8 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send TMP102_I2C STOP Condition */
  I2C_GenerateSTOP(TMP102_I2C, ENABLE);
  
  //point to temperature register
  openPointerRegister(TEMPERATURE_REGISTER);
} 


  /**
  * @brief  Read the register from the TMP102, specified in point register .
  * @param  None
  * @retval TMP102 register value.
  */

uint16_t TMP102_ReadReg(void)
{
  
  __IO uint16_t RegValue = 0;
  
#if 0
  /* Enable TMP102_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(TMP102_I2C, ENABLE);

  /*--------------------------- Transmission Phase ----------------------------*/
  /* Send TMP102_I2C START condition */
  I2C_GenerateSTART(TMP102_I2C, ENABLE);

  /* Test on TMP102_I2C EV5 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STTMP102 slave address for write */
  I2C_Send7bitAddress(TMP102_I2C, TMP102_ADDR, I2C_Direction_Transmitter);

  /* Test on TMP102_I2C EV6 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(TMP102_I2C, RegName);

  /* Test on TMP102_I2C EV8 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }
#endif
  /*------------------------------ Reception Phase ----------------------------*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(TMP102_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STTMP102 slave address for read */
  I2C_Send7bitAddress(TMP102_I2C, TMP102_ADDR, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }

  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(TMP102_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }

  /* Store TMP102_I2C received data */
  RegValue = (uint16_t)(I2C_ReceiveData(TMP102_I2C) << 8);

  /* Disable TMP102_I2C acknowledgement */
  I2C_AcknowledgeConfig(TMP102_I2C, DISABLE);

  /* Send TMP102_I2C STOP Condition */
  I2C_GenerateSTOP(TMP102_I2C, ENABLE);

  /* Test on RXNE flag */
  while (I2C_GetFlagStatus(TMP102_I2C, I2C_FLAG_RXNE) == RESET)
  {}

  /* Store TMP102_I2C received data */
  RegValue |= I2C_ReceiveData(TMP102_I2C);

  /* Return register value */
  return (RegValue);
}

 /**
  * @brief  Read temperature in degrees celcius from the TMP102.
  * @param  None
  * @retval temperatue float value.
  * @Note 	When reading temperature register, there is no need to call openPointerRegister(TEMPERATURE_REGISTER). 
  * 		The power-up reset value of pointer register points to the temperature register. After reading/writing to any 
  * 		other register, control register must be pointed back to temperature register. This speeds up the temp read and overal 
  * 		performance(since there are more reading to the temp register than there are to any other registers)
  */
int16_t readTempC(void)
{
  uint8_t registerByte[2];	// Store the data from the register here
  uint16_t digitalTempRaw, digitalTemp;  // Temperature stored in TMP102 register
  int32_t temperature;  // Temperature stored in TMP102 register
  
  // Read Temperature
  // ASSUMPTION here is that the pointer address is already set to temperature register (0)
  //openPointerRegister(TEMPERATURE_REGISTER) is not called;
  // Read from temperature register
  digitalTempRaw = TMP102_ReadReg();
  //registerByte[0] = readRegister(0);
  //registerByte[1] = readRegister(1);
  //if(digitalTempRaw == 0xFFFF)
  //{	//there is some form of error, the return value should not be this value
	//return NAN;
  //}
  // Bit 0  will always be 0 in 12-bit readings and 1 in 13-bit
  if(digitalTempRaw&0x01)	// 13 bit mode
  {
	// Combine bytes to create a signed int
    digitalTemp = digitalTempRaw >> 3;
	// Temperature data can be + or -, if it should be negative,
	// convert 13 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0xFFF)
	{
      digitalTemp |= 0xE000;
      digitalTemp *= -1;//get the absolute value
			temperature = (((uint32_t)digitalTemp*625)+ 500)/1000;
			temperature *= (-1);//convert it back to a nagative
    }	
		else
		{//positive temp
			temperature = (((uint32_t)digitalTemp*625)+ 500)/1000; //
		}
  }
  else	// 12 bit mode
  {
	// Combine bytes to create a signed int 
    digitalTemp = digitalTempRaw >> 4;
    //digitalTemp = 0xe70;
	// Temperature data can be + or -, if it should be negative,
	// convert 12 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0x7FF)
		{
      digitalTemp |= 0xF000;
      digitalTemp *= -1;//get the absolute value
			temperature = (((uint32_t)digitalTemp*625)+ 500)/1000;
			temperature *= (-1);//convert it back to a nagative
    }	
		else
		{//positive temp
			temperature = (((uint32_t)digitalTemp*625)+ 500)/1000; //
		}
  }
	return temperature;
	//return 0xffbc;
  //Convert digital reading to analog temperature (1-bit is equal to 0.0625 C)
  //return digitalTemp*0.0625; //float value
}


float readTempF(void)
{
	return readTempC()*9.0/5.0 + 32.0;
}

uint8_t readRegister(bool registerNumber){  
  uint8_t registerByte[2];	// We'll store the data from the registers here
  uint16_t registerByte_16; // Store the data from the register here
  
  // Read current configuration register value
 registerByte_16 = TMP102_ReadReg(); 	// Read two bytes from TMP102
  registerByte[0] = (uint8_t)registerByte_16;	// Read first byte
  registerByte_16 = registerByte_16 >> 8;
  registerByte[1] = (uint8_t)registerByte_16;	// Read second byte
  
  return registerByte[registerNumber];
}
 
void setConversionRate(uint8_t rate)
{
  uint8_t registerByte[2]; 
  uint16_t registerByte_16; // Store the data from the register here
  
  rate = rate&0x03; // Make sure rate is not set higher than 3.
  
  // Change pointer address to configuration register (0x01)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  registerByte_16 = TMP102_ReadReg();

  // Load new conversion rate
  //registerByte[1] &= 0x3F;  // Clear CR0/1 (bit 6 and 7 of second byte)
  //registerByte[1] |= rate<<6;	// Shift in new conversion rate
  registerByte_16 &= 0xFF3F;  // Clear CR0/1 (bit 6 and 7 of second byte)
  registerByte_16 |= rate<<6;	// Shift in new conversion rate

  // Set configuration registers
  TMP102_WriteReg(CONFIG_REGISTER, registerByte_16);
}


void setExtendedMode(bool mode) 
{
  uint8_t registerByte[2]; // Store the data from the register here
  uint16_t registerByte_16; // Store the data from the register here

  // Change pointer address to configuration register (0x01)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
   registerByte_16 = TMP102_ReadReg();

  // Load new value for extention mode
  registerByte_16 &= 0xFFEF;		// Clear EM (bit 4 of second byte)
  registerByte_16 |= mode<<4;	// Shift in new exentended mode bit

  // Set configuration registers
  TMP102_WriteReg(CONFIG_REGISTER, registerByte_16);
}


void tmp102_sleep(void)
{
  uint8_t registerByte; // Store the data from the register here
   uint16_t registerByte_16, ; // Store the data from the register here

  // Change pointer address to configuration register (0x01)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
	registerByte_16 = TMP102_ReadReg();
  registerByte_16 |= 0x0100;	// Set SD (bit 0 of first byte)

 // Set configuration registers
  TMP102_WriteReg(CONFIG_REGISTER, registerByte_16);

}


void tmp102_wakeup(void)
{
  uint8_t registerByte; // Store the data from the register here
  uint16_t registerByte_16; // Store the data from the register here

  // Change pointer address to configuration register (1)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
   registerByte_16 = TMP102_ReadReg();

  registerByte_16 &= 0xFEFF;	// Clear SD (bit 0 of first byte)

  // Set configuration registers
  TMP102_WriteReg(CONFIG_REGISTER, registerByte_16);
}


void setAlertPolarity(bool polarity)
{
  uint8_t registerByte; // Store the data from the register here
  uint16_t registerByte_16, temp; // Store the data from the register here

  // Change pointer address to configuration register (1)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  registerByte_16 = TMP102_ReadReg();

  // Load new value for polarity
  registerByte_16 &= 0xFBFF; // Clear POL (bit 2 of registerByte)
  registerByte = (uint8_t)(registerByte_16 >> 8);
  registerByte |= polarity<<2;  // Shift in new POL bit
  
  temp =  (uint16_t)registerByte << 8;  // Shift in new POL bit
  registerByte_16 |=   temp;  // 

  // Set configuration registers
  TMP102_WriteReg(CONFIG_REGISTER, registerByte_16);
}


bool alert(void)
{
  uint8_t registerByte; // Store the data from the register here

  // Change pointer address to configuration register (1)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  registerByte = readRegister(1);
  
  registerByte &= 0x20;	// Clear everything but the alert bit (bit 5)
  return registerByte>>5;
}


void setLowTempC(float temperature)
{
  uint8_t registerByte[2];	// Store the data from the register here
  uint16_t registerByte_16, temp;
  bool extendedMode;	// Store extended mode bit here 0:-55C to +128C, 1:-55C to +150C
  
  // Prevent temperature from exceeding 150C or -55C
  if(temperature > 150.0f)
  {
    temperature = 150.0f;
  }
  if(temperature < -55.0)
  {
    temperature = -55.0f;
  }
  
  //Check if temperature should be 12 or 13 bits
  openPointerRegister(CONFIG_REGISTER);	// Read configuration register settings
  
  // Read current configuration register value
  registerByte[0] = readRegister(0);
  registerByte[1] = readRegister(1);
  extendedMode = (registerByte[1]&0x10)>>4;	// 0 - temp data will be 12 bits
                                            // 1 - temp data will be 13 bits

  // Convert analog temperature to digital value
  temperature = temperature/0.0625;
  
  // Split temperature into separate bytes
  if(extendedMode)	// 13-bit mode
  {
	registerByte[0] = ((int)temperature)>>5;
    registerByte[1] = (((int)temperature)<<3);
  }
  else	// 12-bit mode
  {
	registerByte[0] = ((int)temperature)>>4;
    registerByte[1] = ((int)temperature)<<4;
  }
  
  // Write to T_LOW Register
  registerByte_16 = registerByte[0];
  registerByte_16 = registerByte_16 << 8;
  registerByte_16 |= registerByte[1];
  
  TMP102_WriteReg(T_LOW_REGISTER, registerByte_16);
}


void setHighTempC(float temperature)
{
  uint8_t registerByte[2];	// Store the data from the register here
  uint16_t registerByte_16, temp;
  bool extendedMode;	// Store extended mode bit here 0:-55C to +128C, 1:-55C to +150C
  
  // Prevent temperature from exceeding 150C
  if(temperature > 150.0f)
  {
    temperature = 150.0f;
  }
  if(temperature < -55.0)
  {
    temperature = -55.0f;
  }
  
  // Check if temperature should be 12 or 13 bits
  openPointerRegister(CONFIG_REGISTER);	// Read configuration register settings
  
  // Read current configuration register value
  registerByte[0] = readRegister(0);
  registerByte[1] = readRegister(1);
  extendedMode = (registerByte[1]&0x10)>>4;	// 0 - temp data will be 12 bits
                                            // 1 - temp data will be 13 bits

  // Convert analog temperature to digital value
  temperature = temperature/0.0625;
  
  // Split temperature into separate bytes
  if(extendedMode)	// 13-bit mode
  {
	registerByte[0] = ((int)temperature)>>5;
    registerByte[1] = (((int)temperature)<<3);
  }
  else	// 12-bit mode
  {
	registerByte[0] = ((int)temperature)>>4;
    registerByte[1] =((int)temperature)<<4;
  }
  
  // Write to T_HIGH Register
   registerByte_16 = registerByte[0];
  registerByte_16 = registerByte_16 << 8;
  registerByte_16 |= registerByte[1];
  
  TMP102_WriteReg(T_LOW_REGISTER, registerByte_16);
}


void setLowTempF(float temperature)
{
  temperature = (temperature - 32)*5/9; // Convert temperature to C
  setLowTempC(temperature); // Set T_LOW
}


void setHighTempF(float temperature)
{
  temperature = (temperature - 32)*5/9; // Convert temperature to C
  setHighTempC(temperature); // Set T_HIGH
}


float readLowTempC(void)
{
  uint8_t registerByte[2];	// Store the data from the register here
  bool extendedMode;	// Store extended mode bit here 0:-55C to +128C, 1:-55C to +150C
  int16_t digitalTemp;		// Store the digital temperature value here
  float temperature;	// Store the analog temperature value here
  
  // Check if temperature should be 12 or 13 bits
  openPointerRegister(CONFIG_REGISTER);	// Read configuration register settings
  // Read current configuration register value
  registerByte[0] = readRegister(0);
  registerByte[1] = readRegister(1);
  extendedMode = (registerByte[1]&0x10)>>4;	// 0 - temp data will be 12 bits
                                            // 1 - temp data will be 13 bits
  openPointerRegister(T_LOW_REGISTER);
  registerByte[0] = readRegister(0);
  registerByte[1] = readRegister(1);
  
  if(extendedMode)	// 13 bit mode
  {
	// Combine bytes to create a signed int
    digitalTemp = ((registerByte[0]) << 5) | (registerByte[1] >> 3);
	// Temperature data can be + or -, if it should be negative,
	// convert 13 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0xFFF)
	{
      digitalTemp |= 0xE000;
    }
  }
  else	// 12 bit mode
  {
	// Combine bytes to create a signed int 
    digitalTemp = ((registerByte[0]) << 4) | (registerByte[1] >> 4);
	// Temperature data can be + or -, if it should be negative,
	// convert 12 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0x7FF)
	{
      digitalTemp |= 0xF000;
    }
  }
  // Convert digital reading to analog temperature (1-bit is equal to 0.0625 C)
  return digitalTemp*0.0625;
}


float readHighTempC(void)
{
  uint8_t registerByte[2];	// Store the data from the register here
  bool extendedMode;	// Store extended mode bit here 0:-55C to +128C, 1:-55C to +150C
  int16_t digitalTemp;		// Store the digital temperature value here
  float temperature;	// Store the analog temperature value here
  
  // Check if temperature should be 12 or 13 bits
  openPointerRegister(CONFIG_REGISTER);	// read configuration register settings
  // Read current configuration register value
  registerByte[0] = readRegister(0);
  registerByte[1] = readRegister(1);
  extendedMode = (registerByte[1]&0x10)>>4;	// 0 - temp data will be 12 bits
											// 1 - temp data will be 13 bits
  openPointerRegister(T_HIGH_REGISTER);
  registerByte[0] = readRegister(0);
  registerByte[1] = readRegister(1);
  
  if(extendedMode)	// 13 bit mode
  {
	// Combine bytes to create a signed int
    digitalTemp = ((registerByte[0]) << 5) | (registerByte[1] >> 3);
	// Temperature data can be + or -, if it should be negative,
	// convert 13 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0xFFF)
	{
      digitalTemp |= 0xE000;
    }
  }
  else	// 12 bit mode
  {
	// Combine bytes to create a signed int 
    digitalTemp = ((registerByte[0]) << 4) | (registerByte[1] >> 4);
	// Temperature data can be + or -, if it should be negative,
	// convert 12 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0x7FF)
	{
      digitalTemp |= 0xF000;
    }
  }
  // Convert digital reading to analog temperature (1-bit is equal to 0.0625 C)
  return digitalTemp*0.0625;
}


float readLowTempF(void)
{
  return readLowTempC()*9.0/5.0 + 32.0;
}


float readHighTempF(void)
{
  return readHighTempC()*9.0/5.0 + 32.0;
}


void setFault(uint8_t faultSetting)
{
  uint8_t registerByte; // Store the data from the register here
  uint16_t registerByte_16, temp;

  faultSetting = faultSetting&3; // Make sure rate is not set higher than 3.
  
  // Change pointer address to configuration register (0x01)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  registerByte_16 = TMP102_ReadReg();
  registerByte_16 &= 0xE7FF;  // Clear F0/1 (bit 3 and 4 of first byte)
  temp = registerByte_16;
  temp = temp >> 8;
  registerByte = temp;

  // Load new conversion rate
  //registerByte &= 0xE7;  // Clear F0/1 (bit 3 and 4 of first byte)
  registerByte |= faultSetting<<3;	// Shift new fault setting

  // Set configuration registers
  temp = registerByte;
  temp = temp << 8;
  registerByte_16 |= temp;
  
  TMP102_WriteReg(T_LOW_REGISTER, registerByte_16);
}


void setAlertMode(bool mode)
{
  uint8_t registerByte; // Store the data from the register here
  int16_t registerByte_16, temp;
  
  // Change pointer address to configuration register (1)
  openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  //registerByte = readRegister(0);
   // Read current configuration register value
  registerByte_16 = TMP102_ReadReg();
  registerByte_16 &= 0xFDFF;	// Clear old TM bit (bit 1 of first byte)
  temp = registerByte_16;
  temp = temp >> 8;
  registerByte = temp;

  // Load new conversion rate
  //registerByte &= 0xFD;	// Clear old TM bit (bit 1 of first byte)
  registerByte |= mode<<1;	// Shift in new TM bit

  // Set configuration registers
  temp = registerByte;
  temp = temp << 8;
  registerByte_16 |= temp;
  
  TMP102_WriteReg(T_LOW_REGISTER, registerByte_16);
}

uint8_t oneShot(bool setOneShot)
{
  uint8_t registerByte; //Store the data from the register here
  uint16_t registerByte_16, temp;

  //Read the first byte of the configuration register
  openPointerRegister(CONFIG_REGISTER);
	registerByte_16 = TMP102_ReadReg();
  
  if(setOneShot)	//Enable one-shot by writing a 1 to the OS bit of the configuration register
  {
    registerByte_16 |= (1<<15);
	
	// Set configuration register
  TMP102_WriteReg(CONFIG_REGISTER, registerByte_16);
	return 0;
  }
  else	//Return OS bit of configuration register (0-not ready, 1-conversion complete)
  {
		
		openPointerRegister(TEMPERATURE_REGISTER);//point to temperature reg
		registerByte_16 &= (1<<15);
    registerByte = (registerByte_16>>15);
    return registerByte;
  }
}