/*
 *For Train the trainer #2019
 *
 *---------Tested Ultrasonic HY-SRF05---------
 *Assuming a room temp of 25 degrees centigrade
 *The circuit:
 *VVC connection of the sensor attached to +5V
 *GND connection of the sensor attached to ground
 *TRIG connection of the sensor attached to digital pin 13
 *ECHO connection of the sensor attached to digital pin 12
 *
 *modified 31 Oct 2019
 *by J. Poonsiri
*/
const int TRIG_PIN = 13;
const int ECHO_PIN = 12;
long duration, distanceCm ;

void setup() {
 // initialize digital pin 
 pinMode(TRIG_PIN,OUTPUT);
 pinMode(ECHO_PIN,INPUT);
 // initialize serial communication:
 Serial.begin(9600);
}

void loop()
{
 // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 digitalWrite(TRIG_PIN, LOW);
 delayMicroseconds(2);
 digitalWrite(TRIG_PIN, HIGH);
 delayMicroseconds(10);
 digitalWrite(TRIG_PIN, LOW);

 //pulseIn(): Reads a pulse (either HIGH or LOW) on a pin
 duration = pulseIn(ECHO_PIN,HIGH);
 
 // convert the time into a distance
 // c_air = 331.3 + (0.6 *(Temperature C°))
 // c_air = 331.3 + (0.6 *(25))
 // c_air = 346.3 m/s  <---convert--->  centimeters / microsecond
 // 1 meter / second = 0.0001 centimeters / microsecond
 // c_air = 0.03463 centimeters / microsecond --> Round trip speed
 // c_air = (0.03463/2) centimeters / microsecond --> One way speed
 
 distanceCm = duration * (0.03463 / 2) ;
 
 if (distanceCm <= 0){
   Serial.println("Out of range");
 }
 else {
   Serial.print(distanceCm);
   Serial.print(" cm ");
   Serial.println();
 }
 delay(100);
}
