#include "../timer.h"

class MSTimedTaskExecutor : public TimedTaskExecutor
{
    private:
        uint8_t _millisecondCounter;
        void (*msCallback)(void *context);
        void *ctx;
    public:
        MSTimedTaskExecutor(void (*callback)(void *context), void *context)
        {
            msCallback = callback;
            ctx = context;
        }
        void Increment() override
        {
            // Execute tasks configured for milliseconds
            msCallback(ctx);
            if (++_millisecondCounter % 1000 == 0)
            {
                _millisecondCounter = 0;
                nextTimer->Increment();
            }
        }
};