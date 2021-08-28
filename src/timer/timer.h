#pragma once

class TimedTaskExecutor
{
    protected:
        TimedTaskExecutor* nextTimer;
    public:
        virtual void Increment() = 0;
        virtual void SetNext(TimedTaskExecutor* next) { nextTimer = next; }
};