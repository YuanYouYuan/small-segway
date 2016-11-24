const int M1 = 4;
const int E1 = 5;
const int E2 = 6;
const int M2 = 7;
int d = 1;
void setup()
{
	pinMode(M1, OUTPUT);
	pinMode(M2, OUTPUT);
}

void loop()
{
	d = 1 - d;
	for(int i = 0; i <= 255; i+=5)
	{
		digitalWrite(M1, d);
		digitalWrite(M2, d);
		analogWrite(E1, i);
		analogWrite(E2, i);
		delay(30);
	}
}

