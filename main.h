#pragma once
#include "configs.h"

#define BTNCN ((MDR_PORTC->RXTX & 0x04) == false) // PC2 -> Button: S (SELECT / CENTER)
#define BTNUP ((MDR_PORTB->RXTX & 0x20) == false) // PB5 -> Button: U (UP)
#define BTNDN ((MDR_PORTE->RXTX & 0x02) == false) // PE1 -> Button: D (DOWN)
#define BTNLF ((MDR_PORTE->RXTX & 0x08) == false) // PE3 -> Button: L (LEFT)
#define BTNRT ((MDR_PORTB->RXTX & 0x40) == false) // PB6 -> Button: R (RIGHT)

/* MDR_SSP->SR FLAGS */
#define SSP_FLAG_BSY  			((uint16_t)0x0010)
#define SSP_FLAG_RFF        ((uint16_t)0x0008)
#define SSP_FLAG_RNE        ((uint16_t)0x0004)
#define SSP_FLAG_TNF        ((uint16_t)0x0002)
#define SSP_FLAG_TFE        ((uint16_t)0x0001)

enum Mode
{
	STATE_LOGO = 0,
	STATE_ADC = 1,
	STATE_DAC = 2,
	STATE_SPI = 3,
	STATE_INFO = 4
};

