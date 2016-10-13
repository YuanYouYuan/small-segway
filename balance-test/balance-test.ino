#include "CurieIMU.h"

float kp = 4;
float ki = 0.1;
float kd = 0;
const float K = 0.95;
const int angle_list_number = 5;
const int error_list_number = 10;
int speed = 0;
const int motor_A_enable  = 5;
const int motor_B_enable  = 6;
const int motor_A_output1 = 7;
const int motor_A_output2 = 8;
const int motor_B_output1 = 9;
const int motor_B_output2 = 10;
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
	pinMode(motor_A_enable , OUTPUT);
	pinMode(motor_B_enable , OUTPUT);
	pinMode(motor_A_output1, OUTPUT);
	pinMode(motor_A_output2, OUTPUT);
	pinMode(motor_B_output1, OUTPUT);
	pinMode(motor_B_output2, OUTPUT);

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
}

void loop()
{
	balance(PID_feedback(get_angle()));
	adjusting();
	Serial.println(speed);
	//balance(speed);
	//Serial.println(get_angle());
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
	//Serial.print("P term: ");
	//Serial.print(p_term);
	//Serial.print("\tI term: ");
	//Serial.print(i_term);
	//Serial.print("\tD term: ");
	//Serial.print(d_term);
	//Serial.print("\tError: ");
	//Serial.print(error);
	//Serial.print("\tFeedback: ");
	//Serial.println(feedback);
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

void balance(float feedback)
{
	speed = int(abs(feedback));
	if(feedback < 0)
		go_forward();
	else
		go_backward();
}
