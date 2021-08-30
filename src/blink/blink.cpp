#include "blink.h"

void BlinkTask::Setup()
{
    // D13 as output
    DDRB |= B00100000;
}

void BlinkTask::Execute(){ PORTB ^= (1<<PB5); }

void BlinkTask::AfterExecute()
{

}