#ifndef MY_GPIO_H
#define MY_GPIO_H
#include <driver/gpio.h>

void gpio_init(gpio_num_t PIN_GPIO);

void gpio_control(gpio_num_t PIN_GPIO, int control_num);
//1 level up
//0 level down
#endif

