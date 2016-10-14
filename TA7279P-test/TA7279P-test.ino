const int motor_A_1 = 3;
const int motor_A_2 = 5;
const int motor_B_1 = 6;
const int motor_B_2 = 9;
int speed = 0;

void setup()
{
	pinMode(motor_A_1, OUTPUT);
	pinMode(motor_A_2, OUTPUT);
	pinMode(motor_B_1, OUTPUT);
	pinMode(motor_B_2, OUTPUT);
}

void loop()
{
	while(speed <= 255)
	{
		speed += 5;
		move(speed);
		delay(200);
	}
	while(speed >= -255)
	{
		speed -= 5;
		move(speed);
		delay(200);
	}
}

void move(int speed)
{
	if(speed < 0)
	{
		analogWrite(motor_A_1, abs(speed));
		analogWrite(motor_B_1, abs(speed));
		digitalWrite(motor_A_2, LOW);
		digitalWrite(motor_B_2, LOW);
	}
	else
	{
		digitalWrite(motor_A_1, LOW);
		digitalWrite(motor_B_1, LOW);
		analogWrite(motor_A_2, abs(speed));
		analogWrite(motor_B_2, abs(speed));
	}
}
