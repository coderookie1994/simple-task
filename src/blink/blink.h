#pragma once
#include <Arduino.h>

#include "task.h"

class BlinkTask : public Task
{
    private:
        bool _toggle = true;
        int _counter = 0;
    public:
        void Setup() override;
        void Execute() override;
        void AfterExecute() override;
};