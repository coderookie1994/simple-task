#include "blink.h"

void BlinkTask::Setup()
{
    // D13 as output
    DDRB |= B00100000;
}

void BlinkTask::Execute()
{
    if (_toggle)
    {
        PORTB |= B00100000;
    }
    else{
        PORTB &= B11011111;
    }
    _toggle = !_toggle;
    _counter++;
}

void BlinkTask::AfterExecute()
{
}