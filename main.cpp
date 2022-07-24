/**
******************************
* K1986BE92 Board SPI Test 2022
* Date: 2022 July 20
* 
******************************
*/

#include "configs.h"

	#define BTNCN 		((MDR_PORTC->RXTX & 0x04) == false)	// PC2 -> Button: S (SELECT / CENTER)
	#define BTNUP 		((MDR_PORTB->RXTX & 0x20) == false)	// PB5 -> Button: U (UP)	
	#define BTNDN 		((MDR_PORTE->RXTX & 0x02) == false)	// PE1 -> Button: D (DOWN)
	#define BTNLF 		((MDR_PORTE->RXTX & 0x08) == false)	// PE3 -> Button: L (LEFT)		
	#define BTNRT 		((MDR_PORTB->RXTX & 0x40) == false)	// PB6 -> Button: R (RIGHT)	


	
	enum Mode {
	STATE_LOGO = 0,
	STATE_INFO = 1,
	STATE_PROCESS_RUN = 2,
	STATE_MANUAL_MODE = 3
};
	
	bool holdbuttons = false;
	Mode mode;
	
extern "C"{
void SysTick_Handler(void){		
		SysTick->CTRL &= ~((unsigned int)0x00000001);
		holdbuttons = false;
	}
}

void ButtonLeftAction (void){
if (mode == STATE_INFO)	return;
mode = STATE_INFO;	
	DisplayClear();
	DisplayPrintLine(0,0,0,"K1986BE92QI MILANDR");
	DisplayPrintLine(2,0,0,"CORE: ARM Cortex-M3");
	DisplayPrintLine(4,0,0,"FREQ:");
	DisplayPrintInt(4,40,0, SystemCoreClock);
	DisplayPrintLine(4,100,0,"Hz");
	DisplayPrintLine(6,0,0,"CPUID:");
	DisplayPrintHexU32(6,40,0, SCB->CPUID);	
}

void ButtonRightAction (void){
if (mode == STATE_PROCESS_RUN)	return;
mode = STATE_PROCESS_RUN;	
	DisplayClear();
	DisplayPrintLine(0,0,0,"SPI SLAVE DEMO MODE");
	DisplayPrintLine(1,0,0,"DATE 2022/07/22 V01");
	

}


void ButtonsHold(void){
	holdbuttons = true;
	SysTick->CTRL |= ((unsigned int)0x00000001);
}
	
void ButtonsPolling (void){
	if (holdbuttons)
		return;
	
		if (BTNUP) {
			ButtonsHold();
			//ButtonUpAction();	
				return;
			}

		if (BTNDN) {
			ButtonsHold();
				MDR_PORTD->RXTX &= ~0x10;
			
				return;
			}	

			
		if (BTNCN) {
			//S//PI1_CSLow();
			//SPI1_Write(message);
			
			ButtonsHold();
			return;
		}	
			
		if (BTNLF) {			
			ButtonLeftAction();
			ButtonsHold();
			return;
		}	
		
		if (BTNRT) {			
			ButtonRightAction();
			ButtonsHold();
			return;
		}	
}
	
	
int main (void)
{
	CPUClockConfig();	
	PeriphConfig();
	DisplayInit();
	//DisplayClear();	
  DisplayDrawLogo();
	
	while (1 	/* Infinity loop */	 )
		{
			ButtonsPolling();
			
			if (mode == STATE_PROCESS_RUN){
				MDR_SSP2->DR = 0x111F;
				if (!(MDR_SSP2->SR&4)==0) {
				DisplayPrintLine(4,0,0, "MDR-SSP2->DR");
					DisplayPrintHexU32(5,0,0, MDR_SSP2->DR);
					
				}
			}
			
			
	}
}

