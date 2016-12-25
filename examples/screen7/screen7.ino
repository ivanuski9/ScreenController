#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <ScreenController.h>

#define NUM_MENUS (sizeof(titles)/sizeof(char *))
#define POWER_LCD 10
#define TIMEOUT 5000

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ScreenController screen(lcd, 16, 2, POWER_LCD);

int selected = -1;
const char MENU1[] PROGMEM = "Menu 1";
const char MENU2[] PROGMEM = "Menu 2";
const char MENU3[] PROGMEM = "Menu 3";
const char MENU4[] PROGMEM = "Menu 4";
const char MENU5[] PROGMEM = "Menu 5";
const char* const titles[5] PROGMEM = {MENU1, MENU2, MENU3, MENU4, MENU5};
int boton = BUTTON_NONE;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  
  String str = "ScreenController";
  screen.println(lcd, str);
  delay(2000);
  str = "Test 7";
  screen.print(lcd, str);
  delay(2000);
}

void loop() {
  boton = screen.readButton(200);
  
  if (boton != BUTTON_NONE) {
    screen.powerOn(lcd);
    screen.clear(lcd);
    delay(100);
    selected = screen.readItemList(lcd, titles, NUM_MENUS, TIMEOUT, true);
    
    if (selected != -1) {
      screen.clear(lcd);
      String output = "Valor: "+ String(selected+1);
      screen.println(lcd, output);
      screen.print(lcd, "Pulsa SELECT");
      screen.waitButton(BUTTON_SELECT, TIMEOUT);
      delay(200);
    }
  }
  
  screen.powerOff(lcd);  
}
