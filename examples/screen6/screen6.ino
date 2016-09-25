#include <LiquidCrystal.h>
#include <ScreenController.h>

#define NUM_MENUS (sizeof(titles)/sizeof(String))

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ScreenController screen(lcd, 16, 2);

int i = 0;
String variable1 = "";
String title = "Texto 1";

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  //lcd.autoscroll();
  
  String str = "ScreenController";
  screen.println(lcd, str);
  delay(2000);
  str = "Test 6";
  screen.print(lcd, str);
  delay(2000);
}

void loop() {
  if (variable1 == "") {
    screen.clear(lcd);
    variable1 = screen.readString(lcd, title, "");
  } else {
    screen.clear(lcd);
    String output = "Valor: "+ String(variable1);
    screen.println(lcd, output);
    screen.print(lcd, "Pulsa SELECT");
    
    // Esperamos pulsacion para reiniciar
    int button;
    do {
      button = screen.readButton();
      delay(50);
    } while (button != BUTTON_SELECT);
    
    variable1 = "";
    delay(500);
  }
}
