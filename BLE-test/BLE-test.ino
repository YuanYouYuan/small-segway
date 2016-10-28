#include "CurieBLE.h"

const char UUID[] = "19B10010-E8F2-537E-4F6C-D104768A1214"; 
BLEPeripheral BLE_per;
BLEService LED_serv(UUID);
BLEUnsignedIntCharacteristic switch_char(UUID, BLERead | BLEWrite);

const int LED_pin = 13;

void setup()
{
	Serial.begin(57600);
	pinMode(LED_pin, OUTPUT);
	BLE_per.setLocalName("LED service");
	BLE_per.setAdvertisedServiceUuid(LED_serv.uuid());
	BLE_per.addAttribute(LED_serv);
	BLE_per.addAttribute(switch_char);
	switch_char.setValue(0);
	BLE_per.begin();
	Serial.println("BLE LED service.");
}

void loop()
{
	BLECentral cen = BLE_per.central();
	if(cen)
	{
		Serial.print("Connected to central: ");
		Serial.println(cen.address());
		while(cen.connected())
			if(switch_char.written())
			{
				Serial.println(switch_char.value());
				int light = map(switch_char.value(), 10, 49, 0, 255);
				analogWrite(LED_pin, light);
				Serial.println(light);
			}
		Serial.print(F("Disconnected from cental: "));
		Serial.println(cen.address());
	}
}



