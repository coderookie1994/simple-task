#pragma once
#include <stdint.h>

#include "task.h"

#define IN_MILLISECONDS 1
#define IN_SECONDS      2
#define IN_MINUTES      4
#define IN_HOURS        8

class TaskManager
{
    private:
        float _duration = 0.0f;
        unsigned int _totalTasks = 0;
        unsigned short _prescaler = 0;
        unsigned int GetCompareMatchRegisterValue();
    public:
        Task **_tasks;
        bool GlobalWatchdogEnabled = false;
        TaskManager();
        void EnableGlobalWatchdog(const uint8_t& watchDogTimeout);
        void DisableGlobalWatchdog();
        void AddTask(Task* task, const int& timerType, const uint8_t& value);
        void AddTask(Task* task, const int& timerType, const uint8_t& value, const uint8_t& iterations);
        void InitializeSetup();
        void Start();
};