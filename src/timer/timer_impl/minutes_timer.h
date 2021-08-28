#include "../timer.h"

class MinutesTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _minutesCounter = 0;
        void (*minutesCallback)(void *context);
        void *ctx;
    public:
        MinutesTimedTaskExecutor(void (*callback)(void *context), void *context)
        {
            minutesCallback = callback;
            ctx = context;
        }
        void Increment() override
        {            
            // Execute tasks configured for minutes
            minutesCallback(ctx);
            if (++_minutesCounter % 60 == 0)
            {
                nextTimer->Increment();
                _minutesCounter = 0;
            }
        }
};