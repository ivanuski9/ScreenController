#include <LiquidCrystal.h>
#include <ScreenController.h>

#define NUM_MENUS (sizeof(titles)/sizeof(String))

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ScreenController screen(lcd, 16,2);

int i = 0;
String titles[5] = {"Menu 1", "Menu 2", "Menu 3", "Menu 4", "Menu 5"};

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  //lcd.autoscroll();
  
  String str = "ScreenController";
  screen.println(lcd, str);
  delay(2000);
  str = "Test 2";
  screen.print(lcd, str);
  delay(2000);
  
  screen.clear(lcd);
  screen.printList(lcd, titles, NUM_MENUS, 2);
}

void loop() {
  screen.moveWindow(lcd, 0, 4);
  
  delay(300);
}

