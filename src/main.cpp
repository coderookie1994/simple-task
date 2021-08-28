#include <Arduino.h>

#include "task.h"
#include "task_manager.h"
#include "task_executor.h"
#include "blink/blink.h"

TaskManager mgr;

void setup()
{
    BlinkTask* task = new BlinkTask;

    Serial.begin(9600);
    mgr.AddTask(task, IN_MINUTES, 1);
    mgr.InitializeSetup();
    mgr.Start();
}

void loop()
{
    // Serial.println(OCR0A);
    // delay(1000);
}