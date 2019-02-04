
#include <AccelStepper.h>
#define HALFSTEP 8

const int button_fw = 4;
const int button_bw = 5;
const int led_1 = 3;
const int led_2 = 2;

int speed_multiplier = 1; // 1 to 4

#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  7     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

long bw_time = 0, t = 0;
bool change_speed = false;

//////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(button_fw, INPUT_PULLUP);
  pinMode(button_bw, INPUT_PULLUP);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  set_leds();

  stepper.setMaxSpeed(250.0);
  stepper.setAcceleration(10000.0);
  stepper.setSpeed(200.0);
}

//////////////////////////////////////////////////////////////////////////////
void loop() {
  t = millis();
  
  if (!digitalRead(button_bw)) {
    change_speed = true;
    if(t - bw_time > 400){
      stepper.setMaxSpeed(250.0*4);
      stepper.move(1000);
      change_speed = false;
    }
  }
  else
  {
    if(change_speed == true && t - bw_time > 50) {
      speed_multiplier++;
      if(speed_multiplier > 4)
        speed_multiplier = 1;

        set_leds();
    }
    
    bw_time = millis();
    if (!digitalRead(button_fw)) {
      stepper.setMaxSpeed(250.0*speed_multiplier);
      stepper.move(-1000);
    }
    else
      stepper.stop();
  }
      
  stepper.run();
}

void set_leds()
{
  switch(speed_multiplier)
  {
    case 1:
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
      break;
    case 2:
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, LOW);
      break;
    case 3:
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, HIGH);
      break;
    default:
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
  }
}
