#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// ---------- Hardware ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
const int servoPin = 10, buzzer = 11;

// ---------- Keypad ----------
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};
byte rowPins[ROWS] = {2,3,4,5}, colPins[COLS] = {6,7,8,9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------- Password ----------
char pass[] = "2421", input[5];
byte idx = 0;

// ---------- Setup ----------
void setup() {
  servo.attach(servoPin);
  pinMode(buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  WELCOME To");
  lcd.setCursor(0, 1);
  lcd.print("SECURE SYSTEM");
  delay(1500);
  lcd.clear();
  lcd.print("ENTER PASSWORD");
  lcd.setCursor(0, 1);
  servo.write(0); // Door initially closed
}

// ---------- Loop ----------
void loop() {
  char k = keypad.getKey();
  if (k != NO_KEY) {
    if (k == 'C') {
      idx = 0;
      lcd.setCursor(0, 1);
      lcd.print("    ");
      lcd.setCursor(0, 1);
      return;
    }
    if (k >= '0' && k <= '9' && idx < 4) {
      input[idx] = k;
      lcd.setCursor(idx, 1);
      lcd.print('*');
      idx++;
    }
    if (idx == 4) {
      input[idx] = '\0';
      checkPass();
      idx = 0;
      lcd.setCursor(0, 1);
      lcd.print("    ");
    }
  }
}

// ---------- Check Password ----------
void checkPass() {
  if (strcmp(input, pass) == 0) {
    lcd.clear();
    lcd.print("ACCESS GRANTED");
    lcd.setCursor(0, 1);
    lcd.print("DOOR OPENED");
    beep(2);

    servo.write(90); // Door open
    delay(1000);

    // Start countdown
    for (int t = 30; t >= 0; t--) {
      lcd.clear();
      lcd.print("DOOR OPENED");
      lcd.setCursor(0, 1);
      lcd.print("Closes in: ");
      lcd.print(t);
      lcd.print("s");
      delay(1000);
    }

    // Close door after 30 seconds
    lcd.clear();
    lcd.print("DOOR CLOSED");
    servo.write(0);
    beep(1);
    delay(1000);
    lcd.clear();
    lcd.print("ENTER PASSWORD");
  } 
  else {
    lcd.clear();
    lcd.print("WRONG PASSWORD");
    lcd.setCursor(0, 1);
    lcd.print("TRY AGAIN");
    beep(1);
    delay(1000);
    lcd.clear();
    lcd.print("ENTER PASSWORD");
  }
}

// ------------------ Buzzer ------------------
void beep(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzer, HIGH);
    delay(400);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
}
