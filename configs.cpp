#include "configs.h"

void SysTickConfig(void)
{
	SysTick->LOAD = 0x5FFFFF;
	SysTick->CTRL = 0x06;
}

void Timer2Config(void)
{
	MDR_RST_CLK->PER_CLOCK |= (1 << 15); // clock turn on: Timer2
	// MDR_RST_CLK->TIM_CLOCK |= 0x500; 		// TIM2 BRG 0x5 -> TIM2_CLK == HCLK/32
	MDR_RST_CLK->TIM_CLOCK |= (1 << 25); // TIM2_CLK Enabled, TIM2_CLK == HCLK

	MDR_TIMER2->CNTRL = 0x0; // Timer2 initizlization
	MDR_TIMER2->CNT = 0x1;	 // initial value of timer counter
	MDR_TIMER2->PSG = 65000; // Timer2 Prescaller
	MDR_TIMER2->ARR = 250;	 // Timer2 AutoReload Period

	MDR_TIMER2->IE = 0x2; // Timer2 Interrup Enable
	NVIC_EnableIRQ(Timer2_IRQn);
}

void Dac2Config(void)
{
	MDR_RST_CLK->PER_CLOCK |= (1 << 18); // clock turn on: DAC
	MDR_DAC->CFG |= 0x8;				 // DAC2 Enable, Async mode, REF = AUCC
	MDR_DAC->DAC2_DATA = 0x000;			 // DAC2 Data Out
}

void Adc2Config(void)
{
	MDR_RST_CLK->PER_CLOCK |= (1 << 17);  // clock turn on: ADC
	MDR_RST_CLK->ADC_MCO_CLOCK |= 0x2020; // ADC clock enable, ADC_CLK = CPU_C1;
	MDR_ADC->ADC1_CFG = 0xCE01F1;		  // ADC1 Config -> input chanel 31 (temp sensor) , ADC ON
	MDR_ADC->ADC2_CFG = 0x71;			  // ADC2 Config -> input chanel 7 , ADC ON
	MDR_ADC->ADC1_CHSEL = 0xFF;

	MDR_ADC->ADC2_CFG |= 0x02;
	MDR_ADC->ADC1_CFG |= 0x02;
}

void SSP2Config(void)
{
	MDR_RST_CLK->PER_CLOCK |= (1 << 20); // clock turn on: SSP/SPI2

	MDR_RST_CLK->SSP_CLOCK |= (1 << 25); // clock enable: SSP/SPI2

	MDR_SSP2->CPSR |= 0xC; // clk dev by 12
	MDR_SSP2->CR0 |= 0x0F; // word len = 16 bit, SPI Motorola, SPO=0, SPH=0, SCR=0
	// MDR_SSP2->CR1 |= 0xE;		// slave mode, SOD=1, SSP Enabled
	MDR_SSP2->CR1 |= 0x6; // slave mode, SOD=0, SSP Enabled
}

void PeriphConfig(void)
{

	MDR_RST_CLK->PER_CLOCK |= (1 << 21); // clock turn on: PORTA
	MDR_RST_CLK->PER_CLOCK |= (1 << 22); // clock turn on: PORTB
	MDR_RST_CLK->PER_CLOCK |= (1 << 23); // clock turn on: PORTC
	MDR_RST_CLK->PER_CLOCK |= (1 << 24); // clock turn on: PORTD
	MDR_RST_CLK->PER_CLOCK |= (1 << 25); // clock turn on: PORTE
	MDR_RST_CLK->PER_CLOCK |= (1 << 29); // clock turn on: PORTF

	MDR_PORTA->OE |= 0x3F;	   // PA0...PA5 -> Output mode
	MDR_PORTA->ANALOG |= 0x3F; // PA0...PA5 -> Digital mode
	MDR_PORTA->PULL |= 0x3F;   // PA0...PA5 -> Pull-down
	MDR_PORTA->PWR |= 0xFFF;   // PA0...PA5 -> High-speed mode

	MDR_PORTB->OE |= 0x780;		// PB7...PB10 -> Output mode
	MDR_PORTB->ANALOG |= 0x7E0; // PB5...PB10 -> Digital mode
	MDR_PORTB->PULL |= 0x780;	// PB7...PB10 -> Pull-down
	MDR_PORTB->PWR |= 0x3FC000; // PB7...PB10 -> High-speed mode

	MDR_PORTC->OE |= 0x3;	  // PC0, PC1 -> Output mode
	MDR_PORTC->ANALOG |= 0x7; // PC0, PC1, PC2 -> Digital mode
	MDR_PORTC->PULL |= 0x3;	  // PC0, PC1 -> Pull-down
	MDR_PORTC->PWR |= 0x3F;	  // PC0, PC1, PC2 -> High-speed mode

	MDR_PORTD->ANALOG |= 0x7F; // PD7 -> Analog Mode (ADC7)
	MDR_PORTD->OE |= 0x72;	   // PD5, PD6, PD4, PD1 -> Output
	MDR_PORTD->FUNC |= 0x28AA; // PD5, PD6, PD2, PD3 -> SSP2, PD0, PD1 -> UART2, PD4 -> port
	MDR_PORTD->PULL |= 0x0;	   // PD -> no Pull
	MDR_PORTD->PWR |= 0xFFFF;  // PD -> High-speed mode
	MDR_PORTD->PD |= 0x0;

	MDR_PORTE->OE |= 0x1;	  // PE0 -> Output mode
	MDR_PORTE->ANALOG |= 0xA; // PE0 -> Analogue mode| PE1,PE3 - Digital mode
	MDR_PORTE->PULL |= 0x0;	  // PE0 ->	no Pull
	MDR_PORTE->PWR |= 0xFF;	  // PE0 -> high-speed mode

	MDR_PORTF->OE |= 0xC;	  // PF3, PF2 -> Output mode
	MDR_PORTF->ANALOG |= 0xC; // PF3, PF2 -> Digital mode
	MDR_PORTF->PULL |= 0xC;	  // PF3, PF2 -> Pull-down
	MDR_PORTF->PWR |= 0xF0;	  // PF3, PF2 -> High-speed mode

	SysTickConfig();
	Dac2Config();
	Adc2Config();
	SSP2Config();
	Timer2Config();
}

void CPUClockConfig(void)
{
	// HCLK = PLLCPUo = HSE_Value*(3+1) = 32 MHz
	//  #define HSE_Value       ((uint32_t)8000000)

	MDR_RST_CLK->HS_CONTROL = ((uint32_t)0x00000001); // HSE ON
	while ((MDR_RST_CLK->CLOCK_STATUS & ((uint32_t)0x00000004)) != ((uint32_t)0x00000004))
		; // WAIT RST_CLK_CLOCK_STATUS_HSE_RDY

	MDR_RST_CLK->CPU_CLOCK |= ((uint32_t)0x00000002); // RST_CLK_CPU_CLOCK_CPU_C1_SEL_HSE

	MDR_RST_CLK->PLL_CONTROL |= ((uint32_t)0x00000300); // PLL_CPU_MULL = 3, PLLCPUo = HSE_Value*(3+1) = 32 MHz
	MDR_RST_CLK->PLL_CONTROL |= ((uint32_t)0x00000004); // PLL_CPU_ON -> PLL Enable

	while ((MDR_RST_CLK->CLOCK_STATUS & ((uint32_t)0x00000002)) != ((uint32_t)0x00000002))
		; // WAIT RST_CLK_CLOCK_STATUS_PLL_CPU_RDY

	MDR_RST_CLK->CPU_CLOCK |= ((uint32_t)0x00000004); // RST_CLK_CPU_CLOCK_CPU_C2_SEL_PLLCPUo

	MDR_RST_CLK->CPU_CLOCK |= ((uint32_t)0x00000100); // RST_CLK_CPU_CLOCK_HCLK_SEL_CPU_C3

	SystemCoreClockUpdate();
}
