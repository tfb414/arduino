#include <Bounce2.h>
#include <Keyboard.h>

#define BUTTON_KEY1 KEY_F5
#define BUTTON_KEY2 KEY_F6
#define BUTTON_KEY3 KEY_F7
#define BUTTON_KEY4 KEY_F8
#define BUTTON_KEY5 KEY_F9
#define BUTTON_KEY6 KEY_F10
#define BUTTON_KEY7 KEY_F11
#define BUTTON_KEY8 KEY_F12
#define BUTTON_KEY9 KEY_F13
#define BUTTON_KEY10 KEY_F14
#define BUTTON_KEY11 KEY_LEFT_SHIFT
#define BUTTON_KEY12 KEY_LEFT_CTRL
 
// Pin definitions
#define BUTTON_PIN1 2
#define BUTTON_PIN2 3
#define BUTTON_PIN3 4
#define BUTTON_PIN4 5
#define BUTTON_PIN5 6
#define BUTTON_PIN6 7
#define BUTTON_PIN7 8
#define BUTTON_PIN8 9
#define BUTTON_PIN9 10
#define BUTTON_PIN10 16
#define BUTTON_PIN11 14
#define BUTTON_PIN12 15
#define LED_PIN A1

void toggleLight() {
  boolean lightStatus = digitalRead(LED_PIN);
  lightStatus ?  digitalWrite(LED_PIN, LOW) : digitalWrite(LED_PIN, HIGH);
}

// Button helper class for handling press/release and debouncing
class button {
  public:
  const char key;
  const uint8_t pin;
 
  button(uint8_t k, uint8_t p) : key(k), pin(p){}

  void press(boolean state){
    if(state == pressed || (millis() - lastPressed  <= debounceTime)){
      return; // Nothing to see here, folks
    }
     
    lastPressed = millis();
 
    state ? Keyboard.press(key) : Keyboard.release(key);
    if (pin == 10 && !state) 
    {
      toggleLight();
    }
    pressed = state;
  }
 
  void update(){
    
    press(!digitalRead(pin));
  }
 
  private:
  const unsigned long debounceTime = 30;
  unsigned long lastPressed = 0;
  boolean pressed = 0;
} ;

button buttons[] = {
  {BUTTON_KEY1, BUTTON_PIN1},
  {BUTTON_KEY2, BUTTON_PIN2},
  {BUTTON_KEY3, BUTTON_PIN3},
  {BUTTON_KEY4, BUTTON_PIN4},
  {BUTTON_KEY5, BUTTON_PIN5},
  {BUTTON_KEY6, BUTTON_PIN6},
  {BUTTON_KEY7, BUTTON_PIN7},
  {BUTTON_KEY8, BUTTON_PIN8},
  {BUTTON_KEY9, BUTTON_PIN9},
  {BUTTON_KEY10, BUTTON_PIN10},
  {BUTTON_KEY11, BUTTON_PIN11},
  {BUTTON_KEY12, BUTTON_PIN12},
};

const uint8_t NumButtons = sizeof(buttons) / sizeof(button);

void setup() {
  pinMode(1, INPUT_PULLUP);
  if(!digitalRead(1)){
    failsafe();
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  for(int i = 0; i < NumButtons; i++){
    pinMode(buttons[i].pin, INPUT_PULLUP);
  }
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < NumButtons; i++){
    buttons[i].update();
  }
}

void failsafe(){
  for(;;){} // Just going to hang out here for awhile :D
}

/*
 Letter Keys
 KEY_A to KEY_Z 

 Number Keys
 KEY_1 to KEY_0

 Function Keys
 KEY_F1 to KEY_F12

 Numberpad Keys
 KEYPAD_1 to KEYPAD_0
 KEYPAD_SLASH KEYPAD_MINUS KEYPAD_PLUS 
 KEYPAD_ENTER KEYPAD_PERIOD KEYPAD_ASTERIX

 Other Keys
 KEY_ENTER KEY_ESC KEY_BACKSPACE KEY_TAB 
 KEY_SPACE KEY_MINUS KEY_EQUAL KEY_LEFT_BRACE  
 KEY_RIGHT_BRACE KEY_BACKSLASH KEY_SEMICOLON 
 KEY_QUOTE KEY_TILDE KEY_COMMA KEY_PERIOD  
 KEY_SLASH KEY_CAPS_LOCK
 KEY_PRINTSCREEN KEY_SCROLL_LOCK 
 KEY_PAUSE KEY_INSERT  KEY_HOME  KEY_PAGE_UP 
 KEY_DELETE  KEY_END KEY_PAGE_DOWN KEY_RIGHT 
 KEY_LEFT  KEY_DOWN  KEY_UP  KEY_NUM_LOCK  

 Modifier Keys
 MODIFIERKEY_CTRL
 MODIFIERKEY_SHIFT
 MODIFIERKEY_ALT
 MODIFIERKEY_GUI
 */
 
