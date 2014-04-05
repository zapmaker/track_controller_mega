// zapmaker.org (c) zapmaker 2014
// Apache 2.0 license
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','U'},
  {'4','5','6','D'},
  {'7','8','9','S'},
  {'C','0','H','E'}
};
byte rowPins[ROWS] = {36, 34, 32, 30}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28, 26, 24, 22}; //connect to the column pinouts of the keypad

#define MENU_START       0
#define MENU_WAIT_FOR_ENTER   1

#define MENU_INPUT_SIZE 10
char menuInput[MENU_INPUT_SIZE];
int menuInputPos = 0;
int menuState = MENU_START;

int lastAdcTime = 0;
int servoAPin = 2;
int servoBPin = 3;
Servo servoA;
Servo servoB;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int relayPinStart = 31;
boolean analogReadMode = false;

void setup(){
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Train Controller");
  lcd.setCursor(0, 1);
  lcd.print("zapmaker.org");
    
  servoA.attach(servoAPin);
  servoB.attach(servoBPin);
  
  setServo(1, false);
  setServo(2, false);
  int pin = relayPinStart;
  for (int i = 0; i < 12; i++)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    pin += 2;
  }
  
  delay(3000);// for sign on msg
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for cmd");
}
  
  
void loop()
{
  int currtime = millis();
  char key = keypad.getKey();
  
  if (key)
  {
    menuProcessor(key);
  }
  
  
  if (analogReadMode)
  {
    if ((currtime - lastAdcTime) > 200)
    {
      lcdClearLine(0);
      lcdClearLine(1);
      for (int i = 1; i <= 4; i++)
      {
        int val = analogRead(i);
        lcd.setCursor((i - 1) * 4, 0); 
        lcd.print(val);
      }
      for (int i = 5; i <= 8; i++)
      {
        int val = analogRead(i);
        lcd.setCursor((i - 5) * 4, 1); 
        lcd.print(val);
      }
      lastAdcTime = currtime;
    }
  }
}


void menuProcessor(char key)
{
  switch (menuState)
  {
    case MENU_START:
      if (key == 'C')
      {
        if (menuInputPos == 0)
        {
          allRelays(LOW);
          lcdPrintLine(0, "All relays off");
        }
        else
        {
          int val = decodeMenuValue();
          oneRelay(val, LOW);
          lcdPrintLine(0, "One relay off");
          menuInputPos = 0;
        }
      }
      else if (key == 'S')
      {
        if (menuInputPos == 0)
        {
          allRelays(HIGH);
          lcdPrintLine(0, "All relays ON");
        }
        else
        {
          int val = decodeMenuValue();
          oneRelay(val, HIGH);
          lcdPrintLine(0, "One relay ON");
          menuInputPos = 0;
        }
      }
      else if (key == 'U')
      {
        if (menuInputPos > 0)
        {
          int val = decodeMenuValue();
          setServo(val, true);
          if (val == 1)
            lcdPrintLine(0, "Servo Left Up");
          else
            lcdPrintLine(0, "Servo Right Up");
          menuInputPos = 0;
        }
      }
      else if (key == 'D')
      {
        if (menuInputPos > 0)
        {
          int val = decodeMenuValue();
          setServo(val, false);
          if (val == 1)
            lcdPrintLine(0, "Servo Left Down");
          else
            lcdPrintLine(0, "Servo Right Down");
          menuInputPos = 0;
        }
      }
      else if (key >= '0' && key <= '9')
      {
        if (menuInputPos < MENU_INPUT_SIZE)
        {
          if (menuInputPos == 0)
          {
            lcdResetLine(1);
            lcdPrintLine(0, "Waiting for #cmd");
          }
          menuInput[menuInputPos] = key - '0';
          lcd.setCursor(menuInputPos, 1);
          lcd.print(key);
          menuInputPos++;
        }
      }
      else if (key == 'H')
      {
        if (analogReadMode)
        {
          lcdResetLine(0);
          lcdResetLine(1);
          lcdPrintLine(0, "Waiting for cmd");
      
          analogReadMode = false;
        }
        else
          analogReadMode = true;
      }

      break;
  }  
  
}
int decodeMenuValue()
{
  if (!menuInputPos)
    return 0;
  int result = 0;
  int multiplier = 1;
  for (int i = (menuInputPos - 1); i >= 0; i--)
  {
    result += menuInput[i] * multiplier;
    multiplier *= 10;
  }
  return result;
}
void setServo(int servo, boolean pos)
{
  if (servo < 1 || servo > 2)
  {
    lcdPrintLine(0, "Servos 1 and 2 only");
    return;
  }
    
  int val = pos ? 1 : 0;
    
  int mapResult = 0;
  if (servo == 1)  
  {
    mapResult = map(val, 0, 1, 0, 171);
    servoA.write(mapResult);
  }
  else
  {
    mapResult = map(val, 0, 1, 173, 2);
    servoB.write(mapResult);
  }
        
}
void oneRelay(int index, int value)
{
  if (index < 1 || index > 12)
  {
    lcdPrintLine(0, "Relays 1-12 only");
    return;
  }
  
  int pin = relayPinStart + 2 * (index - 1);
  digitalWrite(pin, value);
}
void allRelays(int value)
{
  int pin = relayPinStart;
  for (int i = 0; i < 12; i++)
  {
    digitalWrite(pin, value);
    pin += 2;
  }
}
void lcdPrintLine(int line, char *p)
{
  lcdResetLine(line);
  lcd.print(p);
}
void lcdResetLine(int line)
{
  lcdClearLine(line);
  lcd.setCursor(0, line);
}
void lcdClearLine(int line)
{
  lcd.setCursor(0, line);
  lcd.print("                ");
}

