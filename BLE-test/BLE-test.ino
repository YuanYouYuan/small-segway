#include "CurieBLE.h"

BLEPeripheral BLE_per;
BLEService LED_serv("19B10010-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedIntCharacteristic switch_char("19B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLErite);

const int LED_pin = 13;

void setup()
{
	Serial.begin(57600);
	pinMode(LED_pin, OUTPUT);
	BLE_per.setLocalName("LED service");
	
}



