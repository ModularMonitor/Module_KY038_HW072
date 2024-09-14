#include <Arduino.h>
#include <Wire.h>
#include "mly038.h"
#include "Serial/packaging.h"
#include "Serial/flags.h"

using namespace CS;

PackagedWired* wire;
mLY* ly = nullptr;
constexpr int port_LY038 = GPIO_NUM_27; // mic
constexpr int port_HW072 = GPIO_NUM_26; // ldr
const auto this_device = device_id::LY038_HW072_SENSOR;

void callback(void*, const uint8_t, const char*, const uint8_t);

void setup()
{
    Serial.begin(115200);
    
    ly = new mLY(port_LY038);
    pinMode(port_HW072, INPUT);
    
    wire = new PackagedWired(config()
        .set_slave(this_device)
        .set_slave_callback(callback)
        .set_led(2)
    );
}

void callback(void* rw, const uint8_t expects, const char* received, const uint8_t length)
{
    if (length != sizeof(Requester)) return;
    
    PackagedWired& w = *(PackagedWired*) rw;
    Requester req(received);
    
    switch(req.get_offset()) {
    case 0:
    {
        FlagWrapper fw;
        if (ly->has_issues())              fw |= device_flags::HAS_ISSUES;
        if (ly->has_new_data_autoreset())  fw |= device_flags::HAS_NEW_DATA;
        
        Command cmd("#FLAGS", (uint64_t)fw);
        w.slave_reply_from_callback(cmd);
    }
    break;
    case 1:
    {
        const float val = ly->get_vol();
        Command cmd("/ky038/volume", val);
        w.slave_reply_from_callback(cmd);
        //Serial.printf("Received request {%zu}\nReplying with %.3f\n", req.get_offset(), val);
    }
    break;
    case 2:
    {
        const auto hw072 = analogRead(port_HW072);
        const uint64_t val = hw072 > 400.0f ? 0 : 1;
        
        Command cmd("/hw072/on", val);
        w.slave_reply_from_callback(cmd);
        //Serial.printf("Received request {%zu}\nReplying with %llu\n", req.get_offset(), val);
    }
    break;
    default:
    {
        Command cmd; // invalid
        w.slave_reply_from_callback(cmd);
        //Serial.printf("Received request {%zu}\nConsidered invalid!\n", req.get_offset());
    }
    }
}

// unused
void loop() { vTaskDelete(NULL); }