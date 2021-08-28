#pragma once
#include "../timer.h"

class HoursTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _hours = 0;
        void (*hoursCallback)();
    public:
        HoursTimedTaskExecutor(void (*callback)())
        {
            hoursCallback = callback;
        }
        void Increment() override
        {            
            // Execute tasks configured for seconds
            hoursCallback();
            if (++_hours % 60 == 0)
            {
                _hours = 0;
            }
        }
};