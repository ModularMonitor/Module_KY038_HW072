#include <Arduino.h>
#include <Wire.h>
#include "mky038.h"
#include "Serial/package.h"

mKY* ky = nullptr;
constexpr int port_KY038 = GPIO_NUM_27; // mic
constexpr int port_HW072 = GPIO_NUM_26; // ldr

const auto this_device = CustomSerial::device_id::KY038_HW038_SENSOR;
  
void send_to_wire_on_request();

void setup()
{
  CustomSerial::begin_slave(this_device, send_to_wire_on_request);
  ky = new mKY(port_KY038);
  pinMode(port_HW072, INPUT);
}

void loop() { vTaskDelete(NULL); }

void send_to_wire_on_request()
{
  //const auto ky038 = analogRead(port_KY038);
  const auto hw072 = analogRead(port_HW072);

  //const float ky038_ln = 100.0f * ky038 / 4095.0f;//20.0f * log10(ky038 / 1023.0f);
  const unsigned long long hw072_bl = hw072 > 400.0f ? 1 : 0;

  CustomSerial::command_package cmd(this_device,
    "/mic", ky->get_vol(), // around "30" when no noise, up to 60 when high noise.
    "/ldr", hw072_bl
  );

  CustomSerial::write(cmd);
}
