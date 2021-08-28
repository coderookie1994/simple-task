#include <Arduino.h>
#include <avr/wdt.h>

#include "task_manager.h"
#include "../timer_executor/timer_executor.h"

void msForwarder(void *context);
void secForwarder(void *context);
void minForwarder(void *context);
void hrCallbackForwarder(void *context);

TaskManager::TaskManager()
{
    _millisecondTasks = new Task*[1];
    _secondsTasks = new Task*[1];
    _minutesTasks = new Task*[1];
    _hoursTasks = new Task*[1];
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
    if (timerType == IN_SECONDS)
    {
        this->_secondsTasks[_totalSecondTasks++] = task;
    }
    if (timerType == IN_MILLISECONDS)
        this->_millisecondTasks[_totalMillisecondTasks++] = task;
    if (timerType == IN_MINUTES)
        this->_minutesTasks[_totalMinutesTasks++] = task;
}

void TaskManager::InitializeSetup()
{
    size_t iterator = 0;
    for (iterator = 0; iterator < _totalMillisecondTasks; iterator++) { this->_millisecondTasks[iterator]->Setup(); }
    for (iterator = 0; iterator < _totalSecondTasks; iterator++) { this->_secondsTasks[iterator]->Setup(); }
    for (iterator = 0; iterator < _totalMinutesTasks; iterator++) { this->_minutesTasks[iterator]->Setup(); }
    for (iterator = 0; iterator < _totalHoursTasks; iterator++) { this->_hoursTasks[iterator]->Setup(); }
}

void TaskManager::UseTimerResolution(uint8_t resolution)
{
    if (resolution & IN_SECONDS)
    {
        this->_duration = 1;
        this->_prescaler = 1024;
    }
    if (resolution & IN_MILLISECONDS)
    {
        this->_duration = .001;
        this->_prescaler = 8;
    }
    if (resolution & IN_MINUTES)
    {
        // Use seconds for temporary purposes
        this->_duration = 1;
        this->_prescaler = 1024;
    }
}

void TaskManager::Start()
{
    // Set default resolution to milliseconds if not specifically invoked by the client
    if (_prescaler == 1024 && _duration == 1)
    {
        timerExecutor.UseSecondsResolution();
    }
    else
        timerExecutor.UseMillisecondResolution();
    timerExecutor.StartTimerExecutor(
        msForwarder,
        secForwarder,
        minForwarder,
        hrCallbackForwarder,
        this);
}

void TaskManager::Stop(){ timerExecutor.StopTimerExecutor(); }

void TaskManager::OnSecondTick()
{
    for (size_t i = 0; i < _totalSecondTasks; i++)
    {
        /* code */
        if (this->_secondsTasks[i]->CanExecute())
        {
            this->_secondsTasks[i]->Execute();
            this->_secondsTasks[i]->AfterExecute();
            // if (manager.GlobalWatchdogEnabled)
                // wdt_reset();                        
        }
    }
}

void TaskManager::OnMinuteTick()
{
    for (size_t i = 0; i < _totalMinutesTasks; i++)
    {
        /* code */
        if (this->_minutesTasks[i]->CanExecute())
        {
            this->_minutesTasks[i]->Execute();
            this->_minutesTasks[i]->AfterExecute();
            // if (manager.GlobalWatchdogEnabled)
                // wdt_reset();                        
        }
    }
}

int ctr = 0;
void msForwarder(void *context) { }
void secForwarder(void *context) { Serial.println(++ctr); }
void minForwarder(void *context) { static_cast<TaskManager*>(context)->OnMinuteTick(); }
void hrCallbackForwarder(void *context) { }
