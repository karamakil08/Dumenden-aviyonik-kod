#ifndef BUZZER_H_
#define BUZZER_H_


#include <stdio.h>
#include "stm32f4xx.h"

void buzzer_init(void);
void buzzer_toggle(void);
void tim2_init(void);
void tim2_config(void);
