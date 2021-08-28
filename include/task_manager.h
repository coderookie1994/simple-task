#pragma once
#include <stdint.h>

#include "task.h"

#define IN_MILLISECONDS (uint8_t)1
#define IN_SECONDS      (uint8_t)2
#define IN_MINUTES      (uint8_t)4
#define IN_HOURS        (uint8_t)8

class TaskManager
{
    private:
        uint8_t _duration = 0;
        uint8_t _totalMillisecondTasks = 0;
        uint8_t _totalSecondTasks = 0;
        uint8_t _totalMinutesTasks = 0;
        uint8_t _totalHoursTasks = 0;
        unsigned short _prescaler = 0;
        uint8_t _commonIterator = 0;
        
        uint16_t GetCompareMatchRegisterValue();
    public:
        Task **_millisecondTasks;
        Task **_secondsTasks;
        Task **_minutesTasks;
        Task **_hoursTasks;
        bool GlobalWatchdogEnabled = false;
        TaskManager();
        void EnableGlobalWatchdog(const uint8_t& watchDogTimeout);
        void DisableGlobalWatchdog();
        void UseTimerResolution(uint8_t resolution);
        void AddTask(Task* task, const int& timerType, const uint8_t& value);
        void AddTask(Task* task, const int& timerType, const uint8_t& value, const uint8_t& iterations);
        void InitializeSetup();
        void Start();
        void Stop();
        void OnSecondTick();
        void OnMinuteTick();
};
