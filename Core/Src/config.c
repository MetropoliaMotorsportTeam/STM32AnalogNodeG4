#include "config.h"
#include "flash_doubleword.h"
#include "main.h"
#include "pedal_map.h"
#include "sensors.h"
#include "transfer_functions.h"
#include "virtual_sensors.h"
#include <string.h>

static void config_1(void);
static void config_2(void);
static void config_3(void);
static void config_4(void);

void Config_Setup(void)
{
  init_sensors();
  load_config();
  init_pedal_map();
  read_all_calib_values();
}

void apply_config(uint8_t config)
{
  switch (config)
  {
  case 1:
    config_1();
    break;
  case 2:
    config_2();
    break;
  case 3:
    config_3();
    break;
  case 4:
    config_4();
    break;
  default:
    break;
  }
}

void load_config()
{
  /* uint8_t config = get_saved_conf();
  uint8_t conf = (valid_config(config) ? config : DEFAULT_CONF);
  apply_config(conf); */
  config_1();
}

void process_config(uint8_t config)
{
  uint8_t conf = (valid_config(config) ? config : DEFAULT_CONF);
  if (save_config(conf) != HAL_OK)
    Error_Handler();

  apply_config(conf);
}

static void config_1(void)
{
  Sensor BTN1 = {TF_BTN, BTN1_ID, 200, 0, V5_in0};
  Sensor BTN2 = {TF_BTN, BTN2_ID, 200, 0, V5_in5};
  Sensor BTN3 = {TF_BTN, BTN3_ID, 200, 0, V5_in4};
  Sensor APPS2 = {TF_APPS2, APPS2_ID, 10, 0, V5_in1};
  Sensor APPS1 = {TF_APPS1, APPS1_ID, 10, 0, V24_in1};
  Sensor STEERING_ANGLE = {TF_24V, SteeringAngle_ID, 10, 0, V24_in0};
  Sensor BPPS = {TF_BPPS, BPPS_ID, 10, 0, V5_in2};
  Sensor F_ROLL = {TF_5V, Rolls1_ID, 100, 0, V5_in6};
  Sensor F_HEAVE = {TF_5V, HeavesFront_ID, 100, 0, V5_in3};

  APPS1Pedal = &sensors[APPS1.pin];
  APPS2Pedal = &sensors[APPS2.pin];
  BPPSPedal = &sensors[BPPS.pin];

  sensors[APPS1.pin] = APPS1;
  sensors[APPS2.pin] = APPS2;
  sensors[BPPS.pin] = BPPS;
  sensors[BTN1.pin] = BTN1;
  sensors[BTN2.pin] = BTN2;
  sensors[BTN3.pin] = BTN3;
  sensors[STEERING_ANGLE.pin] = STEERING_ANGLE;
  sensors[F_ROLL.pin] = F_ROLL;
  sensors[F_HEAVE.pin] = F_HEAVE;

  CAN_interval = 20;
}

static void config_2(void)
{
  Sensor APPS2 = {TF_APPS2, 2, 100, 0, V5_in0};
  Sensor APPS1 = {TF_APPS1, 1, 100, 0, V3_in0};
  sensors[APPS1.pin] = APPS1;
  sensors[APPS2.pin] = APPS2;
  APPS1Pedal = &sensors[APPS1.pin];
  APPS2Pedal = &sensors[APPS2.pin];
  CAN_interval = 100;
}

static void config_3(void)
{
  // Virtual sensor config
  Sensor APPS1 = {TF_APPS1, 1, 100, 0, V5_in0};
  Sensor APPS2 = {TF_APPS2, 2, 100, 0, V5_in1};
  sensors[APPS1.pin] = APPS1;
  sensors[APPS2.pin] = APPS2;

  pedalreq.input_count = 2;
  pedalreq.inputs[0] = &sensors[APPS1.pin];
  pedalreq.inputs[1] = &sensors[APPS2.pin];

  init_virtual_sensor(&pedalreq, "pedal request", update_pedalreq, NULL, 17);
  add_input_sensor(&pedalreq, &sensors[APPS1.pin]);
  add_input_sensor(&pedalreq, &sensors[APPS2.pin]);

  CAN_interval = 100;
}

static void config_4(void)
{
  Sensor W_TEMP = {TF_WATER_TEMP, 2, 100, 0, V5_in0};
  Sensor V5_LINE = {TF_5V, 10, 100, 0, V5_in1};

  sensors[W_TEMP.pin] = W_TEMP;
  sensors[V5_LINE.pin] = V5_LINE;

  CAN_interval = 100;
}
