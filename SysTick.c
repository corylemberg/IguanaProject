#include "stm32l476xx.h"

volatile uint32_t TimeDelay;
int elapsed;

// ticks: number of ticks between two interrupts
void SysTick_Initialize(uint32_t ticks) {
	
	// Disable SysTick IRQ and SysTick counter
	SysTick->CTRL = 0;
	
	// Set reload register
	SysTick->LOAD = ticks - 1;
	
	// Set interrupt priority of SysTick
	// Make SysTick least urgent (i.e. highest priority number)
	// __NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
	//NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	NVIC_SetPriority(SysTick_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);					// Enable EXTI0_1 interrupt in NVIC

	
	// Reset the SysTick counter value
	SysTick->VAL = 0;
	
	// Select processor clock
	// 1 = processor clock; 0 = external clock
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	//SysTick->CTRL = 0;
	// Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does no assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	/////////////////////////////////////////////////////////////////////////////
	
	// BUTTON INIT
	
	/////////////////////////////////////////////////////////////////////////////
	
	
	
	/* Enable GPIOs clock */ 	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOAEN;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// IR INPUT BUTTON = PA0
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U<<(0));  
	GPIOA->MODER |= 0U<<(0);      //  input(00)
	
//	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
//	GPIOA->OSPEEDR &= ~(3U<<(0));
//	GPIOA->OSPEEDR |=   3U<<(0);  // High speed
//	
		GPIOA->IDR &= ~1U;
//	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
//	GPIOA->OTYPER &= ~(1U<<2);       // Push-pull
//	
//	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
//	GPIOA->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down


	///////////////////////////////////////////////////////////////////////////////////////////////
	// JOYSTICK OUTPUT BUTTON = PA1 (tail)
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U<<(2*1));  
	GPIOA->MODER |= 1U<<(2*1);      //  output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	 GPIOA->OSPEEDR &= ~(3U<<(2*1));
	 GPIOA->OSPEEDR |=   3U<<(2*1);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
 	 GPIOA->OTYPER &= ~(1U<<1);       // Push-pull
	
	 //GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	 GPIOA->PUPDR   &= ~(3U<<(2*1));  // No pull-up, no pull-down
	 
	 // JOYSTICK OUTPUT BUTTON = PA2 (IN3 = Right Motor)
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U<<(2*2));  
	GPIOA->MODER |= 1U<<(2*2);      //  output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	 GPIOA->OSPEEDR &= ~(3U<<(2*2));
	 GPIOA->OSPEEDR |=   3U<<(2*2);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
 	 GPIOA->OTYPER &= ~(1U<<2);       // Push-pull
	
	 //GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	 GPIOA->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
	 
	 // JOYSTICK OUTPUT BUTTON = PA3 (IN4 = Right Motor)
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA->MODER &= ~(3U<<(2*3));  
	GPIOA->MODER |= 1U<<(2*3);      //  output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	 GPIOA->OSPEEDR &= ~(3U<<(2*3));
	 GPIOA->OSPEEDR |=   3U<<(2*3);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
 	 GPIOA->OTYPER &= ~(1U<<3);       // Push-pull
	
	 //GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	 GPIOA->PUPDR   &= ~(3U<<(2*3));  // No pull-up, no pull-down
}

// SysTick interrupt service routine
void SysTick_Handler(void) {
	elapsed++;
}

// nTime: specifies the delay time length
void delay(uint32_t nTime) {
	TimeDelay = nTime;
	while(TimeDelay != 0);
}

int time() {
	return elapsed;
}
