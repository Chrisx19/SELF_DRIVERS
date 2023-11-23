/***
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/ina219.pdf
*/

#ifndef  _CURR_SENSOR_INA219_H_
#define _CURR_SENSOR_INA219_H_

#pragma pack(1)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"

#define INA219_ADDRESS  (0x40 << 1)         //0th Bit is for R/W(Read or Write)

#define INA219_REG_CONFIG (0x00)
/************************************************************************/
#define INA219_CONFIG_MODE_MASK (0x007)    //Operating Mode (Pg 27)
//MODE Bits 0-2
typedef enum{
    POWER_DOWN              = (0x000U),
    SHUNT_VOLTAGE_TRIGG     = (0x001U),
    BUS_VOLTAGE_TRIGG       = (0x002U),
    SHUNT_BUS_VOLTAGE_TRIGG = (0x003U),
    ADC_OFF                 = (0x004U),
    SHUNT_VOLTAGE_CONT      = (0x005U),
    BUS_VOLTAGE_CONT        = (0x006U),
    SHUNT_BUS_CONT          = (0x007U),     //Default
}MODE_E;

/************************************************************************/
#define INA219_CONFIG_SADC_MASK (0x0078U)    //Shunt ADC Resolution/Averaging (Pg 27)
//SADC Bits 3-6
typedef enum{
    SADC_09BIT_84US     = (0x0000U),
    SADC_10BIT_148US    = (0x0008U),
    SADC_11BIT_276US    = (0x0010U),
    SADC_12BIT_532US    = (0x0018U),         //Default
    SADC_12BIT_532US_2  = (0x0040U),
    SADC_02BIT_1_06MS   = (0x0048U),
    SADC_04BIT_2_13MS   = (0x0050U),
    SADC_08BIT_4_26MS   = (0x0058U),
    SADC_16BIT_8_51MS   = (0x0060U),
    SADC_32BIT_17_02MS  = (0x0068U),
    SADC_64BIT_34_05MS  = (0x0070U),
    SADC_128BIT_68_10MS = (0x0078U),
}SADC_E;

/************************************************************************/
#define INA219_CONFIG_BADC_MASK (0x078U)    //Bus ADC Resolution/Averaging (Pg 28)
//BADC Bits 7-10
typedef enum{
    BADC_09BIT_84US     = (0x0000U),
    BADC_10BIT_148US    = (0x0080U),
    BADC_11BIT_276US    = (0x0100U),
    BADC_12BIT_532US    = (0x0180U),         //Default
    BADC_12BIT_532US_2  = (0x0400U),
    BADC_02BIT_1_06MS   = (0x0480U),
    BADC_04BIT_2_13MS   = (0x0500U),
    BADC_08BIT_4_26MS   = (0x0580U),
    BADC_16BIT_8_51MS   = (0x0600U),
    BADC_32BIT_17_02MS  = (0x0680U),
    BADC_64BIT_34_05MS  = (0x0700U),
    BADC_128BIT_68_10MS = (0x0780U),
}BADC_E;

/************************************************************************/
#define INA219_CONFIG_PG_MASK (0x1800)    //Gain for PGA -- Voltage (Pg 28)
//PG Bits 11-12
typedef enum{
    PG_GAIN_1_40mV    = (0x0000U),
    PG_GAIN_2_40mV    = (0x0800U),
    PG_GAIN_4_40mV    = (0x1000U),
    PG_GAIN_8_40mV    = (0x1800U),         //Default
}PG_E;

/************************************************************************/
#define INA219_CONFIG_BRNG_MASK (0x2000)    //Bus Voltage Range (Pg 28)
//BRNG Bit 13
typedef enum{
    BRNG_FSR_16V    = (0x0000U),
    BRNG_FSR_32V    = (0x2000U),             //default
}BRNG_E;

/************************************************************************/
#define INA219_CONFIG_RESET_MASK (0x8000)    //Reset for the current sensor (Pg 28)
//RST Bit 15
typedef enum{
    RST_OFF   = (0x0000U),               //default
    RST_ON    = (0x8000U),
}RST_E;

/** shunt voltage register **/
#define INA219_REG_SHUNTVOLTAGE (0x01U)

/** bus voltage register **/
#define INA219_REG_BUSVOLTAGE   (0x02U)

/** power register **/
#define INA219_REG_POWER        (0x03U)

/** current register **/
#define INA219_REG_CURRENT      (0x04U)

/** calibration register **/
#define INA219_REG_CALIBRATION  (0x05U)

typedef struct {
    I2C_HandleTypeDef *I2C_handle;

    uint8_t			  send[2];

    uint8_t			  receive[2];

    uint16_t		  data_read_buff[3];

    uint16_t 		  calibrationValue;

    float 			  voltage_value;

    float			  voltage_shunt_value;

    float		      current_value;


}INA219_t;

/******************************************************* Functions for converting and initialize *******************************************************/
HAL_StatusTypeDef INA219_init(INA219_t *my_INA, I2C_HandleTypeDef *I2C_handle);

void INA219_setCalibration_32V_2A(INA219_t *my_INA);

int16_t INA219_GetVoltage_Raw(INA219_t *my_INA);

int16_t INA219_GetShuntVoltage_Raw(INA219_t *my_INA);

int16_t INA219_GetCurrent_Raw(INA219_t *my_INA);

float INA219_GetVoltage_V(INA219_t *my_INA);

float INA219_GetShuntVoltage_mV(INA219_t *my_INA);

float INA219_GetCurrent_mA(INA219_t *my_INA);
/***************************************************** LOW-LEVEL Functions *****************************************************/
HAL_StatusTypeDef INA219_ReadRegister(INA219_t *my_INA, uint8_t reg);

HAL_StatusTypeDef INA219_WriteRegister(INA219_t *my_INA, uint8_t reg, uint16_t data);


#endif
