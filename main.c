#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTick.h"
#include "LED.h"
#include "UART.h"

#include <string.h>

void superDuperFunTime(int risingEdgeCount, int reading);
void Tail_On(void);
void Tail_Off(void);
void Tail_Toggle(void);
void IN3_On(void);
void IN3_Off(void);
void IN4_On(void);
void IN4_Off(void);

ZZZZZZZint main(void){
	
	//CONSTANTS
	int LEFT = 29;
	int UP = 93;
	int DOWN = 157;
	int RIGHT = 237;
	int OK = 221;
	int HOME = 75;
	int SIX = 151;
	int HULU = 96;
	
	//INIT
	System_Clock_Init(); // Switch System Clock = 4 MHz
	LED_Init();
	SysTick_Initialize(2000); //SysTick at 5kHz
	UART2_Init();
	
	int duty = 5;
	uint16_t input, prevInput = 0;
	int dutyCount, startCount, edgeCount, reading, waiting = 0;
	int wagging = 0;
	uint8_t button = 0;
	
		Red_LED_Off();
		Green_LED_Off();
	
	while (1){
		
		//get IR input
		input = !((GPIOA->IDR) & '1');
		
		///////////////////////
		//READ IR COMMANDS
		///////////////////////
		if(!input && !reading && !waiting) {
			startCount = time() + 45; //45 ticks = 9ms pulse
		}
		else if(time() > startCount && input && !waiting) {
			waiting = time() + 157;		//157 ticks = 31ms address, ignore
		}
		else if(waiting > time())
		{
			reading = time() + 157;		//31ms command to read
		}
		
		//READING LOOP
		else if(reading > time())
		{
			if(input) {
				if(edgeCount) {
					button = button<<1;      //read a 0
					if(time() > edgeCount) { //space > .8ms, read a 1 instead
						button +=1;
					}
					edgeCount = 0;
				}
			}
			else {
				if(edgeCount == 0)
					{
						edgeCount = time() + 4; //4 ticks = .8ms
					}
			}
		}
		else
		{
			reading = 0;
			waiting = 0;
		}
		

		//BUTTON STATUS TESTING
//		if(dutyCount == 99) {
//				Green_LED_Off();
//		}
//		else if(dutyCount > 90) {
//				Green_LED_On();
//		}
//		else if(dutyCount == 7) {
//			if(button & 1) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 6) {
//			if(button & 2) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 5) {
//			if(button & 4) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 4) {
//			if(button & 8) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 3) {
//			if(button & 16) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 2) {
//			if(button & 32) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 1) {
//			if(button & 64) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else if(dutyCount == 0) {
//			if(button & 128) {
//				Green_LED_On();
//			}
//			else {
//				Green_LED_Off();
//			}
//		}
//		else {
//			Green_LED_Off();
//		}
		
		
		//ACTIONS - determined by state of button
		if(button == HULU)
		{	
			wagging = 1;
			
			Green_LED_Off(); //Red is IN1 (Right)
			Red_LED_Off(); // Green is IN2 (Left)
			
			IN3_Off();
			IN4_Off();
		}
		else if(button == UP)
		{
			Red_LED_On(); // Red is IN1 (Left)
			Green_LED_Off(); //Green is IN2 (Left)
			
			IN3_On();
			IN4_Off();
		}
		else if(button == DOWN)
		{
			Red_LED_Off();  //Red is IN1 (Right)
			Green_LED_On(); // Green is IN2 (Left)

			IN3_Off();
			IN4_On();
		}
		else if(button == LEFT)
		{
			Green_LED_On(); //Red is IN1 (Right)
			Red_LED_Off(); // Green is IN2 (Left)
			
			IN3_On();
			IN4_Off();
		}
		else if(button == RIGHT)
		{
			Red_LED_On(); // Red is IN1 (Left)
			Green_LED_Off(); //Green is IN2 (Left)
			
			IN3_Off();
			IN4_On();
		}
		else //if(button == OK)
		{
			Green_LED_Off();
			Red_LED_Off();
			
			IN3_Off();
			IN4_Off();
			wagging = 0;
		}
		
		
		//DUTY CYCLES
		
		if(wagging)
		{
			if(time() % 1500 == 0) {
					duty = 5;
				}
			if(time() % 3000 == 0) {
					duty = 12;
			}
		}
		else {
			duty = 8;
		}
			
		if(dutyCount <= duty) {
			Tail_On();
		}
		else {
			Tail_Off();
		}
		
		
		dutyCount = time() % 100;
		prevInput = input;
	}
}



//******************************************************************************************
// Turn PA5 On
//******************************************************************************************
void Tail_On(void){
	GPIOA->ODR |= GPIO_ODR_ODR_1;
}

//******************************************************************************************
// Turn PA5 Off
//******************************************************************************************
void Tail_Off(void){
	GPIOA->ODR &= ~GPIO_ODR_ODR_1;
}

//******************************************************************************************
// Toggle PA5
//******************************************************************************************
void Tail_Toggle(void){
	GPIOA->ODR ^= GPIO_ODR_ODR_1;
}

void IN3_On()
{
	GPIOA->ODR |= GPIO_ODR_ODR_2;
}

void IN3_Off()
{
		GPIOA->ODR &= ~GPIO_ODR_ODR_2;
}

void IN4_On()
{
	GPIOA->ODR |= GPIO_ODR_ODR_3;
}

void IN4_Off()
{
			GPIOA->ODR &= ~GPIO_ODR_ODR_3;
}



// ===== NOTES ON IR SENSING ==============================================================
// initialize variable: command bit counter = 0
// while bit counter <8
// 		check for falling edge
//		1 ms can be threshold, where if signal is still low at this value, it is
// ========================================================================================

