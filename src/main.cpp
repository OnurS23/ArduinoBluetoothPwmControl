// Library import
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdio.h>

// Constant variable definition
const int delay_time = 10;

// Private variable definition
SoftwareSerial bt_communication(7, 6); // RX,TX
String command = "";

int pwm1, pwm2, dir1, dir2;
int last_pwm1, last_pwm2 = 0;

// Private function prototypes
void increase_pwm(int pin, int start, int target);
void decrease_pwm(int pin, int start, int target);
void rake_init(void);
void printVariables(void);
void adjust_pwm(int pin, int start, int target);

void setup()
{
  rake_init();
}

void loop()
{
  if (bt_communication.available())
  {
    char data = bt_communication.read();

    if (data == 'F') //The command gets executed after the finish bit is received.
    {

      Serial.println(command);

      //The received string gets parsed into variables.
      pwm1 = command.substring(2, 5).toInt();
      pwm2 = command.substring(7, 10).toInt();
      dir1 = command.substring(1, 2).toInt();
      dir2 = command.substring(6, 7).toInt();

      //The following variables get printed: pwm1, pwm2, dir1, dir2
      printVariables();

      //PWM values get adjusted according to the direction data
      if (dir1 == 1)
      {
        adjust_pwm(3, last_pwm1, pwm1);
      }
      else if (dir1 == 0)
      {
        adjust_pwm(5, last_pwm1, pwm1);
      }

      if (dir2 == 1)
      {
        adjust_pwm(9,last_pwm2,pwm2);
      }
      else if (dir2 == 0)
      {
        adjust_pwm(10,last_pwm2,pwm2);
      }

      //Current PWM values get saved as last PWM values.
      last_pwm1 = pwm1;
      last_pwm2 = pwm2;

    
      command = "";
    }
    else
    {
      //The received character gets added to the command string.
      command = command + data;
    }
  }
}

// Explicit functions

//PWM signal gets increased with a delay
void increase_pwm(int pin, int start, int target)
{
  for (int i = start; i <= target; i++)
  {
    Serial.print("Analog 1:");
    Serial.println(i);
    analogWrite(pin, i);
    delay(delay_time);
  }
}

//PWM signal gets decreased with a delay
void decrease_pwm(int pin, int start, int target)
{
  for (int i = start; i >= target; i--)
  {
    Serial.print("Analog 2:");
    Serial.println(i);
    analogWrite(pin, i);
    delay(delay_time);
  }
}

//This function decides if the PWM value should get increased or decreased.
void adjust_pwm(int pin, int start, int target){
  if(target-start>0){
    increase_pwm(pin, start, target);
  }
  else if(target-start<0){
    decrease_pwm(pin, start, target);
  }
}

//Necessary initialization commands for pin modes and serial communication
void rake_init(void)
{
  Serial.begin(9600);
  bt_communication.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void printVariables()
{
  Serial.print("Pwm 1:");
  Serial.println(pwm1);
  Serial.print("Pwm 2:");
  Serial.println(pwm2);
  Serial.print("Last Pwm 1:");
  Serial.println(last_pwm1);
  Serial.print("Last Pwm 2:");
  Serial.println(last_pwm2);
  Serial.print("Dir 1:");
  Serial.println(dir1);
  Serial.print("Dir 2:");
  Serial.println(dir2);
}