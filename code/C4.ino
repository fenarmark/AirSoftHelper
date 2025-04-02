#include <TM1637Display.h>
#include <Key.h>
#include <Keypad.h>

#define CLK 5
#define DIO 6

TM1637Display display = TM1637Display(CLK, DIO);




const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

const uint8_t SEG_CODE[] = {
  
	SEG_A | SEG_F | SEG_E | SEG_D,                   // C
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};

const uint8_t SEG_TIME[] = {
  
	SEG_F | SEG_E | SEG_D | SEG_G,                   // t
	SEG_B | SEG_C,                                   // I
  SEG_E | SEG_F | SEG_A | SEG_B | SEG_C,           // M
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};

byte colPins[COLS] = {13, 12, 11}; //column pins
byte rowPins[ROWS] = {10, 9, 8, 7}; //row pins

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup() {
Serial.begin(9600);
display.setBrightness(0x0f);
}
  
char enter_code() {
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };  
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
display.setSegments(blank);
display.setSegments(SEG_CODE);
  char key = 0;
  int i = 0;
  char pin[4];
  while (i <= 3) {
    key = keypad.getKey();
    if (key) {
    Serial.print("key : ");
    Serial.println(key);
    byte key_display = key - 48;
    blank[i] = display.encodeDigit(key_display);
    display.setSegments(blank);
    pin[i] = key;
    key = 0;
    i++;
    }
      }
  pin[4] = '\0';
  Serial.print("PIN set: ");
  Serial.println(pin);
  return pin;

}

char enter_time() {
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };  
uint8_t blank[] = { 0x00, 0b10000000, 0x00, 0x00 };
display.setSegments(SEG_TIME);
  char key = 0;
  int i = 0;
  char time[4];
  while (i <= 3) {
    key = keypad.getKey();
    if (key) {
    Serial.print("key : ");
    Serial.println(key);
    byte key_display = key - 48;
    if (i == 1) { 
            blank[i] = display.encodeDigit(key_display) | 0b10000000; // Ensure colon bit is still set
        }
    else {
            blank[i] = display.encodeDigit(key_display);
    }
    display.setSegments(blank);
    time[i] = key;
    key = 0;
    i++;
    }
      }
  time[4] = '\0';
  Serial.print("TIME set: ");
  Serial.println(time);
  return time;

}


void loop() {
char pin = enter_code();
char time = enter_time();
}


