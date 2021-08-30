
#include "task.h"
#include "task_manager.h"
#include "blink/blink.h"

TaskManager mgr(1, 1, 1, 1);
void setup()
{
    BlinkTask* task = new BlinkTask;

    Serial.begin(115200);
    mgr.UseTimerResolution(IN_SECONDS);
    mgr.AddTask(task, IN_SECONDS);
    mgr.InitializeSetup();
    mgr.Start();
}

void loop()
{
    // Serial.println(OCR0A);
}