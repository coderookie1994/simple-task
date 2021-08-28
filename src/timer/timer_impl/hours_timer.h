#pragma once
#include "../timer.h"

class HoursTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _hours = 0;
        void (*hoursCallback)(void *context);
        void *ctx;
    public:
        HoursTimedTaskExecutor(void (*callback)(void *context), void *context)
        {
            hoursCallback = callback;
            ctx = context;
        }
        void Increment() override
        {            
            // Execute tasks configured for seconds
            hoursCallback(ctx);
            if (++_hours % 60 == 0)
            {
                _hours = 0;
            }
        }
};