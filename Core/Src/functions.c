/*
 * functions.c
 *
 *  Created on: Mar 4, 2024
 *      Author: csort
 */

#include "functions.h"
#include "config.h"
#include "flash_pedal_lut.h"
#include "main.h"
#include "pedal_map.h"
#include "stm32g4xx.h"
#include "stm32g4xx_hal.h"
#include "transfer_functions.h"

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_TxHeaderTypeDef TxHeader;
extern FDCAN_RxHeaderTypeDef RxHeader;

CAN_Message RxMessage;
CAN_Message TxMessage;

static uint8_t sensor_for_calib; // Sensor calibration number
static int8_t calib_select = -1; // Upper or lower calibration
volatile uint8_t CANRxReady = 0;

/*
calib_code = 0  -> no calibration
calib_code = 1  -> low_adc is valid
calib_code = 2  -> high_adc is valid
calib_code = 3  -> both are valid
*/

void print(uint16_t select)
{

  if (sensors[select].CAN_ID)
  {

    sensors[select].data =
        sensors[select].transfer_function(1, sensors[select].averages, &sensors[select]);
    TxMessage.Bytes[0] = (uint16_t)sensors[select].data & 0xFF;
    TxMessage.Bytes[1] = (uint16_t)sensors[select].data >> 8 & 0xFF;
    TxHeader.Identifier = sensors[select].CAN_ID;

    CanSend(TxMessage.Bytes);
  }
}

void sent_calib_done()
{
  if (calib_select == -1)
    return;

  TxHeader.Identifier = CAN_RETURN_MSG_ID;
  TxMessage.Bytes[1] = sensor_for_calib;
  CanSend(TxMessage.Bytes);

  sensors[sensor_for_calib].calib_code = sensors[sensor_for_calib].calib_code | (1 << calib_select);

  ADC_Calib_Update();
}

static uint8_t calibration_counter = 0;
static uint16_t calibration_value = 0;

static uint16_t max_value = 0;
static uint16_t min_value = 65535;

void set_calib_values(uint8_t sensor, int8_t select)
{
  static uint8_t counter = 0;
  if (counter > 0)
    return;
  sensor_for_calib = sensor;
  calib_select = select;
  counter++;
}

void calibration()
{

  if (calib_select == -1)
  {
    return;
  }
  else
  {
    calibration_counter++;
    calibration_value +=
        (sensors[sensor_for_calib].averages - calibration_value) / calibration_counter;

    if (sensors[sensor_for_calib].averages > max_value)
    {
      max_value = sensors[sensor_for_calib].averages;
    }

    if (sensors[sensor_for_calib].averages < min_value)
    {
      min_value = sensors[sensor_for_calib].averages;
    }

    if (calibration_counter >= (1000 / CAN_interval))
    {

      calibration_value = calibration_value / calibration_counter;

      if (calib_select == 0)
        sensors[sensor_for_calib].low_adc = min_value;
      if (calib_select == 1)
        sensors[sensor_for_calib].high_adc = max_value;

      sent_calib_done();

      calib_select = -1;
      calibration_counter = 0;

      max_value = 0;
      min_value = 65535;
    }
  }
}

void decode(CAN_Message msg)
{
  CANRxReady = 0;
  switch (msg.Id)
  {
  case CAN_CALIB_ID:
    sensor_for_calib = msg.Bytes[0];
    calib_select = msg.Bytes[1];
    break;
  case CAN_CHANGE_CONFIG:
    process_config(msg.Bytes[0]);
    break;
  case CAN_CHANGE_PEDAL_PROFILE:
    process_pedal_profile_change(msg);
    break;
  case CAN_ADD_PEDAL_PROFILE:
    process_pedal_profile_add(msg);
    break;
  case CAN_PEDAL_SAVE_FLASH:
    process_pedal_flash_save(msg);
    break;

  default:
    break;
  }
}
