/* standard library headers */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/* BG stack headers */
#include "bg_types.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "infrastructure.h"

/* plugin headers */
#include "connection.h"

#include "thunderboard/mic.h"

/* Own header*/
#include "es.h"

extern uint8_t  RADIO_uvIndex;
extern uint32_t RADIO_ambLight;
extern uint32_t RADIO_rhData;
extern int32_t  RADIO_tempData;
extern float    RADIO_pressure;

extern float    RADIO_soundLevel;
extern uint32_t RADIO_soundLevelBufferCount;
extern float    RADIO_soundLevelAccumulated;

void esInit(void)
{
  return;
}

void esReadUVIndex(void)
{

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_uvindex,
                                                0,
                                                sizeof(RADIO_uvIndex),
                                                (uint8_t *)&RADIO_uvIndex);

  return;
}

void esReadPressure(void)
{
  /* 0.1 Pa */

  /* 1 mbar = 100 Pa. We want 0.1 Pa, so multiply by 1000 */
  uint32_t pressure = (uint32_t)(RADIO_pressure * 1000);


  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_pressure,
                                                0,
                                                sizeof(pressure),
                                                (uint8_t *)&pressure);

  return;
}

extern uint32_t RADIO_rhData;
extern int32_t  RADIO_tempData;
void esReadTemperature(void)
{
  /* 0.01 C */

  int16_t btTemp = RADIO_tempData / 10;


  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_temperature,
                                                0,
                                                sizeof(btTemp),
                                                (uint8_t *)&btTemp);

  return;
}

void esReadHumidity(void)
{
  /* 0.01 %RH */
  uint16_t humidity = RADIO_rhData / 10;


  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_humidity,
                                                0,
                                                sizeof(humidity),
                                                (uint8_t *)&humidity);

  return;
}

void esReadAmbientLight(void)
{
  /* 0.01 Lux */


  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_ambient_light,
                                                0,
                                                sizeof(RADIO_ambLight),
                                                (uint8_t *)&RADIO_ambLight);

  return;
}

void esReadSoundLevel(void)
{
  /* 0.01 dBA */
  static int16_t soundLevel = 0;

  soundLevel = (int16_t)(RADIO_soundLevel * 100);

  RADIO_soundLevelAccumulated = 0;
  RADIO_soundLevelBufferCount = 0;

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_sound_level,
                                                0,
                                                sizeof(soundLevel),
                                                (uint8_t *)&soundLevel);
  return;
}

void esReadControlPoint(void)
{
  static uint32_t cp = 0;

  cp++;


  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_es_control_point,
    0,
    sizeof(cp),
    (uint8_t *)&cp
    );

  return;
}

void esControlPointChange(uint8_t connection, uint16_t clientConfig)
{
  return;
}

/* Write response codes*/
#define ES_WRITE_OK                         0
#define ES_ERR_CCCD_CONF                    0x81
#define ES_ERR_PROC_IN_PROGRESS             0x80
#define ES_NO_CONNECTION                    0xFF

void esControlPointWrite(uint8array *writeValue)
{

  gecko_cmd_gatt_server_send_user_write_response(
    conGetConnectionId(),
    gattdb_es_control_point,
    ES_WRITE_OK
    );

  return;
}

void esControlPointStatusChange(uint8_t connection, uint16_t clientConfig)
{

  return;
}
