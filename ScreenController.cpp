#include <Arduino.h>
#include <LiquidCrystal.h>
#include "ScreenController.h"

byte b_arrow_right[8] = {
   B10000,
   B11000,
   B11100,
   B11110,
   B11110,
   B11100,
   B11000,
   B10000
};

byte b_up_down[8] = {
   B00100,
   B01110,
   B11111,
   B00000,
   B00000,
   B11111,
   B01110,
   B00100
};


ScreenController::ScreenController(LiquidCrystal _lcd, int w, int h) {
	_lcd.createChar(ARROW_RIGHT, b_arrow_right);
        _lcd.createChar(UP_DOWN, b_up_down);
	height = h;
	clear(_lcd);
}

void ScreenController::print(LiquidCrystal _lcd, String msg) {
	int y_abs = positionAbs(writeY);
	buffer[y_abs] = buffer[y_abs] + msg;
	printBuffer(_lcd);
}

void ScreenController::println(LiquidCrystal _lcd, String msg) {
	// Write in buffer
	int y_abs = positionAbs(writeY);
	buffer[y_abs] = buffer[y_abs] + msg;
	
	// Next line
	if (writeY == (LIMIT_BUFFER-1)) {
		// Move circular buffer
		init++;
		if (init >= LIMIT_BUFFER) init = 0;
		end++;
		if (end >= LIMIT_BUFFER) end = 0;
	} else {
		// Move y
		writeY++;
	}
	buffer[positionAbs(writeY)] = "";

	// Move windows and print
	if ((windows_pos+height) <= writeY) {
		windows_pos++;
		//if (windows_pos >= (LIMIT_BUFFER-1)) windows_pos = LIMIT_BUFFER-2;
	}
	printBuffer(_lcd);
}

void ScreenController::printBuffer(LiquidCrystal _lcd) {
	int i;

	_lcd.clear();
	//_lcd.autoscroll();
	_lcd.noAutoscroll();
	for (i=0; i < height; i++) {
		int pos = positionAbs(windows_pos+i);
		_lcd.setCursor(0, i);
		_lcd.print(buffer[pos]);
	}
}

int ScreenController::positionAbs(int posRelative) {
	return (init+posRelative)%LIMIT_BUFFER;
}

void ScreenController::clear(LiquidCrystal _lcd) {
	int i;

        _lcd.clear();

	// Delete all lines
	for(i=0; i < LIMIT_BUFFER; i++) {
		buffer[i] = "";
	}

	// Reset window and position
	init = 0;
	end = LIMIT_BUFFER-1;
	writeY = 0;
	windows_pos = 0;
}

int ScreenController::readButton() {
	int button = read_LCD_buttons();
	while (button == BUTTON_NONE) {
		delay(50);
		button = read_LCD_buttons();
	}
	return button;
}

void ScreenController::waitButton(int wait_button) {
	int button = read_LCD_buttons();
	while (button != wait_button) {
		delay(50);
		button = read_LCD_buttons();
	}
}

int ScreenController::read_LCD_buttons() {
	int adc_key_in = analogRead(0);      // read the value from the sensor 
  	// my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  	// we add approx 50 to those values and check to see if we are close
  	if (adc_key_in > 1000) return BUTTON_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
  	// For V1.1 us this threshold
  	/*if (adc_key_in < 50)   return btnRIGHT;  
  	if (adc_key_in < 250)  return btnUP; 
  	if (adc_key_in < 450)  return btnDOWN; 
  	if (adc_key_in < 650)  return btnLEFT; 
  	if (adc_key_in < 850)  return btnSELECT;*/

  	// For V1.0 comment the other threshold and use the one below:
  	if (adc_key_in < 50)   return BUTTON_RIGHT;  
  	if (adc_key_in < 195)  return BUTTON_UP; 
  	if (adc_key_in < 380)  return BUTTON_DOWN; 
  	if (adc_key_in < 555)  return BUTTON_LEFT; 
  	if (adc_key_in < 790)  return BUTTON_SELECT;   
	  

  	return BUTTON_NONE;  // when all others fail, return this...
}

int ScreenController::moveWindow(LiquidCrystal _lcd, int limitUp, int limitDown) {
	// Wait press button
	int button = readButton();

	moveScreen(_lcd, button, limitUp, limitDown);

   return button;
}

void ScreenController::moveScreen(LiquidCrystal _lcd, int button, int limitUp, int limitDown) {
   // Move windows
	if (button == BUTTON_UP) {
		windows_pos--;
		writeY--;
	} else if (button == BUTTON_DOWN) {
		windows_pos++;
		writeY++;
	}

	if (windows_pos < limitUp) windows_pos = limitUp;
	if (windows_pos >= limitDown) windows_pos = limitDown-1;
	if (writeY < limitUp) writeY = limitUp;
	if (writeY >= limitDown+1) writeY = limitDown;

	// Print new windows
	printBuffer(_lcd);
}

void ScreenController::printItem(LiquidCrystal _lcd, String msg) {
   _lcd.setCursor(0, 0);
   _lcd.print(msg);	
}

void ScreenController::printSubItem(LiquidCrystal _lcd, String msg) {
	_lcd.setCursor(0, 1);
   _lcd.print(" ");
   _lcd.print(msg);
}

void ScreenController::printCorner(LiquidCrystal _lcd, String msg) {
   _lcd.setCursor(11, 0);
   //_lcd.print(msg.substring(0,4));
   _lcd.print(msg);
}

void ScreenController::printList(LiquidCrystal _lcd, String titles[], int size, int selected) {
	int i;

	// Print all lines
	for (i=0; i < size; i++) {
		println(_lcd, titles[i]);
	}
	
	// Move windows
	windows_pos = selected;
	printBuffer(_lcd);
}

int ScreenController::readItemList(LiquidCrystal _lcd, String titles[], int size) {
   int i, button;
   int selected = 0;
   String titles2[size];

   for (i=0; i < size; i++) {
     titles2[i] = " "+titles[i];
   }

   clear(_lcd);
   printList(_lcd, titles2, size);
   _lcd.setCursor(0, 0);
   _lcd.write(byte(ARROW_RIGHT));
   _lcd.setCursor(0, 0);
   //_lcd.blink();//cursor();

    do {
   	button = readButton();

	if (button == BUTTON_DOWN) selected++;
	if (button == BUTTON_UP) selected--;
	
	if (selected < 0) selected = 0;
	if (selected >= size) selected = size-1;

	if (selected > (windows_pos+1) || selected < windows_pos) { 
		moveScreen(_lcd, button, 0, size-1);
	}
	
	if (windows_pos == selected) {
	    _lcd.setCursor(0, 0);
	    _lcd.write(byte(ARROW_RIGHT));
	    _lcd.setCursor(0, 1);
	    _lcd.print(" ");
	    _lcd.setCursor(0, 0); 
	} else {
	    _lcd.setCursor(0, 1);
	    _lcd.write(byte(ARROW_RIGHT));
	    _lcd.setCursor(0, 0);
	    _lcd.print(" ");
	    _lcd.setCursor(0, 1); 
	}
	
	delay(300);
    } while (button != BUTTON_SELECT);

    return selected;
}

float ScreenController::readFloat(LiquidCrystal _lcd, String title, float initValue, int decimal, String unit) {
   float value = initValue;
   float increment = 1 / pow(10, decimal);
   int button, countClick = 0;
   long lastClick = millis();
   
   clear(_lcd);
   printItem(_lcd, title);
   String strValue = String(value) + char(UP_DOWN) + unit;
   printSubItem(_lcd, strValue);

   do {
      // Wait press button
      button = readButton();

      // Move windows
      if (button == BUTTON_UP) {
	 value = value + increment;
      } else if (button == BUTTON_DOWN) {
	 value = value - increment;
      }

      // Print new value
      String strValue = String(value) + char(UP_DOWN) + unit + " ";
      printSubItem(_lcd, strValue);
      
      if (lastClick+360 > millis()) {
	 if (countClick > 3) {
            delay(50);
	 } else {
	    countClick++;
	    delay(300);
	 }
      } else {
	 countClick = 0;
	 delay(500);
      }
      lastClick = millis();
   } while (button != BUTTON_SELECT);

   return value;
}

int ScreenController::readInteger(LiquidCrystal _lcd, String title, int initValue, String unit) {
   int value = initValue;
   int button, countClick = 0;
   long lastClick = millis();
   
   clear(_lcd);
   printItem(_lcd, title);
   String strValue = String(value) + char(UP_DOWN) + unit;
   printSubItem(_lcd, strValue);

   do {
      // Wait press button
      button = readButton();

      // Move windows
      if (button == BUTTON_UP) {
	 value++;
      } else if (button == BUTTON_DOWN) {
	 value--;
      }

      // Print new value
      String strValue = String(value) + char(UP_DOWN) + unit + " ";
      printSubItem(_lcd, strValue);
      
      if (lastClick+360 > millis()) {
	 if (countClick > 3) {
            delay(50);
	 } else {
	    countClick++;
	    delay(300);
	 }
      } else {
	 countClick = 0;
	 delay(500);
      }
      lastClick = millis();
   } while (button != BUTTON_SELECT);

   return value;
}

String ScreenController::readString(LiquidCrystal _lcd, String title, String initValue) {
   String value;
   int button, x = 0;

   if (initValue == "") {
      value = "a";
   } else {
      value = String(initValue);
   }

   clear(_lcd);
   printItem(_lcd, title);
   printSubItem(_lcd, value);
   _lcd.setCursor(1, 1);
   _lcd.blink();

   do {
      // Wait press button
      button = readButton();

      // Move windows
      if (button == BUTTON_UP) {
	 value[x]++;
      } else if (button == BUTTON_DOWN) {
	 value[x]--;
      } else if (button == BUTTON_RIGHT) {
	 x++;
      } else if (button == BUTTON_LEFT) {
	 x--;
      }

      if (x < 0) x = 0;
      if (x >= value.length()) {
	 value = value+"a";
      }

      // Print new value
      String strValue = value;
      printSubItem(_lcd, strValue);
      
      _lcd.setCursor(1+x, 1);

      delay(300);
    } while (button != BUTTON_SELECT);

   return value;
}
