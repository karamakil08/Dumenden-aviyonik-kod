#ifndef BUZZER_H_
#define BUZZER_H_


#include <stdio.h>
#include "stm32f4xx.h"

void buzzer_init(void);
void tim2_config(void);
void start_tim2(void);
void dma1_stream5_config(void);
