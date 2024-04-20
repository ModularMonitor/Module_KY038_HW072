class mKY {
    TaskHandle_t m_thr = nullptr;
    const int m_pin;
    float m_vol = 0.0f;

    void _async();
public:
    mKY(const mKY&) = delete;
    mKY(mKY&&) = delete;
    void operator=(const mKY&) = delete;
    void operator=(mKY&&) = delete;

    mKY(const int);
    ~mKY();

    float get_vol() const;
};

inline void mKY::_async()
{
    pinMode(m_pin, INPUT);

    while(1) {
        const float rn = 100.0f * analogRead(m_pin) / 4095.0f;
        m_vol = (m_vol * 199.0f + rn) / 200.0f;
        delay(5);
    }

    vTaskDelete(NULL);
}

inline mKY::mKY(const int pin)
    : m_pin(pin)
{
    xTaskCreate([](void* thus){((mKY*)thus)->_async();}, "ASYNC_KY", 2048, this, tskIDLE_PRIORITY, &m_thr);
}

inline mKY::~mKY()
{
    vTaskDelete(m_thr);
    m_thr = nullptr;
}

inline float mKY::get_vol() const
{
    return m_vol;
}
