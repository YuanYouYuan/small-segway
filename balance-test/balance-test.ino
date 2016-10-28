#include "CurieIMU.h"

float kp = 24;
float ki = 0.05;
float kd = 15;
const float K = 0.95;
const int angle_list_number = 5;
const int error_list_number = 10;
int speed = 0;
const int motor_A_1 = 3;
const int motor_A_2 = 5;
const int motor_B_1 = 6;
const int motor_B_2 = 9;
float time, time_pre, time_step;
float gyro_angle = 0;
float acce_angle = 0;
float angle_list[angle_list_number];
float pre_error = 0;
float error_list[error_list_number];
float diff_error = 0;
float offset = 0;
int adjust_timer;

void setup()
{
	for(int i = 0; i < angle_list_number; i++)
		angle_list[i] = 0.0;
	for(int i = 0; i < error_list_number; i++)
		error_list[i] = 0.0;
	pinMode(motor_A_1, OUTPUT);
	pinMode(motor_A_2, OUTPUT);
	pinMode(motor_B_1, OUTPUT);
	pinMode(motor_B_2, OUTPUT);
	pinMode(13, OUTPUT);

	Serial.begin(9600);
	Serial.println("Start!!!");
	CurieIMU.begin();
	CurieIMU.setAccelerometerRange(4);
	CurieIMU.setGyroRange(250);
	time = millis();
	for(int i = 0; i < 5; i++)
	{
		Serial.println("Ready...");
		delay(200);	
	}
	int time2 = millis();
	while((millis()-time2) < 2000)
		offset = get_angle();
	adjust_timer = millis();
	digitalWrite(13, HIGH);
}

void loop()
{
	//balance(PID_feedback(get_angle()));
	//adjusting();
	float error = get_angle();
	float feedback = PID_feedback(error);
	if(abs(error) > 70)
	{
		while(true)
		{
			analogWrite(motor_A_1, 0);
			digitalWrite(motor_A_2, LOW);
			analogWrite(motor_B_1, 0);
			digitalWrite(motor_B_2, LOW);
			Serial.println("Stop!!!");
		}
	}
	balance(feedback);
}

void balance(float feedback)
{
	speed = int(feedback);
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

float get_angle()
{
	time_pre = time;
	time = millis();
	time_step = (time - time_pre)/1000;

	float ax, ay, az;
	float gx, gy, gz;
	CurieIMU.readAccelerometerScaled(ax, ay, az);
	CurieIMU.readGyroScaled(gx, gy, gz);

	//Serial.print(ax);
	//Serial.print("\t");
	//Serial.print(ay);
	//Serial.print("\t");
	//Serial.print(az);
	//Serial.print("\t");
	//Serial.print(gx);
	//Serial.print("\t");
	//Serial.print(gy);
	//Serial.print("\t");
	//Serial.print(gz);
	//Serial.println();
	
	gyro_angle += gy*time_step;
	acce_angle = (180/3.141593) * atan(ax/az);
	for(int i = 0; i < angle_list_number-1; i++)
		angle_list[i] = angle_list[i+1];
	angle_list[angle_list_number-1] = K * acce_angle + (1-K) * gyro_angle;
	float mean_angle;
	mean_angle = 0.0;
	for(int i = 0; i < angle_list_number; i++)
		mean_angle += angle_list[i];
	mean_angle /= 5;
	mean_angle -= offset;
	return mean_angle;

}

float PID_feedback(float error)
{
	for(int i = 0; i < error_list_number-1; i++)
		error_list[i] = error_list[i+1];
	error_list[error_list_number-1] = error;

	float sum_error = 0;
	for(int i = 0; i < error_list_number; i++)
		sum_error += error_list[i];
	diff_error = error - pre_error;
	pre_error = error;
	float p_term = kp * error;
	float i_term = ki * sum_error;
	float d_term = kd * diff_error;
	float feedback = p_term + i_term + d_term;
	if(feedback >= 255)
		feedback = 255;
	else if(feedback <= -255)
		feedback = -255;
	// Serial.print("P_term: ");
	// Serial.print(p_term);
	// Serial.print("\tI_term: ");
	// Serial.print(i_term);
	// Serial.print("\tD_term: ");
	// Serial.print(d_term);
	// Serial.print("\tError: ");
	// Serial.print(error);
	// Serial.print("\tFeedback: ");
	// Serial.println(feedback);
	return feedback;
}

void adjusting()
{
	float mean_error = 0.0;
	for(int i = 0; i < error_list_number; i++)
		mean_error += error_list[i];
	mean_error /= error_list_number;
	float mean_diff_error = 0.0;
	for(int i = 0; i < error_list_number/2; i++)
		mean_diff_error += (error_list[i]-error_list[i+1]);
	if(abs(mean_error) > 15 && kp <= 14)
		kp += 0.2;
	if(abs(mean_diff_error) > 18 && kp >= 2)
		kp -= 0.2;

	Serial.print("mean_error: ");
	Serial.print(mean_error);
	Serial.print("\tmean_diff_error: ");
	Serial.print(mean_diff_error);
	Serial.print("\tkp: ");
	Serial.println(kp);

}
