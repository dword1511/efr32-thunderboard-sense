/***********************************************************************************************//**
 * \file   connection-callback.c
 * \brief  Callbacks related to connection
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* standard library headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Own header*/
#include "connection-callback.h"

#include "app.h"
#include "app_ble_adv.h"

/***************************************************************************************************
 * Local Macros and Definitions
 **************************************************************************************************/


/***************************************************************************************************
 * Function Definitions
 **************************************************************************************************/
void connectionOpenedEventCallback(void)
{
  appBleAdvStop();
  appServiceStart();
  return;
}

void connectionTerminatedEventCallback(void)
{
  appServiceStop();
  appBleAdvStart();
  return;
}
