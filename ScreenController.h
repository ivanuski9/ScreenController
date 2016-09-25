#ifndef __SCREEN_CONTROLLER__
#define __SCREEN_CONTROLLER__

#include <Arduino.h>
#include <LiquidCrystal.h>

#define LIMIT_BUFFER 50
#define BUTTON_RIGHT  0
#define BUTTON_UP     1
#define BUTTON_DOWN   2
#define BUTTON_LEFT   3
#define BUTTON_SELECT 4
#define BUTTON_NONE   5

// Caracteres especiales
#define ARROW_RIGHT 0
#define UP_DOWN 1

class ScreenController {

	public:
		ScreenController(LiquidCrystal _lcd, int width=16, int height=2);
		void print(LiquidCrystal _lcd, String msg);
		void println(LiquidCrystal _lcd, String msg);
		void clear(LiquidCrystal _lcd);
		int readButton();
		void waitButton(int button);
		int moveWindow(LiquidCrystal _lcd, int limitUp = 0, int limitDown = LIMIT_BUFFER-1);
		void printItem(LiquidCrystal _lcd, String msg);
		void printSubItem(LiquidCrystal _lcd, String msg);
		void printCorner(LiquidCrystal _lcd, String msg);
		void printList(LiquidCrystal _lcd, String titles[], int size, int selected=0);
		int readItemList(LiquidCrystal _lcd, String titles[], int size);
		float readFloat(LiquidCrystal _lcd, String title, float initValue, int decimal, String unit="");
		int readInteger(LiquidCrystal _lcd, String title, int initValue, String unit="");
		String readString(LiquidCrystal _lcd, String title, String initValue);

	//protected:
		

	private:
		int read_LCD_buttons();
		int positionAbs(int posRelative);
		void moveScreen(LiquidCrystal _lcd, int button, int limitUp = 0, int limitDown = LIMIT_BUFFER-1);
		void printBuffer(LiquidCrystal _lcd);
		String buffer[LIMIT_BUFFER];
		int height;
		int writeY;
		int windows_pos;
		int init;
		int end;


};

#endif // __SCREEN_CONTROLLER__
