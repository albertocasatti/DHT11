# DHT11
libreria para utilizar con arduino

El sensor DHT11 se utilizó junto a una Raspberry PI y unas librerías en python. este dió muy buen resultado por lo que se decidió utilizarla en conjunto a un arduino pro mini (328) de 8Mhz y 3.3V.

Luego de fracasar con varias librerías decidí implementar una propia para ver donde estaba fallando con arduino.


Paquete de datos:
DHT11 transmite un paquete de 40bit de los cuales los dos primeros bytes son para la humedad, los dos siguientes son para la temperatura y el ultimo byte es para un sensillo checksum.
el resultado de la suma de los primeros 4 bytes debe dar el mismo valor que el del byte de chk.

Protocolo: (supondremos digital 12 para los datos)
Arduino necesita el pin para la recepcion de datos en pullup.

DHT espera un low de 20ms para iniciar la transmision de datos

pinMode(12,OUTPUT);
digitalWrite(12,LOW);
delay(20;
pinMode(12,INPUT_PULLUP);

DHT envia un low de 80uS seguido de un high de 80uS
esto se utiliza para ajustar la base de tiempo

luego del low contamos cuanto dura el high

el tiempo que dura el pulso en high se divide por dos para utilizarlo como umbral de decision entre un uno y un cero

uint64_t buff = 0;
uint8_t to = 0;
uint8_t h;
int i;

to = 0;
while (digitalRead(12) == 1 && to < 255) to++; // medir ancho de pulso
if (to == 255)	goto TIMEOUT;

h = to/2;

los siguientes pulsos son los bits del paquete de datos.
pulsos menores a h/2 se toman como cero
pulsos mayores a h/2 se toman como uno


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


comprobar chksum
i = (buff >> 33) & 0xFF;
i += (buff >> 25) & 0xFF;
i += (buff >> 17) & 0xFF;
i += (buff >> 9) & 0xFF;

if (i != ((buff >> 1) & 0xFF)) { // error de chk

	sei();
	return 3; // error de chksum
}


ahora tenemos en buff todos los bits enviados por el DHT. para darle forma bastará hacer uno desplazamientos

temp_current_h = buff >> 33;
temp_current_t = buff >> 17;

sei();

return 1; // paquete recibido correctamente






