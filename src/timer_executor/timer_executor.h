#pragma once

#include "../timer/timer.h"
#include "../timer/timer_impl/hours_timer.h"
#include "../timer/timer_impl/minutes_timer.h"
#include "../timer/timer_impl/seconds_timer.h"
#include "../timer/timer_impl/milliseconds_timer.h"
#include "task.h"

void millisecondCallbackForwarder();
void secondsCallbackForwarder();
void minutesCallbackForwarder();
void hoursCallbackForwarder();

class TimerExecutor
{
    private:
        TimedTaskExecutor* _millisecondTimed;
        TimedTaskExecutor* _secondsTimed;
        TimedTaskExecutor* _minutesTimed;
        TimedTaskExecutor* _hoursTimed;

        uint16_t  _prescaler;
        uint8_t _duration;
        uint16_t _prescalerSelect;

        // Callbacks
        void (*_onMilliSecondTick)(void *context);
        void (*_onSecondTick)(void *context);
        void (*_onMinuteTick)(void *context);
        void (*_onHourTick)(void *context);
        void *ctx;

        // Pointer to the configured TimedTaskExecutor to execute the tasks
        TimedTaskExecutor* _handle;
        uint16_t GetCompareMatchRegisterValue()
        {
            unsigned long num = (16 * 1000000 * _duration) / _prescaler;
            // float denom = _prescaler / _duration;
            // unsigned long result = (num / denom) - 1; 
            // return (int)result;
            return (uint16_t)num;
        }
        void SetupTimer1()
        {
            if (_prescaler == 1024)
            {
                _prescalerSelect = (1<<CS12) | (1<<CS10);
            }
            if (_prescaler == 8)
            {
                _prescalerSelect = (1<<CS11);
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
            TCCR1B |= (1<<WGM12) | _prescalerSelect;
            // Setup compare match interrupt
            TIMSK1 |= (1<<OCIE1A);
            // Enable interrupts
            SREG = oldSREG;
        }
        void StopTimer1()
        {
            uint8_t oldSREG = SREG;
            cli();
            TCNT1 = 0;
            TCCR1A = 0;
            TCCR1B = 0;
            OCR1A = 0;
            TCCR1B &= ~(1<<WGM12) | ~_prescalerSelect;
            TIMSK1 &= ~(1<<OCIE1A);
            SREG = oldSREG;
        }
    public:
        TimerExecutor()
        {
            _millisecondTimed    =  new MSTimedTaskExecutor(millisecondCallbackForwarder);
            _secondsTimed        =  new SecondsTimedTaskExecutor(secondsCallbackForwarder);
            _minutesTimed        =  new MinutesTimedTaskExecutor(minutesCallbackForwarder);
            _hoursTimed          =  new HoursTimedTaskExecutor(hoursCallbackForwarder);

            // Build execution pipeline
            _millisecondTimed->SetNext(_secondsTimed);
            _secondsTimed->SetNext(_minutesTimed);
            _minutesTimed->SetNext(_hoursTimed);
        }
        void UseMillisecondResolution()
        {
            _duration = 0.001;
            _prescaler = 8;
            this->_handle = this->_millisecondTimed;
        }
        void UseSecondsResolution()
        {
            _duration = 1;
            _prescaler = 1024;
            this->_handle = this->_secondsTimed;
        }
        void StartTimerExecutor(
            void (*onMilliSecondTick)(void *context),
            void (*onSecondTick)(void *context),
            void (*onMinuteTick)(void *context),
            void (*onHourTick)(void *context),
            void* context) 
        { 
            _onMilliSecondTick = onMilliSecondTick;
            _onSecondTick = onSecondTick;
            _onMinuteTick = onMinuteTick;
            _onHourTick = onHourTick;
            ctx = context;
            SetupTimer1(); 
        }
        void StopTimerExecutor() { StopTimer1(); }
        void ExecuteTimedTasks(){ _handle->Increment(); }
        void OnMillisecondTick(){ _onMilliSecondTick(ctx); }
        void OnSecondTick(){ _onSecondTick(ctx); }
        void OnMinuteTick() { _onMinuteTick(ctx); }
        void OnHourTick() { _onHourTick(ctx); }
};

TimerExecutor timerExecutor;

ISR(TIMER1_COMPA_vect) { timerExecutor.ExecuteTimedTasks(); }

void millisecondCallbackForwarder() { timerExecutor.OnMillisecondTick(); }
void secondsCallbackForwarder() { timerExecutor.OnSecondTick(); }
void minutesCallbackForwarder() { timerExecutor.OnMinuteTick(); }
void hoursCallbackForwarder() { timerExecutor.OnHourTick(); }