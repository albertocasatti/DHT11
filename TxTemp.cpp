#include "TxTemp.h"


uint8_t temp_current_t = 0;
uint8_t temp_current_h = 0;


void temp_Init(void) {

	

}


// para evitar quedar en loops infinitos, se agrega el salto por timeout

uint8_t temp_Read(void) {

	uint64_t buff = 0;
	uint8_t to = 0;
	uint8_t h;
	int i;
	

	pinMode(12, OUTPUT); 			// enviar señal de inicializacion para transmision de datos
	digitalWrite(12, 0);
	delay(20);						// aguardar por lo menos 18ms segun hoja de datos
	pinMode(12, INPUT_PULLUP);		// pasar a input con pullup

		
	cli(); // sin interrupciones

	to = 0;
	while (digitalRead(12) == 1 && to < 255) to++; // esperar primer pulso de sincro (80uS LOW)	
	if (to == 255) goto TIMEOUT; 

		
	to = 0;
	while (digitalRead(12) == 0 && to < 255) to++;  // esperar que termine el pulso LOW de 80uS
	if (to == 255) goto TIMEOUT;


	to = 0;
	while (digitalRead(12) == 1 && to < 255) to++; // medir ancho de pulso HIGH de 80uS
	if (to == 255)	goto TIMEOUT;

	// dividir por 2 el ancho del pulso
	// menor a h/2 se considera un cero
	// mayor a h/2 se considera un uno
	h = to/2; 
	

	i = 0;
	while (i < 40) {

		// esperar a que se pase a HIGH
		to = 0;
		while (digitalRead(12) == 0 && to < 255) to++;
		if (to == 255) goto TIMEOUT;

		// cronometrar HIGH
		to = 0;
		while (digitalRead(12) == 1 && to < 255) to++; // medir ancho de pulso
		if (to == 255)	goto TIMEOUT;

		// asignar y desplazar bit
		if (to >= h)	buff |= 1;
		buff <<= 1;
		i++;

	}
	
		
	// hacer suma para control de chksum
	i = (buff >> 33) & 0xFF;
	i += (buff >> 25) & 0xFF;
	i += (buff >> 17) & 0xFF;
	i += (buff >> 9) & 0xFF;

	if (i != ((buff >> 1) & 0xFF)) { // error de chk
		sei();
		return 3;
	}
	

	// asignar valores de temperatura y humedad en variables globales
	temp_current_h = buff >> 33;
	temp_current_t = buff >> 17;

	
	// activar interrupciones y devolver OK
	sei();
	return 1;



	
TIMEOUT:

	// devolver error por timeout
	sei();
	return 2;

}


uint8_t temp_ReadT(void) {

	return temp_current_t;

}


uint8_t temp_ReadH(void) {

	
	return temp_current_h;

}


