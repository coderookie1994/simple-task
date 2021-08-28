#pragma once
#include "../timer.h"

class MSTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint16_t _millisecondCounter;
        void (*msCallback)();
    public:
        MSTimedTaskExecutor(void (*callback)())
        {
            msCallback = callback;
        }
        void Increment() override
        {
            // Execute tasks configured for milliseconds
            msCallback();
            if (++_millisecondCounter % 1000 == 0)
            {
                _millisecondCounter = 0;
                nextTimer->Increment();
            }
        }
};