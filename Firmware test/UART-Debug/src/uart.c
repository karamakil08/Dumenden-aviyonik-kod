#include "uart.h"

/*This program is mainly used for debuggin and printing on serial program
 * Uses pins PA9 PA10 for rx tx
 * Uses DMA
 * Initially only TX is working
 * TX uses DMA1 stream 3 channel 4
 * RX uses DMA1 stream 2 channel 4*/

/*************NOTES********************/
/* Make stream continious by enabling the circular mode -----> Check line 113
 * Make data transmission on demand function e.g. UART_write*/





#define GPIOENR			(1 << 0)
#define USART1ENR		(1 << 4)
#define USART1_CR3_DMAT		(1 << 7)
#define Sysclock		16000000
#define BAUDRATE		115200
#define UART_TE			(1 << 3)
#define UE			(1 << 13)
#define USART_TC 		(1 << 6)
#define DMA1_ENR		(1 << 21)
#define DMA_SE			(1 << 0)
#define DMA_TCIE		(1 << 4)
#define DMA_MINC		(1 << 10)
#define DMA_CRCENR		(1 << 8)
volatile uint8_t msg[] = "DUMENDEN AVIYONIK KOD\n\r";

void uart_init(void){
	/* Allow clock access to GPIOA port*/
	RCC->AHB1ENR |= GPIOENR;

	/*Set pin9 to alternate function */
	GPIOA->MODER |= (1 << 19);
	GPIOA->MODER &=~(1 << 18);

	/*Set pin10 to alternate function*/
	GPIOA->MODER &=~(1 << 20);
	GPIOA->MODER |= (1 << 21);

	/*Set pin PA9 alternate function tx to USART1*/
	GPIOA->AFR[1] |= (1 << 4);
	GPIOA->AFR[1] |= (1 << 5);
	GPIOA->AFR[1] |= (1 << 6);
	GPIOA->AFR[1] &=~(1 << 7);

	/*Set pin PA10 alternate fucntion rx to USART1*/
	GPIOA->AFR[1] |= (1 << 8);
	GPIOA->AFR[1] |= (1 << 9);
	GPIOA->AFR[1] |= (1 << 10);
	GPIOA->AFR[1] &=~(1 << 11);
/**************config USART1*****************/

	/* Allow clock access to USART1*/
	RCC->APB2ENR |= USART1ENR;

	/*Enable transmit DMA*/
	USART1->CR3 |= USART1_CR3_DMAT;

	/*Set baudrate*/
	uart_set_baudrate(Sysclock,BAUDRATE);

	/*USART Transmit enable*/
	USART1->CR1 |= UART_TE;

	/*Enable UART*/
	USART1->CR1 |= UE;

	/*Clear TC bit*/
	USART1->SR &=~USART_TC;

/*************DMA config*********************/

	/*Allow clock access to DMA1*/
	RCC->AHB1ENR |= DMA1_ENR;

	/*Clear control register*/
	DMA1_Stream3->CR = 0;
	
	/*Wait until Stream disasbled*/
	while(DMA1_Stream3->CR& DMA_SE);

	/* Select channel*/
	DMA1_Stream3->CR |= (1 << 27);
	DMA1_Stream3->CR &=~(1 << 26);
	DMA1_Stream3->CR &=~(1 << 25);

	/* No need to specify Data size --> already 00 for byte*/

	/*Enable memory increment mode*/
	DMA1_Stream3->CR |= DMA_MINC;

	/*Set prioity high*/
	DMA1_Stream3->CR |= (1 << 17);
	DMA1_Stream3->CR &=~(1 << 16);

	/*Set data transfer direction to be memory to periph*/
	DMA1_Stream3->CR |= (1 << 6);
	DMA1_Stream3->CR &=~(1 << 7);

	/* Source memory address*/
	DMA1_Stream3->M0AR = (uint32_t)msg;

	/* Destination memory address*/
	DMA1_Stream3->PAR = (uint32_t)&(USART1->DR);

	/* Number of items to be transferred*/
	DMA1_Stream3->NDTR = sizeof(msg);
	
	/* Select circular mode for continous data transfer*/
	DMA1_Stream3->CR |= DMA_CRCENR;

	/* enable stream*/
	DMA1_Stream3->CR |= DMA_SE;
}


uint16_t static inline calc_baudrate(uint32_t clock,uint32_t baudrate){
	return ((clock + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t clock, uint32_t baudrate){
	USART1->BRR = calc_baudrate(clock, baudrate);
}
