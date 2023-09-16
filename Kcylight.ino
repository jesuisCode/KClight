
#include <RtcDS1302.h>

ThreeWire myWire(6, 5, 7); // IO, SCLK, CE // 5-CLK, 6-DAT, 7-RST 
RtcDS1302<ThreeWire> Rtc(myWire);

//Automatic Day and Night Light
bool cz1s = false; //sensor corridor status
bool cz2s = false; //sensor bathroom status
float cz3s = 0; //sensor light
int cz1p = 3; //sensor motion corridor pin - SF defined - can be changed
int cz2p = 10; //sensor motion bathroom pin - SF defined - can be changed
int cz3p = A0; //sensor light pin - HW defined
int white = 11; //white pin control - HW defined - can not be changed
int red = 12; //red pin control - HW defined - can not be changed
int blue = 13; //blue pin control - HW defined - can not be changed
float light_int_trshld = 3.5; // light intensity sensor treshold value
int times[5] ; //actual time
int timeref = 0; //reference time sunset/dawn


void setup()
{
  pinMode(cz1p, INPUT); //Set sensor pin as output
  pinMode(cz2p, INPUT);
  pinMode(cz3p, INPUT);
  Serial.begin(9600);
  Rtc.Begin();
  // attachInterrupt(digitalPinToInterrupt(3), detect, RISING); //if movement detected, interruption occurs.
  // attachInterrupt(digitalPinToInterrupt(2), detect, RISING);
}


void loop()
{
  //get actual time
  RtcDateTime now = Rtc.GetDateTime();
  Serial.println("alive");

  delay(1000);
  detect();

  //turn light on while sensor status
}

//RTC date and time code 
int getDateTime(const RtcDateTime& dt, int n)
{
  int argay[] = {dt.Year(), dt.Month(), dt.Day(), dt.Hour(), dt.Minute()};
  return argay[n];
}

//reference time generator
float timerefcalc(int x, int t) //time per year day calculation in 1/100 format, x - no. of day of year, t - rise or dawn
{
  float A[4];
  if (t == 1)
  {
    float B[] = {18.84, 75.03, 188.69, 2.47};
    memcpy(A, B, sizeof(B));
  }
  else
  {
    float C[] = {6.72, 270.95, 189.94, 2.42};
    memcpy(A, C, sizeof(C));
  }

  float y = A[0] + (A[3] * sin(3.1415 * (x - A[1]) / A[2]));

  return y;
}

//main function
void detect()
{
  //read sensors
  cz1s = digitalRead(cz1p); //sensor motion corridor pin
  cz2s = digitalRead(cz2p); //sensor motion bathroom pin
  cz3s = analogRead(cz3p); //sensor light pin

  //get actual time
  RtcDateTime now = Rtc.GetDateTime();

  //create day table
  for (int i = 0; i < 5; i++) 
  {
    times[i] = getDateTime(now, i); // 0-Y, 1-M, 2-D, 3-h, 4-m
  }
  int days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}; //day of year per month
  int doy = days[times[1] - 1] + times[2]; //day of year value

  //main conditional loop 
  if (cz1s or cz2s == true) //if one of 2 motion sensors have detected motion, calculate time to activate light
  {
    float timeref[2];
    timeref[0] = timerefcalc(doy, 1); // dawn
    timeref[1] = timerefcalc(doy, 0); // rise
    Serial.println("Detect");
    float x = times[3];
    float y = times[4];
    float t = (x + y/60); // connect hour and minute in 1/100 format
    
    if (cz3s < light_int_trshld or 1.0 >= t or t > timeref[1]) //light is turn on if low light detected or hour between sun- -dawn and -rise.
    {
      Serial.println("White");
      Serial.println(cz3s/200);
      analogWrite(white, 255);
    }
    
    else if ( 2.0 > t and t > 1.0)
    {
      Serial.println("Red100");
      analogWrite(red, 255);
    }

    else if ( timeref[0] > t and t > 2.0)
    {
      Serial.println("Red10");
      analogWrite(red, 25);
    }

  }
  else
  {
    analogWrite(white, 0);
    analogWrite(red, 0);
    analogWrite(blue, 0);
  }

}
