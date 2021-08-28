#include "../timer/timer.h"
#include "../timer/timer_impl/hours_timer.h"
#include "../timer/timer_impl/minutes_timer.h"
#include "../timer/timer_impl/seconds_timer.h"
#include "../timer/timer_impl/milliseconds_timer.h"
#include "task.h"

class TimerExecutor
{
    private:
        TimedTaskExecutor* _millisecondTimed;
        TimedTaskExecutor* _secondsTimed;
        TimedTaskExecutor* _minutesTimed;
        TimedTaskExecutor* _hoursTimed;

        Task** millisecondTasks;
        Task** secondsTasks;
        Task** minutesTasks;
        Task** hoursTasks;

        unsigned int _prescaler;
        unsigned int _duration;

        // Pointer to the configured TimedTaskExecutor to execute the tasks
        TimedTaskExecutor* _handle;
        unsigned int GetCompareMatchRegisterValue()
        {
            unsigned long num = 16 * 1000000;
            float denom = _prescaler / _duration;
            unsigned long result = (num / denom) - 1; 
            return (int)result;
        }
        void SetupTimer1()
        {
            uint8_t prescalerSelect;
            if (_prescaler == 1024)
            {
                prescalerSelect = (1<<CS12) | (1<<CS10);
            }
            if (_prescaler == 8)
            {
                prescalerSelect = (1<<CS11);
            }
            uint8_t oldSREG = SREG;
            // Disable interrupts
            cli();    

            // For Timer 1
            // Set TCNT1 and TCCR1A/B to 0
            TCNT1 = 0;
            TCCR1A = 0;
            TCCR1B = 0;
            OCR1A = this->GetCompareMatchRegisterValue();
            // Set 1024 or 8 as the prescaler and enable counter mode
            TCCR1B |= (1<<WGM12) | prescalerSelect;
            // Setup compare match interrupt
            TIMSK1 |= (1<<OCIE1A);
            // Enable interrupts
            SREG = oldSREG;
        }
    public:
        TimerExecutor()
        {
            _millisecondTimed    =  new MSTimedTaskExecutor(millisecondCallbackForwarder, this);
            _secondsTimed        =  new SecondsTimedTaskExecutor(secondsCallbackForwarder, this);
            _minutesTimed        =  new MinutesTimedTaskExecutor(minutesCallbackForwarder, this);
            _hoursTimed          =  new HoursTimedTaskExecutor(hoursCallbackForwarder, this);

            // Build execution pipeline
            _millisecondTimed->SetNext(_secondsTimed);
            _secondsTimed->SetNext(_minutesTimed);
            _minutesTimed->SetNext(_hoursTimed);
        }
        void UseMillisecondResolution()
        {
            _duration = 0.001;
            _prescaler = 8;
            SetupTimer1();
            this->_handle = this->_millisecondTimed;
        }
        void UseSecondsResolution()
        {
            _duration = 1;
            _prescaler = 1024;
            SetupTimer1();
            this->_handle = this->_secondsTimed;
        }
        void ExecuteTimedTasks()
        {
            _handle->Increment();
        }
        void ExecuteMillisecondTasks()
        {

        }
        void ExecuteSecondsTasks()
        {

        }
        void ExecuteMinutesTasks()
        {

        }
        void ExecuteHoursTasks() { }
};

TimerExecutor timerExecutor;

ISR(TIMER1_COMPA_vect)
{
    timerExecutor.ExecuteTimedTasks();
}

void millisecondCallbackForwarder(void *context)
{
    timerExecutor.ExecuteMillisecondTasks();
}
void secondsCallbackForwarder(void *context)
{
    timerExecutor.ExecuteSecondsTasks();
}
void minutesCallbackForwarder(void *context)
{
    timerExecutor.ExecuteMinutesTasks();
}
void hoursCallbackForwarder(void *context)
{
    timerExecutor.ExecuteHoursTasks();
}