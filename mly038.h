#pragma once

#include "SelfThreadable/self_threadable.h"

// mLY wraps LY038 in a async class. This way, less things to worry about.
class mLY : protected SelfThreadable {
    const int m_pin;
    float m_vol = 0.0f;
    
    bool m_has_new_data = false;

    void async() {
        pinMode(m_pin, INPUT);

        while(1) {
            const float rn = 100.0f * analogRead(m_pin) / 4095.0f;
            m_vol = (m_vol * 199.0f + rn) / 200.0f;
            m_has_new_data = true;
            delay(5);
        }

        vTaskDelete(NULL);
    }
public:
    mLY(const int pin) : SelfThreadable("ASYNC"), m_pin(pin) { async_start(); }

    float get_vol() const { return m_vol; }
    
    bool has_issues() const { return false; }
    bool has_new_data_autoreset() { bool had = m_has_new_data; m_has_new_data = false; return had; }
};