// #pragma once
// #include <avr/wdt.h>
// #include <stdlib.h>
// #include <Arduino.h>

// #include "task_manager.h"

// class TaskExecutor
// {
//     private:
//         TaskManager manager;
//     public:
//         void EnableTaskExecutions(const TaskManager& mgr)
//         {
//             manager = mgr;
//         }
//         void ExecuteTasks()
//         {
//             for (size_t i = 0; i < 1; i++)
//             {
//                 /* code */
//                 if (manager._tasks[i]->CanExecute())
//                 {
//                     manager._tasks[i]->Execute();
//                     manager._tasks[i]->AfterExecute();
                    
//                     if (manager.GlobalWatchdogEnabled)
//                         wdt_reset();                        
//                 }
//             }   
//         }
// };

// TaskExecutor Executor;

// ISR(TIMER1_COMPA_vect)
// {
//     // Timer 1 ISR
//     int elapsed = millis() / 1000;
//     Serial.println(elapsed);
//     Executor.ExecuteTasks();
// }