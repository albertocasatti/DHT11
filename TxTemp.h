// TxTemp.h

#ifndef _TXTEMP_h
#define _TXTEMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define PIN_TH		12

void temp_Init(void);
uint8_t temp_Read(void);
uint8_t temp_ReadT(void);
uint8_t temp_ReadH(void);


#endif

