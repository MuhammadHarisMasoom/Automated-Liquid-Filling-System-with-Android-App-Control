#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);

int motor = 8; //motor driver pin
int bottle = 3; //IR Sensor to detect bottle
int relay = 2; // Relay to turn on pump and soleniod
int buzzer = 9; // Buzzer Sound

int i = 0;
//for button
int redPin = 7;
int buttonPin = 12;
int dt = 100;
int LEDState = 0;
int buttonOldVal = 1;
int buttonNewVal;
//end for button

int data; // for bluetooth detection
int bt_state = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(bottle, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  pinMode(buzzer, OUTPUT);

  lcd.begin();//Initializing display
  lcd.backlight();//To Power ON the back light
  welcome();


  // for button
  pinMode(redPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);

}
void loop() {
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.println(data);
  }
    if (data == 49) {
      digitalWrite(motor, LOW);
      buttonDetect();
    }
    else if (data == 50) {
      digitalWrite(motor, HIGH);
      buttonDetect();
    }


    buttonDetect();
    //code for motor

    if (digitalRead(bottle) == LOW) {
      // Stop the motor
      Buzzer();
      i = i + 1;
      lcdPrintBottles(i);
      delay(100);// delay to stop the bottle infront of pump
      digitalWrite(motor, LOW);
      delay(300);
      digitalWrite(relay, LOW);
      delay(8000);
      Buzzer();
      digitalWrite(relay, HIGH);


    }
    else if (digitalRead(bottle) == HIGH && LEDState == 0) {
      // Start the motor
      digitalWrite(motor, HIGH);
    }
  }


void Buzzer() {
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);
}
void welcome() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Automatic Bottle");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Filling System");
}
String lcdPrintBottles(int i) {
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 0);
  lcd.print("Bottle Filled");
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print(i);
}

void buttonDetect() {
  //code for button
  buttonNewVal = digitalRead(buttonPin);
  if (buttonOldVal == 0 && buttonNewVal == 1) {
    if (LEDState == 0) {
      data = 79;
      digitalWrite(redPin, HIGH);
      LEDState = 1;
    }
    else {
      data = 79;
      digitalWrite(redPin, LOW);
      LEDState = 0;
    }
  }
  buttonOldVal = buttonNewVal;
  delay(dt);
  //  return LEDState;

  if (LEDState == 1) {
    digitalWrite(motor, LOW);
  }
}
