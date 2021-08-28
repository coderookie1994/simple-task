#include <Arduino.h>
#include <avr/wdt.h>

#include "task_manager.h"
#include "../timer_executor/timer_executor.h"

TaskManager::TaskManager()
{
    _duration = 1;
    _tasks = new Task*[1];
}

void TaskManager::EnableGlobalWatchdog(const uint8_t& watchDogTimeout)
{
    // So it doesn't go in a loop
    GlobalWatchdogEnabled = true;
    delay(3000);
    wdt_enable(watchDogTimeout);
    Serial.print("Enabled watchdog timeout for ");
    Serial.println(watchDogTimeout);
}

void TaskManager::DisableGlobalWatchdog()
{
    cli();
    wdt_disable();
    GlobalWatchdogEnabled = false;
    sei();
    Serial.print("Disabled watchdog timeout");
}

void TaskManager::AddTask(Task* task, const int& timerType, const uint8_t& value)
{
    _totalTasks++;
    this->_tasks[0] = task;
}

void TaskManager::InitializeSetup()
{
    for (size_t i = 0; i < _totalTasks; i++)
    {
        this->_tasks[i]->Setup();
    }
}

void TaskManager::Start()
{
    // Set default resolution to milliseconds if not specifically invoked by the client
    if (_prescaler == 0 || _duration == 0)
        timerExecutor.UseMillisecondResolution();
}
