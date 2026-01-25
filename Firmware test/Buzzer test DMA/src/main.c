#include <stdio.h>
#include "buzzer.h"
/* This program uses Timer2 with DMA to toggle a buzzer every other sec.*/


int main(void){
	/* Initiate buzzer*/
	buzzer_init(void);
	
	/* Configure timer*/
	tim2_config(void);
	
	/*Configure DMA*/
	dma1_stream5_config(void);

	/*Start timer2*/
	start_tim2(void);

	while(1){}
}i






