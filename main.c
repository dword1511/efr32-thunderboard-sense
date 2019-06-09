#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

#include "radio.h"
#include "token.h"

#include "retargetserial.h"

#include "main.h"
#include "appl_ver.h"

#include "native_gecko.h"

#include "thunderboard/util.h"
#include "thunderboard/board.h"
#include "thunderboard/bap.h"
#include "thunderboard/si7021.h"
#include "thunderboard/si1133.h"
#include "thunderboard/ccs811.h"
#include "thunderboard/mic.h"

#include "app.h"
#include "radio.h"
#include "radio_ble.h"

#define MIC_SAMPLE_RATE            1000
#define MIC_SAMPLE_BUFFER_SIZE     512

static uint16_t micSampleBuffer[MIC_SAMPLE_BUFFER_SIZE];

static void     init                (bool radio);

uint16_t RADIO_xoTune = 344;

int main(void)
{
  /**************************************************************************/
  /* Device errata init                                                     */
  /**************************************************************************/
  CHIP_Init();

  /**************************************************************************/
  /* Board init                                                             */
  /**************************************************************************/
  init(true);

  /**************************************************************************/
  /* Application loop                                                       */
  /**************************************************************************/
  while (1) {
    struct gecko_cmd_packet* evt;
    /* Check for stack event. */
    evt = gecko_wait_event();
    /* Run application and event handler. */
    appHandleEvents(evt);
  }
}

void MAIN_initSensors()
{
  uint8_t bapDeviceId;
  uint32_t status;

  SI7021_init();
  SI1133_init();
  BAP_init(&bapDeviceId);

  if ( UTIL_isLowPower() == false ) {
    CCS811_init();
    status = CCS811_startApplication();
    if ( status == CCS811_OK ) {
      status = CCS811_setMeasureMode(CCS811_MEASURE_MODE_DRIVE_MODE_10SEC);
    }
  }

  MIC_init(MIC_SAMPLE_RATE, micSampleBuffer, MIC_SAMPLE_BUFFER_SIZE);

  BOARD_rgbledSetRawColor(0, 0, 0);

  return;
}

void MAIN_deInitSensors()
{
  SI7021_deInit();
  SI1133_deInit();
  BAP_deInit();
  BOARD_envSensEnable(false);

  if ( UTIL_isLowPower() == false ) {
    CCS811_deInit();
  }

  MIC_deInit();

  BOARD_ledSet(0);
  BOARD_rgbledSetRawColor(0, 0, 0);
  BOARD_rgbledEnable(false, 0xFF);

  return;
}

#define RADIO_XO_TUNE_VALUE 344
void init(bool radio)
{
  uint8_t  supplyType;
  float    supplyVoltage;
  float    supplyIR;

  /**************************************************************************/
  /* Module init                                                            */
  /**************************************************************************/
  UTIL_init();
  BOARD_init();

  BOARD_picGetDeviceId();
  BOARD_picGetHwRevision();

  UTIL_supplyProbe();
  UTIL_supplyGetCharacteristics(&supplyType, &supplyVoltage, &supplyIR);

  /**************************************************************************/
  /* System clock and timer init                                            */
  /**************************************************************************/
  if ( radio ) {
    RADIO_bleStackInit();
  } else {
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  }

  /* Re-initialize serial port and UTIL which depend on the HF clock frequency */
  RETARGET_SerialInit();
  UTIL_init();
  BOARD_init();

  /* In low power mode, sensors are enabled and disabled when entering/leaving connected mode */
  if ( !UTIL_isLowPower() ) {
    MAIN_initSensors();
  }

  GPIO_PinModeSet(gpioPortD, 14, gpioModeInput, 0);
  GPIO_PinModeSet(gpioPortD, 15, gpioModeInput, 0);

  return;
}
