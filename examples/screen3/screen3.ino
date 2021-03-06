#include <LiquidCrystal.h>
#include <ScreenController.h>

#define NUM_MENUS (sizeof(titles)/sizeof(char *))

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
ScreenController screen(lcd, 16, 2);

int i = 0;
int selected = -1;
char* titles[5] = {"Menu 1", "Menu 2", "Menu 3", "Menu 4", "Menu 5"};

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  //lcd.autoscroll();
  
  String str = "ScreenController";
  screen.println(lcd, str);
  delay(2000);
  str = "Test 3";
  screen.print(lcd, str);
  delay(2000);
}

void loop() {
  if (selected < 0) {
    screen.clear(lcd);
    selected = screen.readItemList(lcd, titles, NUM_MENUS);
  } else {
    screen.clear(lcd);
    String output = "Seleccionado: "+ String(selected+1);
    screen.println(lcd, output);
    screen.print(lcd, "Pulsa SELECT");
    
    // Esperamos pulsacion para reiniciar
    int button;
    do {
      button = screen.readButton();
      delay(50);
    } while (button != BUTTON_SELECT);
    
    selected = -1;
    delay(100);
  }
}
