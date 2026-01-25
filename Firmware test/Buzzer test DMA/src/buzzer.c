#include "buzzer.h"


#define GPIOENR 		(1U << 0)
#define TIM2ENR			(1U << 0)
#define TIM2UDE 		(1U << 8)
#define CRER			(1U << 0)
#define DMA1ENR 		(1U << 21)
#define DMA_CR_EN		(1U << 0)
#define SENR 			(1U << 0)

void buzzer_init(void){
	
	/* Enable clock access to port A for buzzer*/
	RCC->AHB1ENR |= GPIOAENR;
	
	/*Set buzzer mode as output*/
	GPIOA->MODER |= (1 << 2);
	GPIOA->MODER &= ~(1 << 3);
	/*reset buzzer to avoid unexpected behaviour*/
	GPIOA->BBR |= (1 << (1 + 16));
}

uint16_t brr_states[2] = {
	(1 << 1), (1 << (1 +16 ))
};

void tim2_config(void){
	/*Enable clock access to tim2*/
	RCC->APB1ENR |= TIM2ENR;

	/* SEt prescalar*/
	TIM2->PSC = 255;
	
	/*Set reload value*/
	TIM2->ARR = 62499;
	
	/*Enable timer DMA request*/
	TIM2->DIER |= TIM2UDE;

}
/*WARNING!
 * should be used after timer is configured only serves to start the counter*/
void start_tim2(void){
	TIM2->CR1 |= CRER;
}



void dma1_stream5_config(void){
	/* Allow clock access to DMA1*/
	RCC->AHB1ENR |= DMA1ENR;

	/*DMA stream*/
	DMA1_Stream5->CR = 0;

	/* Wait until stream disabled*/
	while(DMA1_Stream5->CR&DMA_CR_EN){}

	/*Set peripheral address*/
	DMA1_Stream5->PAR = (uint32_t)&GPIOA->BRR;
	/*Set memory address*/
	DMA_Stream5->M0AR = (uint32_t)brr_states; /* No need to use & for address since in C reffering to a list is using its address*/
	/* Configure DMA*/
	DMA1_Stream5->CR |= (1 << 6) & ~(1 << 7)| // memory → peripheral
    		(1 << 10) |       		  // increment memory
    		(1 << 8) |       		  // circular mode
    		(1 << 14) & ~(1 <<13)|    	  // 32-bit memory
    		(1 << 12) & ~(1 << 11)|     	  // 32-bit peripheral
		(1 << 0); 			  // Enable stream
}

