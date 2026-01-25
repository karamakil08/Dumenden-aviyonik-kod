#include <stdio.h>
#include "buzzer.h"


void TIM2_IRQHandler(void)
{
    static uint32_t i = 1;

    // clear interrupt status
    if (TIM2->DIER & 0x01) {

        if (TIM2->SR & 0x01) 
	{

            TIM2->SR &= ~(1U << 0);

        }

    }

    buzzer_toggle(void);

}


int main(void){

	/* Initiate buzzer*/
	buzzer_init(void);

	/* Initiate timer*/
	tim2_init(void);

	/* Configure timer*/
	tim2_config(void);

	while(1){}
}






