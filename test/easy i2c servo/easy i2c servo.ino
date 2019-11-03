// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
	Name:       easy i2c servo.ino
	Created:	01-Nov-2019 14:19:39
	Author:     CLUSTER-HCI\clust
*/

#define PIN_PWMA  9
#define PIN_DIRA  8
#define PIN_PWMB  6
#define PIN_DIRB  7
#define I2C_DEVICE_ADDR 0x40
#define TIME_ACTION_INTERVAL  10

#include <Wire.h>
#include <math.h>

// the setup function runs once when you press reset or power the board
void setup()
{

#pragma region init

	Wire.begin();
	Serial.begin(115200);

#pragma endregion

	Serial.println("[Initialize Servo]");

	// sleep device
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(0x00);
	Wire.write(0x10); // 0x10
	Wire.endTransmission();

	// set pre-scaler
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(0xfe);
	Wire.write(121);
	Wire.endTransmission();
	/*
	# counter max = 4095
	# T = 20 ms, 4095
	# t = 1 ms, 204.7 ~ 205
	# t = 1.5 ms, 307.125 ~ 307
	# t = 2 ms, 409.5 ~ 410
	*/

	// reset device
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(0x00);
	Wire.write(0x80);
	Wire.endTransmission();

	Serial.println("[START]");

}

// the loop function runs over and over again until power down or reset
void loop()
{

	Serial.println("0");
	MySetPosition(-1, 0);
	delay(1000);

	Serial.println("180");
	MySetPosition(-1, 180);
	delay(1000);

}

/*
	MySetPosition
		ch			หมายเลข channel (0 ถึง 15) ที่ต้องการควบคุมการทำงาน
		position	ตำแหน่งเป้าหมายของ Servo มีค่าเท่ากับ 0 ถึง 180 หน่วยเป็น องศา
*/
bool MySetPosition(int ch, int position)
{

	const int	TIME_MIN = 150;	// Min Counter for 1.0 ms => 0 degree
	const int	TIME_MAX = 600;	// Max Counter for 2.0 ms => 180 degree

	// calculate ch
	if (ch < 0 || ch > 15)
		ch = 0xfa;
	else
		ch = 6 + (ch * 4);

	// calculate position
	/*
		(TIME_MAX - TIME_MIN) / 180 = ? / position
	*/

	/*Serial.print("ch=");
	Serial.println(ch);
	Serial.print("position=");
	Serial.println(position);*/

	double tmp = (double)(TIME_MAX - TIME_MIN) / 180.0;
	tmp *= (double)position;
	tmp += TIME_MIN;
	int result = round(tmp);
/*
	Serial.print("tmp=");
	Serial.println(tmp);
*/
	// set position
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(ch);
	Wire.write(0);
	Wire.endTransmission();
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(ch + 1);
	Wire.write(0);
	Wire.endTransmission();
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(ch + 2);
	Wire.write(result & 0xff);
	Wire.endTransmission();
	Wire.beginTransmission(I2C_DEVICE_ADDR);
	Wire.write(ch + 3);
	Wire.write((result & 0xf00) >> 8);
	Wire.endTransmission();

}