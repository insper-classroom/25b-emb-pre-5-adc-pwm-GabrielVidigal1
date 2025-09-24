#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "data.h"
QueueHandle_t xQueueData;

// não mexer! Alimenta a fila com os dados do sinal
void data_task(void *p) {
    vTaskDelay(pdMS_TO_TICKS(400));

    int data_len = sizeof(sine_wave_four_cycles) / sizeof(sine_wave_four_cycles[0]);
    for (int i = 0; i < data_len; i++) {
        xQueueSend(xQueueData, &sine_wave_four_cycles[i], 1000000);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void process_task(void *p) {
    int data = 0;
    int num_array =0;
    int total =0;
    int array[5];
    int prox_num;

    while (true) {
        if (xQueueReceive(xQueueData, &data, 100)) {
            if(num_array<5){
                array[num_array]=data;
                total+=data;
                prox_num=total/5;
            } else{
                printf("%d\n", prox_num);
                
                total=total-array[0]+data;
                prox_num=total/5;

                array[0]=array[1];
                array[1]=array[2];
                array[2]=array[3];
                array[3]=array[4];
                array[4]=data;
                

                
                //enviar prox_num
            
            }
            
            num_array+=1;
            
            
            
            // implementar filtro aqui!
             
                     




            // deixar esse delay!
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

int main() {
    stdio_init_all();

    xQueueData = xQueueCreate(64, sizeof(int));

    xTaskCreate(data_task, "Data task ", 4096, NULL, 1, NULL);
    xTaskCreate(process_task, "Process task", 4096, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}
