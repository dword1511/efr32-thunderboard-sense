/***************************************************************************//**
 * @file util_supply.c
 * @brief Power Supply Related Utility Functions for the Thunderboard Sense
 * @version 5.5.0
 *******************************************************************************
 * # License
 * <b>Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stdio.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_adc.h"
#include "em_cmu.h"
#include "em_prs.h"

#include "thunderboard/board.h"
#include "thunderboard/util.h"
#include "thunderboard/si7021.h"

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/
/***************************************************************************//**
 * @addtogroup Util
 * @{
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

float UTIL_supplyVoltage;  /**< Loaded supply voltage                */
float UTIL_supplyIR;       /**< Internal resistance of the supply    */
uint8_t UTIL_supplyType;   /**< Type of the connected supply         */

/** @endcond */

/***************************************************************************//**
 * @brief
 *    Probes the connected supply and determines its type. The results are
 *    stored in global variables.
 *
 * @return
 *    None
 ******************************************************************************/
void UTIL_supplyProbe(void)
{
  UTIL_supplyVoltage = 2.5;
  UTIL_supplyIR = 10;
  UTIL_supplyType = UTIL_SUPPLY_TYPE_CR2032;
  return;
}

/***************************************************************************//**
 * @brief
 *    Retrieves the supply characteristic variables
 *
 * @param[in] type
 *    Supply type
 *
 * @param[in] voltage
 *    Loaded supply voltage
 *
 * @param[in] ir
 *    Internal resistance of the supply
 *
 * @return
 *    None
 ******************************************************************************/
void UTIL_supplyGetCharacteristics(uint8_t *type, float *voltage, float *ir)
{
  *type    = UTIL_supplyType;
  *voltage = UTIL_supplyVoltage;
  *ir      = UTIL_supplyIR;

  return;
}

/***************************************************************************//**
 * @brief
 *    Returns the type of the power supply
 *
 * @return
 *    The type of the supply
 ******************************************************************************/
uint8_t UTIL_supplyGetType(void)
{
  return UTIL_supplyType;
}

/***************************************************************************//**
 * @brief
 *    Checks if the current power supply has low power capability
 *
 * @return
 *    True if the supply is low power type, false otherwise
 ******************************************************************************/
bool UTIL_isLowPower(void)
{
  return true;
}

/** @} */
/** @} */
