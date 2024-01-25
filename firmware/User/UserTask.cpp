#include "UserTask.h"
#include <FreeRTOS.h>
#include <task.h>

#include "gpio.h"
#include "main.h"

static StaticTask_t idleTaskTCB;
static StackType_t idleTaskStack[ configMINIMAL_STACK_SIZE ];
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

extern "C" {
        
    void createUserTasks(void) {
        xTaskCreateStatic(idleTask, "idleTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, idleTaskStack, &idleTaskTCB);
    }

}

