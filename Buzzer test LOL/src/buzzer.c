//DESCRIPTION
//This program toggles a buzzer on pin PA1 on and off using a timer interrupt every 1 sec




// Inlcude header file for project
#include "buzzer.h"


// Definitions
#define GPIOE 			(1U << 0)
#define TIM2ENR 		(1U << 0)
#define PESCALAR   		255 /*CAUTION!!*/
#define AUTO_RELOAD_VALUE   	62499
#define UIE 			(1U << 0)

void buzzer_init(void){
	// Allow clock access to gpioa
	RCC->AHB1ENR |= GPIOE;
	// Set pin mode to output
	GPIOA->MODER |= 	(1U << 2);
	GPIOA->MODER &= 	~(1U << 3);
	/* Turn off initailly to avoid undefined bevaiour*/
	GPIOA->ODR = &= 	~(1U << 1);

}
void buzzer_toggle(void){
	//Toggle buzzer using bit masking
	GPIOA->ODR ^= 		(1U << 1);
}

void tim2_init(void){
	/*Allow clock access*/
	RCC->APB1ENR |= TIM2ENR;
}

void tim2_config(void){
	/* Set timer prescalar*/
	TIM2->PSC = PESCALAR;
	/* When timer reaches this value causes an interrupt*/
	TIM2->ARR = AUTO_RELOAD_VALUE;
	/* Allow timer2 interrupts*/
	NVIC_EnableIRQ(TIM2_IRQn);
	/*Update interrupt enable*/
	TIM2->DIER |= UIE;
}


