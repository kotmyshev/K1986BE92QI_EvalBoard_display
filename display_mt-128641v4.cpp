
#include "display_mt-128641v4.h"


void simpleDelay(unsigned int delayTime)
{
	unsigned int i; // very important value :-)
	for (i = 0; i < delayTime; i++)
		;
}

void DisplaySend(unsigned char sdata) // Display Write Function
{

	MDR_PORTA->RXTX |= (sdata & 0x3F);		  // bitwise AND with mask 00111111 -> write to PA5...PA0
	MDR_PORTF->RXTX |= ((sdata & 0xC0) >> 4); // bitwise AND with mask 11000000, shift right at 4 -> write to PF3, PF2
	simpleDelay(100);
	MDR_PORTC->RXTX |= 0x2; // Generation STROB signal (set bit PC1)
	simpleDelay(10);
	MDR_PORTC->RXTX &= ~0x2; // Reset bit PC1
	simpleDelay(400);

	MDR_PORTA->RXTX &= ~0x3F; // Reset Data Bits (PA5...PA0)
	MDR_PORTF->RXTX &= ~0xC;  // Reset Data Bits (PF3, PF2)
}

void DisplaySet_L(void)
{
	MDR_PORTB->RXTX &= ~0x100; // LCD E2=0 reset PB8
	MDR_PORTB->RXTX |= 0x80;   // LCD E1=1 set PB7
}

void DisplaySet_R(void)
{
	MDR_PORTB->RXTX &= ~0x80; // LCD E2=0 reset PB7
	MDR_PORTB->RXTX |= 0x100; // LCD E1=1 set PB8
}

void DisplayInit(void)
{

	MDR_PORTB->RXTX &= ~0x400; // LCD RW=0 -> write state (reset PB10)
	MDR_PORTB->RXTX &= ~0x200; // LCD RES=0 (reset PB9)
	simpleDelay(100);
	MDR_PORTB->RXTX |= 0x200; // LCD RES=1  (set PB9)
	simpleDelay(1000);

	COMAND;
	DisplaySet_L();
	DisplaySend(0x3F); // COMAND Display ON (LEFT SIDE)
	simpleDelay(100);
	DisplaySet_R();
	DisplaySend(0x3F); // COMAND Display ON (RIGHT SIDE)
	simpleDelay(100);
}

void DisplaySetPosition(const char &page, const char &address)
{
	COMAND;
	DisplaySend(0xB8 + page);
	DisplaySend(0x40 + address);
	WRDATA;
}

void DisplayWriteSymbol(unsigned char symbol)
{
	unsigned char font_index = (symbol > 0x20) ? (symbol - 0x20) : 0x20;
	for (unsigned char i = 0; i < 7; i++)
	{
		DisplaySend(Font_OCR_A[font_index * 7 + i]);
	}
}

void DisplayPrintLn(const char lineNumber, const char lineString[])
{

	DisplaySetPosition(lineNumber, 0);

	const char *strPtr;
	strPtr = lineString;

	int i = 0;
	while (strPtr[i] != '\0')
	{
		DisplayWriteSymbol(strPtr[i]);
		i++;
	}
}

void DisplayPrintInt(const char lineNumber, const char offset, const char interval, int number)
{
	char newString[11];
	char *strPtr = newString;
	itoa(number, strPtr);
	DisplayPrintLine(lineNumber, offset, interval, strPtr);
}

void DisplayPrintDouble(const char lineNumber, const char offset, const char interval, const char tolerance, double number)
{
	char newString[11];
	char *strPtr = newString;
	DoubleToStr(number, tolerance ,strPtr);
	DisplayPrintLine(lineNumber, offset, interval, strPtr);
}


void DisplayPrintHexU16(const char lineNumber, const char offset, const char interval, const unsigned int number){
	char newHex[5];
	char *strPtr = newHex;
	u16toHexStr (number, strPtr);
	DisplayPrintLine(lineNumber, offset, interval, strPtr);	
}

void DisplayPrintHexU32(const char lineNumber, const char offset, const char interval, const unsigned int number)
{
	char newHex[10];
	char *strPtr = newHex;
	u32toHexStr (number, strPtr);
	DisplayPrintLine(lineNumber, offset, interval, strPtr);
}

void DisplayPrintLine(const char lineNumber, const char offset, const char interval, const char lineString[])
{
	DisplaySet_L();
	unsigned char p = 0;
	DisplaySetPosition(lineNumber, offset);
	unsigned char x = offset;
	unsigned char v = (interval > 2) ? 2 : interval;

	const char *strPtr;
	strPtr = lineString;

	unsigned char npos = 0;
	while (strPtr[npos] != '\0')
	{

		unsigned char font_index = (strPtr[npos] > 0x20) ? (strPtr[npos] - 0x20) : 0x00;

		for (unsigned int i = 0; i < (6 + v); i++)
		{
			if (x > 127)
				return;
			if (x > 63 & p == 0)
			{
				DisplaySet_R();
				DisplaySetPosition(lineNumber, (x - 64));
				p = 1;
			}

			DisplaySend(Font_OCR_A[font_index * 7 + i]);
			++x;
		}
		npos++;
	}
}

void DisplayDrawLogo(void)
{

	for (unsigned char y = 0; y < 8; ++y)
	{
		DisplaySet_L();
		DisplaySetPosition(y, 0);
		unsigned char p = 0;

		for (unsigned char x = 0; x < 128; ++x)
		{
			if (x > 63 & p == 0)
			{
				DisplaySet_R();
				DisplaySetPosition(y, 0);
				p = 1;
			}
			DisplaySend(Logo[y * 128 + x]);
		}
	}
}

void DisplayClear(void)
{

	for (unsigned int E = 0; E < 2; E++)
	{
		if (E == 0)
			DisplaySet_L();
		if (E == 1)
			DisplaySet_R();

		for (unsigned int p = 0; p < 8; p++)
		{
			COMAND;
			DisplaySend(0xB8 + p);
			DisplaySend(0x40);
			WRDATA;

			for (unsigned int i = 0; i < 64; i++)
			{
				DisplaySend(0x00);
			}
		}
	}
}
