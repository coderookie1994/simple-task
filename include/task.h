#pragma once
#include <stdint.h>

class Task
{
    private:
        // Used to keep track whether a task needs to run or not
        uint8_t _internalCounter;
    protected:
        bool _canExecute = true;
        // Default set to 1, can run for 1ms, 1s, 1min, 1hr, depending upon the task type
        uint8_t _interval = 1;
    public:
        Task() { _internalCounter = _interval - 1; }
        virtual void Setup() { };
        virtual void BeforeExecute() { };
        virtual void Execute() { };
        virtual void AfterExecute() { };
        virtual void ShouldExecute(bool value) { _canExecute = value; };
        bool CanExecute() {
            if (!_canExecute) return false; 
            if (++_internalCounter % _interval == 0)
                return true;
            return false;
        }
};