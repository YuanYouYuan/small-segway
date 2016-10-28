const int motor_A_enable  = 5;
const int motor_B_enable  = 6;
const int motor_A_output1 = 7;
const int motor_A_output2 = 8;
const int motor_B_output1 = 9;
const int motor_B_output2 = 10;
int speed = 250;

void setup()
{
	speed = 0;
	pinMode(motor_A_enable , OUTPUT);
	pinMode(motor_B_enable , OUTPUT);
	pinMode(motor_A_output1, OUTPUT);
	pinMode(motor_A_output2, OUTPUT);
	pinMode(motor_B_output1, OUTPUT);
	pinMode(motor_B_output2, OUTPUT);
}

void loop()
{
	analogWrite (motor_A_enable , speed);
	analogWrite (motor_B_enable , speed);
	digitalWrite(motor_A_output1, HIGH );
	digitalWrite(motor_A_output2, LOW  );
	digitalWrite(motor_B_output1, HIGH );
	digitalWrite(motor_B_output2, LOW  );
}
