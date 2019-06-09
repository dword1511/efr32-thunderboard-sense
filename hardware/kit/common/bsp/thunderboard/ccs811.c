/***************************************************************************//**
 * @file ccs811.c
 * @brief Driver for the Cambridge CMOS Sensors CCS811 gas and indoor air
 * quality sensor
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

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "i2cspm.h"

#include "thunderboard/board.h"
#include "thunderboard/ccs811.h"
#include "thunderboard/util.h"

/**************************************************************************//**
* @defgroup CCS811 CCS811 - Indoor Air Quality Sensor
* @{
* @brief Driver for the Cambridge CMOS Sensors CCS811 gas and indoor air
* quality sensor
******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if (CCS811_FIRMWARE_UPDATE > 0)
  #pragma message("CCS811 firmware update included")
  #include "ccs811_firmware.h"
static uint32_t CCS811_firmwareVerificationAndUpdate(void);

static uint32_t CCS811_eraseApplication   (void);
static uint32_t CCS811_verifyApplication  (bool *valid);

static void     debug_printf(const char *format, ...);
#endif

/** @endcond */

/***************************************************************************//**
 * @brief
 *    Initializes the chip and performs firmware upgrade if required
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_init(void)
{
	  /* Disable the sensor  */
	  BOARD_gasSensorWake(false);
	  BOARD_gasSensorEnable(false);

  return CCS811_OK;
}

/***************************************************************************//**
 * @brief
 *    De-initializes the chip
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_deInit(void)
{
  /* Disable the sensor  */
  BOARD_gasSensorWake(false);
  BOARD_gasSensorEnable(false);

  return CCS811_OK;
}

/***************************************************************************//**
 * @brief
 *    Reads Hardware ID from the CSS811 sensor
 *
 * @param[out] hardwareID
 *    The Hardware ID of the chip (should be 0x81)
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_getHardwareID(uint8_t *hardwareID)
{
  return 0x81;
}

/**************************************************************************//**
* @brief
*    Reads the status of the CSS811 sensor
*
* @param[out] status
*    The content of the CSS811 Status Register
*
* @return
*    Returns zero on OK, non-zero otherwise.
******************************************************************************/
uint32_t CCS811_getStatus(uint8_t *status)
{
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Checks if new measurement data available
 *
 * @return
 *    True if new data available, otherwise false
 ******************************************************************************/
bool CCS811_isDataAvailable(void)
{
  return true;
}

/***************************************************************************//**
 * @brief
 *    Dumps the registers of the CSS811
 *
 * @return
 *    None
 ******************************************************************************/
void CCS811_dumpRegisters(void)
{
  return;
}

/***************************************************************************//**
 * @brief
 *    Switches the CSS811 chip from boot to application mode
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_startApplication(void)
{
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Reads data from a specific Mailbox address
 *
 * @param[in] id
 *    The address of the Mailbox register
 *
 * @param[in] length
 *    The number of bytes to read
 *
 * @param[out] data
 *    The data read from the sensor
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_readMailbox(uint8_t id, uint8_t length, uint8_t *data)
{
  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;
  uint8_t i2c_write_data[1];
  uint32_t retval;

  retval = CCS811_OK;

  BOARD_gasSensorWake(true);

  /* Write data */
  i2c_write_data[0] = id;

  /* Configure I2C bus transaction */
  seq.addr        = CCS811_BUS_ADDRESS;
  seq.flags       = I2C_FLAG_WRITE_READ;
  seq.buf[0].data = i2c_write_data;
  seq.buf[0].len  = 1;

  /* Select length of data to be read */
  seq.buf[1].data = data;
  seq.buf[1].len = length;

  BOARD_i2cBusSelect(BOARD_I2C_BUS_SELECT_GAS);

  ret = I2CSPM_Transfer(CCS811_I2C_DEVICE, &seq);
  if ( ret != i2cTransferDone ) {
    retval = CCS811_ERROR_I2C_TRANSACTION_FAILED;
  }

  BOARD_gasSensorWake(false);

  return retval;
}

/***************************************************************************//**
 * @brief
 *    Reads measurement data (eCO2 and TVOC) from the CSS811 sensor
 *
 * @param[out] eco2
 *    The eCO2 level read from the sensor
 *
 * @param[out] tvoc
 *    The TVOC level read from the sensor
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_getMeasurement(uint16_t *eco2, uint16_t *tvoc)
{
  *eco2 = 0;
  *tvoc = 0;
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Gets the latest readings from the sense resistor of the CSS811 sensor
 *
 * @param[out] current
 *    The value of current through the sensor
 *
 * @param[out] rawData
 *    The raw ADC reading of the voltage across the sensor with the selected
 *    current
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 *****************************************************************************/
uint32_t CCS811_getRawData(uint16_t *current, uint16_t *rawData)
{
  *current = 0;
  *rawData = 0;

  return 0;
}

/***************************************************************************//**
 * @brief
 *    Performs software reset on the CCS811
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_softwareReset(void)
{
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Sets the measurement mode of the CSS811 sensor
 *
 * @param[in] measMode
 *    The desired measurement mode
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_setMeasureMode(uint8_t measMode)
{
  return 0;
}

#if (CCS811_FIRMWARE_UPDATE > 0)

/***************************************************************************//**
 * @brief
 *    Erases the application firmware on the CCS811
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
static uint32_t CCS811_eraseApplication(void)
{
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Writes 8 bytes of data to the firmware memory of the CCS811
 *
 * @param[in] buffer
 *    An 8-byte lenght buffer containig the data to write
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
static uint32_t CCS811_programFirmware(uint8_t buffer[])
{
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Verifies if the application firmware downloaded to the CCS811
 *    was received error free
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 *****************************************************************************/
static uint32_t CCS811_verifyApplication(bool *appValid)
{
  return 0;
}

/***************************************************************************//**
 * @brief
 *    Checks the current firmware version, compares to the available update
 *    file version and peforms the update and verification if the file is
 *    newer than the current one.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
static uint32_t CCS811_firmwareVerificationAndUpdate(void)
{
  return 0;
}

static void debug_printf(const char *format, ...)
{
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
#endif
}

#endif

/** @endcond */

/***************************************************************************//**
 * @brief
 *    Writes temperature and humidity values to the environmental data regs
 *
 * @param[in] tempData
 *    The environmental temperature in milli-Celsius
 *
 * @param[in] rhData
 *    The relative humidity in milli-percent
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t CCS811_setEnvData(int32_t tempData, uint32_t rhData)
{
  return 0;
}
/** @} (end defgroup CCS811) */
/** @} {end addtogroup TBSense_BSP} */
