// Bubble catcher from wizbrewery

/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
*/

// Connect the opto signal pin to the first INT pin (digital pin 2 on an Arduino Uno)
#define resolution 800000 //only capture pulse that are greater than this to avoid noise
#define sample 4 // sample size of pulses for average starts at 0

volatile unsigned long pulseThen;
volatile unsigned long pulseNow;
volatile unsigned long numPulses;
volatile unsigned long keeppulse[sample];
volatile int count=0;

void isr()
{
 unsigned long now = micros();
 float pulselength;
 pulseNow = now;
 int averagepulse;
 int totalsample = 0;
 
 if ((pulseNow - pulseThen) > resolution)
 {
   Serial.print("numPulses ");
   Serial.print(numPulses+1);
   Serial.print(" ");
   pulselength = (float)(pulseNow - pulseThen) /1000000;
   Serial.print(pulselength);
//   Serial.print(" ");
//   Serial.print("keeppulse ");
//   Serial.print(count);
   Serial.print("\n");
   if( count < sample )
   {
    keeppulse[count] = pulselength;
    count++;
   }
   else
   {
    count = 0;
    Serial.print("Average in a sample of ");
    Serial.print(sample+1);
    Serial.print(" is ");
    for( int x=0; (x<sample); x++ ) {
      totalsample += (int)keeppulse[x];
     }
    averagepulse = totalsample/sample;
    Serial.print(averagepulse);
    Serial.print("\n\n");
    }

   pulseThen = pulseNow;
   ++numPulses;
 }
}

void setup()
{
 Serial.begin(19200); 
 Serial.println("Wizbrewery Bubble Counter\n");
 pinMode(3, OUTPUT); //probe output on pin 3
 attachInterrupt(0, isr, RISING);
 numPulses = 0;                      // prime the system to start a new reading
}

void loop()
{

}


