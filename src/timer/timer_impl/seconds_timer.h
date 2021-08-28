#pragma once
#include "../timer.h"

class SecondsTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _secondCounter = 0;
        void (*secondsCallback)();
    public:
        SecondsTimedTaskExecutor(void (*callback)())
        {
            secondsCallback = callback;
        }
        void Increment() override
        {            
            // Execute tasks configured for seconds
            secondsCallback();
            if (++_secondCounter % 60 == 0)
            {
                nextTimer->Increment();
                _secondCounter = 0;
            }
        }
};