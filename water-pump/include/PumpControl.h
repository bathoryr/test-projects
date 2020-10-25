class PumpControl
{
    public:
        enum class PumpState {STOP, START, MANUAL_STOP};
        PumpControl();
        PumpState CheckStateLoop();
        
    private:
        PumpState m_state;
        
};