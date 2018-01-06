#include "TxTemp.h"
#include "TxTools.h"


uint8_t temp_current_t = 0;
uint8_t temp_current_h = 0;


void temp_Init(void) {

	
	

}


uint8_t temp_Read(void) {

	uint64_t buff = 0;
	uint8_t to = 0;
	uint8_t h;
	int i;
	

	pinMode(12, OUTPUT);
	digitalWrite(12, 0);
	delay(20);
	pinMode(12, INPUT_PULLUP);

	cli();

	to = 0;
	while (digitalRead(12) == 1 && to < 255) to++; // esperar primer pulso de sincro	
	if (to == 255) goto TIMEOUT;

		
	to = 0;
	while (digitalRead(12) == 0 && to < 255) to++; 
	if (to == 255) goto TIMEOUT;


	to = 0;
	while (digitalRead(12) == 1 && to < 255) to++; // medir ancho de pulso
	if (to == 255)	goto TIMEOUT;

	h = to/2;
	

	i = 0;
	while (i < 40) {

		to = 0;
		while (digitalRead(12) == 0 && to < 255) to++;
		if (to == 255) goto TIMEOUT;

		to = 0;
		while (digitalRead(12) == 1 && to < 255) to++; // medir ancho de pulso
		if (to == 255)	goto TIMEOUT;

		if (to >= h)	buff |= 1;
		buff <<= 1;
		i++;

	}
	
		
	
	i = (buff >> 33) & 0xFF;
	i += (buff >> 25) & 0xFF;
	i += (buff >> 17) & 0xFF;
	i += (buff >> 9) & 0xFF;

	if (i != ((buff >> 1) & 0xFF)) { // error de chk

		sei();
		return 3;
	}
	

	temp_current_h = buff >> 33;
	temp_current_t = buff >> 17;

	

	sei();

	return 1;




	
TIMEOUT:

	sei();
	return 2;






}


uint8_t temp_ReadT(void) {

	return temp_current_t;

}


uint8_t temp_ReadH(void) {

	
	return temp_current_h;

}


