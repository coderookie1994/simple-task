#pragma once
#include <Arduino.h>

#include "task.h"

class BlinkTask : public Task
{
    private:
    public:
        void Setup() override;
        void Execute() override;
        void AfterExecute() override;
};