/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


 #include <stdio.h>
 #include "pico/stdlib.h"
 #include "hardware/gpio.h"
 #include "hardware/timer.h"
 #include "hardware/adc.h"
 
 const int PIN_LED_B = 4;

 const int poten = 28;
 
 const float conversion_factor = 3.3f / (1 << 12);

 const int LED_PIN = 4;

 volatile bool led_timer = false;
 
 /**
  * 0..1.0V: Desligado
  * 1..2.0V: 150 ms
  * 2..3.3V: 400 ms
  */

bool timer_callback(repeating_timer_t *rt) {
    led_timer = true;
    return true;
}
 int main() {

    repeating_timer_t timer;
    bool timer_ativo = false;

    stdio_init_all();

    adc_init();

    adc_gpio_init(28);
    adc_select_input(2);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    bool led_state = false;

     while (1) {
        

        uint16_t result = adc_read();
        float voltage = conversion_factor*result;
        printf("%f\n",voltage);

        if (voltage<1.0){
            if(timer_ativo){
                cancel_repeating_timer(&timer);
                timer_ativo = false;
                
                led_state = false;
                gpio_put(LED_PIN, led_state);
            }

        } else if (voltage<2.0){
            if(!timer_ativo){
                if (add_repeating_timer_ms(150, timer_callback, NULL, &timer)) {
                    timer_ativo = true;
                } else {
                    cancel_repeating_timer(&timer);
                    timer_ativo = false;
                
                    led_state = false;
                    gpio_put(LED_PIN, led_state);
                }
            }

        } else if (voltage<3.3){
            if(!timer_ativo){
                if (add_repeating_timer_ms(250, timer_callback, NULL, &timer)) {
                    timer_ativo = true;
                } else {
                    cancel_repeating_timer(&timer);
                    timer_ativo = false;
                
                    led_state = false;
                    gpio_put(LED_PIN, led_state);
                }
            }
        } else {
            if(timer_ativo){
                cancel_repeating_timer(&timer);
                timer_ativo = false;
                
                led_state = false;
                gpio_put(LED_PIN, led_state);
            }
        }

        if (led_timer) {
            led_timer = false;

            led_state = !led_state;
            gpio_put(LED_PIN, led_state);
        }

      

   
     }
 }
