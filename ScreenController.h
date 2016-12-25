#ifndef __SCREEN_CONTROLLER__
#define __SCREEN_CONTROLLER__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <string.h>
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
		ScreenController(LiquidCrystal _lcd, int width=16, int height=2, int _pinOnOff=-1);
		void powerOn(LiquidCrystal _lcd);
		void powerOff(LiquidCrystal _lcd);
		void print(LiquidCrystal _lcd, String msg);
		void print(LiquidCrystal _lcd, char msg[]);
		void println(LiquidCrystal _lcd, String msg);
		void println(LiquidCrystal _lcd, char msg[]);
		void clear(LiquidCrystal _lcd);
		int readButton(int timeout=-1);
		void waitButton(int button, int timeout=-1);
   		int waitButtons(int wait_buttons[], int timeout=-1);
		int moveWindow(LiquidCrystal _lcd, int limitUp = 0, int limitDown = LIMIT_BUFFER-1);
		void printItem(LiquidCrystal _lcd, String msg);
		void printSubItem(LiquidCrystal _lcd, String msg);
		void printCorner(LiquidCrystal _lcd, String msg);
		void printList(LiquidCrystal _lcd, String titles[], int size, int selected=0, bool subItem=false);
		void printList(LiquidCrystal _lcd, char* titles[], int size, int selected=0, bool progmem=false, bool subItem=false);
		int readItemList(LiquidCrystal _lcd, String titles[], int size, int timeout=-1);
		int readItemList(LiquidCrystal _lcd, char* titles[], int size, int timeout=-1, bool progmem=false);
		float readFloat(LiquidCrystal _lcd, String title, float initValue, int decimal, String unit="", int timeout=-1);
		int readInteger(LiquidCrystal _lcd, String title, int initValue, String unit="", int timeout=-1);
		String readString(LiquidCrystal _lcd, String title, String initValue, int timeout=-1);
		String readStringNumber(LiquidCrystal _lcd, String title, String initValue, int maxLength=15, int timeout=-1);

	//protected:
		

	private:
		int read_LCD_buttons();
		int positionAbs(int posRelative);
		void moveScreen(LiquidCrystal _lcd, int button, int limitUp = 0, int limitDown = LIMIT_BUFFER-1);
		void printBuffer(LiquidCrystal _lcd);
   		bool contains(int array[], int value);
		String buffer[LIMIT_BUFFER];
		int height;
		int pinOnOff;
		int writeY;
		int windows_pos;
		int init;
		int end;


};

#endif // __SCREEN_CONTROLLER__
