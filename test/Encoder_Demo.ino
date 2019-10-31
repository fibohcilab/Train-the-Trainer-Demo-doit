/*
 *For Train the trainer #2019
 *
 *---------Tested Encoder LPD3806-400BM-G5-24C---------
 *The circuit:
 *RED PIN [V_cc] connection of the sensor attached to +5V
 *BLK PIN [V_0]  connection of the sensor attached to ground / GND
 *GRN PIN [A] connection of the sensor attached to resistor e.g. 100 ohm +-5% and digital pin 2
 *WTI PIN [B] connection of the sensor attached to resistor e.g. 100 ohm +-5% and digital pin 3
 *
 *modified 31 Oct 2019
 *by J. Poonsiri
*/

int a_GRN = 2;
int b_WTI = 3;
volatile unsigned int temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder

void setup() {
  Serial.begin (9600);
  pinMode(a_GRN, INPUT_PULLUP); // internal is pullup input pin 2 
  pinMode(b_WTI, INPUT_PULLUP); // internal is pullup input pin 3
  
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
}
   
void loop() {
  display_encoder();
}
   
void ai0() {
    // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
    // Check pin 3 to determine the direction
    if(digitalRead(b_WTI)==LOW) {
      counter++;
    }else{
      counter--;
    }
  }
   
void ai1() {
    // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
    // Check with pin 2 to determine the direction
    if(digitalRead(a_GRN)==LOW) {
      counter--;
    }else{
      counter++;
    }
}
void display_encoder(){
    // Send the value of counter
    if( counter != temp ){
      Serial.println (counter);
      temp = counter;
    }
    
    //NOTED
    //Serial.print(digitalRead(a_GRN));
    //Serial.print(" ");
    //Serial.println(digitalRead(b_WTI));
    
    /*
    if (counter > temp){
      Serial.print ("CW : ");
      Serial.println (counter);
      temp = counter;
    }
    else if (counter < temp){
      Serial.print ("CCW : ");
      Serial.print (counter);
      temp = counter;
    }
    */
}
