#include "UserTask.h"
#include <FreeRTOS.h>
#include <task.h>

#include "main.h"

#include "AS5048.h"

static StaticTask_t idleTaskTCB;
static StackType_t idleTaskStack[ 32 ];
void idleTask( void * parameters )
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    for( ; ; )
    {
        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static StaticTask_t as5048TaskTCB;
static StackType_t as5048TaskStack[ configMINIMAL_STACK_SIZE ];
void as5048Task( void * parameters )
{
    static AS5048 as5048(&hspi1);
    for( ; ; )
    {
        as5048.update();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

extern "C" {
        
    void createUserTasks(void) {
        xTaskCreateStatic(idleTask, "idleTask", 32, NULL, 0, idleTaskStack, &idleTaskTCB);
        xTaskCreateStatic(as5048Task, "as5048Task", configMINIMAL_STACK_SIZE, NULL, 10, as5048TaskStack, &as5048TaskTCB);
    }

}

