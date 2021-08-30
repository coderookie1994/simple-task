#include <Arduino.h>
#include <avr/wdt.h>

#include "task_manager.h"
#include "../timer_executor/timer_executor.h"

void msForwarder(void *context);
void secForwarder(void *context);
void minForwarder(void *context);
void hrCallbackForwarder(void *context);

TaskManager::TaskManager(const uint8_t& ms, const uint8_t& s, const uint8_t& min, const uint8_t& hr)
{
    _millisecondTasks = new Task*[ms];
    _secondsTasks = new Task*[s];
    _minutesTasks = new Task*[min];
    _hoursTasks = new Task*[hr];
}

void TaskManager::EnableGlobalWatchdog(const uint8_t& watchDogTimeout)
{
    // So it doesn't go in a loop
    GlobalWatchdogEnabled = true;
    delay((uint16_t)3000);
    wdt_enable(watchDogTimeout);
    Serial.println(watchDogTimeout);
}

void TaskManager::DisableGlobalWatchdog()
{
    cli();
    wdt_disable();
    GlobalWatchdogEnabled = false;
    sei();
}

void TaskManager::AddTask(Task* const& task, const int& timerType)
{
    if (timerType == IN_SECONDS) { this->_secondsTasks[_totalSecondTasks++] = task; }
    if (timerType == IN_MILLISECONDS){ this->_millisecondTasks[_totalMillisecondTasks++] = task;}
    if (timerType == IN_MINUTES) { this->_minutesTasks[_totalMinutesTasks++] = task; }
    if (timerType == IN_HOURS) { this->_hoursTasks[_totalHoursTasks++] = task; }
}

void TaskManager::InitializeSetup()
{
    size_t iterator = 0;
    for (iterator = 0; iterator < _totalMillisecondTasks; iterator++) { this->_millisecondTasks[iterator]->Setup(); }
    for (iterator = 0; iterator < _totalSecondTasks; iterator++) { this->_secondsTasks[iterator]->Setup(); }
    for (iterator = 0; iterator < _totalMinutesTasks; iterator++) { this->_minutesTasks[iterator]->Setup(); }
    for (iterator = 0; iterator < _totalHoursTasks; iterator++) { this->_hoursTasks[iterator]->Setup(); }
}

void TaskManager::UseTimerResolution(const uint8_t& resolution)
{
    if (resolution & IN_MILLISECONDS)
    {
        _resolution = IN_MILLISECONDS;
        return;
    }
    _resolution = IN_SECONDS;
}

void TaskManager::Start()
{
    // Set default resolution to milliseconds if not specifically invoked by the client
    if (_resolution & IN_SECONDS) { timerExecutor.UseSecondsResolution(); }
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

void TaskManager::OnMsTick()
{
    for (_commonIterator = 0; _commonIterator < _totalMillisecondTasks; _commonIterator++)
    {
        /* code */
        if (this->_millisecondTasks[_commonIterator]->CanExecute())
        {
            this->_millisecondTasks[_commonIterator]->Execute();
            this->_millisecondTasks[_commonIterator]->AfterExecute();
            // if (manager.GlobalWatchdogEnabled)
                // wdt_reset();                        
        }
    }
}

void TaskManager::OnSecondTick()
{
    for (_commonIterator = 0; _commonIterator < _totalSecondTasks; _commonIterator++)
    {
        /* code */
        if (this->_secondsTasks[_commonIterator]->CanExecute())
        {
            this->_secondsTasks[_commonIterator]->Execute();
            this->_secondsTasks[_commonIterator]->AfterExecute();
            // if (manager.GlobalWatchdogEnabled)
                // wdt_reset();                        
        }
    }
}

void TaskManager::OnMinuteTick()
{
    for (_commonIterator = 0; _commonIterator < _totalMinutesTasks; _commonIterator++)
    {
        /* code */
        if (this->_minutesTasks[_commonIterator]->CanExecute())
        {
            this->_minutesTasks[_commonIterator]->Execute();
            this->_minutesTasks[_commonIterator]->AfterExecute();
            // if (manager.GlobalWatchdogEnabled)
                // wdt_reset();                        
        }
    }
}

void TaskManager::OnHourTick()
{
    for (_commonIterator = 0; _commonIterator < _totalHoursTasks; _commonIterator++)
    {
        /* code */
        if (this->_hoursTasks[_commonIterator]->CanExecute())
        {
            this->_hoursTasks[_commonIterator]->Execute();
            this->_hoursTasks[_commonIterator]->AfterExecute();
            // if (manager.GlobalWatchdogEnabled)
                // wdt_reset();                        
        }
    }
}

void msForwarder(void *context) { static_cast<TaskManager*>(context)->OnMsTick(); }
void secForwarder(void *context) { static_cast<TaskManager*>(context)->OnSecondTick(); }
void minForwarder(void *context) { static_cast<TaskManager*>(context)->OnMinuteTick(); }
void hrCallbackForwarder(void *context) { static_cast<TaskManager*>(context)->OnHourTick(); }
