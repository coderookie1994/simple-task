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
        uint16_t _cmrValue;

        // Callbacks
        void (*_onMilliSecondTick)(void *context);
        void (*_onSecondTick)(void *context);
        void (*_onMinuteTick)(void *context);
        void (*_onHourTick)(void *context);
        void *ctx;

        // Pointer to the configured TimedTaskExecutor to execute the tasks
        TimedTaskExecutor* _handle;
        uint16_t GetCompareMatchRegisterValue() { return _cmrValue; }
        void SetupTimer1()
        {
            if (_prescaler == 1024)
            {
                // Reusing the prescaler value for CS bits 
                _prescaler = (_prescaler & B00000000) | (1<<CS12) | (1<<CS10);
            }
            if (_prescaler == 8)
            {
                _prescaler = (_prescaler & B00000000) | (1<<CS11);
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
            TCCR1B |= (1<<WGM12) | _prescaler;
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
            TCCR1B &= ~(1<<WGM12) | ~_prescaler;
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
            _prescaler = 8;
            _cmrValue = 1999;
            this->_handle = this->_millisecondTimed;
        }
        void UseSecondsResolution()
        {
            _prescaler = 1024;
            _cmrValue = 15624;
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