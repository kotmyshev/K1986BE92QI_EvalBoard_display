/**
******************************
* K1986BE92 Board SPI Test 2022
* Date: 2022 July 28
*
******************************
*/

#include "main.h"

bool holdbuttons = false;
unsigned short adcresult = 0x0000;
unsigned short etsresult = 0x0000;
unsigned short dacoutput = 0x0000;
Mode mode;

void StateDacAction(void);
void StateAdcAction(void);
void StateProcessSPIRunAction(void);
void ResultAdcReading(void);

extern "C"
{
	void SysTick_Handler(void)
	{
		SysTick->CTRL &= ~((unsigned int)0x00000001);
		holdbuttons = false;
	}

	void Timer2_IRQHandler(void)
	{
		MDR_TIMER2->STATUS &= ~0x2; // Clear "CNT ARR EVENT" bit
		ResultAdcReading();
	}
}

void ButtonLeftAction(void)
{
	if (mode == STATE_ADC)
		return;
	mode = STATE_ADC;
	DisplayClear();
	DisplayPrintLine(0, 0, 0, "K1986BE92QI ADC TEST");

	// DisplayPrintHexU32(8, 40, 0, SCB->CPUID);
}

void ButtonRightAction(void)
{
	if (mode == STATE_SPI)
		return;
	mode = STATE_SPI;
	DisplayClear();
	DisplayPrintLine(0, 0, 0, "K1986BE92QI SPI SLAVE");
	DisplayPrintLine(2, 0, 0, "RX (MOSI):");
	DisplayPrintLine(3, 0, 0, "DAC:");
	DisplayPrintLine(5, 0, 0, "ADC2:");
	DisplayPrintLine(6, 0, 0, "DATA:");
}

void ButtonCentrAction(void)
{
	if (mode == STATE_DAC)
		return;
	mode = STATE_DAC;
	DisplayClear();
	DisplayPrintLine(0, 0, 0, "K1986BE92QI DAC TEST");
	DisplayPrintLine(2, 0, 2, " PRESS UP/DOWN");
}

void ButtonsHold(void)
{
	holdbuttons = true;
	SysTick->CTRL |= ((unsigned int)0x00000001);
}

void ButtonsPolling(void)
{
	if (holdbuttons)
		return;

	if (BTNUP)
	{
		if (mode == STATE_DAC)
		{
			dacoutput = (dacoutput >= (0xFFF - 0x10)) ? dacoutput = 0xFFF : dacoutput + 0x10;
		}
		ButtonsHold();
		return;
	}

	if (BTNDN)
	{
		if (mode == STATE_DAC)
		{
			dacoutput = (dacoutput <= 0x10) ? dacoutput = 0x00 : dacoutput - 0x10;
		}
		ButtonsHold();
		return;
	}

	if (BTNCN)
	{
		ButtonCentrAction();
		ButtonsHold();
		return;
	}

	if (BTNLF)
	{
		ButtonLeftAction();
		ButtonsHold();
		return;
	}

	if (BTNRT)
	{
		ButtonRightAction();
		ButtonsHold();
		return;
	}
}

void StateDacAction(void)
{

	double voltage = dacoutput * 3.3 / 4096;

	DisplayPrintLine(4, 0, 0, "DAC2 OUT:");
	DisplayPrintHexU16(4, 60, 0, dacoutput);
	DisplayPrintLine(6, 0, 0, " VOLTAGE:");
	DisplayPrintDouble(6, 60, 0, 3, voltage);
	DisplayPrintLine(6, 95, 0, "V");

	MDR_DAC->DAC2_DATA = (dacoutput & 0xFFF);
}

void StateAdcAction(void)
{

	DisplayPrintLine(2, 0, 0, "ADC TEMP:");
	DisplayPrintHexU16(2, 60, 0, etsresult);
	DisplayPrintLine(3, 0, 0, " VOLTAGE:");
	DisplayPrintDouble(3, 60, 0, 3, (etsresult * 3.3) / 4096);
	DisplayPrintLine(3, 95, 0, "V");
	DisplayPrintLine(5, 0, 0, "ADC2 PD7:");
	DisplayPrintHexU16(5, 60, 0, adcresult);
	DisplayPrintLine(6, 0, 0, " VOLTAGE:");
	DisplayPrintDouble(6, 60, 0, 3, (adcresult * 3.3) / 4096);
	DisplayPrintLine(6, 95, 0, "V");
}

unsigned short SPI2WriteRead(unsigned short data)
{

	/* Wait for SPI2 Tx buffer empty */
	while (!(MDR_SSP2->SR & SSP_FLAG_TFE))
	{
	}
	/* Send SPI2 data */
	MDR_SSP2->DR = data;
	/* Wait for SPI2 data reception */
	while (!(MDR_SSP2->SR & SSP_FLAG_RNE))
	{
	}
	/* Read SPI2 received data */
	return MDR_SSP2->DR;
}

void StateProcessSPIRunAction(void)
{
	DisplayPrintLine(6, 0, 0, "DATA:");
	DisplayPrintHexU16(6, 35, 0, adcresult);
	DisplayPrintDouble(6, 80, 0, 3, (adcresult * 3.3 / 4096));
	DisplayPrintLine(6, 115, 0, "V");

	if (MDR_SSP2->SR & SSP_FLAG_BSY)
		return;

	if (MDR_SSP2->SR & SSP_FLAG_TFE)
	{
		MDR_SSP2->DR = adcresult;
	}

	if ((MDR_SSP2->SR & SSP_FLAG_RNE))
	{
		unsigned short rxdata = MDR_SSP2->DR;

		DisplayPrintLine(3, 0, 0, "DATA:");
		DisplayPrintHexU16(3, 35, 0, rxdata);
		dacoutput = (rxdata & 0xFFF);
		MDR_DAC->DAC2_DATA = dacoutput;
		DisplayPrintDouble(3, 80, 0, 3, (dacoutput * 3.3 / 4096));
		DisplayPrintLine(3, 115, 0, "V");
	}
}

void ResultAdcReading(void)
{

	if (MDR_ADC->ADC2_STATUS & 0x04)
	{
		adcresult = (MDR_ADC->ADC2_RESULT & 0xFFF);
		MDR_ADC->ADC2_CFG |= 0x02;
	}

	if (MDR_ADC->ADC1_STATUS & 0x04)
	{
		etsresult = (MDR_ADC->ADC1_RESULT & 0xFFF);
		MDR_ADC->ADC1_CFG |= 0x02;
	}
}

int main(void)
{

	CPUClockConfig();
	PeriphConfig();
	DisplayInit();
	DisplayDrawLogo();

	MDR_TIMER2->CNTRL |= 0x1; // Timer GO

	while (1)
	{

		/* Infinity loop */

		ButtonsPolling();

		switch (mode)
		{

		case STATE_ADC:
			StateAdcAction();
			break;

		case STATE_DAC:
			StateDacAction();
			break;

		case STATE_SPI:
			StateProcessSPIRunAction();
			break;

		default:
			break;
		}
	}
}
