#pragma once
#include "../timer.h"

class MinutesTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _minutesCounter = 0;
        void (*minutesCallback)();
    public:
        MinutesTimedTaskExecutor(void (*callback)())
        {
            minutesCallback = callback;
        }
        void Increment() override
        {            
            // Execute tasks configured for minutes
            minutesCallback();
            if (++_minutesCounter % 60 == 0)
            {
                nextTimer->Increment();
                _minutesCounter = 0;
            }
        }
};