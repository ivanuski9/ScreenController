#include <LiquidCrystal.h>
#include <ScreenController.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ScreenController screen(lcd, 16,2);

int i = 0;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  //lcd.autoscroll();
  
  String str = "ScreenController";
  screen.println(lcd, str);
  delay(2000);
  str = "Test 1";
  screen.print(lcd, str);
}

void loop() {
  String str = " ";
  
  if (i < 10) {
    int button = screen.readButton();
    //str = str + button;
    screen.println(lcd, str);
    if (button == BUTTON_RIGHT) {
      str = "Derecha";
    } else if (button == BUTTON_UP) {
      str = "Arriba";
    } else if (button == BUTTON_DOWN) {
      str = "Abajo"; 
    } else if (button == BUTTON_LEFT) {
      str = "Izquierda"; 
    } else if (button == BUTTON_SELECT) {
      str = "OK"; 
    } else {
      str = "Error"; 
    }
    screen.print(lcd, str);
    i++;
  } else {
    screen.moveWindow(lcd);
  }
  
  delay(300);
}

