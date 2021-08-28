#pragma once
#include "../timer.h"

class SecondsTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _secondCounter = 0;
        void (*secondsCallback)(void *context);
        void *ctx;
    public:
        SecondsTimedTaskExecutor(void (*callback)(void *context), void *context)
        {
            secondsCallback = callback;
            ctx = context;
        }
        void Increment() override
        {            
            // Execute tasks configured for seconds
            secondsCallback(ctx);
            if (++_secondCounter % 60 == 0)
            {
                nextTimer->Increment();
                _secondCounter = 0;
            }
        }
};