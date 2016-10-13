const int motor_A_enable  = 5;
const int motor_B_enable  = 6;
const int motor_A_output1 = 7;
const int motor_A_output2 = 8;
const int motor_B_output1 = 9;
const int motor_B_output2 = 10;
int speed;

void setup()
{
	speed = 0;
	pinMode(motor_A_enable , OUTPUT);
	pinMode(motor_B_enable , OUTPUT);
	pinMode(motor_A_output1, OUTPUT);
	pinMode(motor_A_output2, OUTPUT);
	pinMode(motor_B_output1, OUTPUT);
	pinMode(motor_B_output2, OUTPUT);
	Serial.begin(115200);
	while(!Serial);
	Serial.println("Strat!!!");
	info();
}

void loop()
{
	if(Serial.available())
	{
		int key = Serial.read();
		if(key == 'w')
			go_forward();
		else if(key == 'x')
			go_backward();
		else if(key == 'a')
			turn_left();
		else if(key == 'd')
			turn_right();
		else if(key == 's')
			stop();
		else if(key == 'z')
		{
			speed += 5;
			if (speed > 255)
				speed = 255;
			analogWrite (motor_A_enable , speed);
			analogWrite (motor_B_enable , speed);
		}
		else if(key == 'c')
		{
			speed -= 5;
			if (speed < 0)
				speed = 0;
			analogWrite (motor_A_enable , speed);
			analogWrite (motor_B_enable , speed);
		}
		info();
	}

}

void info()
{
	Serial.println("Press w to go forward,");
	Serial.println("Press x to go backward,");
	Serial.println("Press a to turn left,");
	Serial.println("Press d to turn right,");
	Serial.println("Press s to stop,");
	Serial.println("Press z/c to increase/decrease the speed,");
	Serial.print("Now speed: ");
	Serial.println(speed);
	Serial.println();
}

void go_forward()
{
	analogWrite (motor_A_enable , speed);
	analogWrite (motor_B_enable , speed);
	digitalWrite(motor_A_output1, HIGH );
	digitalWrite(motor_A_output2, LOW  );
	digitalWrite(motor_B_output1, HIGH );
	digitalWrite(motor_B_output2, LOW  );
}

void go_backward()
{
	analogWrite (motor_A_enable , speed);
	analogWrite (motor_B_enable , speed);
	digitalWrite(motor_A_output1, LOW  );
	digitalWrite(motor_A_output2, HIGH );
	digitalWrite(motor_B_output1, LOW  );
	digitalWrite(motor_B_output2, HIGH );
}

void turn_left()
{
	analogWrite (motor_A_enable , speed);
	analogWrite (motor_B_enable , speed);
	digitalWrite(motor_A_output1, LOW  );
	digitalWrite(motor_A_output2, HIGH );
	digitalWrite(motor_B_output1, HIGH );
	digitalWrite(motor_B_output2, LOW  );
}

void turn_right()
{
	analogWrite (motor_A_enable , speed);
	analogWrite (motor_B_enable , speed);
	digitalWrite(motor_A_output1, HIGH );
	digitalWrite(motor_A_output2, LOW  );
	digitalWrite(motor_B_output1, LOW  );
	digitalWrite(motor_B_output2, HIGH );
}

void stop()
{
	digitalWrite(motor_A_output1, LOW  );
	digitalWrite(motor_A_output2, LOW  );
	digitalWrite(motor_B_output1, LOW  );
	digitalWrite(motor_B_output2, LOW  );
}
