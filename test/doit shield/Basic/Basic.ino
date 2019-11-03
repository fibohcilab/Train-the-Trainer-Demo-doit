/*
 Name:    Basic.ino
 Created: 22-06-2018 02:14:54
 Author:  cluster

 # Arduino
 ## i2c
  - https://www.arduino.cc/en/reference/wire
    - A4 (SDA), A5 (SCL)
  - https://www.arduino.cc/en/Tutorial/MasterReader

# DOIT
## PIN
  - PWMB = D6
  - DIRB = D7
  - PWMA = D8
  - DIRA = D9

## I2C
  - PCA9685
    - https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf

*/

#include <Wire.h>

#define PIN_PWMA  9
#define PIN_DIRA  8
#define PIN_PWMB  6
#define PIN_DIRB  7
#define I2C_DEVICE_ADDR 0x40
#define TIME_ACTION_INTERVAL  10

unsigned long TIME_ACTION_LAST;

#pragma region sweep pwm

int PWMA_DIR = 1;
int PWMA = 0;
int DIRA = 1;

#pragma endregion


// the setup function runs once when you press reset or power the board
void setup()
{

#pragma region init

  pinMode(PIN_PWMA, OUTPUT);  // PWMA
  pinMode(PIN_DIRA, OUTPUT);  // DIRA
  //pinMode(PIN_PWMB, OUTPUT);  // PWMB
  //pinMode(PIN_DIRB, OUTPUT);  // DIRB

  Wire.begin();
  //Wire.setClock(60000); // 60 KHz
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

  TIME_ACTION_LAST = millis();

}

// the loop function runs over and over again until power down or reset
void loop()
{

  if (millis() - TIME_ACTION_LAST >= TIME_ACTION_INTERVAL)
  {

    // action
    analogWrite(PIN_PWMA, PWMA);
    digitalWrite(PIN_DIRA, DIRA);
    PWMA += PWMA_DIR;

    // checking dir
#pragma region check dir

    if (PWMA_DIR > 0)
    {

      if (PWMA >= 255)
      {
        PWMA_DIR = -1;

        Serial.print("Switch dir to ");
        Serial.println(PWMA_DIR);

#pragma region servo

        Serial.println("min");
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa);
        Wire.write(0);
        Wire.endTransmission();
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa + 1);
        Wire.write(0);
        Wire.endTransmission();
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa + 2);
        Wire.write(210);
        Wire.endTransmission();
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa + 3);
        Wire.write(0);
        Wire.endTransmission();

#pragma endregion

      }

    }
    else if (PWMA_DIR < 0)
    {

      if (PWMA <= 0)
      {
        PWMA_DIR = 1;

        Serial.print("Switch dir to ");
        Serial.println(PWMA_DIR);

        DIRA = !DIRA;
        Serial.print("DIRB=");
        Serial.println(DIRA);

#pragma region servo

        Serial.println("max");
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa);
        Wire.write(0);
        Wire.endTransmission();
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa + 1);
        Wire.write(0);
        Wire.endTransmission();
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa + 2);
        Wire.write(0x9f);
        Wire.endTransmission();
        Wire.beginTransmission(I2C_DEVICE_ADDR);
        Wire.write(0xfa + 3);
        Wire.write(0x01);
        Wire.endTransmission();

#pragma endregion

      }

    }
    else
    {

      // fix bug
      PWMA_DIR = 1;

    }

#pragma endregion

    // update
    TIME_ACTION_LAST = millis();

  }

}

char* My2Bytes(char one, char two)
{

  char  res[] = { one, two };

  return  res;

}

//void MySetPosition
